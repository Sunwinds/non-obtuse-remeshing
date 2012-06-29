/* dpotrf.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;
static doublereal c_b13 = -1.;
static doublereal c_b14 = 1.;

/* Subroutine */ int dpotrf_(uplo, n, a, lda, info, uplo_len)
char *uplo;
integer *n;
doublereal *a;
integer *lda, *info;
ftnlen uplo_len;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int dgemm_();
    extern logical lsame_();
    extern /* Subroutine */ int dtrsm_();
    static logical upper;
    extern /* Subroutine */ int dsyrk_(), dpotf2_();
    static integer jb, nb;
    extern /* Subroutine */ int xerbla_();
    extern integer ilaenv_();


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     March 31, 1993 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DPOTRF computes the Cholesky factorization of a real symmetric */
/*  positive definite matrix A. */

/*  The factorization has the form */
/*     A = U**T * U,  if UPLO = 'U', or */
/*     A = L  * L**T,  if UPLO = 'L', */
/*  where U is an upper triangular matrix and L is lower triangular. */

/*  This is the block version of the algorithm, calling Level 3 BLAS. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A is stored; */
/*          = 'L':  Lower triangle of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N) */
/*          On entry, the symmetric matrix A.  If UPLO = 'U', the leading */
/*          N-by-N upper triangular part of A contains the upper */
/*          triangular part of the matrix A, and the strictly lower */
/*          triangular part of A is not referenced.  If UPLO = 'L', the */
/*          leading N-by-N lower triangular part of A contains the lower */
/*          triangular part of the matrix A, and the strictly upper */
/*          triangular part of A is not referenced. */

/*          On exit, if INFO = 0, the factor U or L from the Cholesky */
/*          factorization A = U**T*U or A = L*L**T. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, the leading minor of order i is not */
/*                positive definite, and the factorization could not be */
/*                completed. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DPOTRF", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Determine the block size for this environment. */

    nb = ilaenv_(&c__1, "DPOTRF", uplo, n, &c_n1, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    if (nb <= 1 || nb >= *n) {

/*        Use unblocked code. */

	dpotf2_(uplo, n, &a[a_offset], lda, info, (ftnlen)1);
    } else {

/*        Use blocked code. */

	if (upper) {

/*           Compute the Cholesky factorization A = U'*U. */

	    i__1 = *n;
	    i__2 = nb;
	    for (j = 1; i__2 < 0 ? j >= i__1 : j <= i__1; j += i__2) {

/*              Update and factorize the current diagonal block and test */
/*              for non-positive-definiteness. */

/* Computing MIN */
		i__3 = nb, i__4 = *n - j + 1;
		jb = min(i__3,i__4);
		i__3 = j - 1;
		dsyrk_("Upper", "Transpose", &jb, &i__3, &c_b13, &a[j * 
			a_dim1 + 1], lda, &c_b14, &a[j + j * a_dim1], lda, (
			ftnlen)5, (ftnlen)9);
		dpotf2_("Upper", &jb, &a[j + j * a_dim1], lda, info, (ftnlen)
			5);
		if (*info != 0) {
		    goto L30;
		}
		if (j + jb <= *n) {

/*                 Compute the current block row. */

		    i__3 = *n - j - jb + 1;
		    i__4 = j - 1;
		    dgemm_("Transpose", "No transpose", &jb, &i__3, &i__4, &
			    c_b13, &a[j * a_dim1 + 1], lda, &a[(j + jb) * 
			    a_dim1 + 1], lda, &c_b14, &a[j + (j + jb) * 
			    a_dim1], lda, (ftnlen)9, (ftnlen)12);
		    i__3 = *n - j - jb + 1;
		    dtrsm_("Left", "Upper", "Transpose", "Non-unit", &jb, &
			    i__3, &c_b14, &a[j + j * a_dim1], lda, &a[j + (j 
			    + jb) * a_dim1], lda, (ftnlen)4, (ftnlen)5, (
			    ftnlen)9, (ftnlen)8);
		}
/* L10: */
	    }

	} else {

/*           Compute the Cholesky factorization A = L*L'. */

	    i__2 = *n;
	    i__1 = nb;
	    for (j = 1; i__1 < 0 ? j >= i__2 : j <= i__2; j += i__1) {

/*              Update and factorize the current diagonal block and test */
/*              for non-positive-definiteness. */

/* Computing MIN */
		i__3 = nb, i__4 = *n - j + 1;
		jb = min(i__3,i__4);
		i__3 = j - 1;
		dsyrk_("Lower", "No transpose", &jb, &i__3, &c_b13, &a[j + 
			a_dim1], lda, &c_b14, &a[j + j * a_dim1], lda, (
			ftnlen)5, (ftnlen)12);
		dpotf2_("Lower", &jb, &a[j + j * a_dim1], lda, info, (ftnlen)
			5);
		if (*info != 0) {
		    goto L30;
		}
		if (j + jb <= *n) {

/*                 Compute the current block column. */

		    i__3 = *n - j - jb + 1;
		    i__4 = j - 1;
		    dgemm_("No transpose", "Transpose", &i__3, &jb, &i__4, &
			    c_b13, &a[j + jb + a_dim1], lda, &a[j + a_dim1], 
			    lda, &c_b14, &a[j + jb + j * a_dim1], lda, (
			    ftnlen)12, (ftnlen)9);
		    i__3 = *n - j - jb + 1;
		    dtrsm_("Right", "Lower", "Transpose", "Non-unit", &i__3, &
			    jb, &c_b14, &a[j + j * a_dim1], lda, &a[j + jb + 
			    j * a_dim1], lda, (ftnlen)5, (ftnlen)5, (ftnlen)9,
			     (ftnlen)8);
		}
/* L20: */
	    }
	}
    }
    goto L40;

L30:
    *info = *info + j - 1;

L40:
    return 0;

/*     End of DPOTRF */

} /* dpotrf_ */

/* Subroutine */ int dpotf2_(uplo, n, a, lda, info, uplo_len)
char *uplo;
integer *n;
doublereal *a;
integer *lda, *info;
ftnlen uplo_len;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    extern doublereal ddot_();
    static integer j;
    extern /* Subroutine */ int dscal_();
    extern logical lsame_();
    extern /* Subroutine */ int dgemv_();
    static logical upper;
    extern /* Subroutine */ int xerbla_();
    static doublereal ajj;


/*  -- LAPACK routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     February 29, 1992 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DPOTF2 computes the Cholesky factorization of a real symmetric */
/*  positive definite matrix A. */

/*  The factorization has the form */
/*     A = U' * U ,  if UPLO = 'U', or */
/*     A = L  * L',  if UPLO = 'L', */
/*  where U is an upper triangular matrix and L is lower triangular. */

/*  This is the unblocked version of the algorithm, calling Level 2 BLAS. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          Specifies whether the upper or lower triangular part of the */
/*          symmetric matrix A is stored. */
/*          = 'U':  Upper triangular */
/*          = 'L':  Lower triangular */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N) */
/*          On entry, the symmetric matrix A.  If UPLO = 'U', the leading */
/*          n by n upper triangular part of A contains the upper */
/*          triangular part of the matrix A, and the strictly lower */
/*          triangular part of A is not referenced.  If UPLO = 'L', the */
/*          leading n by n lower triangular part of A contains the lower */
/*          triangular part of the matrix A, and the strictly upper */
/*          triangular part of A is not referenced. */

/*          On exit, if INFO = 0, the factor U or L from the Cholesky */
/*          factorization A = U'*U  or A = L*L'. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -k, the k-th argument had an illegal value */
/*          > 0: if INFO = k, the leading minor of order k is not */
/*               positive definite, and the factorization could not be */
/*               completed. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DPOTF2", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (upper) {

/*        Compute the Cholesky factorization A = U'*U. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {

/*           Compute U(J,J) and test for non-positive-definiteness. */

	    i__2 = j - 1;
	    ajj = a[j + j * a_dim1] - ddot_(&i__2, &a[j * a_dim1 + 1], &c__1, 
		    &a[j * a_dim1 + 1], &c__1);
	    if (ajj <= 0.) {
		a[j + j * a_dim1] = ajj;
		goto L30;
	    }
	    ajj = sqrt(ajj);
	    a[j + j * a_dim1] = ajj;

/*           Compute elements J+1:N of row J. */

	    if (j < *n) {
		i__2 = j - 1;
		i__3 = *n - j;
		dgemv_("Transpose", &i__2, &i__3, &c_b13, &a[(j + 1) * a_dim1 
			+ 1], lda, &a[j * a_dim1 + 1], &c__1, &c_b14, &a[j + (
			j + 1) * a_dim1], lda, (ftnlen)9);
		i__2 = *n - j;
		d__1 = 1. / ajj;
		dscal_(&i__2, &d__1, &a[j + (j + 1) * a_dim1], lda);
	    }
/* L10: */
	}
    } else {

/*        Compute the Cholesky factorization A = L*L'. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {

/*           Compute L(J,J) and test for non-positive-definiteness. */

	    i__2 = j - 1;
	    ajj = a[j + j * a_dim1] - ddot_(&i__2, &a[j + a_dim1], lda, &a[j 
		    + a_dim1], lda);
	    if (ajj <= 0.) {
		a[j + j * a_dim1] = ajj;
		goto L30;
	    }
	    ajj = sqrt(ajj);
	    a[j + j * a_dim1] = ajj;

/*           Compute elements J+1:N of column J. */

	    if (j < *n) {
		i__2 = *n - j;
		i__3 = j - 1;
		dgemv_("No transpose", &i__2, &i__3, &c_b13, &a[j + 1 + 
			a_dim1], lda, &a[j + a_dim1], lda, &c_b14, &a[j + 1 + 
			j * a_dim1], &c__1, (ftnlen)12);
		i__2 = *n - j;
		d__1 = 1. / ajj;
		dscal_(&i__2, &d__1, &a[j + 1 + j * a_dim1], &c__1);
	    }
/* L20: */
	}
    }
    goto L40;

L30:
    *info = j;

L40:
    return 0;

/*     End of DPOTF2 */

} /* dpotf2_ */

