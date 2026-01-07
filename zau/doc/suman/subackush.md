# subackush



## Synopsis

```bash
subackush < vp_vs_rho.su >stdout [options]
```

## Required Parameters

none
Optional parameter:
navg=101	number of depth samples in Backus avg window
Intrinsic anisotropy of shale layers can be included ...
gr=0		no gamma ray log input for shale
=1 input is vp_vs_rho_gr
grs=100	pure shale gamma ray value (API units)
grc=10	0% shale gamma ray value (API units)
smode=1	include shale anis params prop to shale volume
=0 include shale anis for pure shale only
se=0.209	shale epsilon (Thomsen parameter)
sd=0.033	shale delta (Thomsen parameter)
sg=0.203	shale gamma (Thomsen parameter)

## Examples

```
las2su < logs.las nskip=34 nlog=4 > logs.su
suwind < logs.su  key=tracl min=2 max=3 | suop op=s2vm > v.su
suwind < logs.su  key=tracl min=4 max=4 | suop op=d2m > d.su
fcat v.su d.su > vp_vs_rho.su
subackus < vp_vs_rho.su > vp0_vs0_rho_eps_delta_gamma.su
In this example we start with a well las file containing
34 header lines and 4 log tracks (depth,p_son,s_son,den).
This is converted to su format by las2su.  Then we pull off
the sonic logs and convert them to velocity in metric units.
Then the density log is pulled off and converted to metric.
All three metric curves are bundled into one su file which
is the input to subackus.
Related programs: subackus, sulprime
```

## Notes

1. Input are (vp,vs,rho) traces in metric units
2. Output are
tracl	=(1,2,3,4,5,6)
quantity	=(vp0,vs0,<rho>,epsilon,delta,gamma)
units	=(m/s,m/s,kg/m^3,nd,nd,nd) nd=dimensionless
tracl	=(7,8)
quantity	=(Vsh,shaleEps) Vsh=shale volume fraction
units	=(nd,nd)
3. (epsilon,delta,etc.) can be isolated by tracl header field
4. (vp0,vs0) are backus averaged vertical wavespeeds
5. <rho> is backus averaged density, etc.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
