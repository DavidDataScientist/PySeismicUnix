# suinterp

SUINTERP - interpolate traces using automatic event picking suinterp < stdin > stdout ninterp=1    number of traces to output between each pair of input traces

## Synopsis

```bash
suinterp < stdin > stdout
```

## Notes

This program outputs 'ninterp' interpolated traces between each pair of
input traces.  The values for lagc, freq1, and freq2 are only used for
event tracking. The output data will be full bandwidth with no agc.  The
default parameters typically will do a satisfactory job of interpolation
for dips up to about 12 ms/trace.  Using a larger value for freq2 causes
the algorithm to do a better job on the shallow dips, but to fail on the
steep dips.  Only one dip is assumed at each time sample between each pair
of input traces.
The key assumption used here is that the low frequency data are unaliased
and can be used for event tracking. Those dip picks are used to interpolate
the original full-bandwidth data, giving some measure of interpolation
at higher frequencies which otherwise would be aliased.  Using iopt equal
to 1 allows you to visually check whether the low-pass picking model is
aliased.
Trace headers for interpolated traces are not updated correctly.
The output header for an interpolated traces equals that for the preceding
trace in the original input data.  The original input traces are passed
through this module without modification.
The place this code is most likely to fail is on the first breaks.
Example run:    suplane | suinterp | suxwigb &

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
