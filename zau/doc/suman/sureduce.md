# sureduce

SUREDUCE - convert traces to display in reduced time sureduce <stdin >stdout rv=

## Synopsis

```bash
sureduce <stdin >stdout rv=
```

## Required Parameters

dt=tr.dt	if not set in header, dt is mandatory

## Optional Parameters

rv=8.0		reducing velocity in km/sec
Note: Useful for plotting refraction seismic data.
To remove reduction, do:
suflip < reduceddata.su flip=3 | sureduce rv=RV > flip.su
suflip < flip.su flip=3 > unreduceddata.su
Trace header fields accessed: dt, ns, offset
Trace header fields modified: none

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
