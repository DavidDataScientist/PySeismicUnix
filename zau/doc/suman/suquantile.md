# suquantile

SUQUANTILE - display some quantiles or ranks of a data set

## Synopsis

```bash
suquantile <stdin >stdout [optional parameters]
```

## Required Parameters

none (no-op)

## Optional Parameters

panel=1		flag; 0 = do trace by trace (vs. whole data set)
quantiles=1	flag; 0 = give ranks instead of quantiles
verbose=0	verbose = 1 echoes information
tmpdir= 	 if non-empty, use the value as a directory path
prefix for storing temporary files; else if the
the CWP_TMPDIR environment variable is set use
its value for the path; else use tmpfile()

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
