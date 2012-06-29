/* srotg.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static real c_b4 = (float)1.;

/* Subroutine */ int srotg_(sa, sb, c__, s)
real *sa, *sb, *c__, *s;
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(), r_sign();

    /* Local variables */
    static real r__, scale, z__, roe;


/*     construct givens plane rotation. */
/*     jack dongarra, linpack, 3/11/78. */


    roe = *sb;
    if (dabs(*sa) > dabs(*sb)) {
	roe = *sa;
    }
    scale = dabs(*sa) + dabs(*sb);
    if (scale != (float)0.) {
	goto L10;
    }
    *c__ = (float)1.;
    *s = (float)0.;
    r__ = (float)0.;
    z__ = (float)0.;
    goto L20;
L10:
/* Computing 2nd power */
    r__1 = *sa / scale;
/* Computing 2nd power */
    r__2 = *sb / scale;
    r__ = scale * sqrt(r__1 * r__1 + r__2 * r__2);
    r__ = r_sign(&c_b4, &roe) * r__;
    *c__ = *sa / r__;
    *s = *sb / r__;
    z__ = (float)1.;
    if (dabs(*sa) > dabs(*sb)) {
	z__ = *s;
    }
    if (dabs(*sb) >= dabs(*sa) && *c__ != (float)0.) {
	z__ = (float)1. / *c__;
    }
L20:
    *sa = r__;
    *sb = z__;
    return 0;
} /* srotg_ */

