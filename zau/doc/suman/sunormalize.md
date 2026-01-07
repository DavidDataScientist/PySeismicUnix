# sunormalize

SUNORMALIZE - Trace NORMALIZation by rms, max, or median or median balancing sunormalize <stdin >stdout t0=0 t1=TMAX norm=rms

## Synopsis

```bash
sunormalize <stdin >stdout t0=0 t1=TMAX norm=rms
```

## Required Parameters

dt=tr.dt    if not set in header, dt is mandatory
ns=tr.ns    if not set in header, ns is mandatory

## Optional Parameters

norm=rms    type of norm rms, max, med , balmed
t0=0.0      startimg time for window
t1=TMAX     ending time for window

## Notes

Traces are divided by either the root mean squared amplitude,
trace maximum, or the median value. The option "balmed" is
median balancing which is a shift of the amplitudes by the
median value of the amplitudes.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
