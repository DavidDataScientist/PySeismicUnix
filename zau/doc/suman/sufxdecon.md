# sufxdecon

SUFXDECON - random noise attenuation by FX-DECONvolution sufxdecon <stdin >stdout [...]

## Synopsis

```bash
sufxdecon <stdin >stdout [...]
```

## Optional Parameters

taper=.1	length of taper
fmin=6.       minimum frequency to process in Hz  (accord to twlen)
fmax=.6/(2*dt)  maximum frequency to process in Hz
twlen=entire trace  time window length (minimum .3 for lower freqs)
ntrw=10       number of traces in window
ntrf=4        number of traces for filter (smaller than ntrw)
verbose=0	=1 for diagnostic print
tmpdir=	if non-empty, use the value as a directory path	prefix
for storing temporary files; else, if the CWP_TMPDIR
environment variable is set, use its value for the path;
else use tmpfile()
Notes: Each trace is transformed to the frequency domain.
For each frequency, Wiener filtering, with unity prediction in
space, is used to predict the next sample.
At the end of the process, data is mapped back to t-x domain.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
