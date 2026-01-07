# sunmo

SUNMO - NMO for an arbitrary velocity function of time and CDP

## Synopsis

```bash
sunmo <stdin >stdout [optional parameters]
```

## Optional Parameters

tnmo=0,...		NMO times corresponding to velocities in vnmo
vnmo=1500,...		NMO velocities corresponding to times in tnmo
cdp=			CDPs for which vnmo & tnmo are specified (see Notes)
smute=1.5		samples with NMO stretch exceeding smute are zeroed
lmute=25		length (in samples) of linear ramp for stretch mute
sscale=1		=1 to divide output samples by NMO stretch factor
invert=0		=1 to perform (approximate) inverse NMO
upward=0		=1 to scan upward to find first sample to kill
voutfile=		if set, interplolated velocity function v[cdp][t] is
output to named file.

## Notes

For constant-velocity NMO, specify only one vnmo=constant and omit tnmo.
NMO interpolation error is less than 1% for frequencies less than 60% of
the Nyquist frequency.
Exact inverse NMO is impossible, particularly for early times at large
offsets and for frequencies near Nyquist with large interpolation errors.
The "offset" header field must be set.
Use suazimuth to set offset header field when sx,sy,gx,gy are all
nonzero.
For NMO with a velocity function of time only, specify the arrays
vnmo=v1,v2,... tnmo=t1,t2,...
where v1 is the velocity at time t1, v2 is the velocity at time t2, ...
The times specified in the tnmo array must be monotonically increasing.
Linear interpolation and constant extrapolation of the specified velocities
is used to compute the velocities at times not specified.
The same holds for the anisotropy coefficients as a function of time only.
For NMO with a velocity function of time and CDP, specify the array
cdp=cdp1,cdp2,...
and, for each CDP specified, specify the vnmo and tnmo arrays as described
above. The first (vnmo,tnmo) pair corresponds to the first cdp, and so on.
Linear interpolation and constant extrapolation of 1/velocity^2 is used
to compute velocities at CDPs not specified.
The format of the output interpolated velocity file is unformatted C floats
with vout[cdp][t], with time as the fast dimension and may be used as an
input velocity file for further processing.
Note that this version of sunmo does not attempt to deal with	anisotropy.
The version of sunmo with experimental anisotropy support is "sunmo_a"

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
