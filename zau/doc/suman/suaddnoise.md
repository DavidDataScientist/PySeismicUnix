# suaddnoise

SUADDNOISE - add noise to traces suaddnoise <stdin >stdout  sn=20  noise=gauss  seed=from_clock

## Synopsis

```bash
suaddnoise <stdin >stdout  sn=20  noise=gauss  seed=from_clock
```

## Required Parameters

if any of f=f1,f2,... and amp=a1,a2,... are specified by the user
and if dt is not set in header, then dt is mandatory

## Optional Parameters

sn=20			signal to noise ratio
noise=gauss		noise probability distribution
=flat for uniform; default Gaussian
seed=from_clock		random number seed (integer)
f=f1,f2,...		array of filter frequencies (as in sufilter)
amps=a1,a2,...		array of filter amplitudes
dt= (from header)	time sampling interval (sec)
verbose=0		=1 for echoing useful information
tmpdir=	 if non-empty, use the value as a directory path
prefix for storing temporary files; else if the
the CWP_TMPDIR environment variable is set use
its value for the path; else use tmpfile()

## Examples

```
low freqency:    suaddnoise < data f=40,50 amps=1,0 | ...
high freqency:   suaddnoise < data f=40,50 amps=0,1 | ...
near monochromatic: suaddnoise < data f=30,40,50 amps=0,1,0 | ...
with a notch:    suaddnoise < data f=30,40,50 amps=1,0,1 | ...
bandlimited:     suaddnoise < data f=20,30,40,50 amps=0,1,1,0 | ...
```

## Notes

Output = Signal +  scale * Noise
scale = (1/sn) * (absmax_signal/sqrt(2))/sqrt(energy_per_sample)
If the signal is already band-limited, f=f1,f2,... and amps=a1,a2,...
can be used, as in sufilter, to bandlimit the noise traces to match
the signal band prior to computing the scale defined above.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
