# sucentsamp

SUCENTSAMP - CENTRoid SAMPle seismic traces

## Synopsis

```bash
sucentsamp <stdin [optional parameters] >sdout
```

## Required Parameters

none

## Optional Parameters

dt=from header		sampling interval
verbose=1		=0 to stop advisory messages

## Notes

This program takes seismic traces as input, and returns traces
consisting of spikes of height equal to the area of each lobe
of each oscillation, located at the centroid of the lobe in
question. The height of each spike equal to the area of the
corresponding lobe.
Caveat: No check is made that the data are real time traces!

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
