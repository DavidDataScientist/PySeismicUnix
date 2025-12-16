# sucleave

SUCLEAVE - Cleave traces from 1 input into multiple output files. (split to multiple output files, retaining input order). sucleave <stdin key=offset low=50 size=100 abs=1

## Synopsis

```bash
sucleave <stdin key=offset low=50 size=100 abs=1
```

## Examples

```
cleave_offset_50.su   cleave_offset_150.su
print=0    Do not print.
=1    Print filename, trace count, average key value for
each created output file. Note that ranges with 0 traces
will make no print.
ATTENTION: No traces are deleted. All traces less than the minimum
key range are written to one extra file, and all traces
greater than the maximum key range are written to another
extra file.     WARNINGS are printed if this occurs.
These extra files can be recognized because the number in
their names is outside the low,high ranges you specified.
```

## Notes

1. See simple tests (examples) in src/demos/Utilities/Sucleave
2. Output files are not created until a trace is written to them.
So, specifying excessive low= and high= values has little
consequences except for some small memory allocation herein.
3. Bad choice of size can create an output file for every trace.
EXAMPLE USES:
1. You may want to deliberately partition your traces for different
processing. You can cleave your traces into offset ranges, while
retaining cdp order in those offset-ranged files.
You can then use subraid to bring them back together.
2. In streamer-cable-marine you may want to partition your shot
gathers by cables (or port-starboard shot, cable combinations).
This is sometimes called nominal subsurface line processing.
You can then use subraid to bring them back together.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
