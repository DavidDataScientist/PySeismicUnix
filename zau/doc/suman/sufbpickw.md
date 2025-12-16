# sufbpickw

SUFBPICKW - First break auto picker sufbpickw < infile >outfile

## Synopsis

```bash
sufbpickw < infile >outfile
```

## Required Parameters

none

## Optional Parameters

keyg=ep
window=.03	Length of forward and backward windows (s)
test=1	Output the characteristic function
This can be used for testing window size
Template
o=		offset...
t=		time pairs for defining first break search
window centre
tdv=.05	Half length of the search window
If the template is specified the maximum value of the
characteristic function is searched in the window
defined by the template only.Default is the whole trace.
The time of the pick is stored in header word unscale

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
