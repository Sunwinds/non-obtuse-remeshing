/* ma27.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__2 = 2;

/* ******************************************************************* */
/* COPYRIGHT (c) 1982 Hyprotech UK */
/* All rights reserved. */

/* None of the comments in this Copyright notice between the lines */
/* of asterisks shall be removed or altered in any way. */

/* This Package is intended for compilation without modification, */
/* so most of the embedded comments have been removed. */

/* ALL USE IS SUBJECT TO LICENCE. For full details of an HSL ARCHIVE */
/* Licence, see http://hsl.rl.ac.uk/archive/cou.html */

/* Please note that for an HSL ARCHIVE Licence: */

/* 1. The Package must not be copied for use by any other person. */
/*    Supply of any part of the library by the Licensee to a third party */
/*    shall be subject to prior written agreement between AEA */
/*    Hyprotech UK Limited and the Licensee on suitable terms and */
/*    conditions, which will include financial conditions. */
/* 2. All information on the Package is provided to the Licensee on the */
/*    understanding that the details thereof are confidential. */
/* 3. All publications issued by the Licensee that include results obtained */
/*    with the help of one or more of the Packages shall acknowledge the */
/*    use of the Packages. The Licensee will notify the Numerical Analysis */
/*    Group at Rutherford Appleton Laboratory of any such publication. */
/* 4. The Packages may be modified by or on behalf of the Licensee */
/*    for such use in research applications but at no time shall such */
/*    Packages or modifications thereof become the property of the */
/*    Licensee. The Licensee shall make available free of charge to the */
/*    copyright holder for any purpose all information relating to */
/*    any modification. */
/* 5. Neither CCLRC nor Hyprotech UK Limited shall be liable for any */
/*    direct or consequential loss or damage whatsoever arising out of */
/*    the use of Packages by the Licensee. */
/* ******************************************************************* */

/* ######DATE 20 September 2001 */
/*  September 2001: threadsafe version of MA27 */
/*  19/3/03. Array ICNTL in MA27GD made assumed size. */
/* Subroutine */ int ma27id_(icntl, cntl)
integer *icntl;
doublereal *cntl;
{
    /* Parameter adjustments */
    --cntl;
    --icntl;

    /* Function Body */
    icntl[1] = 6;
    icntl[2] = 6;
    icntl[3] = 0;
    icntl[4] = 2139062143;
    icntl[5] = 1;
    icntl[6] = 32639;
    icntl[7] = 32639;
    icntl[8] = 32639;
    icntl[9] = 32639;
    icntl[10] = 14;
    icntl[11] = 9;
    icntl[12] = 8;
    icntl[13] = 8;
    icntl[14] = 9;
    icntl[15] = 10;
    icntl[16] = 32639;
    icntl[17] = 32639;
    icntl[18] = 32639;
    icntl[19] = 32689;
    icntl[20] = 24;
    icntl[21] = 11;
    icntl[22] = 9;
    icntl[23] = 8;
    icntl[24] = 9;
    icntl[25] = 10;
    icntl[26] = 0;
    icntl[27] = 0;
    icntl[28] = 0;
    icntl[29] = 0;
    icntl[30] = 0;
    cntl[1] = .1;
    cntl[2] = 1.;
    cntl[3] = 0.;
    cntl[4] = (float)0.;
    cntl[5] = (float)0.;
    return 0;
} /* ma27id_ */

/* Subroutine */ int ma27ad_(n, nz, irn, icn, iw, liw, ikeep, iw1, nsteps, 
	iflag, icntl, cntl, info, ops)
integer *n, *nz, *irn, *icn, *iw, *liw, *ikeep, *iw1, *nsteps, *iflag, *icntl;
doublereal *cntl;
integer *info;
doublereal *ops;
{
    /* Format strings */
    static char fmt_10[] = "(/,/,\002 ENTERING MA27AD WITH      N     NZ    \
  LIW  IFLAG\002,/,21x,i7,i7,i9,i7)";
    static char fmt_20[] = "(\002 MATRIX NON-ZEROS\002,/,4(i9,i6),/,(i9,i6,i\
9,i6,i9,i6,i9,i6))";
    static char fmt_30[] = "(\002 IKEEP(.,1)=\002,10i6,/,(12x,10i6))";
    static char fmt_80[] = "(\002 **** ERROR RETURN FROM MA27AD **** INFO(\
1)=\002,i3)";
    static char fmt_90[] = "(\002 VALUE OF N OUT OF RANGE ... =\002,i10)";
    static char fmt_110[] = "(\002 VALUE OF NZ OUT OF RANGE .. =\002,i10)";
    static char fmt_150[] = "(\002 LIW TOO SMALL, MUST BE INCREASED FROM\002\
,i10,\002 TO AT LEAST\002,i10)";
    static char fmt_170[] = "(/,\002 LEAVING MA27AD WITH NSTEPS  INFO(1)    \
OPS IERROR\002,\002 NRLTOT NIRTOT\002,/,20x,2i7,f7.0,3i7,/,20x,\002 NRLNEC N\
IRNEC NRLADU NIRADU  NCMPA\002,/,20x,6i7)";
    static char fmt_180[] = "(\002 IKEEP(.,2)=\002,10i6,/,(12x,10i6))";
    static char fmt_190[] = "(\002 IKEEP(.,3)=\002,10i6,/,(12x,10i6))";

    /* System generated locals */
    integer ikeep_dim1, ikeep_offset, iw1_dim1, iw1_offset, i__1;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer iwfr, lliw, i__, k;
    extern /* Subroutine */ int ma27gd_(), ma27hd_(), ma27jd_(), ma27kd_(), 
	    ma27ld_(), ma27md_();
    static integer l1, l2;

    /* Fortran I/O blocks */
    static cilist io___2 = { 0, 0, 0, fmt_10, 0 };
    static cilist io___4 = { 0, 0, 0, fmt_20, 0 };
    static cilist io___5 = { 0, 0, 0, fmt_30, 0 };
    static cilist io___10 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___11 = { 0, 0, 0, fmt_90, 0 };
    static cilist io___12 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___13 = { 0, 0, 0, fmt_110, 0 };
    static cilist io___14 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___15 = { 0, 0, 0, fmt_150, 0 };
    static cilist io___16 = { 0, 0, 0, fmt_170, 0 };
    static cilist io___17 = { 0, 0, 0, fmt_30, 0 };
    static cilist io___18 = { 0, 0, 0, fmt_180, 0 };
    static cilist io___19 = { 0, 0, 0, fmt_190, 0 };


    /* Parameter adjustments */
    iw1_dim1 = *n;
    iw1_offset = 1 + iw1_dim1 * 1;
    iw1 -= iw1_offset;
    ikeep_dim1 = *n;
    ikeep_offset = 1 + ikeep_dim1 * 1;
    ikeep -= ikeep_offset;
    --irn;
    --icn;
    --iw;
    --icntl;
    --cntl;
    --info;

    /* Function Body */
    for (i__ = 1; i__ <= 15; ++i__) {
	info[i__] = 0;
/* L5: */
    }
    if (icntl[3] <= 0 || icntl[2] <= 0) {
	goto L40;
    }
    io___2.ciunit = icntl[2];
    s_wsfe(&io___2);
    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*nz), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*liw), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*iflag), (ftnlen)sizeof(integer));
    e_wsfe();
    *nsteps = 0;
    k = min(8,*nz);
    if (icntl[3] > 1) {
	k = *nz;
    }
    if (k > 0) {
	io___4.ciunit = icntl[2];
	s_wsfe(&io___4);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&irn[i__], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&icn[i__], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }
    k = min(10,*n);
    if (icntl[3] > 1) {
	k = *n;
    }
    if (*iflag == 1 && k > 0) {
	io___5.ciunit = icntl[2];
	s_wsfe(&io___5);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + ikeep_dim1], (ftnlen)sizeof(
		    integer));
	}
	e_wsfe();
    }
L40:
    if (*n < 1 || *n > icntl[4]) {
	goto L70;
    }
    if (*nz < 0) {
	goto L100;
    }
    lliw = *liw - (*n << 1);
    l1 = lliw + 1;
    l2 = l1 + *n;
    if (*iflag == 1) {
	goto L50;
    }
    if (*liw < (*nz << 1) + *n * 3 + 1) {
	goto L130;
    }
    ma27gd_(n, nz, &irn[1], &icn[1], &iw[1], &lliw, &iw1[iw1_offset], &iw1[(
	    iw1_dim1 << 1) + 1], &iw[l1], &iwfr, &icntl[1], &info[1]);
    ma27hd_(n, &iw1[iw1_offset], &iw[1], &lliw, &iwfr, &iw[l1], &iw[l2], &
	    ikeep[(ikeep_dim1 << 1) + 1], &ikeep[ikeep_dim1 * 3 + 1], &ikeep[
	    ikeep_offset], &icntl[4], &info[11], &cntl[2]);
    goto L60;
L50:
    if (*liw < *nz + *n * 3 + 1) {
	goto L120;
    }
    ma27jd_(n, nz, &irn[1], &icn[1], &ikeep[ikeep_offset], &iw[1], &lliw, &
	    iw1[iw1_offset], &iw1[(iw1_dim1 << 1) + 1], &iw[l1], &iwfr, &
	    icntl[1], &info[1]);
    ma27kd_(n, &iw1[iw1_offset], &iw[1], &lliw, &iwfr, &ikeep[ikeep_offset], &
	    ikeep[(ikeep_dim1 << 1) + 1], &iw[l1], &iw[l2], &info[11]);
L60:
    ma27ld_(n, &iw1[iw1_offset], &iw[l1], &ikeep[ikeep_offset], &ikeep[(
	    ikeep_dim1 << 1) + 1], &ikeep[ikeep_dim1 * 3 + 1], &iw[l2], 
	    nsteps, &icntl[5]);
    if (*nz >= 1) {
	iw[1] = irn[1] + 1;
    }
    ma27md_(n, nz, &irn[1], &icn[1], &ikeep[ikeep_offset], &ikeep[ikeep_dim1 *
	     3 + 1], &ikeep[(ikeep_dim1 << 1) + 1], &iw[l2], nsteps, &iw1[
	    iw1_offset], &iw1[(iw1_dim1 << 1) + 1], &iw[1], &info[1], ops);
    goto L160;
L70:
    info[1] = -1;
    if (icntl[1] > 0) {
	io___10.ciunit = icntl[1];
	s_wsfe(&io___10);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___11.ciunit = icntl[1];
	s_wsfe(&io___11);
	do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L160;
L100:
    info[1] = -2;
    if (icntl[1] > 0) {
	io___12.ciunit = icntl[1];
	s_wsfe(&io___12);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___13.ciunit = icntl[1];
	s_wsfe(&io___13);
	do_fio(&c__1, (char *)&(*nz), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L160;
L120:
    info[2] = *nz + *n * 3 + 1;
    goto L140;
L130:
    info[2] = (*nz << 1) + *n * 3 + 1;
L140:
    info[1] = -3;
    if (icntl[1] > 0) {
	io___14.ciunit = icntl[1];
	s_wsfe(&io___14);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___15.ciunit = icntl[1];
	s_wsfe(&io___15);
	do_fio(&c__1, (char *)&(*liw), (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
	e_wsfe();
    }
L160:
    if (icntl[3] <= 0 || icntl[2] <= 0 || info[1] < 0) {
	goto L200;
    }
    io___16.ciunit = icntl[2];
    s_wsfe(&io___16);
    do_fio(&c__1, (char *)&(*nsteps), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*ops), (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[3], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[4], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[5], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[6], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[7], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[8], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[11], (ftnlen)sizeof(integer));
    e_wsfe();
    k = min(9,*n);
    if (icntl[3] > 1) {
	k = *n;
    }
    if (k > 0) {
	io___17.ciunit = icntl[2];
	s_wsfe(&io___17);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + ikeep_dim1], (ftnlen)sizeof(
		    integer));
	}
	e_wsfe();
    }
    k = min(k,*nsteps);
    if (k > 0) {
	io___18.ciunit = icntl[2];
	s_wsfe(&io___18);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + (ikeep_dim1 << 1)], (ftnlen)
		    sizeof(integer));
	}
	e_wsfe();
    }
    if (k > 0) {
	io___19.ciunit = icntl[2];
	s_wsfe(&io___19);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + ikeep_dim1 * 3], (ftnlen)
		    sizeof(integer));
	}
	e_wsfe();
    }
L200:
    return 0;
} /* ma27ad_ */

/* Subroutine */ int ma27bd_(n, nz, irn, icn, a, la, iw, liw, ikeep, nsteps, 
	maxfrt, iw1, icntl, cntl, info)
integer *n, *nz, *irn, *icn;
doublereal *a;
integer *la, *iw, *liw, *ikeep, *nsteps, *maxfrt, *iw1, *icntl;
doublereal *cntl;
integer *info;
{
    /* Format strings */
    static char fmt_10[] = "(/,/,\002 ENTERING MA27BD WITH      N     NZ    \
   LA      LIW\002,\002 NSTEPS      U\002,/,21x,i7,i7,i9,i9,i7,1pd10.2)";
    static char fmt_20[] = "(\002 MATRIX NON-ZEROS\002,/,1x,2(1p,d16.3,2i6),\
/,(1x,1p,d16.3,2i6,1p,d16.3,2i6))";
    static char fmt_30[] = "(\002 IKEEP(.,1)=\002,10i6,/,(12x,10i6))";
    static char fmt_40[] = "(\002 IKEEP(.,2)=\002,10i6,/,(12x,10i6))";
    static char fmt_50[] = "(\002 IKEEP(.,3)=\002,10i6,/,(12x,10i6))";
    static char fmt_65[] = "(\002 *** WARNING MESSAGE FROM SUBROUTINE MA27B\
D\002,\002  *** INFO(1) =\002,i2,/,5x,\002MATRIX IS SINGULAR. RANK=\002,i5)";
    static char fmt_80[] = "(\002 **** ERROR RETURN FROM MA27BD **** INFO(\
1)=\002,i3)";
    static char fmt_90[] = "(\002 VALUE OF N OUT OF RANGE ... =\002,i10)";
    static char fmt_110[] = "(\002 VALUE OF NZ OUT OF RANGE .. =\002,i10)";
    static char fmt_140[] = "(\002 LIW TOO SMALL, MUST BE INCREASED FROM\002\
,i10,\002 TO\002,\002 AT LEAST\002,i10)";
    static char fmt_170[] = "(\002 LA TOO SMALL, MUST BE INCREASED FROM \002\
,i10,\002 TO\002,\002 AT LEAST\002,i10)";
    static char fmt_190[] = "(\002 ZERO PIVOT AT STAGE\002,i10,\002 WHEN INP\
UT MATRIX DECLARED DEFINITE\002)";
    static char fmt_210[] = "(\002 CHANGE IN SIGN OF PIVOT ENCOUNTERED\002\
,\002 WHEN FACTORING ALLEGEDLY DEFINITE MATRIX\002)";
    static char fmt_230[] = "(/,\002 LEAVING MA27BD WITH\002,/,10x,\002  MAX\
FRT  INFO(1) NRLBDU NIRBDU NCMPBR\002,\002 NCMPBI   NTWO IERROR\002,/,11x,8i\
7)";
    static char fmt_250[] = "(\002 BLOCK PIVOT =\002,i8,\002 NROWS =\002,i8\
,\002 NCOLS =\002,i8)";
    static char fmt_260[] = "(\002 COLUMN INDICES =\002,10i6,/,(17x,10i6))";
    static char fmt_270[] = "(\002 REAL ENTRIES .. EACH ROW STARTS ON A NEW \
LINE\002)";
    static char fmt_280[] = "(1p,5d13.3)";

    /* System generated locals */
    integer ikeep_dim1, ikeep_offset, i__1, i__2, i__3;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer iblk, kblk, ipos, i__, k;
    extern /* Subroutine */ int ma27nd_(), ma27od_();
    static integer iapos, ncols, j1, j2, irows, nrows, jj, kz, nz1, len;

    /* Fortran I/O blocks */
    static cilist io___20 = { 0, 0, 0, fmt_10, 0 };
    static cilist io___22 = { 0, 0, 0, fmt_20, 0 };
    static cilist io___24 = { 0, 0, 0, fmt_30, 0 };
    static cilist io___26 = { 0, 0, 0, fmt_40, 0 };
    static cilist io___27 = { 0, 0, 0, fmt_50, 0 };
    static cilist io___29 = { 0, 0, 0, fmt_65, 0 };
    static cilist io___30 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___31 = { 0, 0, 0, fmt_90, 0 };
    static cilist io___32 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___33 = { 0, 0, 0, fmt_110, 0 };
    static cilist io___34 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___35 = { 0, 0, 0, fmt_140, 0 };
    static cilist io___36 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___37 = { 0, 0, 0, fmt_170, 0 };
    static cilist io___38 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___39 = { 0, 0, 0, "(A)", 0 };
    static cilist io___40 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___41 = { 0, 0, 0, fmt_190, 0 };
    static cilist io___42 = { 0, 0, 0, fmt_80, 0 };
    static cilist io___43 = { 0, 0, 0, fmt_210, 0 };
    static cilist io___44 = { 0, 0, 0, fmt_230, 0 };
    static cilist io___52 = { 0, 0, 0, fmt_250, 0 };
    static cilist io___54 = { 0, 0, 0, fmt_260, 0 };
    static cilist io___56 = { 0, 0, 0, fmt_270, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_280, 0 };


    /* Parameter adjustments */
    --iw1;
    ikeep_dim1 = *n;
    ikeep_offset = 1 + ikeep_dim1 * 1;
    ikeep -= ikeep_offset;
    --irn;
    --icn;
    --a;
    --iw;
    --icntl;
    --cntl;
    --info;

    /* Function Body */
    info[1] = 0;
    if (icntl[3] <= 0 || icntl[2] <= 0) {
	goto L60;
    }
    io___20.ciunit = icntl[2];
    s_wsfe(&io___20);
    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*nz), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*la), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*liw), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*nsteps), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&cntl[1], (ftnlen)sizeof(doublereal));
    e_wsfe();
    kz = min(6,*nz);
    if (icntl[3] > 1) {
	kz = *nz;
    }
    if (*nz > 0) {
	io___22.ciunit = icntl[2];
	s_wsfe(&io___22);
	i__1 = kz;
	for (k = 1; k <= i__1; ++k) {
	    do_fio(&c__1, (char *)&a[k], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&irn[k], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&icn[k], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }
    k = min(9,*n);
    if (icntl[3] > 1) {
	k = *n;
    }
    if (k > 0) {
	io___24.ciunit = icntl[2];
	s_wsfe(&io___24);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + ikeep_dim1], (ftnlen)sizeof(
		    integer));
	}
	e_wsfe();
    }
    k = min(k,*nsteps);
    if (k > 0) {
	io___26.ciunit = icntl[2];
	s_wsfe(&io___26);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + (ikeep_dim1 << 1)], (ftnlen)
		    sizeof(integer));
	}
	e_wsfe();
    }
    if (k > 0) {
	io___27.ciunit = icntl[2];
	s_wsfe(&io___27);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&ikeep[i__ + ikeep_dim1 * 3], (ftnlen)
		    sizeof(integer));
	}
	e_wsfe();
    }
L60:
    if (*n < 1 || *n > icntl[4]) {
	goto L70;
    }
    if (*nz < 0) {
	goto L100;
    }
    if (*liw < *nz) {
	goto L120;
    }
    if (*la < *nz + *n) {
	goto L150;
    }
    if (*nsteps < 1 || *nsteps > *n) {
	goto L175;
    }
    ma27nd_(n, nz, &nz1, &a[1], la, &irn[1], &icn[1], &iw[1], liw, &ikeep[
	    ikeep_offset], &iw1[1], &icntl[1], &info[1]);
    if (info[1] == -3) {
	goto L130;
    }
    if (info[1] == -4) {
	goto L160;
    }
    ma27od_(n, &nz1, &a[1], la, &iw[1], liw, &ikeep[ikeep_offset], &ikeep[
	    ikeep_dim1 * 3 + 1], nsteps, maxfrt, &ikeep[(ikeep_dim1 << 1) + 1]
	    , &iw1[1], &icntl[1], &cntl[1], &info[1]);
    if (info[1] == -3) {
	goto L130;
    }
    if (info[1] == -4) {
	goto L160;
    }
    if (info[1] == -5) {
	goto L180;
    }
    if (info[1] == -6) {
	goto L200;
    }
    if (info[1] == 3 && icntl[2] > 0) {
	io___29.ciunit = icntl[2];
	s_wsfe(&io___29);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L70:
    info[1] = -1;
    if (icntl[1] > 0) {
	io___30.ciunit = icntl[1];
	s_wsfe(&io___30);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___31.ciunit = icntl[1];
	s_wsfe(&io___31);
	do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L100:
    info[1] = -2;
    if (icntl[1] > 0) {
	io___32.ciunit = icntl[1];
	s_wsfe(&io___32);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___33.ciunit = icntl[1];
	s_wsfe(&io___33);
	do_fio(&c__1, (char *)&(*nz), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L120:
    info[1] = -3;
    info[2] = *nz;
L130:
    if (icntl[1] > 0) {
	io___34.ciunit = icntl[1];
	s_wsfe(&io___34);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___35.ciunit = icntl[1];
	s_wsfe(&io___35);
	do_fio(&c__1, (char *)&(*liw), (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L150:
    info[1] = -4;
    info[2] = *nz + *n;
L160:
    if (icntl[1] > 0) {
	io___36.ciunit = icntl[1];
	s_wsfe(&io___36);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___37.ciunit = icntl[1];
	s_wsfe(&io___37);
	do_fio(&c__1, (char *)&(*la), (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L175:
    info[1] = -7;
    if (icntl[1] > 0) {
	io___38.ciunit = icntl[1];
	s_wsfe(&io___38);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___39.ciunit = icntl[1];
	s_wsfe(&io___39);
	do_fio(&c__1, " NSTEPS is out of range", (ftnlen)23);
	e_wsfe();
    }
    goto L220;
L180:
    if (icntl[1] > 0) {
	io___40.ciunit = icntl[1];
	s_wsfe(&io___40);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___41.ciunit = icntl[1];
	s_wsfe(&io___41);
	do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L220;
L200:
    if (icntl[1] > 0) {
	io___42.ciunit = icntl[1];
	s_wsfe(&io___42);
	do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	e_wsfe();
    }
    if (icntl[1] > 0) {
	io___43.ciunit = icntl[1];
	s_wsfe(&io___43);
	e_wsfe();
    }
L220:
    if (icntl[3] <= 0 || icntl[2] <= 0 || info[1] < 0) {
	goto L310;
    }
    io___44.ciunit = icntl[2];
    s_wsfe(&io___44);
    do_fio(&c__1, (char *)&(*maxfrt), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[9], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[10], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[12], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[13], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[14], (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&info[2], (ftnlen)sizeof(integer));
    e_wsfe();
    if (info[1] < 0) {
	goto L310;
    }
    kblk = abs(iw[1]);
    if (kblk == 0) {
	goto L310;
    }
    if (icntl[3] == 1) {
	kblk = 1;
    }
    ipos = 2;
    iapos = 1;
    i__1 = kblk;
    for (iblk = 1; iblk <= i__1; ++iblk) {
	ncols = iw[ipos];
	nrows = iw[ipos + 1];
	j1 = ipos + 2;
	if (ncols > 0) {
	    goto L240;
	}
	ncols = -ncols;
	nrows = 1;
	--j1;
L240:
	io___52.ciunit = icntl[2];
	s_wsfe(&io___52);
	do_fio(&c__1, (char *)&iblk, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&nrows, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&ncols, (ftnlen)sizeof(integer));
	e_wsfe();
	j2 = j1 + ncols - 1;
	ipos = j2 + 1;
	io___54.ciunit = icntl[2];
	s_wsfe(&io___54);
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    do_fio(&c__1, (char *)&iw[jj], (ftnlen)sizeof(integer));
	}
	e_wsfe();
	io___56.ciunit = icntl[2];
	s_wsfe(&io___56);
	e_wsfe();
	len = ncols;
	i__2 = nrows;
	for (irows = 1; irows <= i__2; ++irows) {
	    j1 = iapos;
	    j2 = iapos + len - 1;
	    io___59.ciunit = icntl[2];
	    s_wsfe(&io___59);
	    i__3 = j2;
	    for (jj = j1; jj <= i__3; ++jj) {
		do_fio(&c__1, (char *)&a[jj], (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	    --len;
	    iapos = j2 + 1;
/* L290: */
	}
/* L300: */
    }
L310:
    return 0;
} /* ma27bd_ */

/* Subroutine */ int ma27cd_(n, a, la, iw, liw, w, maxfrt, rhs, iw1, nsteps, 
	icntl, info)
integer *n;
doublereal *a;
integer *la, *iw, *liw;
doublereal *w;
integer *maxfrt;
doublereal *rhs;
integer *iw1, *nsteps, *icntl, *info;
{
    /* Format strings */
    static char fmt_10[] = "(/,/,\002 ENTERING MA27CD WITH      N     LA    \
LIW MAXFRT\002,\002  NSTEPS\002,/,21x,5i7)";
    static char fmt_30[] = "(\002 BLOCK PIVOT =\002,i8,\002 NROWS =\002,i8\
,\002 NCOLS =\002,i8)";
    static char fmt_40[] = "(\002 COLUMN INDICES =\002,10i6,/,(17x,10i6))";
    static char fmt_50[] = "(\002 REAL ENTRIES .. EACH ROW STARTS ON A NEW L\
INE\002)";
    static char fmt_60[] = "(1p,5d13.3)";
    static char fmt_100[] = "(\002 RHS\002,1p,5d13.3,/,(4x,1p,5d13.3))";
    static char fmt_160[] = "(/,/,\002 LEAVING MA27CD WITH\002)";

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer iblk, kblk, nblk, ipos, i__, k;
    extern /* Subroutine */ int ma27qd_(), ma27rd_();
    static integer iapos, ncols, latop, j1, j2, irows, nrows, jj, len;

    /* Fortran I/O blocks */
    static cilist io___60 = { 0, 0, 0, fmt_10, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_30, 0 };
    static cilist io___70 = { 0, 0, 0, fmt_40, 0 };
    static cilist io___72 = { 0, 0, 0, fmt_50, 0 };
    static cilist io___75 = { 0, 0, 0, fmt_60, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_100, 0 };
    static cilist io___81 = { 0, 0, 0, fmt_160, 0 };
    static cilist io___82 = { 0, 0, 0, fmt_100, 0 };


    /* Parameter adjustments */
    --rhs;
    --a;
    --iw;
    --w;
    --iw1;
    --icntl;
    --info;

    /* Function Body */
    info[1] = 0;
    if (icntl[3] <= 0 || icntl[2] <= 0) {
	goto L110;
    }
    io___60.ciunit = icntl[2];
    s_wsfe(&io___60);
    do_fio(&c__1, (char *)&(*n), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*la), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*liw), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*maxfrt), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&(*nsteps), (ftnlen)sizeof(integer));
    e_wsfe();
    kblk = abs(iw[1]);
    if (kblk == 0) {
	goto L90;
    }
    if (icntl[3] == 1) {
	kblk = 1;
    }
    ipos = 2;
    iapos = 1;
    i__1 = kblk;
    for (iblk = 1; iblk <= i__1; ++iblk) {
	ncols = iw[ipos];
	nrows = iw[ipos + 1];
	j1 = ipos + 2;
	if (ncols > 0) {
	    goto L20;
	}
	ncols = -ncols;
	nrows = 1;
	--j1;
L20:
	io___68.ciunit = icntl[2];
	s_wsfe(&io___68);
	do_fio(&c__1, (char *)&iblk, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&nrows, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&ncols, (ftnlen)sizeof(integer));
	e_wsfe();
	j2 = j1 + ncols - 1;
	ipos = j2 + 1;
	io___70.ciunit = icntl[2];
	s_wsfe(&io___70);
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    do_fio(&c__1, (char *)&iw[jj], (ftnlen)sizeof(integer));
	}
	e_wsfe();
	io___72.ciunit = icntl[2];
	s_wsfe(&io___72);
	e_wsfe();
	len = ncols;
	i__2 = nrows;
	for (irows = 1; irows <= i__2; ++irows) {
	    j1 = iapos;
	    j2 = iapos + len - 1;
	    io___75.ciunit = icntl[2];
	    s_wsfe(&io___75);
	    i__3 = j2;
	    for (jj = j1; jj <= i__3; ++jj) {
		do_fio(&c__1, (char *)&a[jj], (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	    --len;
	    iapos = j2 + 1;
/* L70: */
	}
/* L80: */
    }
L90:
    k = min(10,*n);
    if (icntl[3] > 1) {
	k = *n;
    }
    if (*n > 0) {
	io___77.ciunit = icntl[2];
	s_wsfe(&io___77);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&rhs[i__], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
L110:
    if (iw[1] < 0) {
	goto L130;
    }
    nblk = iw[1];
    if (nblk > 0) {
	goto L140;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rhs[i__] = 0.;
/* L120: */
    }
    goto L150;
L130:
    nblk = -iw[1];
L140:
    i__1 = *liw - 1;
    ma27qd_(n, &a[1], la, &iw[2], &i__1, &w[1], maxfrt, &rhs[1], &iw1[1], &
	    nblk, &latop, &icntl[1]);
    i__1 = *liw - 1;
    ma27rd_(n, &a[1], la, &iw[2], &i__1, &w[1], maxfrt, &rhs[1], &iw1[1], &
	    nblk, &latop, &icntl[1]);
L150:
    if (icntl[3] <= 0 || icntl[2] <= 0) {
	goto L170;
    }
    io___81.ciunit = icntl[2];
    s_wsfe(&io___81);
    e_wsfe();
    if (*n > 0) {
	io___82.ciunit = icntl[2];
	s_wsfe(&io___82);
	i__1 = k;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&rhs[i__], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
L170:
    return 0;
} /* ma27cd_ */

/* Subroutine */ int ma27gd_(n, nz, irn, icn, iw, lw, ipe, iq, flag__, iwfr, 
	icntl, info)
integer *n, *nz, *irn, *icn, *iw, *lw, *ipe, *iq, *flag__, *iwfr, *icntl, *
	info;
{
    /* Format strings */
    static char fmt_60[] = "(\002 *** WARNING MESSAGE FROM SUBROUTINE MA27A\
D\002,\002  *** INFO(1) =\002,i2)";
    static char fmt_70[] = "(i6,\002TH NON-ZERO (IN ROW\002,i6,\002 AND COLU\
MN\002,i6,\002) IGNORED\002)";

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer last, ndup, i__, j, k, l, k1, k2, n1, id, jn, lr;

    /* Fortran I/O blocks */
    static cilist io___87 = { 0, 0, 0, fmt_60, 0 };
    static cilist io___88 = { 0, 0, 0, fmt_70, 0 };


    /* Parameter adjustments */
    --flag__;
    --iq;
    --ipe;
    --irn;
    --icn;
    --iw;
    --icntl;
    --info;

    /* Function Body */
    info[2] = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ipe[i__] = 0;
/* L10: */
    }
    lr = *nz;
    if (*nz == 0) {
	goto L120;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	i__ = irn[k];
	j = icn[k];
	if (i__ < j) {
	    if (i__ >= 1 && j <= *n) {
		goto L90;
	    }
	} else if (i__ > j) {
	    if (j >= 1 && i__ <= *n) {
		goto L90;
	    }
	} else {
	    if (i__ >= 1 && i__ <= *n) {
		goto L80;
	    }
	}
	++info[2];
	info[1] = 1;
	if (info[2] <= 1 && icntl[2] > 0) {
	    io___87.ciunit = icntl[2];
	    s_wsfe(&io___87);
	    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	if (info[2] <= 10 && icntl[2] > 0) {
	    io___88.ciunit = icntl[2];
	    s_wsfe(&io___88);
	    do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
L80:
	i__ = 0;
	j = 0;
	goto L100;
L90:
	++ipe[i__];
	++ipe[j];
L100:
	iw[k] = j;
	++lr;
	iw[lr] = i__;
/* L110: */
    }
L120:
    iq[1] = 1;
    n1 = *n - 1;
    if (n1 <= 0) {
	goto L140;
    }
    i__1 = n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	flag__[i__] = 0;
	if (ipe[i__] == 0) {
	    ipe[i__] = -1;
	}
	iq[i__ + 1] = ipe[i__] + iq[i__] + 1;
	ipe[i__] = iq[i__];
/* L130: */
    }
L140:
    last = ipe[*n] + iq[*n];
    flag__[*n] = 0;
    if (lr >= last) {
	goto L160;
    }
    k1 = lr + 1;
    i__1 = last;
    for (k = k1; k <= i__1; ++k) {
	iw[k] = 0;
/* L150: */
    }
L160:
    ipe[*n] = iq[*n];
    *iwfr = last + 1;
    if (*nz == 0) {
	goto L230;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	j = iw[k];
	if (j <= 0) {
	    goto L220;
	}
	l = k;
	iw[k] = 0;
	i__2 = *nz;
	for (id = 1; id <= i__2; ++id) {
	    if (l > *nz) {
		goto L170;
	    }
	    l += *nz;
	    goto L180;
L170:
	    l -= *nz;
L180:
	    i__ = iw[l];
	    iw[l] = 0;
	    if (i__ < j) {
		goto L190;
	    }
	    l = iq[j] + 1;
	    iq[j] = l;
	    jn = iw[l];
	    iw[l] = -i__;
	    goto L200;
L190:
	    l = iq[i__] + 1;
	    iq[i__] = l;
	    jn = iw[l];
	    iw[l] = -j;
L200:
	    j = jn;
	    if (j <= 0) {
		goto L220;
	    }
/* L210: */
	}
L220:
	;
    }
L230:
    ndup = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k1 = ipe[i__] + 1;
	k2 = iq[i__];
	if (k1 <= k2) {
	    goto L240;
	}
	ipe[i__] = 0;
	iq[i__] = 0;
	goto L280;
L240:
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    j = -iw[k];
	    if (j <= 0) {
		goto L270;
	    }
	    l = iq[j] + 1;
	    iq[j] = l;
	    iw[l] = i__;
	    iw[k] = j;
	    if (flag__[j] != i__) {
		goto L250;
	    }
	    ++ndup;
	    iw[l] = 0;
	    iw[k] = 0;
L250:
	    flag__[j] = i__;
/* L260: */
	}
L270:
	iq[i__] -= ipe[i__];
	if (ndup == 0) {
	    iw[k1 - 1] = iq[i__];
	}
L280:
	;
    }
    if (ndup == 0) {
	goto L310;
    }
    *iwfr = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k1 = ipe[i__] + 1;
	if (k1 == 1) {
	    goto L300;
	}
	k2 = iq[i__] + ipe[i__];
	l = *iwfr;
	ipe[i__] = *iwfr;
	++(*iwfr);
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    if (iw[k] == 0) {
		goto L290;
	    }
	    iw[*iwfr] = iw[k];
	    ++(*iwfr);
L290:
	    ;
	}
	iw[l] = *iwfr - l - 1;
L300:
	;
    }
L310:
    return 0;
} /* ma27gd_ */

/* Subroutine */ int ma27hd_(n, ipe, iw, lw, iwfr, nv, nxt, lst, ipd, flag__, 
	iovflo, ncmpa, fratio)
integer *n, *ipe, *iw, *lw, *iwfr, *nv, *nxt, *lst, *ipd, *flag__, *iovflo, *
	ncmpa;
doublereal *fratio;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublereal d__1;

    /* Builtin functions */
    integer i_dnnt();

    /* Local variables */
    static integer nflg, lwfr, root, i__, k, l;
    extern /* Subroutine */ int ma27ud_();
    static integer limit, k1, k2, nvpiv, id, ie, ke, md, me, ip, jp, kp, is, 
	    js, ks, ln, ls, ml, ms, np, ns, jp1, jp2, kp0, kp1, kp2, np0, 
	    nvroot, idl, idn, len, nel;

    /* Parameter adjustments */
    --flag__;
    --ipd;
    --lst;
    --nxt;
    --nv;
    --ipe;
    --iw;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ipd[i__] = 0;
	nv[i__] = 1;
	flag__[i__] = *iovflo;
/* L10: */
    }
    md = 1;
    *ncmpa = 0;
    nflg = *iovflo;
    nel = 0;
    root = *n + 1;
    nvroot = 0;
    i__1 = *n;
    for (is = 1; is <= i__1; ++is) {
	k = ipe[is];
	if (k <= 0) {
	    goto L20;
	}
	id = iw[k] + 1;
	ns = ipd[id];
	if (ns > 0) {
	    lst[ns] = is;
	}
	nxt[is] = ns;
	ipd[id] = is;
	lst[is] = -id;
	goto L30;
L20:
	++nel;
	flag__[is] = -1;
	nxt[is] = 0;
	lst[is] = 0;
L30:
	;
    }
    i__1 = *n;
    for (ml = 1; ml <= i__1; ++ml) {
	if (nel + nvroot + 1 >= *n) {
	    goto L350;
	}
	i__2 = *n;
	for (id = md; id <= i__2; ++id) {
	    ms = ipd[id];
	    if (ms > 0) {
		goto L50;
	    }
/* L40: */
	}
L50:
	md = id;
	nvpiv = nv[ms];
	ns = nxt[ms];
	nxt[ms] = 0;
	lst[ms] = 0;
	if (ns > 0) {
	    lst[ns] = -id;
	}
	ipd[id] = ns;
	me = ms;
	nel += nvpiv;
	idn = 0;
	kp = ipe[me];
	flag__[ms] = -1;
	ip = *iwfr;
	len = iw[kp];
	i__2 = len;
	for (kp1 = 1; kp1 <= i__2; ++kp1) {
	    ++kp;
	    ke = iw[kp];
	    if (flag__[ke] <= -2) {
		goto L60;
	    }
	    if (flag__[ke] <= 0) {
		if (ipe[ke] != -root) {
		    goto L140;
		}
		ke = root;
		if (flag__[ke] <= 0) {
		    goto L140;
		}
	    }
	    jp = kp - 1;
	    ln = len - kp1 + 1;
	    ie = ms;
	    goto L70;
L60:
	    ie = ke;
	    jp = ipe[ie];
	    ln = iw[jp];
L70:
	    i__3 = ln;
	    for (jp1 = 1; jp1 <= i__3; ++jp1) {
		++jp;
		is = iw[jp];
		if (flag__[is] <= 0) {
		    if (ipe[is] == -root) {
			is = root;
			iw[jp] = root;
			if (flag__[is] <= 0) {
			    goto L130;
			}
		    } else {
			goto L130;
		    }
		}
		flag__[is] = 0;
		if (*iwfr < *lw) {
		    goto L100;
		}
		ipe[ms] = kp;
		iw[kp] = len - kp1;
		ipe[ie] = jp;
		iw[jp] = ln - jp1;
		i__4 = ip - 1;
		ma27ud_(n, &ipe[1], &iw[1], &i__4, &lwfr, ncmpa);
		jp2 = *iwfr - 1;
		*iwfr = lwfr;
		if (ip > jp2) {
		    goto L90;
		}
		i__4 = jp2;
		for (jp = ip; jp <= i__4; ++jp) {
		    iw[*iwfr] = iw[jp];
		    ++(*iwfr);
/* L80: */
		}
L90:
		ip = lwfr;
		jp = ipe[ie];
		kp = ipe[me];
L100:
		iw[*iwfr] = is;
		idn += nv[is];
		++(*iwfr);
		ls = lst[is];
		lst[is] = 0;
		ns = nxt[is];
		nxt[is] = 0;
		if (ns > 0) {
		    lst[ns] = ls;
		}
		if (ls < 0) {
		    ls = -ls;
		    ipd[ls] = ns;
		} else if (ls > 0) {
		    nxt[ls] = ns;
		}
L130:
		;
	    }
	    if (ie == ms) {
		goto L150;
	    }
	    ipe[ie] = -me;
	    flag__[ie] = -1;
L140:
	    ;
	}
L150:
	nv[ms] = idn + nvpiv;
	if (*iwfr == ip) {
	    goto L330;
	}
	k1 = ip;
	k2 = *iwfr - 1;
	d__1 = *fratio * (*n - nel);
	limit = i_dnnt(&d__1);
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    is = iw[k];
	    if (is == root) {
		goto L310;
	    }
	    if (nflg > 2) {
		goto L170;
	    }
	    i__3 = *n;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		if (flag__[i__] > 0) {
		    flag__[i__] = *iovflo;
		}
		if (flag__[i__] <= -2) {
		    flag__[i__] = -(*iovflo);
		}
/* L160: */
	    }
	    nflg = *iovflo;
L170:
	    --nflg;
	    id = idn;
	    kp1 = ipe[is] + 1;
	    np = kp1;
	    kp2 = iw[kp1 - 1] + kp1 - 1;
	    i__3 = kp2;
	    for (kp = kp1; kp <= i__3; ++kp) {
		ke = iw[kp];
		if (flag__[ke] == -1) {
		    if (ipe[ke] != -root) {
			goto L220;
		    }
		    ke = root;
		    iw[kp] = root;
		    if (flag__[ke] == -1) {
			goto L220;
		    }
		}
		if (flag__[ke] >= 0) {
		    goto L230;
		}
		jp1 = ipe[ke] + 1;
		jp2 = iw[jp1 - 1] + jp1 - 1;
		idl = id;
		i__4 = jp2;
		for (jp = jp1; jp <= i__4; ++jp) {
		    js = iw[jp];
		    if (flag__[js] <= nflg) {
			goto L190;
		    }
		    id += nv[js];
		    flag__[js] = nflg;
L190:
		    ;
		}
		if (id > idl) {
		    goto L210;
		}
		i__4 = jp2;
		for (jp = jp1; jp <= i__4; ++jp) {
		    js = iw[jp];
		    if (flag__[js] != 0) {
			goto L210;
		    }
/* L200: */
		}
		ipe[ke] = -me;
		flag__[ke] = -1;
		goto L220;
L210:
		iw[np] = ke;
		flag__[ke] = -nflg;
		++np;
L220:
		;
	    }
	    np0 = np;
	    goto L250;
L230:
	    kp0 = kp;
	    np0 = np;
	    i__3 = kp2;
	    for (kp = kp0; kp <= i__3; ++kp) {
		ks = iw[kp];
		if (flag__[ks] <= nflg) {
		    if (ipe[ks] == -root) {
			ks = root;
			iw[kp] = root;
			if (flag__[ks] <= nflg) {
			    goto L240;
			}
		    } else {
			goto L240;
		    }
		}
		id += nv[ks];
		flag__[ks] = nflg;
		iw[np] = ks;
		++np;
L240:
		;
	    }
L250:
	    if (id >= limit) {
		goto L295;
	    }
	    iw[np] = iw[np0];
	    iw[np0] = iw[kp1];
	    iw[kp1] = me;
	    iw[kp1 - 1] = np - kp1 + 1;
	    js = ipd[id];
	    i__3 = *n;
	    for (l = 1; l <= i__3; ++l) {
		if (js <= 0) {
		    goto L300;
		}
		kp1 = ipe[js] + 1;
		if (iw[kp1] != me) {
		    goto L300;
		}
		kp2 = kp1 - 1 + iw[kp1 - 1];
		i__4 = kp2;
		for (kp = kp1; kp <= i__4; ++kp) {
		    ie = iw[kp];
		    if ((i__5 = flag__[ie], abs(i__5)) > nflg) {
			goto L270;
		    }
/* L260: */
		}
		goto L290;
L270:
		js = nxt[js];
/* L280: */
	    }
L290:
	    ipe[js] = -is;
	    nv[is] += nv[js];
	    nv[js] = 0;
	    flag__[js] = -1;
	    ns = nxt[js];
	    ls = lst[js];
	    if (ns > 0) {
		lst[ns] = is;
	    }
	    if (ls > 0) {
		nxt[ls] = is;
	    }
	    lst[is] = ls;
	    nxt[is] = ns;
	    lst[js] = 0;
	    nxt[js] = 0;
	    if (ipd[id] == js) {
		ipd[id] = is;
	    }
	    goto L310;
L295:
	    if (nvroot == 0) {
		root = is;
		ipe[is] = 0;
	    } else {
		iw[k] = root;
		ipe[is] = -root;
		nv[root] += nv[is];
		nv[is] = 0;
		flag__[is] = -1;
	    }
	    nvroot = nv[root];
	    goto L310;
L300:
	    ns = ipd[id];
	    if (ns > 0) {
		lst[ns] = is;
	    }
	    nxt[is] = ns;
	    ipd[id] = is;
	    lst[is] = -id;
	    md = min(md,id);
L310:
	    ;
	}
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    is = iw[k];
	    if (nv[is] == 0) {
		goto L320;
	    }
	    flag__[is] = nflg;
	    iw[ip] = is;
	    ++ip;
L320:
	    ;
	}
	*iwfr = k1;
	flag__[me] = -nflg;
	iw[ip] = iw[k1];
	iw[k1] = ip - k1;
	ipe[me] = k1;
	*iwfr = ip + 1;
	goto L335;
L330:
	ipe[me] = 0;
L335:
/* L340: */
	;
    }
L350:
    i__1 = *n;
    for (is = 1; is <= i__1; ++is) {
	if (nxt[is] != 0 || lst[is] != 0) {
	    if (nvroot == 0) {
		root = is;
		ipe[is] = 0;
	    } else {
		ipe[is] = -root;
	    }
	    nvroot += nv[is];
	    nv[is] = 0;
	}
/* L360: */
    }
    i__1 = *n;
    for (ie = 1; ie <= i__1; ++ie) {
	if (ipe[ie] > 0) {
	    ipe[ie] = -root;
	}
/* L370: */
    }
    if (nvroot > 0) {
	nv[root] = nvroot;
    }
} /* ma27hd_ */

/* Subroutine */ int ma27ud_(n, ipe, iw, lw, iwfr, ncmpa)
integer *n, *ipe, *iw, *lw, *iwfr, *ncmpa;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer lwfr, i__, k, k1, k2, ir;

    /* Parameter adjustments */
    --ipe;
    --iw;

    /* Function Body */
    ++(*ncmpa);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k1 = ipe[i__];
	if (k1 <= 0) {
	    goto L10;
	}
	ipe[i__] = iw[k1];
	iw[k1] = -i__;
L10:
	;
    }
    *iwfr = 1;
    lwfr = *iwfr;
    i__1 = *n;
    for (ir = 1; ir <= i__1; ++ir) {
	if (lwfr > *lw) {
	    goto L70;
	}
	i__2 = *lw;
	for (k = lwfr; k <= i__2; ++k) {
	    if (iw[k] < 0) {
		goto L30;
	    }
/* L20: */
	}
	goto L70;
L30:
	i__ = -iw[k];
	iw[*iwfr] = ipe[i__];
	ipe[i__] = *iwfr;
	k1 = k + 1;
	k2 = k + iw[*iwfr];
	++(*iwfr);
	if (k1 > k2) {
	    goto L50;
	}
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    iw[*iwfr] = iw[k];
	    ++(*iwfr);
/* L40: */
	}
L50:
	lwfr = k2 + 1;
/* L60: */
    }
L70:
    return 0;
} /* ma27ud_ */

/* Subroutine */ int ma27jd_(n, nz, irn, icn, perm, iw, lw, ipe, iq, flag__, 
	iwfr, icntl, info)
integer *n, *nz, *irn, *icn, *perm, *iw, *lw, *ipe, *iq, *flag__, *iwfr, *
	icntl, *info;
{
    /* Format strings */
    static char fmt_60[] = "(\002 *** WARNING MESSAGE FROM SUBROUTINE MA27A\
D\002,\002  *** INFO(1) =\002,i2)";
    static char fmt_70[] = "(i6,\002TH NON-ZERO (IN ROW\002,i6,\002 AND COLU\
MN\002,i6,\002) IGNORED\002)";

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer lbig, i__, j, k, l, k1, k2, id, in, jdummy, len;

    /* Fortran I/O blocks */
    static cilist io___144 = { 0, 0, 0, fmt_60, 0 };
    static cilist io___145 = { 0, 0, 0, fmt_70, 0 };


    /* Parameter adjustments */
    --flag__;
    --iq;
    --ipe;
    --perm;
    --irn;
    --icn;
    --iw;
    --icntl;
    --info;

    /* Function Body */
    info[1] = 0;
    info[2] = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iq[i__] = 0;
/* L10: */
    }
    if (*nz == 0) {
	goto L110;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	i__ = irn[k];
	j = icn[k];
	iw[k] = -i__;
	if (i__ < j) {
	    if (i__ >= 1 && j <= *n) {
		goto L80;
	    }
	} else if (i__ > j) {
	    if (j >= 1 && i__ <= *n) {
		goto L80;
	    }
	} else {
	    iw[k] = 0;
	    if (i__ >= 1 && i__ <= *n) {
		goto L100;
	    }
	}
	++info[2];
	info[1] = 1;
	iw[k] = 0;
	if (info[2] <= 1 && icntl[2] > 0) {
	    io___144.ciunit = icntl[2];
	    s_wsfe(&io___144);
	    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	if (info[2] <= 10 && icntl[2] > 0) {
	    io___145.ciunit = icntl[2];
	    s_wsfe(&io___145);
	    do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	goto L100;
L80:
	if (perm[j] > perm[i__]) {
	    goto L90;
	}
	++iq[j];
	goto L100;
L90:
	++iq[i__];
L100:
	;
    }
L110:
    *iwfr = 1;
    lbig = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = iq[i__];
	lbig = max(l,lbig);
	*iwfr += l;
	ipe[i__] = *iwfr - 1;
/* L120: */
    }
    if (*nz == 0) {
	goto L250;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	i__ = -iw[k];
	if (i__ <= 0) {
	    goto L160;
	}
	l = k;
	iw[k] = 0;
	i__2 = *nz;
	for (id = 1; id <= i__2; ++id) {
	    j = icn[l];
	    if (perm[i__] < perm[j]) {
		goto L130;
	    }
	    l = ipe[j];
	    ipe[j] = l - 1;
	    in = iw[l];
	    iw[l] = i__;
	    goto L140;
L130:
	    l = ipe[i__];
	    ipe[i__] = l - 1;
	    in = iw[l];
	    iw[l] = j;
L140:
	    i__ = -in;
	    if (i__ <= 0) {
		goto L160;
	    }
/* L150: */
	}
L160:
	;
    }
    k = *iwfr - 1;
    l = k + *n;
    *iwfr = l + 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	flag__[i__] = 0;
	j = *n + 1 - i__;
	len = iq[j];
	if (len <= 0) {
	    goto L180;
	}
	i__2 = len;
	for (jdummy = 1; jdummy <= i__2; ++jdummy) {
	    iw[l] = iw[k];
	    --k;
	    --l;
/* L170: */
	}
L180:
	ipe[j] = l;
	--l;
/* L190: */
    }
    if (lbig >= icntl[4]) {
	goto L210;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = ipe[i__];
	iw[k] = iq[i__];
	if (iq[i__] == 0) {
	    ipe[i__] = 0;
	}
/* L200: */
    }
    goto L250;
L210:
    *iwfr = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k1 = ipe[i__] + 1;
	k2 = ipe[i__] + iq[i__];
	if (k1 <= k2) {
	    goto L220;
	}
	ipe[i__] = 0;
	goto L240;
L220:
	ipe[i__] = *iwfr;
	++(*iwfr);
	i__2 = k2;
	for (k = k1; k <= i__2; ++k) {
	    j = iw[k];
	    if (flag__[j] == i__) {
		goto L230;
	    }
	    iw[*iwfr] = j;
	    ++(*iwfr);
	    flag__[j] = i__;
L230:
	    ;
	}
	k = ipe[i__];
	iw[k] = *iwfr - k - 1;
L240:
	;
    }
L250:
    return 0;
} /* ma27jd_ */

/* Subroutine */ int ma27kd_(n, ipe, iw, lw, iwfr, ips, ipv, nv, flag__, 
	ncmpa)
integer *n, *ipe, *iw, *lw, *iwfr, *ips, *ipv, *nv, *flag__, *ncmpa;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;

    /* Local variables */
    static integer lwfr, i__, j;
    extern /* Subroutine */ int ma27ud_();
    static integer minjs, ie, je, me, ip, jp, ln, ml, js, ms, kdummy, jp1, 
	    jp2;

    /* Parameter adjustments */
    --flag__;
    --nv;
    --ipv;
    --ips;
    --ipe;
    --iw;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	flag__[i__] = 0;
	nv[i__] = 0;
	j = ips[i__];
	ipv[j] = i__;
/* L10: */
    }
    *ncmpa = 0;
    i__1 = *n;
    for (ml = 1; ml <= i__1; ++ml) {
	ms = ipv[ml];
	me = ms;
	flag__[ms] = me;
	ip = *iwfr;
	minjs = *n;
	ie = me;
	i__2 = *n;
	for (kdummy = 1; kdummy <= i__2; ++kdummy) {
	    jp = ipe[ie];
	    ln = 0;
	    if (jp <= 0) {
		goto L60;
	    }
	    ln = iw[jp];
	    i__3 = ln;
	    for (jp1 = 1; jp1 <= i__3; ++jp1) {
		++jp;
		js = iw[jp];
		if (flag__[js] == me) {
		    goto L50;
		}
		flag__[js] = me;
		if (*iwfr < *lw) {
		    goto L40;
		}
		ipe[ie] = jp;
		iw[jp] = ln - jp1;
		i__4 = ip - 1;
		ma27ud_(n, &ipe[1], &iw[1], &i__4, &lwfr, ncmpa);
		jp2 = *iwfr - 1;
		*iwfr = lwfr;
		if (ip > jp2) {
		    goto L30;
		}
		i__4 = jp2;
		for (jp = ip; jp <= i__4; ++jp) {
		    iw[*iwfr] = iw[jp];
		    ++(*iwfr);
/* L20: */
		}
L30:
		ip = lwfr;
		jp = ipe[ie];
L40:
		iw[*iwfr] = js;
/* Computing MIN */
		i__4 = minjs, i__5 = ips[js];
		minjs = min(i__4,i__5);
		++(*iwfr);
L50:
		;
	    }
L60:
	    ipe[ie] = -me;
	    je = nv[ie];
	    nv[ie] = ln + 1;
	    ie = je;
	    if (ie == 0) {
		goto L80;
	    }
/* L70: */
	}
L80:
	if (*iwfr > ip) {
	    goto L90;
	}
	ipe[me] = 0;
	nv[me] = 1;
	goto L100;
L90:
	minjs = ipv[minjs];
	nv[me] = nv[minjs];
	nv[minjs] = me;
	iw[*iwfr] = iw[ip];
	iw[ip] = *iwfr - ip;
	ipe[me] = ip;
	++(*iwfr);
L100:
	;
    }
    return 0;
} /* ma27kd_ */

/* Subroutine */ int ma27ld_(n, ipe, nv, ips, ne, na, nd, nsteps, nemin)
integer *n, *ipe, *nv, *ips, *ne, *na, *nd, *nsteps, *nemin;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer ison, i__, k, l, ib, if__, il, is, nr;

    /* Parameter adjustments */
    --nd;
    --na;
    --ne;
    --ips;
    --nv;
    --ipe;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ips[i__] = 0;
	ne[i__] = 0;
/* L10: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nv[i__] > 0) {
	    goto L20;
	}
	if__ = -ipe[i__];
	is = -ips[if__];
	if (is > 0) {
	    ipe[i__] = is;
	}
	ips[if__] = -i__;
L20:
	;
    }
    nr = *n + 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nv[i__] <= 0) {
	    goto L50;
	}
	if__ = -ipe[i__];
	if (if__ == 0) {
	    goto L40;
	}
	is = -ips[if__];
	if (is <= 0) {
	    goto L30;
	}
	ipe[i__] = is;
L30:
	ips[if__] = -i__;
	goto L50;
L40:
	--nr;
	ne[nr] = i__;
L50:
	;
    }
    is = 1;
    i__ = 0;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (i__ > 0) {
	    goto L60;
	}
	i__ = ne[nr];
	ne[nr] = 0;
	++nr;
	il = *n;
	na[*n] = 0;
L60:
	i__2 = *n;
	for (l = 1; l <= i__2; ++l) {
	    if (ips[i__] >= 0) {
		goto L80;
	    }
	    ison = -ips[i__];
	    ips[i__] = 0;
	    i__ = ison;
	    --il;
	    na[il] = 0;
/* L70: */
	}
L80:
	ips[i__] = k;
	++ne[is];
	if (nv[i__] <= 0) {
	    goto L120;
	}
	if (il < *n) {
	    ++na[il + 1];
	}
	na[is] = na[il];
	nd[is] = nv[i__];
	if (na[is] != 1) {
	    goto L90;
	}
	if (nd[is - 1] - ne[is - 1] == nd[is]) {
	    goto L100;
	}
L90:
	if (ne[is] >= *nemin) {
	    goto L110;
	}
	if (na[is] == 0) {
	    goto L110;
	}
	if (ne[is - 1] >= *nemin) {
	    goto L110;
	}
L100:
	na[is - 1] = na[is - 1] + na[is] - 1;
	nd[is - 1] = nd[is] + ne[is - 1];
	ne[is - 1] = ne[is] + ne[is - 1];
	ne[is] = 0;
	goto L120;
L110:
	++is;
L120:
	ib = ipe[i__];
	if (ib >= 0) {
	    if (ib > 0) {
		na[il] = 0;
	    }
	    i__ = ib;
	} else {
	    i__ = -ib;
	    ++il;
	}
/* L160: */
    }
    *nsteps = is - 1;
    return 0;
} /* ma27ld_ */

/* Subroutine */ int ma27md_(n, nz, irn, icn, perm, na, ne, nd, nsteps, lstki,
	 lstkr, iw, info, ops)
integer *n, *nz, *irn, *icn, *perm, *na, *ne, *nd, *nsteps, *lstki, *lstkr, *
	iw, *info;
doublereal *ops;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer iold, jold, iorg, jorg, inew, itop, lstk, nstk, irow, i__, 
	    k;
    static doublereal delim;
    static integer nelim, itree, istki, nassr, istkr, nirnec, nrlnec, niradu, 
	    nrladu, numorg, nirtot, nrltot, nz1, nz2, nfr;

    /* Parameter adjustments */
    --lstkr;
    --lstki;
    --perm;
    --irn;
    --icn;
    --nd;
    --ne;
    --na;
    --iw;
    --info;

    /* Function Body */
    if (*nz == 0) {
	goto L20;
    }
    if (irn[1] != iw[1]) {
	goto L20;
    }
    irn[1] = iw[1] - 1;
    nz2 = 0;
    i__1 = *n;
    for (iold = 1; iold <= i__1; ++iold) {
	inew = perm[iold];
	lstki[inew] = lstkr[iold] + 1;
	nz2 += lstkr[iold];
/* L10: */
    }
    nz1 = nz2 / 2 + *n;
    nz2 += *n;
    goto L60;
L20:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	lstki[i__] = 1;
/* L30: */
    }
    nz1 = *n;
    if (*nz == 0) {
	goto L50;
    }
    i__1 = *nz;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iold = irn[i__];
	jold = icn[i__];
	if (iold < 1 || iold > *n) {
	    goto L40;
	}
	if (jold < 1 || jold > *n) {
	    goto L40;
	}
	if (iold == jold) {
	    goto L40;
	}
	++nz1;
/* Computing MIN */
	i__2 = perm[iold], i__3 = perm[jold];
	irow = min(i__2,i__3);
	++lstki[irow];
L40:
	;
    }
L50:
    nz2 = nz1;
L60:
    istki = 0;
    istkr = 0;
    *ops = 0.;
    nrladu = 0;
    niradu = 1;
    nirtot = nz1;
    nrltot = nz1;
    nirnec = nz2;
    nrlnec = nz2;
    numorg = 0;
    itop = 0;
    i__1 = *nsteps;
    for (itree = 1; itree <= i__1; ++itree) {
	nelim = ne[itree];
	delim = (doublereal) nelim;
	nfr = nd[itree];
	nstk = na[itree];
	nassr = nfr * (nfr + 1) / 2;
	if (nstk != 0) {
	    nassr = nassr - lstkr[itop] + 1;
	}
/* Computing MAX */
	i__2 = nrltot, i__3 = nrladu + nassr + istkr + nz1;
	nrltot = max(i__2,i__3);
/* Computing MAX */
	i__2 = nirtot, i__3 = niradu + nfr + 2 + istki + nz1;
	nirtot = max(i__2,i__3);
/* Computing MAX */
	i__2 = nrlnec, i__3 = nrladu + nassr + istkr + nz2;
	nrlnec = max(i__2,i__3);
/* Computing MAX */
	i__2 = nirnec, i__3 = niradu + nfr + 2 + istki + nz2;
	nirnec = max(i__2,i__3);
	i__2 = nelim;
	for (iorg = 1; iorg <= i__2; ++iorg) {
	    jorg = numorg + iorg;
	    nz2 -= lstki[jorg];
/* L70: */
	}
	numorg += nelim;
	if (nstk <= 0) {
	    goto L90;
	}
	i__2 = nstk;
	for (k = 1; k <= i__2; ++k) {
	    lstk = lstkr[itop];
	    istkr -= lstk;
	    lstk = lstki[itop];
	    istki -= lstk;
	    --itop;
/* L80: */
	}
L90:
	nrladu += nelim * ((nfr << 1) - nelim + 1) / 2;
	niradu = niradu + 2 + nfr;
	if (nelim == 1) {
	    --niradu;
	}
	*ops += (nfr * delim * (nfr + 1) - ((nfr << 1) + 1) * delim * (delim 
		+ 1) / 2 + delim * (delim + 1) * (delim * 2 + 1) / 6) / 2;
	if (itree == *nsteps) {
	    goto L100;
	}
	if (nfr == nelim) {
	    goto L100;
	}
	++itop;
	lstkr[itop] = (nfr - nelim) * (nfr - nelim + 1) / 2;
	lstki[itop] = nfr - nelim + 1;
	istki += lstki[itop];
	istkr += lstkr[itop];
/* Computing MAX */
	i__2 = nirtot, i__3 = niradu + istki + nz1;
	nirtot = max(i__2,i__3);
/* Computing MAX */
	i__2 = nirnec, i__3 = niradu + istki + nz2;
	nirnec = max(i__2,i__3);
L100:
	;
    }
/* Computing MAX */
    i__1 = nrlnec, i__2 = *n + max(*nz,nz1);
    nrlnec = max(i__1,i__2);
/* Computing MAX */
    i__1 = nrltot, i__2 = *n + max(*nz,nz1);
    nrltot = max(i__1,i__2);
    nrlnec = min(nrlnec,nrltot);
    nirnec = max(*nz,nirnec);
    nirtot = max(*nz,nirtot);
    nirnec = min(nirnec,nirtot);
    info[3] = nrltot;
    info[4] = nirtot;
    info[5] = nrlnec;
    info[6] = nirnec;
    info[7] = nrladu;
    info[8] = niradu;
    return 0;
} /* ma27md_ */

/* Subroutine */ int ma27nd_(n, nz, nz1, a, la, irn, icn, iw, liw, perm, iw2, 
	icntl, info)
integer *n, *nz, *nz1;
doublereal *a;
integer *la, *irn, *icn, *iw, *liw, *perm, *iw2, *icntl, *info;
{
    /* Format strings */
    static char fmt_40[] = "(\002 *** WARNING MESSAGE FROM SUBROUTINE MA27B\
D\002,\002  *** INFO(1) =\002,i2)";
    static char fmt_50[] = "(i6,\002TH NON-ZERO (IN ROW\002,i6,\002 AND COLU\
MN\002,i6,\002) IGNORED\002)";

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer iold, jold, inew;
    static doublereal anow;
    static integer jnew, ipos, jpos, i__, k;
    static doublereal anext;
    static integer j1, j2, ia, ii, jj, ich, iiw;

    /* Fortran I/O blocks */
    static cilist io___212 = { 0, 0, 0, fmt_40, 0 };
    static cilist io___213 = { 0, 0, 0, fmt_50, 0 };


    /* Parameter adjustments */
    --iw2;
    --perm;
    --a;
    --irn;
    --icn;
    --iw;
    --icntl;
    --info;

    /* Function Body */
    info[1] = 0;
    ia = *la;
    i__1 = *n;
    for (iold = 1; iold <= i__1; ++iold) {
	iw2[iold] = 1;
	a[ia] = 0.;
	--ia;
/* L10: */
    }
    info[2] = 0;
    *nz1 = *n;
    if (*nz == 0) {
	goto L80;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	iold = irn[k];
	if (iold > *n || iold <= 0) {
	    goto L30;
	}
	jold = icn[k];
	if (jold > *n || jold <= 0) {
	    goto L30;
	}
	inew = perm[iold];
	jnew = perm[jold];
	if (inew != jnew) {
	    goto L20;
	}
	ia = *la - *n + iold;
	a[ia] += a[k];
	goto L60;
L20:
	inew = min(inew,jnew);
	++iw2[inew];
	iw[k] = -iold;
	++(*nz1);
	goto L70;
L30:
	info[1] = 1;
	++info[2];
	if (info[2] <= 1 && icntl[2] > 0) {
	    io___212.ciunit = icntl[2];
	    s_wsfe(&io___212);
	    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
	    e_wsfe();
	}
	if (info[2] <= 10 && icntl[2] > 0) {
	    io___213.ciunit = icntl[2];
	    s_wsfe(&io___213);
	    do_fio(&c__1, (char *)&k, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&irn[k], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&icn[k], (ftnlen)sizeof(integer));
	    e_wsfe();
	}
L60:
	iw[k] = 0;
L70:
	;
    }
L80:
    if (*nz < *nz1 && *nz1 != *n) {
	goto L100;
    }
    k = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k += iw2[i__];
	iw2[i__] = k;
/* L90: */
    }
    goto L120;
L100:
    k = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = k + iw2[i__] - 1;
	iw2[i__] = k;
/* L110: */
    }
L120:
    if (*nz1 > *liw) {
	goto L210;
    }
    if (*nz1 + *n > *la) {
	goto L220;
    }
    if (*nz1 == *n) {
	goto L180;
    }
    i__1 = *nz;
    for (k = 1; k <= i__1; ++k) {
	iold = -iw[k];
	if (iold <= 0) {
	    goto L140;
	}
	jold = icn[k];
	anow = a[k];
	iw[k] = 0;
	i__2 = *nz;
	for (ich = 1; ich <= i__2; ++ich) {
	    inew = perm[iold];
	    jnew = perm[jold];
	    inew = min(inew,jnew);
	    if (inew == perm[jold]) {
		jold = iold;
	    }
	    jpos = iw2[inew] - 1;
	    iold = -iw[jpos];
	    anext = a[jpos];
	    a[jpos] = anow;
	    iw[jpos] = jold;
	    iw2[inew] = jpos;
	    if (iold == 0) {
		goto L140;
	    }
	    anow = anext;
	    jold = icn[jpos];
/* L130: */
	}
L140:
	;
    }
    if (*nz >= *nz1) {
	goto L180;
    }
    ipos = *nz1;
    jpos = *nz1 - *n;
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = *n - ii + 1;
	j1 = iw2[i__];
	j2 = jpos;
	if (j1 > jpos) {
	    goto L160;
	}
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    iw[ipos] = iw[jpos];
	    a[ipos] = a[jpos];
	    --ipos;
	    --jpos;
/* L150: */
	}
L160:
	iw2[i__] = ipos + 1;
	--ipos;
/* L170: */
    }
L180:
    i__1 = *n;
    for (iold = 1; iold <= i__1; ++iold) {
	inew = perm[iold];
	jpos = iw2[inew] - 1;
	ia = *la - *n + iold;
	a[jpos] = a[ia];
	iw[jpos] = -iold;
/* L190: */
    }
    ipos = *nz1;
    ia = *la;
    iiw = *liw;
    i__1 = *nz1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[ia] = a[ipos];
	iw[iiw] = iw[ipos];
	--ipos;
	--ia;
	--iiw;
/* L200: */
    }
    goto L230;
L210:
    info[1] = -3;
    info[2] = *nz1;
    goto L230;
L220:
    info[1] = -4;
    info[2] = *nz1 + *n;
L230:
    return 0;
} /* ma27nd_ */

/* Subroutine */ int ma27od_(n, nz, a, la, iw, liw, perm, nstk, nsteps, 
	maxfrt, nelim, iw2, icntl, cntl, info)
integer *n, *nz;
doublereal *a;
integer *la, *iw, *liw, *perm, *nstk, *nsteps, *maxfrt, *nelim, *iw2, *icntl;
doublereal *cntl;
integer *info;
{
    /* Format strings */
    static char fmt_310[] = "(\002 *** WARNING MESSAGE FROM SUBROUTINE MA2\
7BD\002,\002  *** INFO(1) =\002,i2,/,\002 PIVOT\002,i6,\002 HAS DIFFERENT SI\
GN FROM THE PREVIOUS ONE\002)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer ibeg, iend, neig, iell;
    static doublereal amax;
    static integer jcol, nblk, iass, iorg, apos, astk, jmax, jnew;
    static doublereal rmax;
    static integer ipiv;
    static doublereal tmax;
    static integer ipos, istk, jpiv, jpos, kmax, irow, krow, nass, npiv, ntwo;
    static doublereal swop;
    static integer apos1, apos2, apos3, astk2, istk2, i__, j, k, laell;
    extern /* Subroutine */ int ma27pd_();
    static integer iexch, liell, newel, jlast, azero, lnass;
    static doublereal amult;
    static integer ipmnp, jnext, lnpiv, j1, j2, k1, k2, iswop, iwpos, lapos2;
    static doublereal amult1, amult2;
    static integer npivp1, jj, kk, pospv1, pospv2, lt, ncmpbi;
    static doublereal uu;
    static integer posfac, ncmpbr, nirbdu, nrlbdu, ioldps;
    static doublereal detpiv, thresh;
    static integer ainput, jfirst, idummy, jdummy, jj1, jmxmip, iinput, 
	    isnpiv, kdummy, nfront, numass, numorg, numstk, pivsiz, ltopst, 
	    ntotpv, jjj, ifr;

    /* Fortran I/O blocks */
    static cilist io___280 = { 0, 0, 0, fmt_310, 0 };


    /* Parameter adjustments */
    --iw2;
    --perm;
    --a;
    --iw;
    --nelim;
    --nstk;
    --icntl;
    --cntl;
    --info;

    /* Function Body */
    nblk = 0;
    ntwo = 0;
    neig = 0;
    ncmpbi = 0;
    ncmpbr = 0;
    *maxfrt = 0;
    nrlbdu = 0;
    nirbdu = 0;
    uu = min(cntl[1],.5);
    uu = max(uu,-.5);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iw2[i__] = 0;
/* L10: */
    }
    iwpos = 2;
    posfac = 1;
    istk = *liw - *nz + 1;
    istk2 = istk - 1;
    astk = *la - *nz + 1;
    astk2 = astk - 1;
    iinput = istk;
    ainput = astk;
    azero = 0;
    ntotpv = 0;
    numass = 0;
    i__1 = *nsteps;
    for (iass = 1; iass <= i__1; ++iass) {
	nass = nelim[iass];
	newel = iwpos + 1;
	jfirst = *n + 1;
	nfront = 0;
	numstk = nstk[iass];
	ltopst = 1;
	lnass = 0;
	if (numstk == 0) {
	    goto L80;
	}
	j2 = istk - 1;
	lnass = nass;
	ltopst = (iw[istk] + 1) * iw[istk] / 2;
	i__2 = numstk;
	for (iell = 1; iell <= i__2; ++iell) {
	    jnext = jfirst;
	    jlast = *n + 1;
	    j1 = j2 + 2;
	    j2 = j1 - 1 + iw[j1 - 1];
	    i__3 = j2;
	    for (jj = j1; jj <= i__3; ++jj) {
		j = iw[jj];
		if (iw2[j] > 0) {
		    goto L60;
		}
		jnew = perm[j];
		if (jnew <= numass) {
		    ++nass;
		}
		i__4 = *n;
		for (idummy = 1; idummy <= i__4; ++idummy) {
		    if (jnext == *n + 1) {
			goto L30;
		    }
		    if (perm[jnext] > jnew) {
			goto L30;
		    }
		    jlast = jnext;
		    jnext = iw2[jlast];
/* L20: */
		}
L30:
		if (jlast != *n + 1) {
		    goto L40;
		}
		jfirst = j;
		goto L50;
L40:
		iw2[jlast] = j;
L50:
		iw2[j] = jnext;
		jlast = j;
		++nfront;
L60:
		;
	    }
/* L70: */
	}
	lnass = nass - lnass;
L80:
	numorg = nelim[iass];
	j1 = iinput;
	i__2 = numorg;
	for (iorg = 1; iorg <= i__2; ++iorg) {
	    j = -iw[j1];
	    i__3 = *liw;
	    for (idummy = 1; idummy <= i__3; ++idummy) {
		jnew = perm[j];
		if (iw2[j] > 0) {
		    goto L130;
		}
		jlast = *n + 1;
		jnext = jfirst;
		i__4 = *n;
		for (jdummy = 1; jdummy <= i__4; ++jdummy) {
		    if (jnext == *n + 1) {
			goto L100;
		    }
		    if (perm[jnext] > jnew) {
			goto L100;
		    }
		    jlast = jnext;
		    jnext = iw2[jlast];
/* L90: */
		}
L100:
		if (jlast != *n + 1) {
		    goto L110;
		}
		jfirst = j;
		goto L120;
L110:
		iw2[jlast] = j;
L120:
		iw2[j] = jnext;
		++nfront;
L130:
		++j1;
		if (j1 > *liw) {
		    goto L150;
		}
		j = iw[j1];
		if (j < 0) {
		    goto L150;
		}
/* L140: */
	    }
L150:
	    ;
	}
	if (newel + nfront < istk) {
	    goto L160;
	}
	ma27pd_(&a[1], &iw[1], &istk, &istk2, &iinput, &c__2, &ncmpbr, &
		ncmpbi);
	if (newel + nfront < istk) {
	    goto L160;
	}
	info[2] = *liw + 1 + newel + nfront - istk;
	goto L770;
L160:
	j = jfirst;
	i__2 = nfront;
	for (ifr = 1; ifr <= i__2; ++ifr) {
	    ++newel;
	    iw[newel] = j;
	    jnext = iw2[j];
	    iw2[j] = newel - (iwpos + 1);
	    j = jnext;
/* L170: */
	}
	*maxfrt = max(*maxfrt,nfront);
	iw[iwpos] = nfront;
	laell = (nfront + 1) * nfront / 2;
	apos2 = posfac + laell - 1;
	if (numstk != 0) {
	    lnass = lnass * ((nfront << 1) - lnass + 1) / 2;
	}
	if (posfac + lnass - 1 >= astk) {
	    goto L180;
	}
	if (apos2 < astk + ltopst - 1) {
	    goto L190;
	}
L180:
	ma27pd_(&a[1], &iw[1], &astk, &astk2, &ainput, &c__1, &ncmpbr, &
		ncmpbi);
	if (posfac + lnass - 1 >= astk) {
	    goto L780;
	}
	if (apos2 >= astk + ltopst - 1) {
	    goto L780;
	}
L190:
	if (apos2 <= azero) {
	    goto L220;
	}
	apos = azero + 1;
/* Computing MIN */
	i__2 = apos2, i__3 = astk - 1;
	lapos2 = min(i__2,i__3);
	if (lapos2 < apos) {
	    goto L210;
	}
	i__2 = lapos2;
	for (k = apos; k <= i__2; ++k) {
	    a[k] = 0.;
/* L200: */
	}
L210:
	azero = apos2;
L220:
	if (numstk == 0) {
	    goto L260;
	}
	i__2 = numstk;
	for (iell = 1; iell <= i__2; ++iell) {
	    j1 = istk + 1;
	    j2 = istk + iw[istk];
	    i__3 = j2;
	    for (jj = j1; jj <= i__3; ++jj) {
		irow = iw[jj];
		irow = iw2[irow];
		apos = posfac + (irow - 1) * (2 * nfront - irow + 2) / 2;
		i__4 = j2;
		for (jjj = jj; jjj <= i__4; ++jjj) {
		    j = iw[jjj];
		    apos2 = apos + iw2[j] - irow;
		    a[apos2] += a[astk];
		    a[astk] = 0.;
		    ++astk;
/* L230: */
		}
/* L240: */
	    }
	    istk = j2 + 1;
/* L250: */
	}
L260:
	i__2 = numorg;
	for (iorg = 1; iorg <= i__2; ++iorg) {
	    j = -iw[iinput];
	    irow = iw2[j];
	    apos = posfac + (irow - 1) * (2 * nfront - irow + 2) / 2;
	    i__3 = *nz;
	    for (idummy = 1; idummy <= i__3; ++idummy) {
		apos2 = apos + iw2[j] - irow;
		a[apos2] += a[ainput];
		++ainput;
		++iinput;
		if (iinput > *liw) {
		    goto L280;
		}
		j = iw[iinput];
		if (j < 0) {
		    goto L280;
		}
/* L270: */
	    }
L280:
	    ;
	}
	numass += numorg;
	j1 = iwpos + 2;
	j2 = iwpos + nfront + 1;
	i__2 = j2;
	for (k = j1; k <= i__2; ++k) {
	    j = iw[k];
	    iw2[j] = 0;
/* L290: */
	}
	lnpiv = -1;
	npiv = 0;
	i__2 = nass;
	for (kdummy = 1; kdummy <= i__2; ++kdummy) {
	    if (npiv == nass) {
		goto L660;
	    }
	    if (npiv == lnpiv) {
		goto L660;
	    }
	    lnpiv = npiv;
	    npivp1 = npiv + 1;
	    jpiv = 1;
	    i__3 = nass;
	    for (ipiv = npivp1; ipiv <= i__3; ++ipiv) {
		--jpiv;
		if (jpiv == 1) {
		    goto L640;
		}
		i__4 = nfront - npiv;
		i__5 = ipiv - npiv;
		apos = posfac + (i__5 - 1) * (2 * i__4 - i__5 + 2) / 2;
		if (uu > 0.) {
		    goto L320;
		}
		if ((d__1 = a[apos], abs(d__1)) <= cntl[3]) {
		    goto L790;
		}
		if (ntotpv > 0) {
		    goto L300;
		}
		if (a[apos] > 0.) {
		    isnpiv = 1;
		}
		if (a[apos] < 0.) {
		    isnpiv = -1;
		}
L300:
		if (a[apos] > 0. && isnpiv == 1) {
		    goto L560;
		}
		if (a[apos] < 0. && isnpiv == -1) {
		    goto L560;
		}
		if (info[1] != 2) {
		    info[2] = 0;
		}
		++info[2];
		info[1] = 2;
		i__ = ntotpv + 1;
		if (icntl[2] > 0 && info[2] <= 10) {
		    io___280.ciunit = icntl[2];
		    s_wsfe(&io___280);
		    do_fio(&c__1, (char *)&info[1], (ftnlen)sizeof(integer));
		    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
		    e_wsfe();
		}
		isnpiv = -isnpiv;
		if (uu == 0.) {
		    goto L560;
		}
		goto L800;
L320:
		amax = 0.;
		tmax = amax;
		j1 = apos + 1;
		j2 = apos + nass - ipiv;
		if (j2 < j1) {
		    goto L340;
		}
		i__4 = j2;
		for (jj = j1; jj <= i__4; ++jj) {
		    if ((d__1 = a[jj], abs(d__1)) <= amax) {
			goto L330;
		    }
		    jmax = ipiv + jj - j1 + 1;
		    amax = (d__1 = a[jj], abs(d__1));
L330:
		    ;
		}
L340:
		j1 = j2 + 1;
		j2 = apos + nfront - ipiv;
		if (j2 < j1) {
		    goto L360;
		}
		i__4 = j2;
		for (jj = j1; jj <= i__4; ++jj) {
/* Computing MAX */
		    d__2 = (d__1 = a[jj], abs(d__1));
		    tmax = max(d__2,tmax);
/* L350: */
		}
L360:
		rmax = max(tmax,amax);
		apos1 = apos;
		kk = nfront - ipiv;
		lt = ipiv - (npiv + 1);
		if (lt == 0) {
		    goto L380;
		}
		i__4 = lt;
		for (k = 1; k <= i__4; ++k) {
		    ++kk;
		    apos1 -= kk;
/* Computing MAX */
		    d__2 = rmax, d__3 = (d__1 = a[apos1], abs(d__1));
		    rmax = max(d__2,d__3);
/* L370: */
		}
L380:
/* Computing MAX */
		d__2 = cntl[3], d__3 = uu * rmax;
		if ((d__1 = a[apos], abs(d__1)) > max(d__2,d__3)) {
		    goto L450;
		}
		if (abs(amax) <= cntl[3]) {
		    goto L640;
		}
		i__4 = nfront - npiv;
		i__5 = jmax - npiv;
		apos2 = posfac + (i__5 - 1) * (2 * i__4 - i__5 + 2) / 2;
		detpiv = a[apos] * a[apos2] - amax * amax;
		thresh = abs(detpiv);
/* Computing MAX */
		d__3 = (d__1 = a[apos], abs(d__1)) + amax, d__4 = (d__2 = a[
			apos2], abs(d__2)) + amax;
		thresh /= uu * max(d__3,d__4);
		if (thresh <= rmax) {
		    goto L640;
		}
		rmax = 0.;
		j1 = apos2 + 1;
		j2 = apos2 + nfront - jmax;
		if (j2 < j1) {
		    goto L400;
		}
		i__4 = j2;
		for (jj = j1; jj <= i__4; ++jj) {
/* Computing MAX */
		    d__2 = rmax, d__3 = (d__1 = a[jj], abs(d__1));
		    rmax = max(d__2,d__3);
/* L390: */
		}
L400:
		kk = nfront - jmax + 1;
		apos3 = apos2;
		jmxmip = jmax - ipiv - 1;
		if (jmxmip == 0) {
		    goto L420;
		}
		i__4 = jmxmip;
		for (k = 1; k <= i__4; ++k) {
		    apos2 -= kk;
		    ++kk;
/* Computing MAX */
		    d__2 = rmax, d__3 = (d__1 = a[apos2], abs(d__1));
		    rmax = max(d__2,d__3);
/* L410: */
		}
L420:
		ipmnp = ipiv - npiv - 1;
		if (ipmnp == 0) {
		    goto L440;
		}
		apos2 -= kk;
		++kk;
		i__4 = ipmnp;
		for (k = 1; k <= i__4; ++k) {
		    apos2 -= kk;
		    ++kk;
/* Computing MAX */
		    d__2 = rmax, d__3 = (d__1 = a[apos2], abs(d__1));
		    rmax = max(d__2,d__3);
/* L430: */
		}
L440:
		if (thresh <= rmax) {
		    goto L640;
		}
		pivsiz = 2;
		goto L460;
L450:
		pivsiz = 1;
L460:
		irow = ipiv - npiv;
		i__4 = pivsiz;
		for (krow = 1; krow <= i__4; ++krow) {
		    if (irow == 1) {
			goto L530;
		    }
		    j1 = posfac + irow;
		    j2 = posfac + nfront - (npiv + 1);
		    if (j2 < j1) {
			goto L480;
		    }
		    apos2 = apos + 1;
		    i__5 = j2;
		    for (jj = j1; jj <= i__5; ++jj) {
			swop = a[apos2];
			a[apos2] = a[jj];
			a[jj] = swop;
			++apos2;
/* L470: */
		    }
L480:
		    j1 = posfac + 1;
		    j2 = posfac + irow - 2;
		    apos2 = apos;
		    kk = nfront - (irow + npiv);
		    if (j2 < j1) {
			goto L500;
		    }
		    i__5 = j2;
		    for (jjj = j1; jjj <= i__5; ++jjj) {
			jj = j2 - jjj + j1;
			++kk;
			apos2 -= kk;
			swop = a[apos2];
			a[apos2] = a[jj];
			a[jj] = swop;
/* L490: */
		    }
L500:
		    if (npiv == 0) {
			goto L520;
		    }
		    apos1 = posfac;
		    ++kk;
		    apos2 -= kk;
		    i__5 = npiv;
		    for (jj = 1; jj <= i__5; ++jj) {
			++kk;
			apos1 -= kk;
			apos2 -= kk;
			swop = a[apos2];
			a[apos2] = a[apos1];
			a[apos1] = swop;
/* L510: */
		    }
L520:
		    swop = a[apos];
		    a[apos] = a[posfac];
		    a[posfac] = swop;
		    ipos = iwpos + npiv + 2;
		    iexch = iwpos + irow + npiv + 1;
		    iswop = iw[ipos];
		    iw[ipos] = iw[iexch];
		    iw[iexch] = iswop;
L530:
		    if (pivsiz == 1) {
			goto L550;
		    }
		    if (krow == 2) {
			goto L540;
		    }
		    irow = jmax - (npiv + 1);
		    jpos = posfac;
		    posfac = posfac + nfront - npiv;
		    ++npiv;
		    apos = apos3;
		    goto L550;
L540:
		    --npiv;
		    posfac = jpos;
L550:
		    ;
		}
		if (pivsiz == 2) {
		    goto L600;
		}
L560:
		a[posfac] = 1. / a[posfac];
		if (a[posfac] < 0.) {
		    ++neig;
		}
		j1 = posfac + 1;
		j2 = posfac + nfront - (npiv + 1);
		if (j2 < j1) {
		    goto L590;
		}
		ibeg = j2 + 1;
		i__4 = j2;
		for (jj = j1; jj <= i__4; ++jj) {
		    amult = -a[jj] * a[posfac];
		    iend = ibeg + nfront - (npiv + jj - j1 + 2);
/* DIR$ IVDEP */
		    i__5 = iend;
		    for (irow = ibeg; irow <= i__5; ++irow) {
			jcol = jj + irow - ibeg;
			a[irow] += amult * a[jcol];
/* L570: */
		    }
		    ibeg = iend + 1;
		    a[jj] = amult;
/* L580: */
		}
L590:
		++npiv;
		++ntotpv;
		jpiv = 1;
		posfac = posfac + nfront - npiv + 1;
		goto L640;
L600:
		ipos = iwpos + npiv + 2;
		++ntwo;
		iw[ipos] = -iw[ipos];
		pospv1 = posfac;
		pospv2 = posfac + nfront - npiv;
		swop = a[pospv2];
		if (detpiv < 0.) {
		    ++neig;
		}
		if (detpiv > 0. && swop < 0.) {
		    neig += 2;
		}
		a[pospv2] = a[pospv1] / detpiv;
		a[pospv1] = swop / detpiv;
		a[pospv1 + 1] = -a[pospv1 + 1] / detpiv;
		j1 = pospv1 + 2;
		j2 = pospv1 + nfront - (npiv + 1);
		if (j2 < j1) {
		    goto L630;
		}
		jj1 = pospv2;
		ibeg = pospv2 + nfront - (npiv + 1);
		i__4 = j2;
		for (jj = j1; jj <= i__4; ++jj) {
		    ++jj1;
		    amult1 = -(a[pospv1] * a[jj] + a[pospv1 + 1] * a[jj1]);
		    amult2 = -(a[pospv1 + 1] * a[jj] + a[pospv2] * a[jj1]);
		    iend = ibeg + nfront - (npiv + jj - j1 + 3);
/* DIR$ IVDEP */
		    i__5 = iend;
		    for (irow = ibeg; irow <= i__5; ++irow) {
			k1 = jj + irow - ibeg;
			k2 = jj1 + irow - ibeg;
			a[irow] = a[irow] + amult1 * a[k1] + amult2 * a[k2];
/* L610: */
		    }
		    ibeg = iend + 1;
		    a[jj] = amult1;
		    a[jj1] = amult2;
/* L620: */
		}
L630:
		npiv += 2;
		ntotpv += 2;
		jpiv = 2;
		posfac = pospv2 + nfront - npiv + 1;
L640:
		;
	    }
/* L650: */
	}
L660:
	if (npiv != 0) {
	    ++nblk;
	}
	ioldps = iwpos;
	iwpos = iwpos + nfront + 2;
	if (npiv == 0) {
	    goto L690;
	}
	if (npiv > 1) {
	    goto L680;
	}
	iw[ioldps] = -iw[ioldps];
	i__2 = nfront;
	for (k = 1; k <= i__2; ++k) {
	    j1 = ioldps + k;
	    iw[j1] = iw[j1 + 1];
/* L670: */
	}
	--iwpos;
	goto L690;
L680:
	iw[ioldps + 1] = npiv;
L690:
	liell = nfront - npiv;
	if (liell == 0 || iass == *nsteps) {
	    goto L750;
	}
	if (iwpos + liell < istk) {
	    goto L700;
	}
	ma27pd_(&a[1], &iw[1], &istk, &istk2, &iinput, &c__2, &ncmpbr, &
		ncmpbi);
L700:
	istk = istk - liell - 1;
	iw[istk] = liell;
	j1 = istk;
	kk = iwpos - liell - 1;
/* DIR$ IVDEP */
	i__2 = liell;
	for (k = 1; k <= i__2; ++k) {
	    ++j1;
	    ++kk;
	    iw[j1] = iw[kk];
/* L710: */
	}
	laell = (liell + 1) * liell / 2;
	kk = posfac + laell;
	if (kk != astk) {
	    goto L720;
	}
	astk -= laell;
	goto L740;
L720:
	kmax = kk - 1;
/* DIR$ IVDEP */
	i__2 = laell;
	for (k = 1; k <= i__2; ++k) {
	    --kk;
	    --astk;
	    a[astk] = a[kk];
/* L730: */
	}
/* Computing MIN */
	i__2 = kmax, i__3 = astk - 1;
	kmax = min(i__2,i__3);
	i__2 = kmax;
	for (k = kk; k <= i__2; ++k) {
	    a[k] = 0.;
/* L735: */
	}
L740:
/* Computing MIN */
	i__2 = azero, i__3 = astk - 1;
	azero = min(i__2,i__3);
L750:
	if (npiv == 0) {
	    iwpos = ioldps;
	}
/* L760: */
    }
    iw[1] = nblk;
    if (ntwo > 0) {
	iw[1] = -nblk;
    }
    nrlbdu = posfac - 1;
    nirbdu = iwpos - 1;
    if (ntotpv == *n) {
	goto L810;
    }
    info[1] = 3;
    info[2] = ntotpv;
    goto L810;
L770:
    info[1] = -3;
    goto L810;
L780:
    info[1] = -4;
/* Computing MAX */
    i__1 = posfac + lnass, i__2 = apos2 - ltopst + 2;
    info[2] = *la + max(i__1,i__2) - astk;
    goto L810;
L790:
    info[1] = -5;
    info[2] = ntotpv + 1;
    goto L810;
L800:
    info[1] = -6;
    info[2] = ntotpv + 1;
L810:
    info[9] = nrlbdu;
    info[10] = nirbdu;
    info[12] = ncmpbr;
    info[13] = ncmpbi;
    info[14] = ntwo;
    info[15] = neig;
    return 0;
} /* ma27od_ */

/* Subroutine */ int ma27pd_(a, iw, j1, j2, itop, ireal, ncmpbr, ncmpbi)
doublereal *a;
integer *iw, *j1, *j2, *itop, *ireal, *ncmpbr, *ncmpbi;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer ipos, jj, jjj;

    /* Parameter adjustments */
    --iw;
    --a;

    /* Function Body */
    ipos = *itop - 1;
    if (*j2 == ipos) {
	goto L50;
    }
    if (*ireal == 2) {
	goto L20;
    }
    ++(*ncmpbr);
    if (*j1 > *j2) {
	goto L40;
    }
    i__1 = *j2;
    for (jjj = *j1; jjj <= i__1; ++jjj) {
	jj = *j2 - jjj + *j1;
	a[ipos] = a[jj];
	--ipos;
/* L10: */
    }
    goto L40;
L20:
    ++(*ncmpbi);
    if (*j1 > *j2) {
	goto L40;
    }
    i__1 = *j2;
    for (jjj = *j1; jjj <= i__1; ++jjj) {
	jj = *j2 - jjj + *j1;
	iw[ipos] = iw[jj];
	--ipos;
/* L30: */
    }
L40:
    *j2 = *itop - 1;
    *j1 = ipos + 1;
L50:
    return 0;
} /* ma27pd_ */

/* Subroutine */ int ma27qd_(n, a, la, iw, liw, w, maxfnt, rhs, iw2, nblk, 
	latop, icntl)
integer *n;
doublereal *a;
integer *la, *iw, *liw;
doublereal *w;
integer *maxfnt;
doublereal *rhs;
integer *iw2, *nblk, *latop, *icntl;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer iblk, apos, irhs, ilvl, ipiv, jpiv, ipos, npiv, irow, j, k,
	     liell, j1, j2, j3, k1, k2, k3;
    static doublereal w1, w2;
    static integer jj, ifr, ist;

    /* Parameter adjustments */
    --rhs;
    --a;
    --iw;
    --w;
    --iw2;
    --icntl;

    /* Function Body */
    apos = 1;
    ipos = 1;
    j2 = 0;
    iblk = 0;
    npiv = 0;
    i__1 = *n;
    for (irow = 1; irow <= i__1; ++irow) {
	if (npiv > 0) {
	    goto L90;
	}
	++iblk;
	if (iblk > *nblk) {
	    goto L150;
	}
	ipos = j2 + 1;
	iw2[iblk] = ipos;
	liell = -iw[ipos];
	npiv = 1;
	if (liell > 0) {
	    goto L10;
	}
	liell = -liell;
	++ipos;
	npiv = iw[ipos];
L10:
	j1 = ipos + 1;
	j2 = ipos + liell;
	ilvl = min(npiv,10);
	if (liell < icntl[ilvl + 5]) {
	    goto L90;
	}
	ifr = 0;
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    j = (i__3 = iw[jj], abs(i__3));
	    ++ifr;
	    w[ifr] = rhs[j];
/* L20: */
	}
	jpiv = 1;
	j3 = j1;
	i__2 = npiv;
	for (ipiv = 1; ipiv <= i__2; ++ipiv) {
	    --jpiv;
	    if (jpiv == 1) {
		goto L70;
	    }
	    if (iw[j3] < 0) {
		goto L40;
	    }
	    jpiv = 1;
	    ++j3;
	    ++apos;
	    ist = ipiv + 1;
	    if (liell < ist) {
		goto L70;
	    }
	    w1 = w[ipiv];
	    k = apos;
	    i__3 = liell;
	    for (j = ist; j <= i__3; ++j) {
		w[j] += a[k] * w1;
		++k;
/* L30: */
	    }
	    apos = apos + liell - ist + 1;
	    goto L70;
L40:
	    jpiv = 2;
	    j3 += 2;
	    apos += 2;
	    ist = ipiv + 2;
	    if (liell < ist) {
		goto L60;
	    }
	    w1 = w[ipiv];
	    w2 = w[ipiv + 1];
	    k1 = apos;
	    k2 = apos + liell - ipiv;
	    i__3 = liell;
	    for (j = ist; j <= i__3; ++j) {
		w[j] = w[j] + w1 * a[k1] + w2 * a[k2];
		++k1;
		++k2;
/* L50: */
	    }
L60:
	    apos = apos + ((liell - ist + 1) << 1) + 1;
L70:
	    ;
	}
	ifr = 0;
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    j = (i__3 = iw[jj], abs(i__3));
	    ++ifr;
	    rhs[j] = w[ifr];
/* L80: */
	}
	npiv = 0;
	goto L140;
L90:
	if (iw[j1] < 0) {
	    goto L110;
	}
	--npiv;
	++apos;
	++j1;
	if (j1 > j2) {
	    goto L140;
	}
	irhs = iw[j1 - 1];
	w1 = rhs[irhs];
	k = apos;
	i__2 = j2;
	for (j = j1; j <= i__2; ++j) {
	    irhs = (i__3 = iw[j], abs(i__3));
	    rhs[irhs] += a[k] * w1;
	    ++k;
/* L100: */
	}
	apos = apos + j2 - j1 + 1;
	goto L140;
L110:
	npiv += -2;
	j1 += 2;
	apos += 2;
	if (j1 > j2) {
	    goto L130;
	}
	irhs = -iw[j1 - 2];
	w1 = rhs[irhs];
	irhs = iw[j1 - 1];
	w2 = rhs[irhs];
	k1 = apos;
	k3 = apos + j2 - j1 + 2;
	i__2 = j2;
	for (j = j1; j <= i__2; ++j) {
	    irhs = (i__3 = iw[j], abs(i__3));
	    rhs[irhs] = rhs[irhs] + w1 * a[k1] + w2 * a[k3];
	    ++k1;
	    ++k3;
/* L120: */
	}
L130:
	apos = apos + ((j2 - j1 + 1) << 1) + 1;
L140:
	;
    }
L150:
    *latop = apos - 1;
    return 0;
} /* ma27qd_ */

/* Subroutine */ int ma27rd_(n, a, la, iw, liw, w, maxfnt, rhs, iw2, nblk, 
	latop, icntl)
integer *n;
doublereal *a;
integer *la, *iw, *liw;
doublereal *w;
integer *maxfnt;
doublereal *rhs;
integer *iw2, *nblk, *latop, *icntl;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer iblk, apos, irhs, ilvl, ipiv, jpiv, loop, ipos, jpos, npiv,
	     apos2, i1rhs, i2rhs, j, k, liell, iirhs, iipiv, j1, j2;
    static doublereal w1, w2;
    static integer jj, jj1, jj2, ifr, ist;

    /* Parameter adjustments */
    --rhs;
    --a;
    --iw;
    --w;
    --iw2;
    --icntl;

    /* Function Body */
    apos = *latop + 1;
    npiv = 0;
    iblk = *nblk + 1;
    i__1 = *n;
    for (loop = 1; loop <= i__1; ++loop) {
	if (npiv > 0) {
	    goto L110;
	}
	--iblk;
	if (iblk < 1) {
	    goto L190;
	}
	ipos = iw2[iblk];
	liell = -iw[ipos];
	npiv = 1;
	if (liell > 0) {
	    goto L10;
	}
	liell = -liell;
	++ipos;
	npiv = iw[ipos];
L10:
	jpos = ipos + npiv;
	j2 = ipos + liell;
	ilvl = min(10,npiv) + 10;
	if (liell < icntl[ilvl + 5]) {
	    goto L110;
	}
	j1 = ipos + 1;
	ifr = 0;
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    j = (i__3 = iw[jj], abs(i__3));
	    ++ifr;
	    w[ifr] = rhs[j];
/* L20: */
	}
	jpiv = 1;
	i__2 = npiv;
	for (iipiv = 1; iipiv <= i__2; ++iipiv) {
	    --jpiv;
	    if (jpiv == 1) {
		goto L90;
	    }
	    ipiv = npiv - iipiv + 1;
	    if (ipiv == 1) {
		goto L30;
	    }
	    if (iw[jpos - 1] < 0) {
		goto L60;
	    }
L30:
	    jpiv = 1;
	    apos -= liell + 1 - ipiv;
	    ist = ipiv + 1;
	    w1 = w[ipiv] * a[apos];
	    if (liell < ist) {
		goto L50;
	    }
	    jj1 = apos + 1;
	    i__3 = liell;
	    for (j = ist; j <= i__3; ++j) {
		w1 += a[jj1] * w[j];
		++jj1;
/* L40: */
	    }
L50:
	    w[ipiv] = w1;
	    --jpos;
	    goto L90;
L60:
	    jpiv = 2;
	    apos2 = apos - (liell + 1 - ipiv);
	    apos = apos2 - (liell + 2 - ipiv);
	    ist = ipiv + 1;
	    w1 = w[ipiv - 1] * a[apos] + w[ipiv] * a[apos + 1];
	    w2 = w[ipiv - 1] * a[apos + 1] + w[ipiv] * a[apos2];
	    if (liell < ist) {
		goto L80;
	    }
	    jj1 = apos + 2;
	    jj2 = apos2 + 1;
	    i__3 = liell;
	    for (j = ist; j <= i__3; ++j) {
		w1 += w[j] * a[jj1];
		w2 += w[j] * a[jj2];
		++jj1;
		++jj2;
/* L70: */
	    }
L80:
	    w[ipiv - 1] = w1;
	    w[ipiv] = w2;
	    jpos += -2;
L90:
	    ;
	}
	ifr = 0;
	i__2 = j2;
	for (jj = j1; jj <= i__2; ++jj) {
	    j = (i__3 = iw[jj], abs(i__3));
	    ++ifr;
	    rhs[j] = w[ifr];
/* L100: */
	}
	npiv = 0;
	goto L180;
L110:
	if (npiv == 1) {
	    goto L120;
	}
	if (iw[jpos - 1] < 0) {
	    goto L150;
	}
L120:
	--npiv;
	apos -= j2 - jpos + 1;
	iirhs = iw[jpos];
	w1 = rhs[iirhs] * a[apos];
	j1 = jpos + 1;
	if (j1 > j2) {
	    goto L140;
	}
	k = apos + 1;
	i__2 = j2;
	for (j = j1; j <= i__2; ++j) {
	    irhs = (i__3 = iw[j], abs(i__3));
	    w1 += a[k] * rhs[irhs];
	    ++k;
/* L130: */
	}
L140:
	rhs[iirhs] = w1;
	--jpos;
	goto L180;
L150:
	npiv += -2;
	apos2 = apos - (j2 - jpos + 1);
	apos = apos2 - (j2 - jpos + 2);
	i1rhs = -iw[jpos - 1];
	i2rhs = iw[jpos];
	w1 = rhs[i1rhs] * a[apos] + rhs[i2rhs] * a[apos + 1];
	w2 = rhs[i1rhs] * a[apos + 1] + rhs[i2rhs] * a[apos2];
	j1 = jpos + 1;
	if (j1 > j2) {
	    goto L170;
	}
	jj1 = apos + 2;
	jj2 = apos2 + 1;
	i__2 = j2;
	for (j = j1; j <= i__2; ++j) {
	    irhs = (i__3 = iw[j], abs(i__3));
	    w1 += rhs[irhs] * a[jj1];
	    w2 += rhs[irhs] * a[jj2];
	    ++jj1;
	    ++jj2;
/* L160: */
	}
L170:
	rhs[i1rhs] = w1;
	rhs[i2rhs] = w2;
	jpos += -2;
L180:
	;
    }
L190:
    return 0;
} /* ma27rd_ */

