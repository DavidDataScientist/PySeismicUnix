# sukdmdcr

SUKDMDCR - 2.5D datuming of receivers for prestack, common source data using constant-background data mapping formula. (See selfdoc for specific survey requirements.)

## Synopsis

```bash
sukdmdcr infile=stdin		file for input seismic traces
```

## Optional Parameters

aperx=nxt*dxt/2  	lateral half-aperture
v0=1500(m/s)		reference wavespeed
freq=50               dominant frequency in data, used to determine
the minimum distance below the datum that
the stationary phase calculation is valid.
scale=1.0             user defined scale factor for output
jpfile=stderr		job print file name
mtr=100  		print verbal information at every mtr traces
ntr=100000		maximum number of input traces to be datumed

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
