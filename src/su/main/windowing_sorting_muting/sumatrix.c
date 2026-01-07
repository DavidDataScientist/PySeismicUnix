/* Copyright (c) Colorado School of Mines, 2023.*/
/* All rights reserved.			*/

/* SUMATRIX: $Revision: 1.0 $ ; $Date: 2024/08/24 11:00:01 $	*/

#include "su.h"
#include "segy.h"
#include "header.h"
#include <stdbool.h>
#include "headcase.h"

/*********************** self documentation **********************/
char *sdoc[] = {
"                                                                       ",
" SUMATRIX - Assemble a Matrix of Trace Sample Zones.                   ",
"                                                                       ",
" Notes:                                                                ",
" Typically this program creates output traces which can be displayed   ",
" to highlight surface-consistent differences and/or geometry-errors.   ",
" Each output trace has time zones assembled from multiple input traces.",
"                                                                       ",
" ----------------------------------------------------------------------",
"                                                                       ",
" sumatrix  <stdin                                                      ",
"                                                                       ",
" skeyloc=fldr    List of keys that identify source locations.          ",
"           Note: For 3d surveys you can use fldr (but possibly better  ",
"                 to use grnofr,grnlof if sugeomcsv used SPS2 files).   ",
" 									",
" sdivider=1.0    List of dividers for skeyloc values. The values in    ",   
"                 this list can be set to -1 to reverse the order of    ",   
"                 sources in the output matrix.                         ",   
" 									",
" rkeyloc=gaps    List of keys that identify receiver locations.        ",
"           Note: For 3d surveys you often MUST use 2 keys              ",
"                 (such as grnors,gaps if sugeomcsv used SPS2 files)    ",
" 									",
" rdivider=1.0    List of dividers for rkeyloc values. The values in    ",   
"                 this list can be set to -1 to reverse the order of    ",   
"                 receivers in the output matrix.                       ",   
" 									",
" rfill=1         Output zeroed traces when rkeyloc combination         ",
"                 has no input trace within a numzone output range.     ",
"             =0  Do not output zeroed traces when rkeyloc combination  ",
"                 has no input trace within a numzone output range.     ",
"                 This option might be better for 3d surveys.           ",
"             =2  Output zeroed traces for all rkeyloc combinations.    ",
"                 Note this differs from option 1 in that it ALSO makes ",
"                 traces OUTSIDE each numzone output range.             ",
"           Note: None of these options creates an output trace for a   ",
"                 rkeyloc combination THAT DOES NOT EXIST in input.     ",
" 									",
" lenzone=400.0   Length of Zones (ms., rounded to nearest sample.)     ",   
" 									",
" numzone=20      Number of Zones To Put In Each Output Trace.          ",   
"           Note: Output traces have lenzone*numzone time length.       ",   
"                 Do not exceed the maximum trace samples of SU or      ",   
"                 other software that needs to handle these traces.     ",   
" 									",
" maxsources=10000 Maximum Amount of Sources. This is also the          ",
"                  maximum unique combinations of skeyloc values.       ",
"                                                                       ",
" maxreceivers=20000 Maximum Amount of Receivers. This is also the      ",
"                    maximum unique combinations of rkeyloc values.     ",
"                                                                       ",
" maxtraces=1000000 Maximum Amount of Traces. Note that most memory for ",
"                   the traces is only allocated for the actual amount  ",
"                   of traces read (and just the lenzone, no headers).  ",
"                                                                       ",
"                                                                       ",
" NOTE 1:  Output is sets of traces with numzone zones per each trace.  ",
"          Each zone is made from 1 combination of skeyloc values.      ",
"          Completely missing skeyloc combinations are not output.      ",
"          A set of output traces is made for each increment of numzone.",
"          Each set makes an output trace for each rkeyloc combination  ",
"          that has an input trace in the numzone range of that set.    ",
"                                                                       ",
" NOTE 2:  Output trace headers are COPIES of the FIRST input header    ",
"          with some values reset. The output skeyloc values are set    ",
"          to the skeyloc value combination of the TOP ZONE. The output ",
"          rkeyloc keys are set to the rkeyloc value combination for    ",
"          each trace. And key nhs is set to the number of input trace  ",
"          zones copied to each output trace.                           ",
"                                                                       ",
" NOTE 3:  For 3D surveys you may want to reverse only the line or      ",
"          point ordering. For example, a divider list like 1,-1        ",
"                                                                       ",
" NOTE 4:  Usually, specify source keys in skeyloc and receiver keys    ",
"          in rkeyloc. But you can also specify them vice-versa in      ",
"          order to exchange the matrix orientation.                    ",
"                                                                       ",
" NOTE 5:  For some unusual situations you might want to set the        ",
"          sdivider and/or rdivider to scale the input values.          ",
"          The internal computation is: int(floor(header_value/divider))",
"          which essentially means scale and round down to near integer.",
"                                                                       ",
NULL};

/* Author:
 *  Andre Latour. Aug  2024
 *  1. Started from surescsv.c because it contains the shot and receiver                                          
 *     identifier code (which I did not want to have to re-think).                                                          
 */
/**************** end self doc ***********************************/

segy tr,trz;

struct OrderInfo {
  int ndxj;
  int ndxk;
};
struct OrderInfo *sndx;

int num_keyloc;           /* needed within compareii (and therefor bhighii)   */

int bhighii(int** all, int last, int* guy); 
int compareii (const void * q1, const void * q2) ;

int main(int argc, char **argv) {

  float flenzone = 400.; 
  int lenzone = 0;
  int numzone = 20;
  int **jkzone = NULL;
  int kbeg = -1;
  int kend = -1;
  int rfill = 1;

  cwp_String *skeyloc = NULL;
  int *skaseloc = NULL;
  int num_skeyloc=0;                                                                
  int **sunqlocs=NULL; /* source unique identifiers*/
  int **strclocs=NULL; /* trace source identifiers */
  int *sguy=NULL;
  int maxsources=10000;
  int lastsource=0;
  double *sdivider=NULL;

  cwp_String *rkeyloc = NULL;
  int *rkaseloc = NULL;
  int num_rkeyloc=0;                                                                
  int **runqlocs=NULL; /* receiver unique identifiers */
  int **rtrclocs=NULL; /* trace receiver  identifiers */
  int *rguy=NULL;
  int maxreceivers=20000;
  int lastreceiver=0;
  double *rdivider=NULL;

  float **allsamps=NULL; /* the sample values of all input traces */
  int maxtraces=1000000;
  int lensam=0;

  int i=0;
  int j=0;
  int k=0;
  int m=0;
  int n=0;
  int nend=20;
  int nproct=0;
  int ifold=0;

  int notunique=0;
  int jhere=0;
  int khere=0;

/* hook up getpar */

  initargs(argc, argv);
  requestdoc(1); /* note: cannot put a warn above here */

/* Get key case numbers from GetCase.                                        */

  if(countparval("skeyloc")>0) {
    num_skeyloc = countparval("skeyloc");
    skeyloc = ealloc1(num_skeyloc,sizeof(cwp_String *)); 
    getparstringarray("skeyloc", skeyloc);
  }    
  else {
    num_skeyloc = 1; 
    skeyloc = ealloc1(num_skeyloc,sizeof(cwp_String *)); 
    skeyloc[0] = ealloc1(3,1);
    strcpy(skeyloc[0],"fldr");
  }    
  skaseloc = ealloc1int(num_skeyloc); 
  for (i=0; i<num_skeyloc; ++i) {
    skaseloc[i] = GetCase(skeyloc[i]);
    if(skaseloc[i]<1) err("**** Error: Specified skeyloc name %s is not recognized.",skeyloc[i]);
  }

  sguy = ealloc1int(num_skeyloc);

  if(countparval("maxsources")>0) getparint("maxsources",&maxsources);
  if(maxsources<1) err("**** Error: maxsources must be greater than 0");

  sunqlocs = (int **)ealloc1(sizeof(int*),maxsources);

  sdivider = ealloc1double(num_skeyloc);

  if(countparval("sdivider")>0) {
    if(countparval("sdivider")!=num_skeyloc) err("**** Error: sdivider list must have same amount as skeyloc ");
    getpardouble("sdivider",sdivider);
    for (i=0; i<num_skeyloc; ++i) {
      if(sdivider[i]==0.0) err("**** Error: sdivider values cannot be 0.0 ");
    }    
  }
  else {
    for (i=0; i<num_skeyloc; ++i) sdivider[i] = 1.0; 
  }

  if(countparval("rkeyloc")>0) {
    num_rkeyloc = countparval("rkeyloc");
    rkeyloc = ealloc1(num_rkeyloc,sizeof(cwp_String *)); 
    getparstringarray("rkeyloc", rkeyloc);
  }    
  else {
    num_rkeyloc = 1; 
    rkeyloc = ealloc1(num_rkeyloc,sizeof(cwp_String *)); 
    rkeyloc[0] = ealloc1(3,1);
    strcpy(rkeyloc[0],"gaps");
  }    
  rkaseloc = ealloc1int(num_rkeyloc); 
  for (i=0; i<num_rkeyloc; ++i) {
    rkaseloc[i] = GetCase(rkeyloc[i]);
    if(rkaseloc[i]<1) err("**** Error: Specified rkeyloc name %s is not recognized.",rkeyloc[i]);
  }

  rguy = ealloc1int(num_rkeyloc);

  if(countparval("maxreceivers")>0) getparint("maxreceivers",&maxreceivers);
  if(maxreceivers<1) err("**** Error: maxreceivers must be greater than 0");

  runqlocs = (int **)ealloc1(sizeof(int*),maxreceivers);

  rdivider = ealloc1double(num_rkeyloc);

  if(countparval("rdivider")>0) {
    if(countparval("rdivider")!=num_rkeyloc) err("**** Error: rdivider list must have same amount as rkeyloc ");
    getpardouble("rdivider",rdivider);
    for (i=0; i<num_rkeyloc; ++i) {
      if(rdivider[i]==0.0) err("**** Error: rdivider values cannot be 0.0 ");
    }    
  }
  else {
    for (i=0; i<num_rkeyloc; ++i) rdivider[i] = 1.0; 
  }

  if(countparval("rfill")>0) getparint("rfill",&rfill);
  if(rfill<0 || rfill>2) err("**** Error: rfill parameter out of range.");

  if(countparval("maxtraces")>0) getparint("maxtraces",&maxtraces);
  if(maxtraces<1) err("**** Error: maxtraces must be greater than 0");

  if(countparval("lenzone")>0) getparfloat("lenzone",&flenzone);
  if(flenzone<1.) err("**** Error: lenzone must be greater than 0");

  if(countparval("numzone")>0) getparint("numzone",&numzone);
  if(numzone<1) err("**** Error: numzone must be greater than 0");

  strclocs = (int **)ealloc1(sizeof(int*),maxtraces);
  rtrclocs = (int **)ealloc1(sizeof(int*),maxtraces);

  sndx = calloc(maxtraces,sizeof(struct OrderInfo));

  allsamps = (float **)ealloc1(sizeof(float*),maxtraces);

  checkpars();

/* Loop over the input traces.                                                */

  while(gettr(&tr)) {
 
    if(nproct<1) { 
      memcpy(&trz, &tr, HDRBYTES); /* copy header from first trace             */
      lenzone = NINT((flenzone)/(tr.dt/1000.0));
      trz.ns = lenzone * numzone;
      trz.dt = tr.dt;
      lensam = tr.ns;                      /* careful, can have more or less  */
      if(lensam>lenzone) lensam = lenzone; /* input samples than zone length. */
    }
    if(nproct>=maxtraces) err("error: Number of input traces is greater than maxtraces=%d",maxtraces);

    for (i=0; i<num_skeyloc; ++i) sguy[i] = floor(fromhead(tr,skaseloc[i]) / sdivider[i]);

/* Note that num_keyloc is inside compareii, which is inside bhighii, so it   */
/* must be reset for whichever list is being searched.                        */

    num_keyloc = num_skeyloc;
    jhere = bhighii(sunqlocs, lastsource, sguy);

/* Is this the same location as already encountered ?                         */
/* (If jhere=0 then it is lower than any already encountered).                */

    if(jhere>0 && compareii(sguy,sunqlocs[jhere-1]) == 0) { 
/* No coordinates to check in this program. But keep code structure same as surescsv. */
    }
    else { /* This is a new combination of keyloc values                      */

      if(lastsource>=maxsources) 
        err("error: At input trace=%d  number of unique skeyloc combinations is greater than maxsources=%d",nproct,maxsources);

/* Pull pointers and values down to accomodate new location, get new memory.  */

      for (i=lastsource; i>=jhere; --i) sunqlocs[i+1] = sunqlocs[i];
      sunqlocs[jhere]  = ealloc1int(num_skeyloc);

/* Copy the values for the new location into new memory.                      */

      for (i=0; i<num_skeyloc; ++i) sunqlocs[jhere][i] = sguy[i];

      lastsource++;
    }  /* end of  if(jhere>0 && compareii(sguy,sunqlocs[jhere-1]) == 0) { */

/* Same for receivers.  */

    for (i=0; i<num_rkeyloc; ++i) rguy[i] = floor(fromhead(tr,rkaseloc[i]) / rdivider[i]);
    num_keyloc = num_rkeyloc; 
    khere = bhighii(runqlocs, lastreceiver, rguy);

    if(khere>0 && compareii(rguy,runqlocs[khere-1]) == 0) { 
/* No coordinates to check in this program. But keep code structure same as surescsv. */
    }
    else { 
      if(lastreceiver>=maxreceivers) 
        err("error: At input trace=%d  number of unique rkeyloc combinations is greater than maxreceivers=%d",nproct,maxreceivers);

      for (i=lastreceiver; i>=khere; --i) runqlocs[i+1] = runqlocs[i];
      runqlocs[khere]  = ealloc1int(num_rkeyloc);

      for (i=0; i<num_rkeyloc; ++i) runqlocs[khere][i] = rguy[i];

      lastreceiver++;
    }  /* end of  if(khere>0 && compareii(rguy,runqlocs[khere-1]) == 0) { */

    allsamps[nproct] = ealloc1float(lensam);
    for (i=0; i<lensam; ++i) allsamps[nproct][i] = tr.data[i];

/* Allocate and store source location values for each trace                   */

    strclocs[nproct] = ealloc1int(num_skeyloc);
    for (i=0; i<num_skeyloc; ++i) strclocs[nproct][i] = sguy[i]; 

/* Allocate and store receiver location values for each trace                 */

    rtrclocs[nproct] = ealloc1int(num_rkeyloc);
    for (i=0; i<num_rkeyloc; ++i) rtrclocs[nproct][i] = rguy[i]; 

    nproct++;
  } /* end of  while(gettr(&tr)) {  */

/* Finished with trace input. And source and receiver identification.         */
/* Loop over the individual trace identifiers and find where they are in the  */
/* unique identifier lists.                                                   */

  for (j=0; j<nproct; ++j) {

/* Note that bhighii returns 1 greater when equal. And since    */
/* the unique list has all individual values, always subtract 1.*/

    num_keyloc = num_skeyloc; 
    jhere = bhighii(sunqlocs, lastsource, strclocs[j]) - 1;  

    num_keyloc = num_rkeyloc; 
    khere = bhighii(runqlocs, lastreceiver, rtrclocs[j]) - 1;  

    sndx[j].ndxj = jhere;  /* source index of this trace   */
    sndx[j].ndxk = khere;  /* receiver index of this trace */ 

  } /* end of   for (j=0; j<nproct; ++j) {  */

/*--------------------------------------------------------------------------  */

/* Begin output of traces */

/* Allocate the closest thing that c has to a 2-dimensional array.            */

  jkzone = (int **)ealloc1(sizeof(int*),lastsource);
  for (j=0; j<lastsource; ++j) jkzone[j] = ealloc1int(lastreceiver);

/* Set all the stored zone indexes to un-occupied.                            */

  for (j=0; j<lastsource; ++j) {
    for (k=0; k<lastreceiver; ++k) jkzone[j][k] = -1;
  }

  for (i=0; i<nproct; ++i) {
     j = sndx[i].ndxj;
     k = sndx[i].ndxk;
     if(jkzone[j][k] != -1) notunique++;
     jkzone[j][k] = i;
  }
  
  kbeg = 0;
  kend = lastreceiver;

  for(n=0; n<lastsource; n+=numzone) { 

    nend = n + numzone;
    if(nend>lastsource) nend = lastsource;

    if(rfill<2) {
      kbeg = -1;
      kend = -1;
      for(k=0; k<lastreceiver; k++) { 
        for(j=n; j<nend; j++) { 
          if(jkzone[j][k] > -1) {
            if(kbeg == -1) kbeg = k;
            kend = k+1;
          }
        }
      }
    }
 
/* Set output source key location values to source values of the top zone.    */

    for (i=0; i<num_skeyloc; ++i) tohead(&trz,skaseloc[i],sunqlocs[n][i] * sdivider[i]);

    for(k=kbeg; k<kend; k++) { 
      ifold = 0;
      for(i=0; i<lenzone * numzone; i++) trz.data[i] = 0.;
      for(j=n,m=0; j<nend; j++,m++) { 
        if(jkzone[j][k] > -1) {
          ifold++;
          for(i=0; i<lensam; i++) trz.data[i+m*lenzone] = allsamps[jkzone[j][k]][i];
        }
      }
      if(ifold>0 || rfill>0) {

/* Set output receiver key location values to the receiver of all the zones.  */

        for (i=0; i<num_rkeyloc; ++i) tohead(&trz,rkaseloc[i],runqlocs[k][i] * rdivider[i]);
        trz.nhs = ifold; 
        puttr(&trz);
      } 
    }

  }

  warn("Unique combinations of:  skeyloc values=%d   rkeyloc values=%d",lastsource,lastreceiver);
  if(notunique > 0) warn("There were %d traces that REPLACED another trace in the SAME matrix location.",notunique);

  return(CWP_Exit());
}

/* -------------------------------------------------------------------------- */
/* Specify compare function for bhighii function.                             */

int compareii (const void * q1, const void * q2) {
  
  int n=0; 

  int* p1 = (int*) q1;
  int* p2 = (int*) q2;

  for(n=0; n<num_keyloc; n++) {  
    if(p1[n] < p2[n]) return (-1);
    if(p1[n] > p2[n]) return (1); 
  }

  return (0); 

}

/* ---------------------------------------------------------------------------*/
/* This is just a standard binary search which returns 1 above exact match    */
/* and stays there for exact+0.1                                              */
/* But note that, unlike other bhigh functions I have written, the argument   */
/* int **all is not a contiguous set of structures referenced from            */
/* a single pointer. It is an array of pointers (thus all[mid] is specified). */

int bhighii(int **all, int last, int *guy) {

  int mid;
  int low = 0;
  int high = last;

  while (low < high) {
    mid = low + (high - low) / 2;
    if (compareii(guy,all[mid]) >= 0) low = mid +1;
    else high = mid;
  }

  return low;
}

