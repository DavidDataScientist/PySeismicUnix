# suilog

SUILOG -- time axis inverse log-stretch of seismic traces suilog nt= ntmin=  <stdin >stdout

## Synopsis

```bash
suilog nt= ntmin=  <stdin >stdout
```

## Required Parameters

nt= 	nt output from sulog prog
ntmin= 	ntmin output from sulog prog
dt= 	dt output from sulog prog

## Optional Parameters

none
NOTE:  Parameters needed by suilog to reconstruct the
original data may be input via a parameter file.
EXAMPLE PROCESSING SEQUENCE:
sulog outpar=logpar <data1 >data2
suilog par=logpar <data2 >data3
where logpar is the parameter file

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
