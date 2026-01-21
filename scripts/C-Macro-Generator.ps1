<#
.SYNOPSIS
  Front-end wrapper for djinterp macro generators.

.DESCRIPTION
  Modes:
    --for_each            -> generator_for_each_full.py
    --varg_utils          -> generator_varg_utils.py

  Meta mode:
    --generate-all-modules
      Generates BOTH non-MSVC (interval-bucketed) and MSVC-capped outputs.
      Produces multiple headers under the chosen output directory.

  Interval notation:
    You may pass intervals as:
      [start,end,step]   inclusive
      (start,end,step)   exclusive (this front-end adjusts endpoints)
    or mixed like (start,end,step] etc.

    The Python generators expect the normalized, inclusive form:
      (start,end,step)

.NOTES
  - This script intentionally avoids List[string].AddRange(...) coercion issues
    by building plain PowerShell string arrays.
  - In meta mode, MSVC outputs are always generated as separate files:
      * core (non-tuple) header
      * one tuple header per arity
      * varg utils header
      * inc header
#>

[CmdletBinding(PositionalBinding = $false)]
param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]] $Args
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-PythonCommand {
    $py = Get-Command python -ErrorAction SilentlyContinue
    if ($py) {
        return @{ Exe = "python"; Extra = @() }
    }

    $pyLauncher = Get-Command py -ErrorAction SilentlyContinue
    if ($pyLauncher) {
        # Force python3 if available via launcher
        return @{ Exe = "py"; Extra = @("-3") }
    }

    throw "Python was not found on PATH (tried: python, py)."
}

function Convert-IntervalNotation {
    param([string]$interval)

    # Already normalized "(a,b,step)"
    if ($interval -match '^\([^,]+,[^,]+,[^,]+\)$') {
        return $interval
    }

    # Parse bracket/paren interval: [start,end,step] or (start,end,step) or mixed
    $full = [regex]::Match($interval, '^([\[\(])([^,]+),([^,]+),([^,]+)([\]\)])$')
    if (-not $full.Success) {
        # Support shorthand [start,end] (assumes step=1)
        $short = [regex]::Match($interval, '^([\[\(])([^,]+),([^,]+)([\]\)])$')
        if ($short.Success) {
            return Convert-IntervalNotation ("{0}{1},{2},1{3}" -f $short.Groups[1].Value, $short.Groups[2].Value, $short.Groups[3].Value, $short.Groups[4].Value)
        }
        return $interval
    }

    $open  = $full.Groups[1].Value
    $start = $full.Groups[2].Value.Trim()
    $end   = $full.Groups[3].Value.Trim()
    $step  = $full.Groups[4].Value.Trim()
    $close = $full.Groups[5].Value

    $adjStart = $start
    $adjEnd   = $end

    if ($open -eq '(') {
        if ($step -match '^-?\d+$' -and $start -match '^-?\d+$') {
            $adjStart = ([int]$start + [int]$step).ToString()
        }
    }

    if ($close -eq ')') {
        if ($step -match '^-?\d+$' -and $end -match '^-?\d+$') {
            $adjEnd = ([int]$end - [int]$step).ToString()
        }
    }

    return "($adjStart,$adjEnd,$step)"
}

function Has-Flag {
    param([string[]]$tokens, [string]$flag)
    return ($tokens -contains $flag)
}

function Get-FlagValue {
    param(
        [string[]]$tokens,
        [string]$flag,
        [string]$defaultValue = $null
    )

    for ($i = 0; $i -lt $tokens.Length; $i++) {
        if ($tokens[$i] -eq $flag -and ($i + 1) -lt $tokens.Length) {
            $next = $tokens[$i + 1]
            if ($next -and -not $next.StartsWith('-')) {
                return $next
            }
        }
    }

    return $defaultValue
}

function Get-MultiValuesAfterFlag {
    param(
        [string[]]$tokens,
        [string]$flag
    )

    $values = @()
    for ($i = 0; $i -lt $tokens.Length; $i++) {
        if ($tokens[$i] -eq $flag) {
            $j = $i + 1
            while ($j -lt $tokens.Length) {
                $t = $tokens[$j]
                if ($t.StartsWith('-') -and -not ($t -match '^[\[\(]')) {
                    break
                }
                $values += $t
                $j++
            }
            break
        }
    }
    return ,$values
}

function Parse-IntervalMax {
    param([string]$normalizedInterval)

    if ($normalizedInterval -match '^\(([^,]+),([^,]+),([^,]+)\)$') {
        $end = $Matches[2]
        if ($end -match '^-?\d+$') {
            return [int]$end
        }
    }

    return $null
}

function Get-IntervalMaxima {
    param([string[]]$normalizedIntervals)

    $maxima = @()
    foreach ($iv in $normalizedIntervals) {
        $m = Parse-IntervalMax $iv
        if ($null -ne $m) {
            $maxima += $m
        }
    }

    # Unique + sort
    $maxima = $maxima | Sort-Object -Unique
    return ,$maxima
}

function Ensure-Dir {
    param([string]$path)
    if (-not (Test-Path $path)) {
        New-Item -ItemType Directory -Force -Path $path | Out-Null
    }
}

function Invoke-Py {
    param(
        [hashtable]$py,
        [string]$scriptPath,
        [string[]]$pyArgs
    )

    if (-not (Test-Path $scriptPath)) {
        throw "Python script not found: $scriptPath"
    }

    # IMPORTANT: @($py.Extra) is safe even when empty.
    & $py.Exe @($py.Extra) $scriptPath @pyArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Python script failed ($LASTEXITCODE): $scriptPath"
    }
}

function Extract-TupleArgs {
    param(
        [string[]]$tokens,
        [switch]$DefaultRange
    )

    # If user provided any tuple selectors, forward them.
    $tupleArgs = @()

    $tupleRange = Get-FlagValue $tokens "--tuple-range" $null
    if ($null -ne $tupleRange) {
        $tupleArgs += @("--tuple-range", $tupleRange)
    }

    $tupleMin = Get-FlagValue $tokens "--tuple-min" $null
    if ($null -ne $tupleMin) {
        $tupleArgs += @("--tuple-min", $tupleMin)
    }

    $tupleMax = Get-FlagValue $tokens "--tuple-max" $null
    if ($null -ne $tupleMax) {
        $tupleArgs += @("--tuple-max", $tupleMax)
    }

    $tupleNaming = Get-FlagValue $tokens "--tuple-naming-convention" $null
    if ($null -ne $tupleNaming) {
        $tupleArgs += @("--tuple-naming-convention", $tupleNaming)
    }

    $tupleArities = Get-MultiValuesAfterFlag $tokens "--tuple-arities"
    if ($tupleArities.Count -gt 0) {
        $tupleArgs += "--tuple-arities"
        $tupleArgs += $tupleArities
    }

    # Default tuple range if requested and user provided none
    if ($DefaultRange -and $tupleArgs.Count -eq 0) {
        $tupleArgs += @("--tuple-range", "1-16")
    }

    return ,$tupleArgs
}

# --------------------------------------------------------------------------------------
# META MODE: --generate-all-modules
# --------------------------------------------------------------------------------------

if (Has-Flag $Args "--generate-all-modules" -or Has-Flag $Args "--generate-all") {

    $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

    $genForEach = Join-Path $scriptDir "generator_for_each_full.py"
    $genVarg    = Join-Path $scriptDir "generator_varg_utils.py"
    $genInc     = Join-Path $scriptDir "macro_inc.py"

    $py = Get-PythonCommand

    # Out root
    $outRoot = Get-FlagValue $Args "--outdir" $null
    if (-not $outRoot) {
        $outRoot = ".\\gen"
    }

    # Intervals
    $intervalsRaw = Get-MultiValuesAfterFlag $Args "--intervals"
    if ($intervalsRaw.Count -eq 0) {
        $intervalsRaw = @(
            "[0,64,1]",
            "[65,128,1]",
            "[129,256,1]",
            "[257,512,1]",
            "[513,1024,1]"
        )
    }

    $intervals = @()
    foreach ($iv in $intervalsRaw) {
        $intervals += (Convert-IntervalNotation $iv)
    }

    $maxima = Get-IntervalMaxima $intervals

    # Forward common switches if present
    $includeZero = @()
    if (Has-Flag $Args "--include-zero-arg") {
        $includeZero = @("--include-zero-arg")
    }

    $noWrappers = @()
    if (Has-Flag $Args "--no-wrappers") {
        $noWrappers = @("--no-wrappers")
    }

    # Tuple selection args
    $tupleArgs = Extract-TupleArgs $Args -DefaultRange

    # Output folders
    $outForEachAll   = Join-Path $outRoot "for_each_all"
    $outTuples       = Join-Path $outRoot "tuples"
    $outVarg         = Join-Path $outRoot "varg_utils"
    $outIncOut       = Join-Path $outRoot "inc"
    $outMSVC         = Join-Path $outRoot "msvc"
    $outMSVCTuples   = Join-Path $outMSVC "tuples"

    Ensure-Dir $outForEachAll
    Ensure-Dir $outTuples
    Ensure-Dir $outVarg
    Ensure-Dir $outIncOut
    Ensure-Dir $outMSVC
    Ensure-Dir $outMSVCTuples

    Write-Host "[all-modules] Non-MSVC: for_each_all buckets..." -ForegroundColor Green
    $argsFEAll = @(
        "--name", "for_each_all_#.h",
        "--outdir", $outForEachAll,
        "--intervals"
    ) + $intervals + @(
        "--substitute", "max"
    ) + $tupleArgs + $includeZero + $noWrappers

    Invoke-Py $py $genForEach $argsFEAll

    Write-Host "[all-modules] Non-MSVC: tuple-per-file buckets..." -ForegroundColor Green
    $argsTuples = @(
        "--tuples-only",
        "--tuple-per-file",
        "--name", "for_each_%_tuple_sep_#.h",
        "--outdir", $outTuples,
        "--intervals"
    ) + $intervals + @(
        "--substitute", "max"
    ) + $tupleArgs + $includeZero + $noWrappers

    Invoke-Py $py $genForEach $argsTuples

    Write-Host "[all-modules] Non-MSVC: varg utils (per max)..." -ForegroundColor Green
    foreach ($m in $maxima) {
        $argsV = @(
            "--name", ("varg_utils_{0}.h" -f $m),
            "--outdir", $outVarg,
            "--max", $m.ToString()
        )
        Invoke-Py $py $genVarg $argsV
    }

    Write-Host "[all-modules] Non-MSVC: inc tables (per max)..." -ForegroundColor Green
    foreach ($m in $maxima) {
        $outPath = Join-Path $outIncOut ("inc_{0}.h" -f $m)
        & $py.Exe @($py.Extra) $genInc 0 $m $outPath
        if ($LASTEXITCODE -ne 0) {
            throw "Python script failed ($LASTEXITCODE): $genInc"
        }
    }

    Write-Host "[all-modules] MSVC: core families (non-tuple)..." -ForegroundColor Green
    $argsMSVCCore = @(
        "--mvc",
        "--max", "127",
        "--no-tuples",
        "--name", "for_each_core_msvc_127.h",
        "--outdir", $outMSVC
    ) + $includeZero + $noWrappers

    Invoke-Py $py $genForEach $argsMSVCCore

    Write-Host "[all-modules] MSVC: tuple files (one per arity)..." -ForegroundColor Green
    $argsMSVCTuples = @(
        "--mvc",
        "--max", "127",
        "--tuples-only",
        "--tuple-per-file",
        "--name", "for_each_%_tuple_sep_msvc.h",
        "--outdir", $outMSVCTuples
    ) + $tupleArgs + $includeZero + $noWrappers

    Invoke-Py $py $genForEach $argsMSVCTuples

    Write-Host "[all-modules] MSVC: varg utils..." -ForegroundColor Green
    $argsMSVCVarg = @(
        "--mvc",
        "--name", "varg_utils_msvc.h",
        "--outdir", $outMSVC
    )
    Invoke-Py $py $genVarg $argsMSVCVarg

    Write-Host "[all-modules] MSVC: inc table..." -ForegroundColor Green
    $outIncMSVC = Join-Path $outMSVC "inc_msvc_126.h"
    & $py.Exe @($py.Extra) $genInc 0 126 $outIncMSVC
    if ($LASTEXITCODE -ne 0) {
        throw "Python script failed ($LASTEXITCODE): $genInc"
    }

    Write-Host "[all-modules] Done. Output: $outRoot" -ForegroundColor Green
    exit 0
}

# --------------------------------------------------------------------------------------
# NORMAL MODE: forward to one generator
# --------------------------------------------------------------------------------------

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Mode selection
$mode = $null
$forward = @()

for ($i = 0; $i -lt $Args.Length; $i++) {
    $a = $Args[$i]

    if ($a -eq '--for_each' -or $a -eq '--for-each') {
        $mode = 'for_each'
        continue
    }

    if ($a -eq '--varg_utils' -or $a -eq '--varg-utils') {
        $mode = 'varg_utils'
        continue
    }

    # ignore meta flag if user passes it accidentally without wanting meta mode
    if ($a -eq '--generate-all-modules' -or $a -eq '--generate-all') {
        continue
    }

    # Normalize --intervals list
    if ($a -eq '--intervals') {
        $forward += $a
        $i++
        while ($i -lt $Args.Length) {
            $t = $Args[$i]
            if ($t.StartsWith('-') -and -not ($t -match '^[\[\(]')) {
                $i--
                break
            }
            $forward += (Convert-IntervalNotation $t)
            $i++
        }
        continue
    }

    $forward += $a
}

if (-not $mode) {
    throw "Missing mode. Use --for_each or --varg_utils (or --generate-all-modules)."
}

switch ($mode) {
    'for_each'   { $generator = Join-Path $scriptDir 'generator_for_each_full.py' }
    'varg_utils' { $generator = Join-Path $scriptDir 'generator_varg_utils.py' }
}

$py = Get-PythonCommand
Invoke-Py $py $generator $forward
