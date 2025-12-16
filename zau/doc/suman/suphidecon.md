# suphidecon

SUPHIDECON - PHase Inversion Deconvolution suphidecon < stdin > stdout

## Synopsis

```bash
suphidecon < stdin > stdout
```

## Required Parameters

none

## Optional Parameters

... time range used for wavelet extraction:
tm=-0.1	Pre zero time (maximum phase component )
tp=+0.4	Post zero time (minimum phase component + multiples)
pad=.1	percentage padding for nt prior to cepstrum calculation
pnoise=0.001	Pre-withening (assumed noise to prevent division by zero)

## Notes

The wavelet is separated from the reflectivity and noise based on
their different 'smoothness' in the pseudo cepstrum domain.
The extracted wavelet also includes multiples.
The wavelet is reconstructed in frequency domain, end removed
from the trace. (Method by Lichman and Northwood, 1996.)

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
