# sumatrix

SUMATRIX - Assemble a Matrix of Trace Sample Zones. Notes: Typically this program creates output traces which can be displayed

## Synopsis

```bash
sumatrix <stdin
```

## Notes

(such as grnors,gaps if sugeomcsv used SPS2 files)
rdivider=1.0    List of dividers for rkeyloc values. The values in
this list can be set to -1 to reverse the order of
receivers in the output matrix.
rfill=1         Output zeroed traces when rkeyloc combination
has no input trace within a numzone output range.
=0  Do not output zeroed traces when rkeyloc combination
has no input trace within a numzone output range.
This option might be better for 3d surveys.
=2  Output zeroed traces for all rkeyloc combinations.
Note this differs from option 1 in that it ALSO makes
traces OUTSIDE each numzone output range.
Note: None of these options creates an output trace for a
rkeyloc combination THAT DOES NOT EXIST in input.
lenzone=400.0   Length of Zones (ms., rounded to nearest sample.)
numzone=20      Number of Zones To Put In Each Output Trace.
Note: Output traces have lenzone*numzone time length.
Do not exceed the maximum trace samples of SU or
other software that needs to handle these traces.
maxsources=10000 Maximum Amount of Sources. This is also the
maximum unique combinations of skeyloc values.
maxreceivers=20000 Maximum Amount of Receivers. This is also the
maximum unique combinations of rkeyloc values.
maxtraces=1000000 Maximum Amount of Traces. Note that most memory for
the traces is only allocated for the actual amount
of traces read (and just the lenzone, no headers).
NOTE 1:  Output is sets of traces with numzone zones per each trace.
Each zone is made from 1 combination of skeyloc values.
Completely missing skeyloc combinations are not output.
A set of output traces is made for each increment of numzone.
Each set makes an output trace for each rkeyloc combination
that has an input trace in the numzone range of that set.
NOTE 2:  Output trace headers are COPIES of the FIRST input header
with some values reset. The output skeyloc values are set
to the skeyloc value combination of the TOP ZONE. The output
rkeyloc keys are set to the rkeyloc value combination for
each trace. And key nhs is set to the number of input trace
zones copied to each output trace.
NOTE 3:  For 3D surveys you may want to reverse only the line or
point ordering. For example, a divider list like 1,-1
NOTE 4:  Usually, specify source keys in skeyloc and receiver keys
in rkeyloc. But you can also specify them vice-versa in
order to exchange the matrix orientation.
NOTE 5:  For some unusual situations you might want to set the
sdivider and/or rdivider to scale the input values.
The internal computation is: int(floor(header_value/divider))
which essentially means scale and round down to near integer.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
