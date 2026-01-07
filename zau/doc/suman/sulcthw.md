# sulcthw

SULCTHW - Linear Coordinate Transformation of Header Words sulcthw <infile >outfile xt=0.0	Translation of X

## Synopsis

```bash
sulcthw <infile >outfile
```

## Notes

Translation:
x = x'+ xt;y = y'+ yt;z = z' + zt;
Rotations:
Around Z axis
X = x*cos(zr)+y*sin(zr);
Y = y*cos(zr)-x*sin(zr);
Around Y axis
Z = z*cos(yr)+x*sin(yr);
X = x*cos(yr)-z*sin(yr);
Around X axis
Y = y*cos(xr)+z*sin(xr);
Z = Z*cos(xr)-y*sin(xr);
Header words triplets that are transformed
sx,sy,selev
gx,gy,gelev
The header words restored as 32 bit integers using SEG-Y
convention (with coordinate scalers scalco and scalel).
After transformation they are converted back to integers and stored.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
