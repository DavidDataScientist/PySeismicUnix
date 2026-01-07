# sulprime

SULPRIME - find appropriate Backus average length for a given log suite, frequency, and purpose sulprime < vp_vs_rho.su  [options]		or

## Synopsis

```bash
sulprime < vp_vs_rho.su  [options]		or
```

## Required Parameters

none
Optional parameter:
b=2.0		target value of Backus number
b=2 is transmission limit (ok for proc, mig, etc.)
b=0.3 is scattering limit (ok for modeling)
dz=1		input depth sample interval (ft)
f=60		frequency (Hz)... dominant or max (to be safe)
nmax=301	maximum averaging length (samples)
verbose=1	print intermediate results
=0 print final result only

## Notes

1. input is in sync with subackus, but vp and gr not used
(gr= gamma ray log)
Related codes:  subackus, subackush

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
