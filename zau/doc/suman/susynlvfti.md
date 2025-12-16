# susynlvfti

SUSYNLVFTI - SYNthetic seismograms for Linear Velocity function in a Factorized Transversely Isotropic medium

## Synopsis

```bash
susynlvfti >outfile [optional parameters]
```

## Optional Parameters

nt=101		number of time samples
dt=0.04		time sampling interval (sec)
ft=0.0		first time (sec)
kilounits=1            input length units are km or kilo-feet
=0 for m or ft

## Notes

Offsets are signed - may be positive or negative.  Receiver locations
are computed by adding the signed offset to the source location.
Specify either midpoint sampling or shotpoint sampling, but not both.
If neither is specified, the default is the midpoint sampling above.
More than one ref (reflector) may be specified.  When obliquity factors
are included, then only the left side of each reflector (as the x,z
reflector coordinates are traversed) is reflecting.  For example, if x
coordinates increase, then the top side of a reflector is reflecting.
Note that reflectors are encoded as quoted strings, with an optional
reflector amplitude: preceding the x,z coordinates of each reflector.
Default amplitude is 1.0 if amplitude: part of the string is omitted.
Concerning the choice of delta and epsilon. The difference between delta
and epsilon should not exceed one. A possible break down of the program
is the result. This is caused primarly by the break down in the two point
ray-tracing. Also keep the values of delta and epsilon between 2 and -2.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
