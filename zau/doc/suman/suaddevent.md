# suaddevent

SUADDEVENT - add a linear or hyperbolic moveout event to seismic data

## Synopsis

```bash
suaddevent <stdin >stdout [optional parameters]
```

## Required Parameters

none

## Optional Parameters

type=nmo    =lmo for linear event
t0=1.0      zero-offset intercept time IN SECONDS
vel=3000.   moveout velocity in m/s
amp=1.      amplitude
dt=	 must provide if 0 in headers (seconds)
Typical usage:
sunull nt=500 dt=0.004 ntr=100 | sushw key=offset a=-1000 b=20 \
| suaddevent v=1000 t0=0.05 type=lmo | suaddevent v=1800 t0=0.8 \
| sufilter f=8,12,75,90 | suxwigb clip=1 &

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
