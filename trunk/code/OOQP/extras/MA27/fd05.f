* COPYRIGHT (c) 1988 AEA Technology
*######DATE 21 Jan 1993
C       Toolpack tool decs employed.
C       SAVE statement added.
C 1/10/98 DC(3) not initialized to avoid SUN f90 failure
C 16 October 2001: STOP and WRITE statements removed.

      DOUBLE PRECISION FUNCTION FD05AD(INUM)
C----------------------------------------------------------------
C  Real constants for: IEEE double precision (8-byte arithmetic)
C
C  Obtained from H.S.L. subroutine ZE02AM.
C  Nick Gould and Sid Marlow, Harwell Laboratory, April 1988.
C----------------------------------------------------------------
C     .. Scalar Arguments ..
      INTEGER INUM
C     ..
C     .. Local Arrays ..
      DOUBLE PRECISION DC(5)
C     ..
C     .. Save statement ..
      SAVE DC
C     ..
C     .. Data statements ..
C
C  DC(1) THE SMALLEST POSITIVE NUMBER: 1.0 + DC(1) > 1.0.
C  DC(2) THE SMALLEST POSITIVE NUMBER: 1.0 - DC(2) < 1.0.
C  DC(3) THE SMALLEST NONZERO +VE REAL NUMBER.
C  DC(4) THE SMALLEST FULL PRECISION +VE REAL NUMBER.
C  DC(5) THE LARGEST FINITE +VE REAL NUMBER.
C
      DATA DC(1)/2.2204460492504D-16/
      DATA DC(2)/1.1102230246253D-16/
C     DATA DC(3)/4.9406564584126D-324/
      DATA DC(4)/2.2250738585073D-308/
      DATA DC(5)/1.7976931348622D+308/
C     ..
C     .. Executable Statements ..

      IF ( INUM .LE. 0 ) THEN
         FD05AD = DC( 1 )
      ELSE IF ( INUM .GE. 6 ) THEN
         FD05AD = DC( 5 )
      ELSE IF ( INUM .EQ. 3 ) THEN
         FD05AD = DC(4)/2.0D0**52
      ELSE
         FD05AD = DC( INUM )
      ENDIF
      RETURN
      END
