#include <stdio.h>
#include <stdlib.h>
/* #include <malloc.h> */
#include<math.h>

void forsub_(int *,int *,int *);

int main (argc, argv)

     int argc;
     char *argv[];
{
     int ipass, nbufmax;
     int *buf=NULL;

     ipass = 1;
     forsub_ (&ipass, &nbufmax, buf);  
     buf = (int *) malloc (sizeof (int) * nbufmax);
     if (buf == NULL) exit(1);
     else {
       ipass = 2;
       forsub_ (&ipass, &nbufmax, buf);
       exit(0);
     }
     return ipass;
}

/* Fortran interface for acosh function */
/* Intel Fortran uses uppercase names, so we provide both */
double ACOSHF(double * x)
{
  return acosh(*x);
}

/* Also provide with underscore for compatibility */
double acoshf_(double * x)
{
  return acosh(*x);
}
