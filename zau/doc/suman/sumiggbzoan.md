# sumiggbzoan

SUMIGGBZOAN - MIGration via Gaussian beams ANisotropic media (P-wave)

## Synopsis

```bash
sumiggbzoan <infile >outfile vfile= nt= nx= nz= [optional parameters]
```

## Required Parameters

a3333file=		name of file containing a3333(x,z)
nx=                    number of inline samples (traces)
nz=                    number of depth samples

## Optional Parameters

dt=tr.dt               time sampling interval
dx=tr.d2               inline sampling interval (trace spacing)
dz=1.0                 depth sampling interval
fmin=0.025/dt          minimum frequency
fmax=10*fmin           maximum frequency
amin=-amax             minimum emergence angle; must be > -90 degrees
amax=60                maximum emergence angle; must be < 90 degrees
bwh=0.5*vavg/fmin      beam half-width; vavg denotes average velocity
verbose=0		 silent, =1 chatty
Files for general anisotropic parameters confined to a vertical plane:
a1111file=		name of file containing a1111(x,z)
a1133file=          	name of file containing a1133(x,z)
a1313file=          	name of file containing a1313(x,z)
a1113file=          	name of file containing a1113(x,z)
a3313file=          	name of file containing a3313(x,z)
For transversely isotropic media Thomsen's parameters could be used:
deltafile=		name of file containing delta(x,z)
epsilonfile=		name of file containing epsilon(x,z)
a1313file=          	name of file containing a1313(x,z)
if anisotropy parameters are not given the program considers
the medium to be isotropic.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
