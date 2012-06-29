/* id05.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* COPYRIGHT (c) 1988 AEA Technology */
/* ######DATE 4 Feb 1993 */
/*       Toolpack tool decs employed. */
/*       SAVE statements added. */
/* 16th October 2002: STOP and WRITE statements removed. */

integer id05ad_(inum)
integer *inum;
{
    /* Initialized data */

    static integer ic[10] = { 2,53,11,1,1,-52,-53,-1074,-1022,1023 };

    /* System generated locals */
    integer ret_val;

/* ---------------------------------------------------------------- */
/*  Integer constants for: IEEE double precision (8-byte arithmetic). */

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
/*  IC(1) THE BASE (RADIX) OF THE FLOATING-POINT ARITHMETIC. */
/*  IC(2) THE NUMBER OF BASE IC(1) DIGITS IN THE SIGNIFICAND. */
/*  IC(3) THE NUMBER OF BITS USED FOR THE EXPONENT */
/*  IC(4) = 0 FLOATING-POINT ADDITION CHOPS, = 1 IT ROUNDS. */
/*  IC(5) = 0 A GUARD DIGIT IS NOT USED FOR *, = 1 IT IS. */
/*  IC(6) LARGEST -VE INTEGER:1.0 + DBLE(IC(1))**IC(6) > 1.0. */
/*  IC(7) LARGEST -VE INTEGER:1.0 - DBLE(IC(1))**IC(7) < 1.0. */
/*  IC(8) LARGEST -VE INTEGER: DBLE(IC(1))**IC(8) > 0.0. */
/*  IC(9) LARGEST -VE INTEGER: REAL(IC(1))**IC(9) IS NORMAL. */
/*  IC(10) LARGEST +VE INTEGER: REAL(IC(1))**IC(10) FINITE. */

/*     .. */
/*     .. Executable Statements .. */
    if (*inum <= 0) {
	ret_val = ic[0];
    } else if (*inum >= 11) {
	ret_val = ic[9];
    } else {
	ret_val = ic[*inum - 1];
    }
    return ret_val;
} /* id05ad_ */

