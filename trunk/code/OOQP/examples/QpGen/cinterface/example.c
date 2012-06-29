/* OOQP                                                               *
 * Authors: E. Michael Gertz, Stephen J. Wright                       *
 * (C) 2001 University of Chicago. See Copyright Notification in OOQP */

#include "cQpGenSparse.h"
#include "stdio.h"

#define PRINT_LEVEL 0

const int nx   = 2;
double    c[]  = { 1.5,  -2 };

double  xupp[] = { 20,   0 };
char   ixupp[] = {  1,   0 };

double  xlow[] = {  0,   0 };
char   ixlow[] = {  1,   1 };

const int nnzQ = 3;
int    irowQ[] = {  0,   1,   1 }; 
int    jcolQ[] = {  0,   0,   1 };
double    dQ[] = {  8,   2,  10 };


const int my         = 0;
double * b     = 0;
const int nnzA       = 0;
int * irowA    = 0;
int * jcolA    = 0;
double * dA    = 0;

const int mz   = 2;
double clow[]  = { 2,   0 };
char  iclow[]  = { 1,   0 };

double cupp[]  = { 0,   6 };
char  icupp[]  = { 0,   1 };

const int nnzC = 4;
int   irowC[]  = { 0,   0,   1,   1};
int   jcolC[]  = { 0,   1,   0,   1};
double   dC[]  = { 2,   1,  -1,   2};


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

  qpsolvesp( c, nx, irowQ, nnzQ, jcolQ, dQ, xlow, ixlow, xupp, ixupp,
	     irowA, nnzA, jcolA, dA, b, my,
	     irowC, nnzC, jcolC, dC,
	     clow,  mz,   iclow, cupp, icupp,
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
