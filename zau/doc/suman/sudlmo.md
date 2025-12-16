# sudlmo

SUDLMO -- Dynamic Linear Move Out Correction for Surface Waves

## Synopsis

```bash
sudlmo <indata >outdata [optional parameters]
```

## Optional Parameters

vnmo=400	Velocity corresponding to  fnmo
fnmo=0.0	Frequency corresponding to  vnmo
dt=(from header)	time sample interval (in seconds)
cm = 0 			for offset in m   m
1 for offset in cm
invert = 0             1 to perform invers DLMO
Algorithm:
Gdlmo(t,x) = Re[INVFTT{ ( (sign) iw*off/Cf*(Gdlmo(f,x)}]

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
