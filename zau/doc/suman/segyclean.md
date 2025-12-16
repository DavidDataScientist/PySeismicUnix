# segyclean

SEGYCLEAN - zero out unassigned portion of header segyclean <stdin >stdout Since "foreign" SEG-Y tapes may use the unassigned portion

## Synopsis

```bash
segyclean <stdin >stdout
```

## Examples

```
segyread trmax=200 | segyclean | suximage
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
