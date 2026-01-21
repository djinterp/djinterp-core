# generator_macro_varg_has_args.py
"""
Generates D_HAS_ARGS macro for detecting presence of variadic arguments.

CRITICAL FIX: The number of trailing 1s must equal the number of placeholders
in the helper macro. For a helper with _0 through _N (N+1 placeholders), we
need N+1 trailing ones followed by 0.

The original bug had N trailing ones for N+1 placeholders, causing single-arg
calls to incorrectly return 0.
"""
import argparse
from pathlib import Path


def _is_c_ident(s: str) -> bool:
    return s and (s[0].isalpha() or s[0] == "_") and all(ch.isalnum() or ch == "_" for ch in s)


def _chunk(items: list[str], n: int) -> list[list[str]]:
    return [items[i : i + n] for i in range(0, len(items), n)]


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Generate D_HAS_ARGS macro with correct trailing sequence"
    )
    ap.add_argument("max", type=int, help="Maximum argument count to support (e.g., 64, 128, 256)")
    ap.add_argument("out", type=str, help="Output header path")
    ap.add_argument("--macro-name", default="D_HAS_ARGS", help="Macro name to generate")
    ap.add_argument("--helper-name", default="D_INTERNAL_HAS_ARGS_HELPER", help="Helper macro to call")
    ap.add_argument("--per-line", type=int, default=16, help="How many numbers per line")
    ap.add_argument(
        "--mvc", 
        action="store_true",
        help="MSVC compatibility mode: helper starts at _1 instead of _0"
    )
    args = ap.parse_args()

    if args.max < 1:
        raise SystemExit("max must be >= 1")
    if not _is_c_ident(args.macro_name):
        raise SystemExit(f"--macro-name must be a C identifier (got {args.macro_name!r})")
    if not _is_c_ident(args.helper_name):
        raise SystemExit(f"--helper-name must be a C identifier (got {args.helper_name!r})")
    if args.per_line < 1:
        raise SystemExit("--per-line must be >= 1")

    # Calculate placeholder count based on mode
    # Standard mode: _0 through _max = max+1 placeholders
    # MSVC mode: _1 through _max = max placeholders
    if args.mvc:
        placeholder_count = args.max
        start_index = 1
    else:
        placeholder_count = args.max + 1
        start_index = 0

    # CRITICAL FIX: trailing 1s must equal placeholder count
    # Original bug: used args.max ones regardless of placeholder count
    num_ones = placeholder_count
    
    nums = ["1"] * num_ones + ["0"]
    groups = _chunk(nums, args.per_line)

    lines: list[str] = []
    lines.append("/* Auto-generated. Do not edit by hand. */")
    lines.append(f"/* D_HAS_ARGS: max={args.max}, placeholders={placeholder_count}, trailing_ones={num_ones} */")
    lines.append("")
    lines.append(f"/* --- {args.macro_name} --- */")
    lines.append("")
    lines.append(f"#define {args.macro_name}(...) \\")
    lines.append(f"    {args.helper_name}(__VA_ARGS__, \\")

    for gi, grp in enumerate(groups):
        is_last = (gi == len(groups) - 1)
        joined = ", ".join(grp)
        if is_last:
            lines.append(f"        {joined})")
        else:
            lines.append(f"        {joined}, \\")

    Path(args.out).write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Wrote {args.out}")
    print(f"  max={args.max}, mode={'MSVC' if args.mvc else 'standard'}")
    print(f"  placeholders={placeholder_count} (_{start_index} through _{args.max})")
    print(f"  trailing_ones={num_ones}")


if __name__ == "__main__":
    main()