# segyread

SEGYREAD - read an SEG-Y tape segyread > stdout tape= or

## Synopsis

```bash
segyread > stdout tape=
```

## Optional Parameters

buff=1	for buffered device (9-track reel tape drive)
=0 possibly useful for 8mm EXABYTE drives
verbose=0	silent operation
=1 ; echo every 'vblock' traces
vblock=50	echo every 'vblock' traces under verbose option
hfile=header	file to store ebcdic block (as ascii)
bfile=binary	file to store binary block
xfile=xhdrs	file to store extended text block
over=0	quit if bhed format not equal 1, 2, 3, 5, or 8
= 1 ; override and attempt conversion
format=bh.format	if over=1 try to convert assuming format value
conv=1	convert data to native format
= 0 ; assume data is in native format
ebcdic=1	perform ebcdic to ascii conversion on 3200 byte textural
header. =0 do not perform conversion
ns=bh.hns	number of samples (use if bhed ns wrong)
trcwt=1	apply trace weighting factor (bytes 169-170)
=0, do not apply.  (Default is 0 for formats 1 and 5)
trmin=1		first trace to read
trmax=INT_MAX	last trace to read
endian=(autodetected) =1 for big-endian,  =0 for little-endian byte order
swapbhed=endian	swap binary reel header?
swaphdrs=endian	swap trace headers?
swapdata=endian	swap data?
nextended=(set from binary header) number of extended text headers
errmax=0	allowable number of consecutive tape IO errors
remap=...,...	remap key(s)
byte=...,...	formats to use for header remapping

## Notes

Traditionally tape=/dev/rmt0.	 However, in the modern world tape device
names are much less uniform.  The magic name can often be deduced by
"ls /dev".  Likely man pages with the names of the tape devices are:
"mt", "sd" "st".  Also try "man -k scsi", " man mt", etc.
Sometimes "mt status" will tell the device name.
For a SEG-Y diskfile use tape=filename.
The xfile argument will only be used if the file contains extended
text headers.
Remark: a SEG-Y file is not the same as an su file. A SEG-Y file
consists of three parts: an ebcdic header, a binary reel header, and
the traces.  The traces are (usually) in 32 bit IBM floating point
format.  An SU file consists only of the trace portion written in the
native binary floats.
Formats supported:
1: IBM floating point, 4 byte (32 bits)
2: two's complement integer, 4 byte (32 bits)
3: two's complement integer, 2 byte (16 bits)
5: IEEE floating point, 4 byte (32 bits)
8: two's complement integer, 1 byte (8 bits)
tape=-   read from standard input. Caveat, under Solaris, you will
need to use the buff=1 option, as well.
Header remap:
The value of header word remap is mapped from the values of byte
Map a float at location 221 to sample spacing d1:
segyread <data >outdata remap=d1 byte=221f
Map a long at location 225 to source location sx:
segyread <data >outdata remap=sx byte=225l
Map a short at location 229 to gain constant igc:
segyread <data >outdata remap=igc byte=229s
Or all combined:
segyread <data >outdata remap=d1,sx,igc byte=221f,225l,229s
Segy header words are accessed as Xt where X denotes the byte number
starting at 1 in correspondance with the SEGY standard (1975)
Known types include:	f	float (4 bytes)
l	long int (4 bytes)
s	short int (2 bytes)
b	byte (1 bytes)
type:	  sudoc segyread   for further information

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
