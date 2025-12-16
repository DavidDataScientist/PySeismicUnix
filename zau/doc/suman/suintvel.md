# suintvel

SUINTVEL - convert stacking velocity model to interval velocity model suintvel vs= t0= outpar=/dev/tty

## Synopsis

```bash
suintvel operate on seismic data.  Hence stdin and stdout are not used.
```

## Required Parameters

vs=	stacking velocities
t0=	normal incidence times

## Optional Parameters

mode=0			output h= v= ; =1 output v=  t=
outpar=/dev/tty		output parameter file in the form:
h=layer thicknesses vector
v=interval velocities vector
....or ...
t=vector of times from t0
v=interval velocities vector

## Examples

```
Note: suintvel does not have standard su syntax since it does not
operate on seismic data.  Hence stdin and stdout are not used.
Note: may go away in favor of par program, velconv, by Dave
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
