/* Copyright (c) Colorado School of Mines, 2022.*/
/* All rights reserved.                       */

/* SUNSZONECSV: $Revision: 1.01 $ ; $Date: 2024/06/25 00:00:01 $		*/
 
#include "su.h"
#include "segy.h" 
#include <stdbool.h>
#include "qdefine.h"
#include "headcase.h"


/*********************** self documentation ******************************/
char *sdoc[] = {
"									     ",
" SUNSZONECSV - Near Surface Zone Extract and Shift                          ",
"									     ",
"  sunszonecsv [parameters].                                                 ",
"									     ",
" Parameters:	         						     ",
"                                                                            ",
" qin=      Input file containing q-records. This parameter is required.     ",
"									     ",
" qx=sx         Name of X coordinate at points in the q-file.                ",
"									     ",
" qy=sy         Name of Y coordinate at points in the q-file.                ",
"									     ",
" qvw=vw        Name of top layer velocity at points in the q-file.          ",
"									     ",
" qzw=zw_true   Name of top layer thickness at points in the q-file.         ",
"									     ",
" qvb=vb        Name of second layer velocity at points in the q-file.       ",
"									     ",
" qzb=zb_true   Name of second layer thickness at points in the q-file.      ",
"    =unknown   Second layer has unknown thickness.                          ",
"   ***   Note: Once unknown is specified (or defaults) for a thickness,     ",
"               parameters for deeper layers are not used.                   ",
"									     ",
" qvc=vc        Name of third layer velocity at points in the q-file.        ",
"									     ",
" qzc=zc_true   Name of third layer thickness at points in the q-file.       ",
"    =unknown   Third layer has unknown thickness.                           ",
"                                                                            ",
" qvd=vd        Name of fourth layer velocity at points in the q-file.       ",
"									     ",
" locs=0        Use near surface model from shot and receiver ends of traces.",
"     =1        Use near surface model from shot end only.                   ",
"     =-1       Use near surface model from receiver end only.               ",
"         Note: The model is assumed to be flat at both ends, and raypaths   ",
"               are assumed to be straight with a linear change in layer     ",
"               velocities from shot to receiver. The distance between the   ",
"               shot and receiver ends of the raypaths are adjusted to take  ",
"               differences in elevation and layer thicknesses into account  ",
"               and the shot hole depth is used at the shot end of the paths.",
"               The shot HOLE DEPTHS and the ELEVATION DIFFERENCES ARE USED  ",
"               REGARDLESS of which option is chozen here. Option 1 simply   ",
"               uses the velocities and thicknesses of the shot location     ",
"               for both ends of the raypaths. Option -1 simply uses the     ",
"               velocities and thicknesses of the receiver location for      ",
"               both ends of the raypaths.                                   ",
"									     ",
" tpath=0       Travel path shift value.                                     ",
"               The default (0) is to set the shift to the minimum time of   ",
"               the raypaths computed for all layers and the direct arrival  ",
"               path (also known as the first arrival or first break time).  ",
"      =1       Set shift to top-of-first layer direct arrival time.         ",
"      =2       Set shift to top-of-second layer refraction arrival time.    ",
"      =3       Set shift to top-of-third layer refraction arrival time.     ",
"      =4       Set shift to top-of-fourth layer refraction arrival time.    ",
"									     ",
" tapp=1        Apply the total time shift determined by the tpath option    ",
"               and also set the sstat,gstat,tstat,sec key values.           ",
"               The sstat is set to the time computed for the shot end of    ",
"               the path, the gstat is set to the time computed for the      ",
"               receiver end of the path, and tstat is set to time computed  ",
"               for the adjusted travelpath between shot and receiver.       ",
"               If tpath=0 the sec key is set to the layer number that       ",
"               produced the minimum travel path shift value, otherwise it   ",
"               is set to the same layer number as the tpath parameter.      ",
"      =0       Apply the time shift determined by the tpath option          ",
"               but do not alter the sstat,gstat,tstat,sec key values.       ",
"      =-1      Do not shift. Only set the stat,gstat,tstat,sec key values.  ",
"         Note: The shift is done by the same routine that does shifts for   ",
"               statics and therefor applies fractional millisecond values   ",
"               BUT sstat,gstat,tstat keys are set to nearest millisecond.   ",
"									     ",
" tbulk=200.0   Shift output traces by this additional time in milliseconds  ",
"               (except if tapp=-1). This allows seeing more of the data     ",
"               before the computed refraction wavelet times. This has no    ",
"               effect on computations or sstat,gstat,tstat,sec values.      ",
"									     ",
" apply=-1      Shift Positive trace times towards zero time.                ",
"        1      Shift Positive trace times away from zero time.              ",
"         Note: The concept here is that this parameter allows you to        ",
"               flatten refraction arrivals, apply some data processing,     ",
"               then inverse the flattening.                                 ",
"         Note: In both cases the shift is computed from model (and tbulk),  ",
"               the input sstat,gstat,tstat,sec key values are not used.     ",
"									     ",
"   ***  The trace header key values sx,sy,selev,sdepth,gx,gy,gelev are      ",
"        always accessed from the trace headers. The nearest velocities      ",
"        and thicknesses are accessed from the qin file. Elevations and      ",
"        shot hole depths are only accessed from traces, not from qin file.  ",
"									     ",
"   ***  Input trace header values are scaled using scalco and scalel keys.  ",
"        But q-file standards means qin values should have their true        ",
"        magnitudes and therefor do not get scaled by this program.          ",
"									     ",
"									     ",
" sdist=100 Initial search distance. If positive, add this value to the      ",
"           distance between the previous trace and its nearest qin point    ",
"           and used as the initial search distance for the current trace.   ",
"           If negative, the initial search distance for all traces is set   ",
"           to this absolute value.                                          ",
" smult=2   Search multiplier.                                               ",
"           If the nearest qin point is not found after searching with the   ",
"           initial search distance, the search distance is multiplied by    ",
"           this value, and search is performed again. This repeats until    ",
"           finding the nearest point (or all min,max ranges are exceeded).  ",
"									     ",
"   ------------------------------------------------------------------       ",
"   ------------------------------------------------------------------       ",
"									     ",
NULL};

/* Created: June 2024: Andre Latour                                          */ 
/* This program started from sunearcsv.                                      */ 
/**************** end self doc *******************************************/

segy tr;

struct QInfo *RecInfo; /* Storage for all function location value pointers */

/* Note: I make no claim that this is a particularly good kd tree implementation.     */
/*       It is not explicitly balanced (it has an option to get approximate balance). */
/* Note: tree_nt is unlikely to exist in other kd tree implementations.               */
/*       It exists because some crooked-profiles (land) or coil-profiles (marine)     */
/*       curve back-over-top-of-themselves. These self-intersections mean the profile */
/*       is not a function (in the mathematical sense). That is, just considering XYs */
/*       a trace midpoint can get confused as to which part of the profile it should  */
/*       belong to. Similar issues arise if the profile just curves back NEAR itself. */

typedef struct node {
   unsigned long long elem; 
   struct node * l;
   struct node * r;
} node;

/* Note: Yes, I could also have made a structure named tree, and then passed it into  */
/*       the functions instead of some of the individual arguments. That would have   */
/*       reduced the function arguments, but make it more confusing for new coders.   */

void connect_nodes (node *tree_nodes, unsigned long long tree_numc, double **tree_dl, int tree_numd,
                   int ihop);

void connect_all (node *tree_nodes, unsigned long long tree_numc, double **tree_dl, int tree_numd);

void find_near_rcur (double **tree_dl, int *tree_nt, int tree_numd, node *now_node, int naxe, 
                    double *extent_min, double *extent_max, double *target,
                    unsigned long long *near_elem, double *near_dist, unsigned long long *num_found);

void cycle_for_near (node *tree_nodes, double **tree_dl, int *tree_nt, int tree_numd,
                    double *extent_min, double *extent_max, double *target, 
                    double init_rad, double rad_scal,  
                    unsigned long long *near_elem, double *near_dist, 
                    unsigned long long *num_found, int *ncycles);

/*----------------------------------------------------------------------*/

int main(int argc, char **argv) {

  int ncdp = 0;		/* number of cdps specified */

  int ifixd = 0;          /* flag for all tuples same size or vary   */
  int iztuple = 0;        /* element number where first tuple exists */
  int ktuple = 0;         /* type of tuples (2=pairs, 3=triplets)    */

  cwp_String Pname=NULL;  /* text file name for Q input file      */
  FILE *fpP=NULL;         /* file pointer for Q input file        */

  cwp_String *pname = NULL;                                         
  cwp_String *ndims = NULL;                                                 
  int numpname = 9;
  int nlayer   = 1;

  double *pindepa = NULL;                                               
  int numdind = 0;
	
  int i = 0;                                                                         
  int j = 0;                                                                       
  int errwarn = 0; 

  int tree_numd = 2; 

  double sdist = 100.0;
  double smult = 2.;
  int sdadd   = 1;
  double sdist2 = 100.0;
  double gdist = 100.0;
  double gmult = 2.;
  int gdadd   = 1;
  double gdist2 = 100.0;
  int ncycles = 0;

  unsigned long long near_s = 0;
  unsigned long long near_g = 0;
  unsigned long long num_found = 0;
  double near_dist_s = 0.;
  double near_dist_g = 0.;
  int ihop = 1;
  int nproct = 0;

  double **tree_dl = NULL;
  int *tree_nt = NULL;
  double *extent_min = NULL;
  double *extent_max = NULL;
  double tars[2];
  double targ[2];
  double poff = 0.0;
  unsigned long long tree_ncdp = 0;
  node *tree_nodes = NULL;

  int locs   = 0;
  int iapply = -1;
  int tpath  = 0;
  int tapp   = 1;
  double tbulk = 200.0;
  int ilayer = 0;
  int lentrc = 0;
  float *onetrc=NULL;
  float *oneshift=NULL;
  float sampi=0.0;
  double tshift = 0.0;
  double ptm[4]; /* Note current max nlayer is 3. But +1 needed here.         */
  double stm[4];
  double gtm[4];
  double eps[4];
  double zps[4];
  double vls[4];
  double epg[4];
  double zpg[4];
  double vlg[4];
  double shole = 0.0;

  cwp_String *iname = NULL;                                         
  int numiname = 9;
  int locn[9];

/* hook up getpar */
  initargs(argc, argv);
  requestdoc(1);

/* Maximum dimensions in tree is 9 (just using 2 in this program)             */

  tree_nt = ealloc1int(2);
  extent_min = ealloc1double(2);
  extent_max = ealloc1double(2);
  tree_dl = ealloc1(2,sizeof(double *));

/* Set extent ranges to maximum. And flag to use Pythagorean Nearest          */          

  for(i=0; i<2; i++) {
    extent_min[i] = -DBL_MAX;
    extent_max[i] =  DBL_MAX;
    tree_nt[i] = 1; 
  }

  if(!getparint("locs",&locs)) locs = 0;
  if(locs<-1 || locs>1)  err("**** Error: locs parameter out-of-range."); 

  if(!getparint("apply",&iapply)) iapply = -1;
  if(iapply!=-1 && iapply!=1)  err("**** Error: apply parameter out-of-range."); 

  if(!getpardouble("sdist",&sdist)) sdist = 100.;
  if(sdist==0.)  err("**** Error: sdist cannot be 0."); 

  if(!getpardouble("smult",&smult)) smult = 2.;
  if(smult<0.)  err("**** Error: smult must be non-negative."); 

  if(sdist<0.) {
    sdist = 0. - sdist;
    sdadd = 0;
  }
  sdist2 = sdist;

/* Use same options for receivers */ 

  gmult  = smult;
  gdadd  = sdadd;
  gdist  = sdist;
  gdist2 = sdist2;

/*--------------------------------------------------------------------------  */

  getparstring("qin", &Pname);

  fpP = fopen(Pname, "r");
  if(fpP==NULL) err("error: input Q-file did not open correctly.");
    
/* Set input numpname,pname to store everything in q-file.                    */
/* numpname>0 is a flag to ONLY store values if they are on pname list.       */
/* numpname<1 is a flag to NOT store values if they are on pname list.        */

  pname = ealloc1(9,sizeof(cwp_String *));

  if(countparval("qx") > 0) {
    getparstring("qx",pname);
  }
  else {
    pname[0] = ealloc1(2,1);
    strcpy(pname[0],"sx");
  }
  
  if(countparval("qy") > 0) {
    getparstring("qy",pname+1);
  }
  else {
    pname[1] = ealloc1(2,1);
    strcpy(pname[1],"sy");
  }
  
  if(countparval("qvw") > 0) {
    getparstring("qvw",pname+2);
  }
  else {
    pname[2] = ealloc1(2,1);
    strcpy(pname[2],"vw");
  }
  
  if(countparval("qzw") > 0) {
    getparstring("qzw",pname+3);
  }
  else {
    pname[3] = ealloc1(7,1);
    strcpy(pname[3],"zw_true");
  }
  
  if(countparval("qvb") > 0) {
    getparstring("qvb",pname+4);
  }
  else {
    pname[4] = ealloc1(2,1);
    strcpy(pname[4],"vb");
  }
  
  if(countparval("qzb") > 0) {
    getparstring("qzb",pname+5);
  }
  else {
    pname[5] = ealloc1(7,1);
    strcpy(pname[5],"zb_true");
  }
  
  if(countparval("qvc") > 0) {
    getparstring("qvc",pname+6);
  }
  else {
    pname[6] = ealloc1(2,1);
    strcpy(pname[6],"vc");
  }
  
  if(countparval("qzc") > 0) {
    getparstring("qzc",pname+7);
  }
  else {
    pname[7] = ealloc1(7,1);
    strcpy(pname[7],"zc_true");
  }
  
  if(countparval("qvd") > 0) {
    getparstring("qvd",pname+8);
  }
  else {
    pname[8] = ealloc1(2,1);
    strcpy(pname[8],"vd");
  }
  
/* Note nlayer is last FULL layer. But nlayer+1 has velocity and traveltimes. */

  nlayer = 3;
  numpname = 9;
  if(strcmp(pname[7],"unknown")==0) {
    nlayer = 2;
    numpname = 7;
  }
  if(strcmp(pname[5],"unknown")==0) {
    nlayer = 1;
    numpname = 5;
  }

  if(!getparint("tpath",&tpath)) tpath = 0;
  if(tpath<0 || tpath>nlayer+1)  err("**** Error: tpath parameter out-of-range."); 

  if(!getparint("tapp",&tapp)) tapp = 1;
  if(tapp<-1 || tapp>1)  err("**** Error: tapp parameter out-of-range."); 

  if(!getpardouble("tbulk",&tbulk)) tbulk = 200.0;

/* Note: positive numpname does not mean values come back in pname order.     */
/* Note: and returned numpname is something else, so set numiname.            */

  iname = ealloc1(numpname,sizeof(cwp_String *));
  numiname= numpname;
  for(i=0; i<numpname; i++) {
    iname[i] = ealloc1(strlen(pname[i]),1);
    strcpy(iname[i],pname[i]);
  }

  getviaqfile(fpP, &pname, &numpname, &iztuple, numdind,   
              &ktuple, &ifixd, &RecInfo, &ncdp, 
              &pindepa,  &ndims, &errwarn) ;

  if(errwarn==1) err("getqinfo error: extra C_SU_NAMES record in q-file");
  else if(errwarn==2) err("getqinfo error: extra C_SU_NDIMS record in q-file");
  else if(errwarn==3) err("getqinfo error: C_SU_ID record not found immediately after C_SU_NAMES record.");
  else if(errwarn==11) 
    err("readqhead error: if C_SU_NDIMS not vary, its numbers must align with C_SU_NAMES");
  else if(errwarn==12) 
    err("readqhead error: C_SU_ID record not found immediately after C_SU_NAMES record.");
  else if(errwarn==22) err("getviaqfile error: C_SU_NDIMS record not same length as C_SU_NAMES record.");
  else if(errwarn==23) err("getviaqfile error: C_SU_NAMES tupled names out-of-order, or accidental duplicate name");
  else if(errwarn==24) err("getviaqfile error: C_SU_NDIMS blank where valid number expected");
  else if(errwarn==25) err("getviaqfile error: C_SU_NDIMS non-number where valid number expected");
  else if(errwarn==26) err("getviaqfile error: C_SU_NDIMS value must be same for all members of tuple");
  else if(errwarn==27) err("getviaqfile error: C_SU_NAMES record followed by C_SU_ID record not found.");
  else if(errwarn>100) 
    err("getviaqfile error: record %d (wrong comma count, damaged, non-numbers, ...)",errwarn-99);
  else if(errwarn>0) err("getviaqfile error: unrecognized error code %d",errwarn);


  checkpars(); 

  if(ifixd==0) err("error: input with varying number of tuples is not allowed.");

  for(i=0; i<numiname; i++) {
    locn[i] = -1;
    for(j=0; j<iztuple; j++) {
      if(strcmp(pname[j],iname[i])==0) locn[i] = j;
    }
    if(locn[i] < 0) err("qin file error: name %s not found in non-tuple part of qin Q-file.",iname[i]);
  }

/*--------------------------------------------------------------------------  */

  tree_ncdp = ncdp;
  tree_nodes = ealloc1(tree_ncdp,sizeof(node));

  tree_dl[0] = ealloc1double(tree_ncdp);
  for(j=0; j<tree_ncdp; j++) tree_dl[0][j] = RecInfo[j].dlots[locn[0]];
  tree_dl[1] = ealloc1double(tree_ncdp);
  for(j=0; j<tree_ncdp; j++) tree_dl[1][j] = RecInfo[j].dlots[locn[1]];

  ihop = 1;
  connect_nodes (tree_nodes, tree_ncdp, tree_dl, tree_numd, ihop);

/*--------------------------------------------------------------------------  */

  if (!gettr(&tr))  err("Error: cannot get first trace");

  lentrc = tr.ns;
  onetrc = ealloc1float(lentrc);
  oneshift = ealloc1float(lentrc);
  sampi = ((float)tr.dt) / 1000.0;

/* loop over traces   */ 

  do {

    tars[0] = (double)(tr.sx);
    tars[1] = (double)(tr.sy);
    eps[0]  = (double)(tr.selev);
    shole   = (double)(tr.sdepth);

    targ[0] = (double)(tr.gx);
    targ[1] = (double)(tr.gy);
    epg[0]  = (double)(tr.gelev);

    if(tr.scalco > 1) {
      tars[0] *= tr.scalco;
      tars[1] *= tr.scalco;
      targ[0] *= tr.scalco;
      targ[1] *= tr.scalco;
    }
    else if(tr.scalco < 0) {
      tars[0] /= -tr.scalco;
      tars[1] /= -tr.scalco;
      targ[0] /= -tr.scalco;
      targ[1] /= -tr.scalco;
    }
        
    if(tr.scalel > 1) {
      eps[0] *= tr.scalel;
      shole  *= tr.scalel;
      epg[0] *= tr.scalel;
    }
    else if(tr.scalel < 0) {
      eps[0] /= -tr.scalel;
      shole  /= -tr.scalel;
      epg[0] /= -tr.scalel;
    }

/* Use shot XYs in tars[0], tars[1] to find nearest model location.           */

    if(locs>=0) {
      if(sdadd==1) sdist2 = sdist + sqrt(near_dist_s);
       cycle_for_near(tree_nodes,tree_dl,tree_nt,tree_numd,
                     extent_min, extent_max, tars,
                     sdist2, smult,
                     &near_s, &near_dist_s, &num_found, &ncycles);

/* Note that nlayer is the number of thicknesses. There is an extra velocity. */
/* The shot point elevation eps[0] is from trace header.                      */

      for(i=0; i<nlayer+1; i++) vls[i] = RecInfo[near_s].dlots[locn[2+i*2]];
      for(i=0; i<nlayer;   i++) zps[i] = RecInfo[near_s].dlots[locn[3+i*2]];
      for(i=1; i<nlayer+1; i++) eps[i] = eps[i-1] - zps[i-1];
    }

/* Use receiver XYs in targ[0], targ[1] to find nearest model location?       */

    if(locs<=0) {

      if(gdadd==1) gdist2 = gdist + sqrt(near_dist_g);
      cycle_for_near(tree_nodes,tree_dl,tree_nt,tree_numd,
                     extent_min, extent_max, targ,
                     gdist2, gmult,
                     &near_g, &near_dist_g, &num_found, &ncycles);

      for(i=0; i<nlayer+1; i++) vlg[i] = RecInfo[near_g].dlots[locn[2+i*2]];
      for(i=0; i<nlayer  ; i++) zpg[i] = RecInfo[near_g].dlots[locn[3+i*2]];
      for(i=1; i<nlayer+1; i++) epg[i] = epg[i-1] - zpg[i-1];
    } 

/* Use model from shot. But receiver elevation epg[0] still from trace header.*/

    if(locs==1) {
      for(i=0; i<nlayer+1; i++) vlg[i] = vls[i];
      for(i=0; i<nlayer;   i++) zpg[i] = zps[i];
      for(i=1; i<nlayer+1; i++) epg[i] = epg[i-1] - zps[i-1];
    }

/* Use model from receiver. But shot elevation eps[0] still from trace header.*/

    if(locs==-1) {
      for(i=0; i<nlayer+1; i++) vls[i] = vlg[i];
      for(i=0; i<nlayer;   i++) zps[i] = zpg[i];
      for(i=1; i<nlayer+1; i++) eps[i] = eps[i-1] - zpg[i-1];
    }

/* Compute the time between the paths ends. This is distance between shot and */
/* receiver considering differences in X,Y, and top-of-layer depths divided   */
/* by the average velocity of each top-of-layer at the shot and the receiver. */
/* (Averaging these two is same as linear velocity change from one to other). */

    poff = (tars[0]-targ[0])*(tars[0]-targ[0]) + (tars[1]-targ[1])*(tars[1]-targ[1]); 

    for(i=0; i<nlayer+1; i++) ptm[i] = sqrt(poff + (eps[i]-epg[i])*(eps[i]-epg[i])) / ((vls[i]+vlg[i])/2.0);

/* Adjust thicknesses for shot-end-time-computation considering hole depth.   */

    if(shole>0.0) {
      for(i=0; i<nlayer; i++) {                                                   
        if(shole>=zps[i]) {
          shole -= zps[i];
          zps[i] = 0.0;
        }
        else {
          zps[i] -= shole;
          break;
        }
      }
    }

/* Compute the shot-end-time and receiver-end-time.                           */

    for(i=0; i<nlayer+1; i++) {
      stm[i] = 0.0;
      gtm[i] = 0.0;
      for(j=0; j<i; j++) {
        stm[i] += ( zps[j] * sqrt(vls[i]*vls[i] - vls[j]*vls[j]) ) / vls[j]; 
        gtm[i] += ( zpg[j] * sqrt(vlg[i]*vlg[i] - vlg[j]*vlg[j]) ) / vlg[j]; 
      }
      stm[i] /= vls[i];
      gtm[i] /= vlg[i];
    }
    
    if(tpath==0) {
      tshift = stm[0]+ptm[0]+gtm[0]; /* stm[0] and gtm[0] are 0.0 in this version, but... */
      ilayer = 0;
      for(i=1; i<nlayer+1; i++) {
        if(stm[i]+ptm[i]+gtm[i] < tshift) {
          tshift = stm[i]+ptm[i]+gtm[i];
          ilayer = i;
        }
      } 
    }
    else {
      tshift = stm[tpath-1]+ptm[tpath-1]+gtm[tpath-1];
      ilayer = tpath-1;
    }

    if(tapp>-1) {
      for (i=0; i<lentrc; i++) {
        onetrc[i] = tr.data[i];
        oneshift[i] = (float) i - iapply*(1000.*tshift-tbulk)/sampi;
      }    
      ints8r(lentrc, 1.0, 0.0, onetrc, 0.0, 0.0, lentrc, oneshift, tr.data);
    }

    if(tapp!=0) {                        
      tr.sstat = stm[ilayer] * 1000. + 0.5000000001; 
      tr.gstat = gtm[ilayer] * 1000. + 0.5000000001;
      tr.tstat = ptm[ilayer] * 1000. + 0.5000000001;
      tr.sec   = ilayer + 1;                    
    }

    puttr(&tr);
    nproct++;

  } while (gettr(&tr)); 

  warn("Number of traces=%d ",nproct);

  return 0;

} /* end of main for sunszonecsv */

/* --------------------------------------------------------------------------------------------------- */

void connect_nodes (node *tree_nodes, unsigned long long tree_numc, 
                    double **tree_dl, int tree_numd, int ihop) {

/*          This function connects the already allocated tree nodes.                                   */     
/*                                                                                                     */     
/* Input arguments:                                                                                    */     
/* tree_nodes    The already fully allocated tree nodes.                                               */     
/* tree_numc     Number of nodes in tree_nodes.                                                        */     
/* tree_dl       Pointers to first element of values of each dimension. That is, each coordinate is in */
/*               a separate contiguous array. These are the pointers to the start of those arrays.     */     
/*               Those arrays have size tree_numc.                                                     */     
/* tree_numd     Number of pointers in tree_dl (i.e. number of dimensions).                            */     
/* ihop      = 1 means process the points in dispersed order (approximately random).                   */     
/*               The worst search times for kd trees occur when there are just a few long branches.    */     
/*               This happens in simple kd tree code if the points are ordered in increasing or        */     
/*               decreasing coordinates. Unfortunately, that is often the case for survey files.       */     
/*               The literature about kd trees explains how to create trees with branches of the same  */     
/*               depth (perfectly balanced trees). That is not done here. Instead, this option hops    */     
/*               through the points and loads them in more-or-less random order. This creates a        */     
/*               reasonably-balanced tree (unless you are very unlucky).                               */     
/*               Note that seriously unbalanced trees still work, but they may be very slow.           */     
/*           = 0 process the points in the order they exist within their arrays.                       */     

  unsigned long long nrat = 0;
  unsigned long long nd = 0;
  unsigned long long n = 0;
  unsigned long long m = 0;

  for(m=0; m<tree_numc; m++) {
    tree_nodes[m].l    = 0; 
    tree_nodes[m].r    = 0;  
    tree_nodes[m].elem = m;
  }

/* nrat,nd and the for-loops are just heuristically set (I made them up).         */
/* The basic concept is: Survey points are usually in order. So, we want to load  */
/* them in big hops so that the top nodes will have far-apart coordinates. Then   */
/* reduce hop size so next deeper nodes will also be dispersed nicely. And so on. */

  if(ihop==1) {
    m = 0;
    for (nrat=tree_numc; nrat>0; nrat=nrat*0.6) {
      if(nrat>6) nd = sqrt((double) nrat); 
      else nd = 0;
      for (n=nd+nrat/2; n<tree_numc; n=n+nd+nrat) {
        if (tree_nodes[n].l == 0) {     /* pointer value just used here as a flag */
          tree_nodes[n].l = tree_nodes; /* (so we know we already did this point) */
          tree_nodes[m].elem = n;
          m++;
        }
      }  
    }  
    for(m=0; m<tree_numc; m++) tree_nodes[m].l = 0; /* un-flag it */  
  }

  connect_all(tree_nodes,tree_numc,tree_dl,tree_numd);

  return;

}

/* --------------------------------------------------------------------------------------------------- */

void connect_all(node *tree_nodes, unsigned long long tree_numc, double **tree_dl, int tree_numd) {

/*          This function connects the already allocated tree nodes in their order in elem.            */     
/*                                                                                                     */     
/* Input arguments:                                                                                    */     
/* tree_nodes    The already fully allocated tree nodes.                                               */     
/* tree_numc     Number of nodes in tree_nodes.                                                        */     
/* tree_dl       Pointers to first element of values of each dimension. That is, each coordinate is in */     
/*               a separate contiguous array. These are the pointers to the start of those arrays.     */     
/*               Those arrays have size tree_numc.                                                     */     
/* tree_numd     Number of pointers in tree_dl (i.e. number of dimensions).                            */     
/*                                                                                                     */     
/* Note that, technically, this function processes the nodes in their sequential order in tree_nodes.  */
/* But the coordinates of each node are pointed to via node->elem, which means the points              */
/* are actually loaded into the tree in a different order than the order in their arrays.              */

  unsigned long long m = 1; 
  int naxe = 0;

  for(m=1; m<tree_numc; m++) { /* First m is 1 since tree_nodes[0] is set correct.  */ 

    node *now_node, *next_node, *m_node;

    now_node  = NULL; /* just to prevent compiler warning of possible uninitialed use.*/
    m_node    = tree_nodes + m; /* the m-th node to connect                           */ 
    next_node = tree_nodes;     /* walk down tree from top for each node to connect   */

    naxe = 0;
    while(next_node!=0) {
      now_node = next_node;

      naxe++;
      if(naxe==tree_numd) naxe = 0;

      if(tree_dl[naxe][m_node->elem] < tree_dl[naxe][now_node->elem]) 
           next_node = now_node->l;
      else next_node = now_node->r;
    }

    if(tree_dl[naxe][m_node->elem] < tree_dl[naxe][now_node->elem]) 
         now_node->l = m_node; 
    else now_node->r = m_node;

  }

  return;
}

/* --------------------------------------------------------------------------------------------------- */

void find_near_rcur (double **tree_dl, int *tree_nt, int tree_numd, node * now_node, int naxe, 
                     double *extent_min, double *extent_max, double *target, 
                     unsigned long long *near_elem, double *near_dist, unsigned long long *num_found) {

/*          This function finds a nearest point between specified extents of the dimensions.           */     
/*          This function is usually slower for wide extents compared to function cycle_for_near.      */
/*                                                                                                     */     
/* Input arguments:                                                                                    */     
/* tree_dl       Pointers to first element of values of each dimension. That is, each coordinate is in */     
/*               a separate contiguous array. These are the pointers to the start of those arrays.     */     
/*               Those arrays have size tree_numc.                                                     */     
/* tree_nt       Near type flags. 1 means standard pythagorean nearest. See note for what 0 means.     */     
/* tree_numd     Number of pointers in tree_dl and flags in tree_nt (i.e. number of dimensions).       */     
/* extent_min    Minimum value of this dimension to find nearest point. Size tree_numd.                */     
/*               Greater OR EQUAL to this value is in range.                                           */     
/* extent_max    Maximum value of this dimension to find nearest point. Size tree_numd.                */     
/*               Strictly LESS than this value is in range.                                            */     
/* target        Find the point nearest here considering extents and tree_nt. Size tree_numd.          */     
/*                                                                                                     */     
/* Input/Output arguments:                                                                             */     
/* now_node      The current node to start from.                                                       */     
/* naxe          The current splitting dimension.                                                      */     
/*                                                                                                     */     
/* Output arguments:                                                                                   */     
/* near_elem     The element number of a nearest point in the tree_dl arrays. For instance, a value of */     
/*               m means the coordinates of point are tree_dl[n][m] where n is the dimension number.   */     
/* near_dist     The SQUARED distance between the target and nearest point (if num_found>0).           */
/* num_found     0 if no point is found within the extent ranges.                                      */
/*              >0 is as many equally-near points as are found within the extent ranges.               */
/*                 The returned near_elem is the highest-numbered elem of all the nearest points.      */
/*                                                                                                     */     
/*    NOTE: A tree_nt value of 0 means the difference between the point coordinate and the target      */     
/*          coordinate are not added to the Pythagorean sum for the specified dimension(s).            */     
/*          So, the point distances are determined as if that dimension was not specified at all.      */     
/*          However, the extent_min,extent_max values for that dimension are still used.               */     
/*          The result is therefor the nearest point considering only the other dimensions, but        */     
/*          still restricted by the extent range of that dimension(s).                                 */     

  if(now_node==0) return; 

  bool in = true;          
  int i = 0;
  double rad = 0.;
 
  for(i=0; i<tree_numd; i++) {
    if(tree_dl[i][now_node->elem] < extent_min[i] || tree_dl[i][now_node->elem] >= extent_max[i]) { 
      in = false;
      break;
    }
  }

  if(in) { 
    rad = 0.;
    for(i=0; i<tree_numd; i++) {
      if(tree_nt[i] != 0) {
        rad += (target[i]-tree_dl[i][now_node->elem]) * (target[i]-tree_dl[i][now_node->elem]);
      }
    }

/* The following set of ifs could be done differently. But I think coding it  */
/* this way saves cpu time by immediately rejecting the > cases using 1 if.   */
/* But who knows how modern optimizers will alter this.                       */

    if(rad <= *near_dist) {
      if(rad < *near_dist) { /* if distance is smaller, reset count to 1 */
        *num_found = 1;
        *near_elem = now_node->elem;
        *near_dist = rad; 
      }
      else { /* so, distances are equal. Increment count, set output to higher elem.*/
        *num_found = *num_found + 1;
        if(now_node->elem > *near_elem) *near_elem = now_node->elem;
      }
    }

  }

/* Find more. */

  if(naxe==tree_numd) naxe = 0;

  if(tree_dl[naxe][now_node->elem] >= extent_min[naxe] && now_node->l)
    find_near_rcur(tree_dl, tree_nt, tree_numd, now_node->l, naxe+1,
                   extent_min, extent_max, target, 
                   near_elem, near_dist, num_found);

  if(tree_dl[naxe][now_node->elem] <  extent_max[naxe] && now_node->r)
    find_near_rcur(tree_dl, tree_nt, tree_numd, now_node->r, naxe+1, 
                   extent_min, extent_max, target, 
                   near_elem, near_dist, num_found);

  return;
}

/* --------------------------------------------------------------------------------------------------- */

void cycle_for_near(node *tree_nodes, double **tree_dl, int *tree_nt, int tree_numd,
                    double *extent_min, double *extent_max, double *target, 
                    double init_rad, double rad_scal,  
                    unsigned long long *near_elem, double *near_dist,
                    unsigned long long *num_found, int *ncycles) {

/*          This function finds a nearest point between specified extents of the dimensions.           */     
/*          This function is usually faster for wide extents compared to function find_near.           */
/*                                                                                                     */     
/* Input arguments:                                                                                    */     
/* tree_nodes    The already fully allocated and connected tree nodes.                                 */     
/* tree_dl       Pointers to first element of values of each dimension. That is, each coordinate is in */     
/*               a separate contiguous array. These are the pointers to the start of those arrays.     */     
/*               Those arrays have size tree_numc.                                                     */     
/* tree_nt       Near type flags. 1 means standard pythagorean nearest. See note for what 0 means.     */     
/* tree_numd     Number of pointers in tree_dl and flags in tree_nt (i.e. number of dimensions).       */     
/* extent_min    Minimum value of this dimension to find nearest point. Size tree_numd.                */     
/*               Greater OR EQUAL to this value is in range.                                           */     
/* extent_max    Maximum value of this dimension to find nearest point. Size tree_numd.                */     
/*               Strictly LESS than this value is in range.                                            */     
/* target        Find the point nearest here considering extents and tree_nt. Size tree_numd.          */     
/* init_rd       Initially search within radius init_rad. If no point within extents is found,         */     
/*               init_rad is multiplied by rad_scal and the seach is repeated. And so on.              */     
/* rad_scal      Scale value to apply to init_rd until a point is found within extents, or the         */     
/*               init_rad has been scaled to larger than the extents.                                  */     
/*                                                                                                     */     
/* Output arguments:                                                                                   */     
/* near_elem     The element number of a nearest point in the tree_dl arrays. For instance, a value of */     
/*               m means the coordinates of point are tree_dl[n][m] where n is the dimension number.   */     
/* near_dist     The SQUARED distance between the target and nearest point (if num_found>0).           */
/* num_found     0 if no point is found within the extent ranges.                                      */
/*              >0 is as many equally-near points as are found within the extent ranges.               */
/*                 The returned near_elem is the highest-numbered elem of all the nearest points.      */
/*                                                                                                     */     
/* ncycles       Number of cycles to find nearest point. This is just for informational purposes.      */
/*               This is the number of times that the rad_scal had to be applied before finding the    */
/*               nearest point. This could help set init_rad and rad_scal for faster searches.         */
/*                                                                                                     */     
/*    NOTE: A tree_nt value of 0 means the difference between the point coordinate and the target      */     
/*          coordinate are not added to the Pythagorean sum for the specified dimension(s).            */     
/*          So, the point distances are determined as if that dimension was not specified at all.      */     
/*          However, the extent_min,extent_max values for that dimension are still used.               */     
/*          The result is therefor the nearest point considering only the other dimensions, but        */     
/*          still restricted by the extent range of that dimension(s).                                 */     

  int naxe = 1;
  node *now_node;
  double now_rad = init_rad;
  double loc_min[9];
  double loc_max[9]; 
  int nset = 0;
  int i = 0;
  int nbig = 0;

  for(i=0; i<tree_numd; i++) {
    if(tree_nt[i] == 0) {
      loc_min[i] = extent_min[i];
      loc_max[i] = extent_max[i];
      nset+=2;
    }
  }

  *ncycles = 0;

CYCLE:

  *ncycles = *ncycles + 1;

/* Set extents using current radius. Also set nbig which tells us whether */
/* we are beyond the extents (therefor no reason to keep looking).        */

  nbig = nset;
  for(i=0; i<tree_numd; i++) {
    if(tree_nt[i] != 0) {
      loc_min[i] = target[i] - now_rad;
      loc_max[i] = target[i] + now_rad;
      if(loc_min[i] <= extent_min[i]) {
        loc_min[i] = extent_min[i];
        nbig++;
      }
      if(loc_max[i] >= extent_max[i]) { /* yes, >= is better than > here */ 
        loc_max[i] = extent_max[i];
        nbig++;
      }
    }
  }

  *num_found = 0;
  *near_dist = DBL_MAX;
  *near_elem = 0; 
  
  naxe = 1;
  now_node = tree_nodes; 
  find_near_rcur(tree_dl, tree_nt, tree_numd, now_node, naxe, 
                 loc_min, loc_max, target, 
                 near_elem, near_dist, num_found);

  if(*num_found<1) {
    if(nbig==2*tree_numd) return; /* None found. Are we outside the extents?  */
    now_rad = now_rad * rad_scal;
    goto CYCLE;
  }

/* Here, we need to consider the difference between a square and a circle.    */ 
/* The now_rad value is half the size of the square we just searched. So, if  */
/* current nearest point is in a circle with that radius, we are finished.    */
/* But, otherwise, there might be nearer points hiding in the area outside    */
/* the searched-square, but inside the circle. So increase the search size    */
/* to the CURRENT nearest point radius. On the next cycle, we will definitly  */        
/* get the nearest point because the CURRENT point is definitly within the    */
/* square with the now_rad that we are now setting. So the CURRENT point here */
/* will be among the points returned by the next cycle. And its radius will   */
/* definitly satisfy this condition because we explicitly made now_rad big    */
/* enough (but another point hiding in the square-circle area might sneak in).*/

  if(*near_dist > now_rad*now_rad && nbig!=2*tree_numd) {
    now_rad = sqrt(*near_dist) * 1.001;
    goto CYCLE;
  }

  return;
}

