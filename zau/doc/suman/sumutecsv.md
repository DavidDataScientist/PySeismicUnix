# sumutecsv

SUMUTECSV - MUTE above (or below) bilinearly interpolated polygonal curves

## Synopsis

```bash
sumutecsv <stdin >stdout [required parameters] [optional parameters]
```

## Optional Parameters

qin=             Mute functions can be input via this file.
This file is optional, but if you do not input it,
you must use parameters cdp=, offs=, tims=.
See external document Q_FILE_STANDARDS.
The following 3 parameters cannot be specified if
you input mute functions via the qin= file.
cdp=             CDPs for which offs & tims are specified.
offs=            offsets corresponding to times in tims.
tims=            times corresponding to offsets in offs.
If any tims value is greater than 100 all times are
assumed to be milliseconds. Otherwise seconds.
***   If qin= is not specified, all 3 previous parameters
must be specified. There must be at least 1 number
in the cdp= list. There must be the same number of
tims= parameters as numbers in the cdp= list.
There must be the same number of offs= parameters
as numbers in the cdp= list, or, there can be just
one offs= parameter provided there are the same
number of tims in all tims= lists.
rfile=           If set, read a K-file containing 3D grid definition.
Assume 2D if no K-file and no grid definition is
supplied via command line parameters. The required
command line parameters are: grid_xa,grid_ya,
grid_xb,grid_yb, grid_xc,grid_yc, grid_wb, grid_wc.
(See program SUBINCSV for 3D grid details).
If this is a 3D then the input CDPs for the mute
locations and the seismic trace CDP key should be
3D grid cell numbers as produced by SUBINCSV.
A 3D also forces restrictions on the locations of
the input mute locations. Their CDP locations must

## Notes

The offsets specified in the offs array must be monotonically increasing.
For 3D, user needs to input mute locations (cdp numbers) which form aligned
rectangles. That is, how-ever-many grid inlines the user chooses to put
mute locations on, there must always be the same number of mute locations
on each inline and those functions must be located at same grid crosslines.
For instance, if user inputs locations for inline 7 at crosslines 15,25,40
then the user must input locations at crosslines 15,25,40 for any other
inlines that the user wants to supply locations for. (If user is lucky
enough that the grid has 100 inlines, then the input locations could be at
CDPs 715,725,740 and 1115,1125,1140 and 2015,2025,2040 and 2515,2525,2540.
Note that the CDP numbers specified on the cdp= parameter and also in the
input seismic trace cdp key are translated to inline and crossline numbers
using the input 3D grid definition - so those cdp numbers need to
correspond to the input 3D grid definition.
For trace CDPs not listed in cdp= parameter or qin= file, bilinear
interpolation is done if the trace CDP location is surrounded by 4 mute
functions specified in the cdp= list. If the trace CDP is not surrounded
by 4 input mute functions, the result depends on the extrapi and extrapc
options. The result can be any combination of linear interpolation and
linear extrapolation and constant extrapolation. If input mute functions
are only located along 1 grid inline or 1 grid crossline, result is linear
interpolation in that direction (and linear or constant extrapolation
the outer ending functions).
The interpolation related to cdp is actually done after the interpolation
related to offset. That is, first the trace offset is used to compute times
from the offs,tims arrays for 4, 2, or 1 mute functions and then weighting
based on the cdp locations of those mute functions is applied. Note also
that restricting the mute to the earliest and latest trace time is done
just before application to each trace. A consequence of this is that both
negative offs= and negative tims= values are allowed and honored even if
the abs= option is 1 (the default).

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
