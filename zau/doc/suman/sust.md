# sust

SUST -  Outputs a time-frequency representation of seismic data via the Stockwell transform (S- transform)

## Synopsis

```bash
sust <stdin >stdout [optional parameters]
```

## Required Parameters

if dt is not set in header, then dt is mandatory

## Optional Parameters

dt=(from header)	time sampling interval (sec)
fmin=0			minimum frequency of filter array (hz)
fmax=NYQUIST 		maximum frequency of filter array (hz)
verbose=0		=1 supply additional info
Notes: The S transform provide a time dependent frequency distribution
of the signal. It is similar to the Gabor transform which which utilizes a Gaussian window for for spectral location. In the S transform the
Gaussian window is scalable whith the frequency which provides a better
time freqency resolution

## Examples

```
suvibro | sust | suximage
suvibro | sust | suxmovie n1= n2= n3=
(because suxmovie scales it's amplitudes off of the first panel,
may have to experiment with the wclip and bclip parameters
suvibro | sust | supsimage | ... ( your local PostScript utility)
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
