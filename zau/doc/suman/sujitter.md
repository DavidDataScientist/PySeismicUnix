# sujitter

SUJITTER - Add random time shifts to seismic traces

## Synopsis

```bash
sujitter <stdin >stdout  [optional parameters]
```

## Required Parameters

none

## Optional Parameters

seed=from_clock    	random number seed (integer)
min=1 			minimum random time shift (samples)
max=1 			maximum random time shift (samples)
pon=1 			shift can be positive or negative
=0 shift is positive only
fldr=0 			each trace has new shift
=1 new shift when fldr header field changes

## Notes

Useful for simulating random statics. See also:  suaddstatics

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
