# sumean

SUMEAN - get the mean values of data traces

## Synopsis

```bash
sumean < stdin > stdout [optional parameters]
```

## Required Parameters

power = 2.0		mean to the power
(e.g. = 1.0 mean amplitude, = 2.0 mean energy)

## Optional Parameters

verbose = 0		writes mean value of section to outpar
= 1 writes mean value of each trace / section to
outpar
outpar=/dev/tty   output parameter file
abs = 1             average absolute value
= 0 preserve sign if power=1.0

## Notes

Each sample is raised to the requested power, and the sum of all those
values is averaged for each trace (verbose=1) and the section.
The values power=1.0 and power=2.0 are physical, however other powers
represent other mathematical L-p norms and may be of use, as well.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
