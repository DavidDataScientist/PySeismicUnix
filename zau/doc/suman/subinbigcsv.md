# subinbigcsv

SUBINBIGCSV - Make Some CDPs Larger and Delete All Traces Outside Them. subinbigcsv  rfile=in.csv <in.su >out.su  (other parameters) Parameter overview:

## Synopsis

```bash
subinbigcsv rfile=in.csv <in.su >out.su  (other parameters)
```

## Notes

intype=0   Default 3 if a 3D Grid is input, otherwise -2.
=3   Compute trace midpoint coordinates (sx+gx)/2 and (gx+gy)/2 and
determine cdp,igi,igc values from them (using the 3D Grid).
For this option traces do not need cdp,igi,igc numbers on input
(they do not need to have been through grid program subincsv).
NOTE: For 3D STACKed traces, options 2 or 1 are required (usually).
=2   Use input cdp (cell) key and 3D Grid to determine igi,igc.
=1   Use input igi,igc keys and 3D Grid to determine cdp.
=-2  Use input cdp (cell) key. A 3D Grid cannot be input.
NOTE: For option -2 parameters igiout and igiext refer to cdp key
numbers and igcout, igcext, reigi, reigc cannot be specified
(reigi and reigc both use option 0).
qout=     Output file containing the locations specified by the
igiout (or igilist) and igcout (or igclist) parameters.
recdp=1    Reset the trace cdp key to its central cdp number.
=0    Do not reset cdp key numbers.
reigi=1    Reset the trace igi key to its central igi number.
=0    Do not reset igi key numbers.
reigc=1    Reset the trace igc key to its central igc number.
=0    Do not reset igc key numbers.
check=0  Do not print checking details.
1  Run functions on the 4 grid corner points and print results.
This output print may be useful for users.
EXAMPLE ******************************************************************
igiout=40,9999,40
igcout=5,9999,5
igiext=2
igcext=1
This defines a central cdp every 40 cells in igi direction by 5 cells in
igc direction. Traces will ONLY BE OUTPUT if they are within 2 cells in
igi direction and also within 1 cell in igc direction of the central cdps.
By default, the cdp,igi,igc key values in the output traces will be reset
to the values of the central cdp of those traces. One purpose of this is
to create super-cdps for Velocity analysis.
**************************************************************************
This program does not sort, it deletes traces, and renumbers cdp key.
Since cdp is renumbered the output is NOT cdp ordered even if you input a
cdp ordered dataset.
**************************************************************************
***********************************************************
To output this documentation:  subinbigcsv 2> binbigdoc.txt
***********************************************************
------------------------------------------------------------------------

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
