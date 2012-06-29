/* za02.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* COPYRIGHT (c) 1970 AEA Technology and Council for the Central */
/*                    Laboratory of the Research Councils */
doublereal za02as_(x)
real *x;
{
    /* System generated locals */
    real ret_val;

    /* Local variables */
    extern doublereal etime_();
    static real rrtime[2];

/*  Returns the current CPU time in seconds when compiled */
/*  on a Generic Unix machine.  This version is appropriate */
/*  for Compaq/Dec, HP, Silicon Graphics, Sun and Intel(Linux) */
/*  machines, using default compilers. */
/*  Note: DTIME can be used for time difference */
    ret_val = etime_(rrtime);
    return ret_val;
} /* za02as_ */

