# sugeomcsv

SUGEOMCSV - 3D/2D land/OBotm geometry from Comma Separated or fixed text. sugeomcsv <intraces >outtraces rfile=in.txt   (other parameters) Parameter overview:

## Synopsis

```bash
sugeomcsv <intraces >outtraces rfile=in.txt   (other parameters)
```

## Required Parameters

rfile=  text values file (fixed or comma separated values)

## Optional Parameters

rtype= type of records in rfile text file. Default is csv if the
file name ends in csv, otherwise defaults to fixed.
=csv     comma separated values
=fixed   This option is required for standard SPS files
that have not been converted to csv by SUTOOLCSV
- along with other specifications in names= list

## Examples

```
ufile=  file to search for C_SU parameter records. The default is
to search in the rfile text file.
=command   means ignore any C_SU records in rfile text file.
All required parameters must be specified on the command line.
The following 3 parameters must be found on the command line or in
their corresponding C_SU records in the rfile (or ufile) text file.
match= any number of input trace header keys needed to find exact
record in the rfile text file. Must also be listed in names=
Example on command line:
match=fldr,tracf
match=SPS2 This is just a standard way to specify the match= list
for SPS Revison 2 files (see examples in SUTOOLCSV).
The setid= option must also be X,S, or R.
match=SPS1 This is just a standard way to specify the match= list
for SPS Revison 1 files (see examples in SUTOOLCSV).
The setid= option must also be X,S, or R.
names= is used to assign names to values in rfile text file (you
are telling this program the names of values in text file).
For files with comma-separated values, a name must be listed
sequentially for each field in the rfile text file.
The names must also include the match= SU keys above.
Note C_su_id means this is field used for record acceptance.
***  Read the note   c_su_id IS SPECIAL   later. ***
Special names: null and numb with integer (such as null1)
Means do not read/output this field. (You can
also put nothing between sequential commas).
Example on command line:
names=C_su_id,cdp,null3,cx,cy,,ce
names=SPS2 This is just a standard way to specify the names= list
for SPS Revison 2 files (see examples in SUTOOLCSV).
The setid= option must also be X,S, or R.
names=SPS1 This is just a standard way to specify the names= list
for SPS Revison 1 files (see examples in SUTOOLCSV).
The setid= option must also be X,S, or R.
names=SPS2ALL  Same as SPS2. The SUTOOLCSV program has this option
to output fields with no SU key (using numb names),
but numb names are treated like null names herein.
names=SPS1ALL  Same as SPS1.
setid= is used to accept text records based on their first field.
setid=S     means accept text records if their first field is S
(any characters allowed, such as R,X,cdp,FRED)
Note: this value is automatically upper-cased unless
you surround it by double-quotes.
So s becomes S unless you use double-quotes.
setid=ANY   means read all records (except those starting C_SU)
and those records have an id field at front.
setid=NONE  means read all records (except those starting C_SU)
but those records do not have an id field at front.
(For csv files this means the field before the first
comma is a value, not an identifier).
Example on command line:
setid=S
-----------------
If match= is not specified on command line, this program searches
for a text record starting with C_SU_MATCH and reads keys from it.
Example within the text file:
C_SU_MATCH,fldr,tracf
If names= is not specified on command line, this program searches
for a text record starting with C_SU_NAMES and reads names from
the record after the C_SU_NAMES record.
Example within the text file:
C_SU_NAMES
C_su_id,cdp,null,cx,cy,null,ce
If setid= is not specified on command line, this program searches
for a text record starting with C_SU_SETID and reads id from it.
Example within the text file:
C_SU_SETID,S
Note these C_SU_ parameter records can be in any order within text file.
-----------------
rdelim= If rtype=fixed you cannot specify this parameter.
If rtype=csv the default is comma. You can specify any
single character here either by itself or surrounded by
double-quotes (some characters such as semi-colon may have
trouble getting through the command line).
**Note** Specifying a blank here usually will not give good results
unless your input rfile has exactly 1 blank between fields.
-----------------
scalco= multiply coordinates by this power of 10 (1,10,100...)
before putting them in traces. Default is 10 which means
that sx,sy,gx,gy from the text file are multiplied by 10.
The actual value of scalco in the traces is therefore
set to -10 (meaning these values need to be divided by 10).
*** You cannot specify scalco= if you are not updating any
*** of the 4 coordinate values.
**** The SU key offset value is recomputed if any of sx,sy,gx,gy
**** is in the list of names (and offset itself is NOT in list).
* If you are confident your text files contain coordinates
* with only whole numbers, you can set this to 1. But you
* cannot change this just for some of these 4 coordinates.
** This SEEMS like a problem about size, but it is actually a
** problem about decimal digits. It you use 1 here you will
** find that values like 544444.6 get rounded to 544445
** when SUGEOMCSV updates them to traces.
*** If scalco is in the list of names= then that value is set
in output header. But only the value specified or defaulted
HERE is actually applied. So do not put it in names= unless
you are repairing some odd situation.
scalel= multiply elevation and other related values by this power
of 10 (1,10,100...) before putting them in traces. Default
is 10 which means gelev,selev,sdepth,gdel,sdel,swdep,gwdep
from the text file are multiplied by 10.
The actual value of scalel in the traces is therefore
set to -10 (meaning these values need to be divided by 10).
*** You cannot specify scalel= if you are not updating any
*** of the 7 elevation related values.
* If you are confident your text files contain these values
* with only whole numbers, you can set this to 1. But you
* cannot change this just for some of these 7 values.
** This SEEMS like a problem about size, but it is actually a
** problem about decimal digits. It you use 1 here you will
** find that values like 3333.6 get rounded to 3334
** when SUGEOMCSV updates them to traces.
*** If scalel is in the list of names= then that value is set
in output header. But only the value specified or defaulted
HERE is actually applied. So do not put it in names= unless
you are repairing some odd situation.
unrepeat= The default is not to enable this option.
This option is general but most likely usefull for X-files
where the field record number (fldr) increases but then
re-starts at a lower number. Such as 1->7800 then 5->4000.
Normally, the finding-logic within this program would not
be able to distinguish the first fldr 5 from the second 5
and so on. (For that situation if you do not use this
option you will most likely get a channel-range error as
that X-file is read-in and stored).
unrepeat=1 Read the text file and generate an integer from 1 and
increment by 1 every time the first match= reverses.
Typically, the first match= is fldr for X-files so this
increments +1 when fldr is increasing and then decreases,
and also increments +1 if fldr is decreasing and then
increases. The comparison is done using order of records
as they exist in the text file (before sorting herein).
Another incrementing integer is generated the same way
except using the order of the input traces. These two
integers are used to match which (fldr) value in the
traces belongs to which (fldr) value from X-file.
**  Note that this option is not fool-proof and should be
**  used with caution and copious checking of results. This
**  option is better than a pure sequential re-numbering
**  because it still primarily uses the field record numbers,
**  but it is not guaranteed.
unrepeat=n (any integer). The input text record incrementing integer
will still start at 1 and increment +1. But the trace
incrementing integer starts at this number. This allows
you to input reduced sets of fldr records without need
to edit the X-file.
-----------------
-----------------
nicerecord= record number to start trying to read data values from
the rfile text file (default is 1). The beginning records
of some text files are badly composed (comments or info.)
When the setid= option is not able to reject them,
specify a record number here where setid= will work.
(This program also always knows that records starting
with C_SU are not data records, and will not try to
read data values from them even when setid=ALL). But
it will read C_SU parameter records even if they are
previous to this nicerecord number.
maxrecords= maximum number of records to allocate memory for.
If not specified, this program reads through the records
once and allocates for the number found. Then reads them
again. This double reading takes more time. If you want
to avoid this, specify a maximum via this parameter.
missing= specifies what to do if an input trace match= values cannot
be found in the rfile text file. Default is to error-halt
at the first trace where this occurs.
missing=delete   means delete all traces for which this occurs.
missing=pass     means pass those traces to output without update.
This allows you to, for instance, change statics
values for just some shots or receivers without
having every shot or receiver in the rfile text file
action= specifies how to update the trace header values
Default is to set the headers to the text record values.
The option here is not applied to the values of
names on the match= list.
action=add       Add the text values to the trace header values.
action=subtract  Subtract text values from the trace header values.
-----------------
-----------------
create=    create output traces and update from the rfile text file.
***  Attempting to also input a trace file will error-halt
***  unless spikes=n).
Values of all non-null names on names= list are updated to
traces (including those on match= list). The behavior of
this program is the same as when traces are input. This
includes sorting of the text records into match= order and
therefore created traces are output in that order. Most
text files result in one trace per record, but SPS X-files
(and similar) usually create many traces for each record.
create=all   create traces for all data records in the rfile text file
create=n     create n or all (whichever is fewer)
spikes= ***  This option will error-halt if create= is not specified.
By default, create= outputs only 1 zero-sample at 4 ms.
This parameter accepts a list of time,amplitude pairs.
Example: spikes=4,0.001,800,1000,1500,-2000,3000,0
The maximum time sets the trace length (3000 ms. above).
The first pair in the list is special, it specifies the
sample interval (in ms.) and base-amplitude.
In this example, the sample interval is 4, and the
base-amplitude is 0.001. The base-amplitude is the value
samples are set to if they are not spiked by remainder of
the list. (For geometry checking and other QC and survey
design reasons, base-amplitude of 0.0 is not a nice value.
For instance, 0.0 makes it difficult to check mutes).
All times should be whole multiples of the sample interval
but will be rounded to nearest sample time if not.
Times do not have to increase but cannot be negative.
spikes=n  Where n is the sequential number of an input trace. The
sample values of this trace will be copied to all created
output traces. Values of all non-null names on names= list
are updated to traces (including those on the match= list).
The intention here is to allow you to input a reflectivity
or well-log type of trace, duplicate the samples and change
some header values via the text file (for testing purposes)
-----------------
-----------------
ADVICE: Run create=all for your first relation file (SPS X-file) setup.
Then check the values in the output trace headers.
Then run SPS S-file setup to update those headers (not create).
Then check the values in the output trace headers.
Then run SPS R-file setup to update those headers (not create).
Then check the values in the output trace headers.
This will allow you to check the mechanics of your setups.
-----------------
MORE ADVICE: Use spike options to test your setups of other programs.
For instance, after running the sequence advised above,
put positive and negative spikes on the traces.
Then run a simple bandpass filter. At this point the
results will begin to look like post-NMO shot gathers.
Apply inverse-NMO and offset-Mute to make your traces look
like pre-NMO shot gathers.
Then you can run some interesting tests, like:
1.  Apply surface-consistent residual statics to some shots and
receivers. Surface-consistant statics analysis programs
should easily be able to find the statics you applied.
2.  Put surface-consistant gains on some shots and receivers.
Surface-consistant amplitude analysis programs should
easily find the gains you applied.
3.  Put surface-consistent signatures on some shots and
receivers. Surface-consistent deconvolution programs
should easily find the signatures you applied.
CAUTION: Just because you apply something and another program computes
the correct inverse, it does not mean the program is setup
well for actual seismic situations. These kinds of tests are
too simple to be considered forward-modelling.
OF COURSE: You might do this kind of thing without having any actual
seismic data, just to test a few survey-design ideas.
------------------------------------------------------------------------
------------------------------------------------------------------------
C_su_id IS SPECIAL ********************
(a) Its character range is taken from the length of the value specified
for setid (for instance S has range 1 to 1, FRED has range 1 to 4).
(b) The value specified for id is case-sensitive (r is not R). The id
value is the only thing case-sensitive in this program except for
the file names. So this program does not care if you use parameter
records starting with C_SU or c_su, but you want other programs to
ignore C_SU records, so use a capital C, not a lower case c.
-----------------------------------------------------------------
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
