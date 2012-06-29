/* fd05.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* COPYRIGHT (c) 1988 AEA Technology */
/* ######DATE 21 Jan 1993 */
/*       Toolpack tool decs employed. */
/*       SAVE statement added. */
/* 1/10/98 DC(3) not initialized to avoid SUN f90 failure */
/* 16 October 2001: STOP and WRITE statements removed. */
doublereal fd05ad_(inum)
integer *inum;
{
    /* Initialized data */

    static doublereal dc[5] = { 2.2204460492504e-16,1.1102230246253e-16,0.0,
	    2.2250738585073e-308,1.7976931348622e308 };

    /* System generated locals */
    doublereal ret_val;

/* ---------------------------------------------------------------- */
/*  Real constants for: IEEE double precision (8-byte arithmetic) */

/*  Obtained from H.S.L. subroutine ZE02AM. */
/*  Nick Gould and Sid Marlow, Harwell Laboratory, April 1988. */
/* ---------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. Save statement .. */
/*     .. */
/*     .. Data statements .. */

/*  DC(1) THE SMALLEST POSITIVE NUMBER: 1.0 + DC(1) > 1.0. */
/*  DC(2) THE SMALLEST POSITIVE NUMBER: 1.0 - DC(2) < 1.0. */
/*  DC(3) THE SMALLEST NONZERO +VE REAL NUMBER. */
/*  DC(4) THE SMALLEST FULL PRECISION +VE REAL NUMBER. */
/*  DC(5) THE LARGEST FINITE +VE REAL NUMBER. */

/*     DATA DC(3)/4.9406564584126D-324/ */
/*     .. */
/*     .. Executable Statements .. */
    if (*inum <= 0) {
	ret_val = dc[0];
    } else if (*inum >= 6) {
	ret_val = dc[4];
    } else if (*inum == 3) {
	ret_val = dc[3] / 4503599627370496.;
    } else {
	ret_val = dc[*inum - 1];
    }
    return ret_val;
} /* fd05ad_ */

