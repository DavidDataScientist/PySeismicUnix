# suaddstatics

SUADDSTATICS - ADD random STATICS on seismic data

## Synopsis

```bash
suaddstatics required parameters [optional parameters] > stdout
```

## Required Parameters

shift=		the static shift will be generated
randomly in the interval [+shift,-shif] (ms)
sources=		number of source locations
receivers=		number of receiver locations
cmps=			number of common mid point locations
maxfold=		maximum fold of input data
datafile=		name and COMPLETE path of the input file

## Optional Parameters

dt=tr.dt			time sampling interval (ms)
seed=getpid()		 seed for random number generator
verbose=0			=1 print useful information

## Notes

Input data should be sorted into cdp gathers.
SUADDSTATICS applies static time shifts in a surface consistent way on
seismic data sets. SUADDSTATICS writes the static time shifts in the
header field TSTAT. To perform the actual shifts the user should use
the program SUSTATIC after SUADDSTATICS. SUADDSTATICS outputs the
corrupted data set to stdout.
Header field used by SUADDSTATICS: cdp, sx, gx, tstat, dt.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
