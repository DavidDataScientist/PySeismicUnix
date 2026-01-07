# sufdmod1

SUFDMOD1 - Finite difference modelling (1-D 1rst order) for the acoustic wave equation

## Synopsis

```bash
sufdmod1 <vfile >sfile nz= tmax= sz= [optional parameters]
```

## Required Parameters

<vfile or vfile=	binary file containing velocities[nz]
>sfile or sfile=	SU file containing seimogram[nt]
nz=		 number of z samples
tmax=		maximum propagation time
sz=		 z coordinate of source

## Optional Parameters

dz=1	   z sampling interval
fz=0.0	 first depth sample
rz=1	   coordinate of receiver
sz=1	   coordinate of source
dfile=	 binary input file containing density[nz]
wfile=	 output file for wave field (snapshots in a SU trace panel)
abs=0,1	absorbing conditions on top and bottom
styp=0	 source type (0: gauss, 1: ricker 1, 2: ricker 2)
freq=15.0	approximate source center frequency (Hz)
nt=1+tmax/dt   number od time samples (dt determined for numerical
stability)
zt=1	   trace undersampling factor for trace and snapshots
zd=1	   depth undersampling factor for snapshots
press=1	to record the pressure field; 0 records the particle
velocity
verbose=0	=1 for diagnostic messages

## Notes

This program uses a first order explicit velocity/pressure  finite
difference equation.
The source function is applied on the pressure component.
If no density file is given, constant density is assumed
Wavefield  can be easily viewed with suximage, user must provide f2=0
to the ximage program in order to  get correct time labelling
Seismic trace is shifted in order to get a zero phase source
Source begins and stop when it's amplitude is 10^-4 its maximum
Time and depth undersampling only modify the output trace and snapshots.
These parameters are useful for keeping snapshot file small and
the number of samples under SU_NFLTS.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
