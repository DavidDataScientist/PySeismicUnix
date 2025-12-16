# surefcon

SUREFCON -  Convolution of user-supplied Forward and Reverse refraction shots using XY trace offset in reverse shot surefcon <forshot sufile=revshot  xy=trace offseted  >stdout

## Synopsis

```bash
surefcon <forshot sufile=revshot  xy=trace offseted  >stdout
```

## Required Parameters

sufile=	file containing SU trace to use as reverse shot
xy=		Number of traces offseted from the 1st trace in sufile

## Optional Parameters

none
Trace header fields accessed: ns
Trace header fields modified: ns

## Examples

```
surefcon <DATA sufile=DATA xy=1 | ...
Here, the su data file, "DATA", convolved the nth trace by
(n+xy)th trace in the same file
```

## Notes

This code implements the Refraction Convolution Section (RCS)	method
of generalized reciprocal refraction traveltime analysis developed by
Derecke Palmer and Leoni Jones.
The time sampling interval on the output traces is half of that on the
traces in the input files.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
