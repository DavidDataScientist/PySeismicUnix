# susimmat

SUSIMAT - Correlation similarity matrix for two traces. Output is zero lag of cross-correlation of traces, or linear regression correlation coefficient.

## Synopsis

```bash
susimmat <data12 sufile=data2 >dataout
```

## Required Parameters

sufile=		file containing SU traces to use as filter

## Optional Parameters

panel=0		use only the first trace of sufile as filter
=1 compute sim matrix trace by trace an entire
gather
mt=21			operator window length (odd integer)
eps=1e-3		stability parameter
taper=0		no taper to data fragments
=1 apply exponential taper (1/e at ends)
method=1		use xcorrelation as similarity meausure
=2 same but normalized by (rms+eps)
=3 use linear regression CC
=4 use mt-dimensional vector angle
EXAMPLE PROCESSING SEQUENCES:
1. Look for all possible alignments of OBC P and Z data
susimmat < P_Z.su sufile=OBC_P.su  mt=71 > P_Zxcor.su
Note:  xcor window is collapsed as needed to compute edge values
It is quietly assumed that the time sampling interval on the  single
trace
and the output traces is the same as that on the traces in the input
file.
The sufile may actually have more than one trace, but only the first
trace is used when panel=0. When panel=1 the number of traces in the
sufile MUST be  the same as the number of traces in the input.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
