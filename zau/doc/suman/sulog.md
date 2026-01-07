# sulog

SULOG -- time axis log-stretch of seismic traces

## Synopsis

```bash
sulog [optional parameters] <stdin >stdout
```

## Required Parameters

none

## Optional Parameters

ntmin= .1*nt		minimum time sample of interest
outpar=/dev/tty		output parameter file, contains:
number of samples (nt=)
minimum time sample (ntmin=)
output number of samples (ntau=)
m=3			length of stretched data
is set according to
ntau = nextpow(m*nt)
ntau= pow of 2		override for length of stretched
data (useful for padding zeros
to avoid aliasing)

## Notes

ntmin is required to avoid taking log of zero and to
keep number of outsamples (ntau) from becoming enormous.
Data above ntmin is zeroed out.
The output parameters will be needed by suilog to
reconstruct the original data.
EXAMPLE PROCESSING SEQUENCE:
sulog outpar=logpar <data1 >data2
suilog par=logpar <data2 >data3

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
