# sudumptrace

SUDUMPTRACE - print selected header values and data. Print first num traces. Use SUWIND to skip traces.

## Synopsis

```bash
sudumptrace < stdin [> ascii_file]
```

## Optional Parameters

num=4                    number of traces to dump
key=key1,key2,...        key(s) to print above trace values
hpf=0                    header print format is float
=1 print format is exponential

## Examples

```
sudumptrace < inseis.su            PRINTS: 4 traces, no headers
sudumptrace < inseis.su key=tracf,offset
sudumptrace < inseis.su num=7 key=tracf,offset > info.txt
sudumptrace < inseis.su num=7 key=tracf,offset hpf=1 > info.txt
Related programs: suascii, sugethw
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
