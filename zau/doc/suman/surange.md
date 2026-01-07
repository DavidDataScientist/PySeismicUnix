# surange

SURANGE - get max and min values for non-zero header entries surange <stdin

## Synopsis

```bash
surange <stdin
```

## Optional Parameters

key=		Header key(s) to range (default=all)
dim=0		dim seismic flag
0 = not dim, 1 = coord in ft, 2 = coord in m

## Notes

Output is:
number of traces
keyword min max (first - last)
north-south-east-west limits of shot, receiver and midpoint
if dim then also midpoint interval and line length

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
