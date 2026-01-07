# surecmo

SURECMO - compensate for the continuously moving streamer in marine seismic acquisition (assume far offset is first channel) right now valid for data where tmax < 2*dx/vb

## Synopsis

```bash
surecmo right now valid for data where tmax < 2*dx/vb
```

## Examples

```
surecmo <stdin >sdout vb=2.5 dt=25 dt=4000
(valid for tmax < 20s)
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
