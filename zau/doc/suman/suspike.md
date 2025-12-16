# suspike

SUSPIKE - make a small spike data set

## Synopsis

```bash
suspike [optional parameters] > out_data_file
```

## Optional Parameters

nt=64 		number of time samples
ntr=32		number of traces
dt=0.004 	time sampling interval in seconds
offset=400 	offset
nspk=4		number of spikes
ix1= ntr/4	trace number (from left) for spike #1
it1= nt/4 	time sample to spike #1
ix2 = ntr/4	trace for spike #2
it2 = 3*nt/4 	time for spike #2
ix3 = 3*ntr/4;	trace for spike #3
it3 = nt/4;	time for spike #3
ix4 = 3*ntr/4;	trace for spike #4
it4 = 3*nt/4;	time for spike #4

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
