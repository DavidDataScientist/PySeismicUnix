# sumix

SUMIX - compute weighted moving average (trace MIX) on a panel of seismic data sumix <stdin >sdout

## Synopsis

```bash
sumix <stdin >sdout
```

## Examples

```
sumix <stdin mix=.6,1,1,1,.6 >sdout 	(default) mix over 5 traces weights
sumix <stdin mix=1,1,1 >sdout 	simple 3 trace moving average
```

## Notes

The number of values defined by mix=val1,val2,... determines the number
of traces to be averaged, the values determine the weights.
Mix applied left-to-right (in 2D data) and only begins after the
nmix traces, where nmix is the number of mix weights to avoid
left end edge effects.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
