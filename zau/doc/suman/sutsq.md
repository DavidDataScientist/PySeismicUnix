# sutsq

SUTSQ -- time axis time-squared stretch of seismic traces

## Synopsis

```bash
sutsq [optional parameters] <stdin >stdout
```

## Required Parameters

none

## Optional Parameters

tmin= .1*nt*dt  minimum time sample of interest
(only needed for forward transform)
dt= .004       output sample rate
(only needed for inverse transform)
flag= 1        1=forward transform: time to time squared
-1=inverse transform: time squared to time
Note: The output of the forward transform always starts with
time squared equal to zero.  'tmin' is used to avoid aliasing
the early times.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
