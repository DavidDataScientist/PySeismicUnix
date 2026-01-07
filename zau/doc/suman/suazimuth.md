# suazimuth

SUAZIMUTH - compute trace AZIMUTH, offset, and midpoint coordinates and set user-specified header fields to these values

## Synopsis

```bash
suazimuth <stdin >stdout [optional parameters]
```

## Required Parameters

none

## Optional Parameters

key=otrav      header field to store computed azimuths in
scale=1.0      value(key) = scale * azimuth
az=0           azimuth convention flag
0: 0-179.999 deg, reciprocity assumed
1: 0-359.999 deg, points from receiver to source
-1: 0-359.999 deg, points from source to receiver
sector=1.0     if set, defines output in sectors of size
sector=degrees_per_sector, the default mode is
the full range of angles specified by az
offset=0       if offset=1 then set offset header field
signedflag=0   is offset signed? if =1 signedflag=1
offkey=offset  header field to store computed offsets in
offkey=offset  header field to store computed offsets in
cmp=0          if cmp=1, then compute midpoint coordinates and
set header fields for (cmpx, cmpy)
mxkey=ep       header field to store computed cmpx in
mykey=cdp      header field to store computed cmpy in

## Notes

All values are computed from the values in the coordinate fields
sx,sy (source) and gx,gy (receiver).
The output field "otrav" for the azimuth was chosen arbitrarily as
an example of a little-used header field, however, the user may
choose any field that is convenient for his or her application.
Setting the sector=number_of_degrees_per_sector sets key field to
sector number rather than an angle in degrees.
For az=0, azimuths are measured from the North, however, reciprocity
is assumed, so azimuths go from 0 to 179.9999 degrees. If sector
option is set, then the range is from 0 to 180/sector.
For az=1, azimuths are measured from the North, with the assumption
that the direction vector points from the receiver to the source.
For az=-1, the direction vector points from the source to the
receiver. No reciprocity is assumed in these cases, so the angles go
from 0 to 359.999 degrees.
If the sector option is set, then the range is from 0 to 360/sector.
Caveat:
This program honors the value of scalco in scaling the values of
sx,sy,gx,gy. Type "sukeyword scalco" for more information.
Type "sukeyword -o" to see the keywords and descriptions of all
header fields.
To plot midpoints, use: su3dchart

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
