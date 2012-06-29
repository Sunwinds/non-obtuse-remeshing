/* OOQP                                                               *
 * Authors: E. Michael Gertz, Stephen J. Wright                       *
 * (C) 2001 University of Chicago. See Copyright Notification in OOQP */

#include "cQpGenDense.h"
#include "stdio.h"

#define PRINT_LEVEL 0

const int nx   = 2;
double    c[]  = { 1.5,  -2 };
double    Q[]  = {8, 2, 2, 10};
double  xupp[] = { 20,   0 };
char   ixupp[] = {  1,   0 };

double  xlow[] = {  0,   0 };
char   ixlow[] = {  1,   1 };

const int my         = 0;
double * A = NULL;
double * b     = NULL;

const int mz   = 2;
double clow[]  = { 2,   0 };
char  iclow[]  = { 1,   0 };

double cupp[]  = { 0,   6 };
char  icupp[]  = { 0,   1 };

double   C[]  = { 2,   1,  -1,   2};


int main()
{
  int ierr;
  int i;

  /* double x[nx], gamma[nx], phi[nx]; */
  /* dumb compilers don't treat const correctly */
  double x[2], gamma[2], phi[2];

  double *y = 0;

  /* double z[mz], lambda[mz], pi[mz]; */
  double z[2], lambda[2], pi[2];

  qpsolvede( c, nx, Q, xlow, ixlow, xupp, ixupp,
	     A, my, b,
	     C, mz, clow, iclow, cupp, icupp,
	     x, gamma, phi,
	     y, 
	     z, lambda, pi, PRINT_LEVEL, &ierr );

  if( ierr != 0 ) {
    fprintf( stderr, "Couldn't solve it.\n" );
    return 1;
  } else {
    printf( "Solution:...\n" );
    for( i = 0; i < nx; i++ ) {
      printf( "x[%2d] = %g\n", i, x[i] );
    }
    return 0;
  }
}
