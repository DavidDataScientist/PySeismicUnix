# suflip

SUFLIP - flip a data set in various ways suflip <data1 >data2 flip=1 verbose=0

## Synopsis

```bash
suflip <data1 >data2 flip=1 verbose=0
```

## Required Parameters

none

## Optional Parameters

flip=1 	rotational sense of flip
+1  = flip 90 deg clockwise
-1  = flip 90 deg counter-clockwise
0  = transpose data
2  = flip right-to-left
3  = flip top-to-bottom
tmpdir=	 if non-empty, use the value as a directory path
prefix for storing temporary files; else if
the CWP_TMPDIR environment variable is set use
its value for the path; else use tmpfile()
verbose=0	verbose = 1 echoes flip info
NOTE:  tr.dt header field is lost if flip=-1,+1.  It can be
reset using sushw.
EXAMPLE PROCESSING SEQUENCES:
1.	suflip flip=-1 <data1 | sushw key=dt a=4000 >data2
2.	suflip flip=2 <data1 | suflip flip=2 >data1_again
3.	suflip tmpdir=/scratch <data1 | ...
Caveat:  may fail on large files.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
