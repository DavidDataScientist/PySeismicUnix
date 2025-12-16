# segywrite

SEGYWRITE - write an SEG-Y tape segywrite <stdin tape=

## Synopsis

```bash
segywrite <stdin tape=
```

## Required Parameters

tape=		tape device to use (see sudoc segyread)
Optional parameter:
verbose=0	silent operation
=1 ; echo every 'vblock' traces
vblock=50	echo every 'vblock' traces under verbose option
buff=1		for buffered device (9-track reel tape drive)
=0 possibly useful for 8mm EXABYTE drive
conv=1		=0 don't convert to IBM format
ebcdic=1	convert text header to ebcdic, =0 leave as ascii
hfile=header	ebcdic card image header file
bfile=binary	binary header file
trmin=1 first trace to write
trmax=INT_MAX  last trace to write
endian=(autodetected)	=1 for big-endian and =0 for little-endian byte order
errmax=0	allowable number of consecutive tape IO errors
format=		override value of format in binary header file
Note: The header files may be created with  'segyhdrs'.
Note: For buff=1 (default) tape is accessed with 'write', for buff=0
tape is accessed with fwrite. Try the default setting of buff=1
for all tape types.
Caveat: may be slow on an 8mm streaming (EXABYTE) tapedrive
Warning: segyread or segywrite to 8mm tape is fragile. Allow time
between successive reads and writes.
Precaution: make sure tapedrive is set to read/write variable blocksize
tapefiles.
For more information, type:	sudoc <segywrite>

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
