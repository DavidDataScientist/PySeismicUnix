# sugroll

SUGROLL - Ground roll supression using Karhunen-Loeve transform

## Synopsis

```bash
sugroll <infile >outfile  [optional parameters]
```

## Optional Parameters

dt=tr.dt (from header) 	time sampling interval (secs)
nx=ntr   (counted from data)	number of horizontal samples (traces)
sb=0      1 for the graund-roll
verbose=0	verbose = 1 echoes information
nrot=3        the principal components for the m largest eigenvalues
tmpdir= 	 if non-empty, use the value as a directory path
prefix for storing temporary files; else if the
the CWP_TMPDIR environment variable is set use
its value for the path; else use tmpfile()

## Notes

The method developed here is to extract the ground-roll from the
common-shot gathers using Karhunen-Loeve transform, and then to substract
it from the original data. The advantage is the ground-roll is suppresed
with negligible distortion of the signal.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
