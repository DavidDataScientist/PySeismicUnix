# sucommand

SUCOMMAND - pipe traces having the same key header word to command

## Synopsis

```bash
sucommand <stdin >stdout [Optional parameters]
```

## Required Parameters

none

## Optional Parameters

verbose=0		wordy output
key=cdp			header key word to pipe on
command="suop nop"    command piped into
dir=0		0:  change of header key
-1: break in monotonic decrease of header key
+1: break in monotonic increase of header key

## Notes

This program permits limited parallel processing capability by opening
pipes for processes, signaled by a change in key header word value.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
