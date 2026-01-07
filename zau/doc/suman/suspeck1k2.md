# suspeck1k2

SUSPECK1K2 - 2D (K1,K2) Fourier SPECtrum of (x1,x2) data set

## Synopsis

```bash
suspeck1k2 <infile >outfile [optional parameters]
```

## Optional Parameters

d1=from header(d1) or 1.0	spatial sampling interval in first (fast)
dimension
d2=from header(d2) or 1.0	spatial sampling interval in second
(slow)  dimension
verbose=0		verbose = 1 echoes information
tmpdir= 	 	if non-empty, use the value as a directory path
prefix for storing temporary files; else if the
the CWP_TMPDIR environment variable is set use
its value for the path; else use tmpfile()

## Notes

Because the data are assumed to be purely spatial (i.e. non-seismic),
the data are assumed to have trace id (30), corresponding to (z,x) data
To facilitate further processing, the sampling intervals in wavenumber
as well as the first frequency (0) and the first wavenumber are set in
the output header (as respectively d1, d2, f1, f2).
The relation: w = 2 pi F is well known for frequency, but there
doesn't seem to be a commonly used letter corresponding to F for the
spatial conjugate transform variables.  We use K1 and K2 for this.
More specifically we assume a phase:
-i(k1 x1 + k2 x2) = -2 pi i(K1 x1 + K2 x2).
and K1, K2 define our respective wavenumbers.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
