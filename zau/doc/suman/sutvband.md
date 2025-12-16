# sutvband

SUTVBAND - time-variant bandpass filter (sine-squared taper) sutvband <stdin >stdout tf= f=

## Synopsis

```bash
sutvband <stdin >stdout tf= f=
```

## Required Parameters

dt = (from header)      time sampling interval (sec)
tf=             times for which f-vector is specified
f=f1,f2,f3,f4   Corner frequencies corresponding to the
times in tf. Specify as many f= as
there are entries in tf.
The filters are applied in frequency domain.

## Examples

```
sutvband <data tf=.2,1.5 f=10,12.5,40,50 f=10,12.5,30,40 | ...
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
