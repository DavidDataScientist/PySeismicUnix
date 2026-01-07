# suramp

SURAMP - Linearly taper the start and/or end of traces to zero.

## Synopsis

```bash
suramp <stdin >stdout [optional parameters]
```

## Required Parameters

if dt is not set in header, then dt is mandatory

## Optional Parameters

tmin=tr.delrt/1000	end of starting ramp (sec)
tmax=(nt-1)*dt		beginning of ending ramp (sec)
dt = (from header)	sampling interval (sec)
The taper is a linear ramp from 0 to tmin and/or tmax to the
end of the trace.  Default is a no-op!

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
