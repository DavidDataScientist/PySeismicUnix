# sugabor

SUGABOR -  Outputs a time-frequency representation of seismic data via the Gabor transform-like multifilter analysis technique presented by Dziewonski, Bloch and  Landisman, 1969.

## Synopsis

```bash
sugabor <stdin >stdout [optional parameters]
```

## Required Parameters

if dt is not set in header, then dt is mandatory

## Optional Parameters

dt=(from header)	time sampling interval (sec)
fmin=0			minimum frequency of filter array (hz)
fmax=NYQUIST 		maximum frequency of filter array (hz)
beta=3.0		ln[filter peak amp/filter endpoint amp]
band=.05*NYQUIST	filter bandwidth (hz)
alpha=beta/band^2	filter width parameter
verbose=0		=1 supply additional info
holder=0		=1 output Holder regularity estimate
=2 output linear regularity estimate
Notes: This program produces a muiltifilter (as opposed to moving window)
representation of the instantaneous amplitude of seismic data in the
time-frequency domain. (With Gaussian filters, moving window and multi-
filter analysis can be shown to be equivalent.)
An input trace is passed through a collection of Gaussian filters
to produce a collection of traces, each representing a discrete frequency
range in the input data. For each of these narrow bandwidth traces, a
quadrature trace is computed via the Hilbert transform. Treating the narrow
bandwidth trace and its quadrature trace as the real and imaginary parts
of a "complex" trace permits the "instantaneous" amplitude of each
narrow bandwidth trace to be compute. The output is thus a representation
of instantaneous amplitude as a function of time and frequency.
Some experimentation with the "band" parameter may necessary to produce
the desired time-frequency resolution. A good rule of thumb is to run
sugabor with the default value for band and view the image. If band is
too big, then the t-f plot will consist of stripes parallel to the frequency
axis. Conversely, if band is too small, then the stripes will be parallel
to the time axis.
Caveat:
The Gabor transform is not a wavelet transform, but rather are sharp
frame basis. However, it is nearly a Morlet continuous wavelet transform
so the concept of Holder regularity may have some meaning. If you are
computing Holder regularity of, say, a migrated seismic section, then
set band to 1/3 of the frequency band of your data.

## Examples

```
suvibro | sugabor | suximage
suvibro | sugabor | suxmovie n1= n2= n3=
(because suxmovie scales it's amplitudes off of the first panel,
may have to experiment with the wclip and bclip parameters
suvibro | sugabor | supsimage | ... ( your local PostScript utility)
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
