# suaddhead

SUADDHEAD - put headers on bare traces and set the tracl and ns fields suaddhead <stdin >stdout ns= ftn=0

## Synopsis

```bash
suaddhead <stdin >stdout ns= ftn=0
```

## Examples

```
suaddhead ns=1024 <bare_traces | sushw key=dt a=4000 >segy_traces
This command line adds headers with ns=1024 samples.  The second part
of the pipe sets the trace header field dt to 4 ms.	See also the
selfdocs of related programs  sustrip and supaste.
See:   sudoc supaste
Related Programs:  supaste, sustrip
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
