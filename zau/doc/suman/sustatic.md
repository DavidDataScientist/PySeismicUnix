# sustatic

SUSTATIC - Elevation static corrections, apply corrections from headers or from a source and receiver statics file

## Synopsis

```bash
sustatic <stdin >stdout  [optional parameters]
```

## Required Parameters

none

## Optional Parameters

v0=v1 or user-defined	or from header, weathering velocity
v1=user-defined		or from header, subweathering velocity
hdrs=0			=1 to read statics from headers
=2 to read statics from files
=3 to read from output files of suresstat
sign=1			apply static correction (add tstat values)
=-1 apply negative of tstat values
Options when hdrs=2 and hdrs=3:
sou_file=		input file for source statics (ms)
rec_file=		input file for receiver statics (ms)
ns=240 			number of souces
nr=335 			number of receivers
no=96 			number of offsets

## Examples

```
suchw < CR290.su key1=selev,gelev key2=selev,gelev key3=selev,gelev \
a=-25094431,-25094431 b=1,1 c=0,0 > CR290datum.su
```

## Notes

For hdrs=1, statics calculation is not performed, statics correction
is applied to the data by reading statics (in ms) from the header.
For hdrs=0, field statics are calculated, and
input field sut is assumed measured in ms.
output field sstat = 10^scalel*(sdel - selev + sdepth)/swevel
output field gstat = sstat - sut/1000.
output field tstat = sstat + gstat + 10^scalel*(selev - gelev)/wevel
For hdrs=2, statics are surface consistently obtained from the
statics files. The geometry should be regular.
The source- and receiver-statics files should be unformated C binary
floats and contain the statics (in ms) as a function of surface location.
For hdrs=3, statics are read from the output files of suresstat, with
the same options as hdrs=2 (but use no=max traces per shot and assume
that ns=max shot number and nr=max receiver number).
For each shot number (trace header fldr) and each receiver number
(trace header tracf) the program will look up the appropriate static
correction.  The geometry need not be regular as each trace is treated
independently.
Caveat:  The static shifts are computed with the assumption that the
desired datum is sea level (elevation=0). You may need to shift the
selev and gelev header values via  suchw.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
