# sucvs4fowler

SUCVS4FOWLER --compute constant velocity stacks for input to Fowler codes

## Synopsis

```bash
sucvs4fowler 
```

## Optional Parameters

vminstack=1500.	minimum velocity panel in m/s to output
nvstack=180		number of stacking velocity panels to compute
( Let offmax be the maximum offset, fmax be
the maximum freq to preserve, and tmute be
the starting mute time in sec on offmax, then
the recommended value for nvstack would be
nvstack = 4 +(offmax*offmax*fmax)/(0.6*vmin*vmin*tmute)
---you may want to make do with less---)
lmute=24		length of mute taper in ms
nonhyp=1		1 if do mute at 2*offset/vhyp to avoid
non-hyperbolic moveout, 0 otherwise
vhyp=2500.		velocity to use for non-hyperbolic moveout mute
lbtaper=0		length of bottom taper in ms
lstaper=0		length of side taper in traces
dtout=1.5*dt		output sample rate in s,

## Notes

mxfold=120		maximum number of offsets/input cmp
salias=0.8		fraction of output frequencies to force within sloth
antialias limit.  This controls muting by offset of
the input data prior to computing the cv stacks
for values of choose=1 or choose=2.
Required trace header words on input are ns, dt, cdp, offset.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
