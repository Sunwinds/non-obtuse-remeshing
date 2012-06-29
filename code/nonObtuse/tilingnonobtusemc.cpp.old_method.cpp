/***************************************************************************
 *   Copyright (C) 2006 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/
#include "tilingnonobtusemc.h"

bool operator==(IntersectingTri a, IntersectingTri b)
{
	return (a.intTriIdx == b.intTriIdx);
}

// modified from parent class to make 1 as outside, 0 as inside
template <class T> const int CIsoSurface<T>::m_triTable[256][16] = {
	//
	// list of edges the triangle touches
	// -1 denotes end of list
	// special case if first index is -3, -5, -7, -9, -11, -12, -14
	//
	// -3:	Special Case -3
	// {-3, a, b, c, d, e, f, -1, ... }
	// where a,b,c,d forms a quad, and d,e,f,a forms the other quad
	// The right hand ordering determines the normals
	// A new point m should be added to the midpoint of a,d
	// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
	//
	// -5:  Special Case 5b
	// {-5, a, b, c, d, e, -1, ... }
	// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
	// The right hand ordering determines the normals
	// A new point m should be added to the midpoint of b,e
	// Then, the 5 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, -1}
	//
	// -7:  Special Case -7
	// {-7, a, b, c, d, e, f, i, j, k, -1, ... }
	// where a,b,c,d,e,f forms a hexagon and i,j,k forms a triangle
	// The right hand ordering determines the normals
	// A new point m should be added to the centroid of a,b,c,d,e,f
	// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, i, j, k, -1}
	//
	// -9:  Special Case 9b
	// {-9, a, b, c, d, e, f, -1, ... }
	// where a,b,c,d,e,f forms a hexagon.
	// The right hand ordering determines the normals
	// A new point m should be added to the centroid of a,b,c,d,e,f
	// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
	//
	// -11: Special Case 11c
	// {-11, a, b, c, d, e, f, -1, ... ]
	// where a,b,c,d,e,f forms a bent hexagon.
	//       new point m is added in the center of the cube
	//       b,c,m and e,f,m are right isoscele triangles
	//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
	// The right hand ordering determines the normals
	// A new point m should be added to the center of cube (ie. midpoint of a and d)
	// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
	//
	// -12: Special Case 12b
	// {-12, a, b, c, d, e, i, j, k, -1, ... }
	// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
	//       and i, j, k forms a seperate triangle
	// The right hand ordering determines the normals
	// A new point m should be added to the midpoint of b,e
	// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, a, m, i, j, k, -1}
	//
	// -14: Special Case 14c
	// {-11, a, b, c, d, e, f, -1, ... ]
	// where a,b,c,d,e,f forms a bent hexagon.
	//       new point m is added in the center of the cube
	//       b,c,m and e,f,m are right isoscele triangles
	//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
	// The right hand ordering determines the normals
	// A new point m should be added to the center of cube (ie. midpoint of a and d)
	// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
	
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},// 0000 0000; v none		(case 0)
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0001; v 0		(case 1)
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0010; v 1		(case 1)
	{8, 1, 3, 8, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0011; v 0,1		(case 2)
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0100; v 2		(case 1)
	{0, 3, 8, 2, 1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0101; v 0,2		(case 3)
	{2, 9, 10, 2, 0, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0110; v 1,2		(case 2)
	{-5, 9, 10, 2, 3, 8,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 0111; v 0,1,2		(case 5b)
	{11, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1000; v 3		(case 1)
	{0, 2, 11, 11, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1001; v 0,3		(case 2)
	{9, 1, 0, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1010; v 1,3		(case 3)
	{-5, 8, 9, 1, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1011; v 0,1,3		(case 5b)
	{3, 1, 10, 10, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1100; v 2,3		(case 1)
	{-5, 11, 8, 0, 1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1101; v 0,2,3		(case 5b)
	{-5, 10, 11, 3, 0, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1110; v 1,2,3		(case 5b)
	{8, 9, 10, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0000 1111; v 0,1,2,3		(case 8)
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 0000; v 4		(case 1)
	{3, 4, 0, 3, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 0001; v 0,4		(case 2)
	{1, 0, 9, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 0010; v 1,4		(case 3)
	{-5, 3, 7, 4, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001	0011; v 0,1,4		(case 5b)
	{2, 1, 10, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 0100; v 2,4		(case 4)
	{4, 3, 7, 0, 3, 4, 2, 1, 10, -1, -1, -1, -1, -1, -1, -1},	// 0001 0101; v 0,2,4		(case 6)
	{2, 9, 10, 0, 9, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1},	// 0001 0110; v 1,2,4		(case 6)
	{-14, 4, 9, 10, 2, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 0111; v 0,1,2,4 	(case 14c)
	{4, 8, 7, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 1000; v 3,4		(case 3)
	{-5, 0, 2, 11, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 1001; v 0,3,4		(case 5b)
	{0, 9, 1, 4, 8, 7, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1},	// 0001 1010; v 1,3,4		(case 7)
	{-9, 4, 9, 1, 2, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 1011; v 0,1,3,4		(case 9b)
	{10, 3, 1, 11, 3, 10, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1},	// 0001 1100; v 2,3,4		(case 6)
	{-11, 7, 4, 0, 1, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 1101; v 0,2,3,4		(case 11c)
	{-12, 10, 11, 3, 0, 9, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1},	// 0001 1110; v 1,2,3,4		(case 12b)
	{-5, 10, 11, 7, 4, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0001 1111; v -5.-6,-7	(case -5b)
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0000; v 5		(case 1)
	{9, 4, 5, 8, 0, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0001; v 0,5		(case 3)
	{0, 4, 5, 5, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0010; v 1,5		(case 2)
	{-5, 1, 3, 8, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0011; v 0,1,5		(case 5b)
	{2, 1, 10, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0100; v 2,5		(case 3)
	{0, 3, 8, 2, 1, 10, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1},	// 0010 0101; v 0,2,5		(case 7)
	{-5, 0, 4, 5, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0110; v 1,2,5		(case 5b)
	{-9, 2, 3, 8, 4, 5, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 0111; v 0,1,2,5		(case 9b)
	{9, 4, 5, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 1000; v 3,5		(case 4)
	{11, 0, 2, 8, 0, 11, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1},	// 0010 1001; v 0,3,5		(case 6)
	{0, 4, 5, 1, 0, 5, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1},	// 0010 1010; v 1,3,5		(case 6)
	{-11, 4, 5, 1, 2, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 1011; v 0,1,3,5		(case 11c)
	{10, 11, 3, 1, 10, 3, 5, 9, 4, -1, -1, -1, -1, -1, -1, -1},	// 0010 1100; v 2,3,5		(case 6)
	{-12, 11, 8, 0, 1, 10, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1},	// 0010 1101; v 0,2,3,5		(case 12b)
	{-14, 3, 0, 4, 5, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 1110; v 1,2,3,5		(case 14c)
	{-5, 11, 8, 4, 5, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0010 1111; v -4,-6,-7	(case -5b)
	{9, 8, 7, 7, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0000; v 4,5		(case 2)
	{-5, 7, 5, 9, 0, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0001; v 0,4,5		(case 5b)
	{-5, 5, 1, 0, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0010; v 1,4,5		(case 5b)
	{5, 1, 3, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0011; v 0,1,4,5		(case 8)
	{9, 8, 7, 5, 9, 7, 10, 2, 1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0100; v 2,4,5		(case 6)
	{-12, 7, 5, 9, 0, 3, 10, 2, 1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0101; v 0,2,4,5		(case 12b)
	{-11, 10, 2, 0, 8, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0110; v 1,2,4,5		(case 11c)
	{-5, 7, 5, 10, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 0111; v -3,-6.-7	(case -5b)
	{7, 5, 9, 7, 9, 8, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1},	// 0011 1000; v 3,4,5		(case 6)
	{-14, 11, 7, 5, 9, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 1001; v 0,3,4,5		(case 14c)
	{-12, 5, 1, 0, 8, 7, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1},	// 0011 1010; v 1,3,4,5		(case 12b)
	{-5, 5, 1, 2, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 1011; v -2,-6,-7	(case -5b)
	{5, 9, 8, 5, 8, 7, 1, 10, 3, 3, 10, 11, -1, -1, -1, -1},	// 0011 1100; v 2,3,4,5		(case 10)
	//{7, 5, 9, 7, 9, 0, 11, 7, 0, 0, 1, 11, 10, 11, 1, -1},	// 0011 1101; v -1,-6.-7	(case -6)
	{-6, 0, 1, 10, 11, 7, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 1101; v -1,-6.-7	(case -6)
	//{10, 11, 3, 3, 0, 10, 10, 0, 5, 0, 8, 5, 5, 8, 7, -1},	// 0011 1110; v -0,-6.-7	(case -6)
	{-6, 0, 8, 7, 5, 10, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 1110; v -0,-6.-7	(case -6)
	{10, 11, 5, 11, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0011 1111; v -6,-7		(case -2)
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0000; v 6		(case 1)
	{0, 3, 8, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0001; v 0,6		(case 4)
	{9, 1, 0, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0010; v 1,6		(case 3)
	{1, 3, 8, 1, 8, 9, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0100 0011; v 0,1,6		(case 6)
	{6, 1, 5, 6, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0100; v 2,6		(case 2)
	{1, 5, 6, 2, 1, 6, 0, 3, 8, -1, -1, -1, -1, -1, -1, -1},	// 0100 0101; v 0,2,6		(case 6)
	{-5, 2, 0, 9, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0110; v 1,2,6		(case 5b)
	{-11, 3, 8, 9, 5, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 0111; v 0,1,2,6		(case 11c)
	{3, 2, 11, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 1000; v 3,6		(case 3)
	{11, 8, 0, 11, 0, 2, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0100 1001; v 0,3,6		(case 6)
	{1, 0, 9, 3, 2, 11, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0100 1010; v 1,3,6		(case 7)
	{-12, 8, 9, 1, 2, 11, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0100 1011; v 0,1,3,6		(case 12b)
	{-5, 1, 5, 6, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 1100; v 2,3,6		(case 5b)
	{-14, 6, 11, 8, 0, 1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 1101; v 0,2,3,6		(case 14c)
	{-9, 3, 0, 9, 5, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 1110; v 1,2,3,6		(case 9b)
	{-5, 8, 9, 5, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0100 1111; v -4,-5,-7	(case -5b)
	{10, 5, 6, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0101 0000; v 4,6		(case 3)
	{3, 4, 0, 7, 4, 3, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1},	// 0101 0001; v 0,4,6		(case 6)
	{9, 1, 0, 10, 5, 6, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1},	// 0101 0010; v 1,4,6		(case 7)
	{-12, 3, 7, 4, 9, 1, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0101 0011; v 0,1,4,6		(case 12b)
	{6, 2, 1, 5, 6, 1, 7, 4, 8, -1, -1, -1, -1, -1, -1, -1},	// 0101 0100; v 2,4,6		(case 6)
	{2, 1, 5, 2, 5, 6, 0, 3, 4, 4, 3, 7, -1, -1, -1, -1},		// 0101 0101; v 0,2,4,6		(case 10)
	{-12, 2, 0, 9, 5, 6, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1},	// 0101 0110; v 1,2,4,6		(case 12b)
	//{7, 4, 3, 3, 4, 9, 2, 3, 9, 9, 5, 2, 2, 5, 6, -1},		// 0101 0111; v -3,-5,-7	(case -6)
	{-6, 9, 5, 6, 2, 3, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1},		// 0101 0111; v -3,-5,-7	(case -6)
 	{11, 3, 2, 8, 7, 4, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0101 1000; v 3,4,6		(case 7)
	{-12, 0, 2, 11, 7, 4, 10, 5, 6, -1, -1, -1, -1, -1, -1, -1},	// 0101 1001; v 0,3,4,6		(case 12b)
	{1, 0, 9, 7, 4, 8, 3, 2, 11, 10, 5, 6, -1, -1, -1, -1},		// 0101 1010; v 1,3,4,6		(case 13)
	{-7, 9, 1, 2, 11, 7, 4, 10, 5, 6, -1, -1, -1, -1, -1, -1},	// 0101 1011; v -2,-5,-7	(case -7)
	{-12, 1, 5, 6, 11, 3, 4, 8, 7, -1, -1, -1, -1, -1, -1, -1},	// 0101 1100; v 2,3,4,6		(case 12b)
	//{1, 5, 6, 1, 6, 11, 0, 1, 11, 11, 7, 0, 4, 0, 7, -1},	// 0101 1101; v -1,-5,-7	(case -6)
	{-6, 11, 7, 4, 0, 1, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1},	// 0101 1101; v -1,-5,-7	(case -6)
	{-7, 9, 5, 6, 11, 3, 0, 4, 8, 7, -1, -1, -1, -1, -1, -1},	// 0101 1110; v -0,-5,-7	(case -7)
	{-3, 9, 5, 6, 11, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0101 1111; v -5,-7		(case -3)
	{10, 9, 4, 4, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0000; v 5,6		(case 2)
	{10, 4, 6, 9, 4, 10, 8, 0, 3, -1, -1, -1, -1, -1, -1, -1},	// 0110 0001; v 0,5,6		(case 6)
	{-5, 4, 6, 10, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0010; v 1,5,6		(case 5b)
	{-14, 10, 1, 3, 8, 4, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0011; v 0,1,5,6		(case 14c)
	{-5, 6, 2, 1, 9, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0100; v 2,5,6		(case 5b)
	{-12, 6, 2, 1, 9, 4, 0, 3, 8, -1, -1, -1, -1, -1, -1, -1},	// 0110 0101; v 0,2,5,6		(case 12b)
	{0, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0110; v 1,2,5,6		(case 8)
	{-5, 6, 2, 3, 8, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 0111; v -3,-4,-7	(case -5b)
	{4, 10, 9, 10, 4, 6, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1},	// 0110 1000; v 3,5,6		(case 6)
	{0, 2, 8, 8, 2, 11, 9, 4, 10, 10, 4, 6, -1, -1, -1, -1},	// 0110 1001; v 0,3,5,6		(case 10)
	{-12, 4, 6, 10, 1, 0, 11, 3, 2, -1, -1, -1, -1, -1, -1, -1},	// 0110 1010; v 1,3,5,6		(case 12b)
	//{4, 6, 10, 10, 1, 4, 8, 4, 1, 1, 2, 8, 11, 8, 2, -1},		// 0110 1011; v -2,-4,-7	(case -6)
	{-6, 1, 2, 11, 8, 4, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1},		// 0110 1011; v -2,-4,-7	(case -6)
	{-11, 11, 3, 1, 9, 4, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 1100; v 2,3,5,6		(case 11c)
	//{11, 8, 0, 11, 0, 1, 6, 11, 1, 1, 9, 6, 4, 6, 9, -1},		// 0110 1101; v -1,-4,-7	(case -6)
	{-6, 1, 9, 4, 6, 11, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1},		// 0110 1101; v -1,-4,-7	(case -6)
	{-5, 4, 6, 11, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 1110; v -0,-4,-7	(case -5b)
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0110 1111; v -4,-7		(case -2)
	{-5, 9, 8, 7, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0000; v 4,5,6		(case 5b)
	{-11, 6, 10, 9, 0, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0001; v 0,4,5,6		(case 11c)
	{-9, 10, 1, 0, 8, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0010; v 1,4,5,6		(case 9b)
	{-5, 3, 7, 6, 10, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0011; v -2,-3,-7	(case -5b)
	{-14, 7, 6, 2, 1, 9, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0100; v 2,4,5,6		(case 14c)
	//{6, 2, 1, 6, 1, 9, 7, 6, 9, 9, 0, 7, 3, 7, 0, -1},		// 0111 0101; v -1,-3,-7	(case -6)
	{-6, 9, 0, 3, 7, 6, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1},		// 0111 0101; v -1,-3,-7	(case -6)
	{-5, 2, 0, 8, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0110; v -0,-3,-7	(case -5b)
	{7, 2, 3, 7, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 0111; v -3,-7		(case -2)
	{-12, 9, 8, 7, 6, 10, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1},	// 0111 1000; v 3,4,5,6		(case 12b)
	//{0, 2, 11, 0, 11, 7, 9, 0, 7, 7, 6, 9, 10, 9, 6, -1},		// 0111 1001; v -1,-2,-7	(case -6)
	{-6, 7, 6, 10, 9, 0, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1},		// 0111 1001; v -1,-2,-7	(case -6)
	{-7, 0, 8, 7, 6, 10, 1, 3, 2, 11, -1, -1, -1, -1, -1, -1},	// 0111 1010; v -0,-2,-7	(case -7)
	{-3, 1, 2, 11, 7, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 1011; v -2,-7		(case -3)
	//{9, 8, 7, 9, 7, 6, 1, 9, 6, 6, 11, 1, 3, 1, 11, -1},		// 0111 1100; v -0,-1,-7	(case -6)
	{-6, 6, 11, 3, 1, 9, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1},		// 0111 1100; v -0,-1,-7	(case -6)
	{9, 0, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 1101; v -1,-7		(case -4)
	{-3, 0, 8, 7, 6, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 1110; v -0,-7		(case -3)
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 0111 1111; v -7		(case -1)
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 0000; v 7		(case 1)
	{0, 3, 8, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 0001; v 0,7		(case 3)
	{1, 0, 9, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 0010; v 1,7		(case 4)
	{1, 8, 9, 3, 8, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},	// 1000 0011; v 0,1,7		(case 6)
	{10, 2, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 0100; v 2,7		(case 3)
	{2, 1, 10, 0, 3, 8, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},	// 1000 0101; v 0,2,7		(case 7)
	{2, 0, 9, 10, 2, 9, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},	// 1000 0110; v 1,2,7		(case 6)
	{-12, 9, 10, 2, 3, 8, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},	// 1000 0111; v 0,1,2,7		(case 12b)
	{7, 3, 2, 2, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1000; v 3,7		(case 2)
	{-5, 2, 6, 7, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1001; v 0,3,7		(case 5b)
	{2, 6, 7, 3, 2, 7, 1, 0, 9, -1, -1, -1, -1, -1, -1, -1},	// 1000 1010; v 1,3,7		(case 6)
	{-14, 1, 2, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1},		// 1000 1011; v 0,1,3,7		(case 14c)
	{-5, 3, 1, 10, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1100; v 2,3,7		(case 5b)
	{-9, 7, 8, 0, 1, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1101; v 0,2,3,7		(case 9b)
	{-11, 6, 7, 3, 0, 9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1110; v 1,2,3,7		(case 11c)
	{-5, 9, 10, 6, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1000 1111; v -4,-5,-6	(case -5b)
	{8, 6, 4, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 0000; v 4,7		(case 2)
	{-5, 4, 0, 3, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 0001; v 0,4,7		(case 5b)
	{6, 8, 11, 4, 8, 6, 0, 9, 1, -1, -1, -1, -1, -1, -1, -1},	// 1001 0010; v 1,4,7		(case 6)
	{-11, 11, 6, 4, 9, 1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 0011; v 0,1,4,7		(case 11c)
	{8, 6, 4, 11, 6, 8, 10, 2, 1, -1, -1, -1, -1, -1, -1, -1},	// 1001 0100; v 2,4,7		(case 6)
	{-12, 4, 0, 3, 11, 6, 2, 1, 10, -1, -1, -1, -1, -1, -1, -1},	// 1001 0101; v 0,2,4,7		(case 12b)
	{11, 4, 8, 6, 4, 11, 2, 0, 9, 10, 2, 9, -1, -1, -1, -1},	// 1001 0110; v 1,2,4,7		(case 10)
	//{9, 10, 2, 9, 2, 3, 4, 9, 3, 3, 11, 4, 6, 4, 11, -1},		// 1001 0111; v -3,-5,-6	(case -6)
	{-6, 3, 11, 6, 4, 9, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1},		// 1001 0111; v -3,-5,-6	(case -6)
	{-5, 6, 4, 8, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1000; v 3,4,7		(case 5b)
	{0, 2, 4, 6, 4, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1001; v 0,3,4,7		(case 8)
	{-12, 6, 4, 8, 3, 2, 9, 1, 0, -1, -1, -1, -1, -1, -1, -1},	// 1001 1010; v 1,3,4,7		(case 12b)
	{-5, 6, 4, 9, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1011; v -2,-5,-6	(case -5b)
	{-14, 10, 6, 4, 8, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1100; v 2,3,4,7		(case 14c)
	{-5, 4, 0, 1, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1101; v -1,-5,-6	(case -5b)
	//{6, 4, 8, 6, 8, 3, 10, 6, 3, 3, 0, 10, 9, 10, 0, -1},		// 1001 1110; v -0,-5,-6	(case -6)
	{-6, 3, 0, 9, 10, 6, 4, 8, -1, -1, -1, -1, -1, -1, -1, -1},		// 1001 1110; v -0,-5,-6	(case -6)
	{10, 4, 9, 10, 6, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1001 1111; v -5,-6		(case -2)
	{9, 4, 5, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1010 0000; v 5,7		(case 3)
	{8, 0, 3, 9, 4, 5, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1},	// 1010 0001; v 0,5,7		(case 7)
	{0, 5, 1, 4, 5, 0, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},	// 1010 0010; v 1,5,7		(case 6)
	{-12, 1, 3, 8, 4, 5, 4, 8, 3, -1, -1, -1, -1, -1, -1, -1},	// 1010 0011; v 0.1.5.7		(case 12b)
	{9, 4, 5, 10, 2, 1, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},	// 1010 0100; v 2,5,7		(case 7)
	{11, 6, 7, 2, 1, 10, 8, 0, 3, 9, 4, 5, -1, -1, -1, -1},		// 1010 0101; v 0,2,5,7		(case 13)
	{-12, 0, 4, 5, 10, 2, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},	// 1010 0110; v 1,2,5,7		(case 12b)
	{-7, 5, 10, 2, 3, 8, 4, 11, 6, 7, -1, -1, -1, -1, -1, -1},	// 1010 0111; v -3,-4,-6	(case -7)
	{7, 3, 2, 6, 7, 2, 4, 5, 9, -1, -1, -1, -1, -1, -1, -1},	// 1010 1000; v 3,5,7		(case 6)
	{-12, 2, 6, 7, 8, 0, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1},	// 1010 1001; v 0,3,5,7		(case 12b)
	{6, 3, 2, 3, 6, 7, 5, 1, 0, 4, 5, 0, -1, -1, -1, -1},		// 1010 1010; v 1,3,5,7		(case 10)
	//{2, 6, 7, 2, 7, 8, 1, 2, 8, 8, 4, 1, 1, 4, 5, -1},		// 1010 1011; v -2,-4,-6	(case -6)
	{-6, 8, 4, 5, 1, 2, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1},		// 1010 1011; v -2,-4,-6	(case -6)
	{-12, 3, 1, 10, 6, 7, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1},	// 1010 1100; v 2,3,5,7		(case 12b)
	{-7, 1, 10, 6, 7, 8, 0, 9, 4, 5, -1, -1, -1, -1, -1, -1},	// 1010 1101; v -1,-4,-6	(case -7)
	//{0, 4, 5, 0, 5, 10, 3, 0, 10, 10, 6, 3, 3, 6, 7, -1},	// 1010 1110; v -0,-4,-6	(case -6)
	{-6, 10, 6, 7, 3, 0, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1},	// 1010 1110; v -0,-4,-6	(case -6)
	{-3, 10, 6, 7, 8, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1010 1111; v -4,-6		(case -3)
	{-5, 8, 11, 6, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 0000; v 4,5,7		(case 5b)
	{-9, 6, 5, 9, 0, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 0001; v 0,4,5,7		(case 9b)
	{-14, 6, 5, 1, 0, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 0010; v 1,4,5,7		(case 14c)
	{-5, 1, 3, 11, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 0011; v -2,-3,-6	(case -5b)
	{-12, 8, 11, 6, 5, 9, 2, 1, 10, -1, -1, -1, -1, -1, -1, -1},	// 1011 0100; v 2,4,5,7		(case 12b)
	{-7, 9, 0, 3, 11, 6, 5, 10, 2, 1, -1, -1, -1, -1, -1, -1},	// 1011 0101; v -1,-3,-6	(case -7)
	//{8, 11, 6, 8, 6, 5, 0, 8, 5, 5, 10, 0, 2, 0, 10, -1},		// 1011 0110; v -0,-3,-6	(case -6)
	{-6, 5, 10, 2, 0, 8, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1},		// 1011 0110; v -0,-3,-6	(case -6)
	{-3, 3, 11, 6, 5, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 0111; v -3,-6		(case -3)
	{-11, 3, 2, 6, 5, 9, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1000; v 3,4,5,7		(case 11c)
	{-5, 2, 6, 5, 9, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1001; v -1,-2,-6	(case -5b)
	//{5, 1, 0, 5, 0, 8, 6, 5, 8, 8, 3, 6, 6, 3, 2, -1},		// 1011 1010; v -0,-2,-6	(case -6)
	{-6, 8, 3, 2, 6, 5, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1},		// 1011 1010; v -0,-2,-6	(case -6)
	{1, 6, 5, 1, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1011; v -2,-6		(case -2)
	//{3, 1, 10, 3, 10, 6, 8, 3, 6, 6, 5, 8, 9, 8, 5, -1},		// 1011 1100; v -0,-1,-6	(case -6)
	{-6, 6, 5, 9, 8, 3, 1, 10, -1, -1, -1, -1, -1, -1, -1, -1},		// 1011 1100; v -0,-1,-6	(case -6)
	{-3, 0, 1, 10, 6, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1101; v -1,-6		(case -3)
	{0, 8, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1110; v -0,-6		(case -4)
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1011 1111; v -6		(case -1)
	{5, 11, 10, 5, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 0000; v 6,7		(case 2)
	{5, 11, 10, 11, 5, 7, 3, 8, 0, -1, -1, -1, -1, -1, -1, -1},	// 1100 0001; v 0,6,7		(case 6)
	{11, 5, 7, 10, 5, 11, 9, 1, 0, -1, -1, -1, -1, -1, -1, -1},	// 1100 0010; v 1,6,7		(case 6)
	{10, 5, 7, 11, 10, 7, 8, 9, 1, 3, 8, 1, -1, -1, -1, -1},	// 1100 0011; v 0,1,6,7		(case 10)
	{-5, 5, 7, 11, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 0100; v 2,6,7		(case 5b)
	{-12, 5, 7, 11, 2, 1, 8, 0, 3, -1, -1, -1, -1, -1, -1, -1},	// 1100 0101; v 0,2,6,7		(case 12b)
	{-14, 11, 2, 0, 9, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 0110; v 1,2,6,7		(case 14c)
	//{5, 7, 11, 11, 2, 5, 9, 5, 2, 2, 3, 9, 8, 9, 3, -1},		// 1100 0111; v -3,-4,-5	(case -6)
	{-6, 2, 3, 8, 9, 5, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1},		// 1100 0111; v -3,-4,-5	(case -6)
	{-5, 7, 3, 2, 10, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1000; v 3,6,7		(case 5b)
	{-11, 10, 5, 7, 8, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1001; v 0,3,6,7		(case 11c)
	{-12, 7, 3, 2, 10, 5, 0, 9, 1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1010; v 1,3,6,7		(case 12b)
	//{8, 9, 1, 8, 1, 2, 7, 8, 2, 2, 10, 7, 5, 7, 10, -1},		// 1100 1011; v -2,-4,-5	(case -6)
	{-6, 2, 10, 5, 7, 8, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1},		// 1100 1011; v -2,-4,-5	(case -6)
	{3, 1, 5, 7, 3, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1100; v 2,3,6,7		(case 8)
	{-5, 5, 7, 8, 0, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1101; v -1,-4,-5	(case -5b)
	{-5, 7, 3, 0, 9, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1110; v -0,-4,-5	(case -5b)
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1100 1111; v -4,-5		(case -2)
	{-5, 11, 10, 5, 4, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0000; v 4,6,7		(case 5b)
	{-14, 3, 11, 10, 5, 4, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0001; v 0,4,6,7		(case 14c)
	{-12, 11, 10, 5, 4, 8, 1, 0, 9, -1, -1, -1, -1, -1, -1, -1},	// 1101 0010; v 1,4,6,7		(case 12b)
	//{11, 10, 5, 11, 5, 4, 11, 4, 3, 9, 3, 4, 1, 3, 9, -1},	// 1101 0011; v -2,-3,-5	(case -6)
	{-6, 4, 9, 1, 3, 11, 10, 5, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0011; v -2,-3,-5	(case -6)
	{-11, 4, 8, 11, 2, 1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0100; v 2,4,6,7		(case 11c)
	//{0, 3, 4, 4, 3, 11, 5, 4, 11, 11, 2, 5, 5, 2, 1, -1},	// 1101 0101; v -1,-3,-5	(case -6)
	{-6, 11, 2, 1, 5, 4, 0, 3, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0101; v -1,-3,-5	(case -6)
	//{2, 0, 9, 2, 9, 5, 11, 2, 5, 5, 4, 11, 8, 11, 4, -1},		// 1101 0110; v -0,-3,-5	(case -6)
	{-6, 5, 4, 8, 11, 2, 0, 9, -1, -1, -1, -1, -1, -1, -1, -1},		// 1101 0110; v -0,-3,-5	(case -6)
	{4, 9, 5, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 0111; v -3,-5		(case -4)
	{-9, 5, 4, 8, 3, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1000; v 3,4,6,7		(case 9b)
	{-5, 0, 2, 10, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1001; v -1,-2,-5	(case -5b)
	{-7, 5, 4, 8, 3, 2, 10, 9, 1, 0, -1, -1, -1, -1, -1, -1},	// 1101 1010; v -0,-2,-5	(case -7)
	{-3, 2, 10, 5, 4, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1011; v -2,-5		(case -3)
	{-5, 1, 5, 4, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1100; v -0,-1,-5	(case -5b)
	{4, 0, 5, 0, 1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1101; v -1,-5		(case -2)
	{-3, 5, 4, 8, 3, 0, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1110; v -0,-5		(case -3)
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1101 1111; v -5		(case -1)
	{-5, 10, 9, 4, 7, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 0000; v 5,6,7		(case 5b)
	{-12, 10, 9, 4, 7, 11, 8, 0, 3, -1, -1, -1, -1, -1, -1, -1},	// 1110 0001; v 0,5,6,7		(case 12b)
	{-11, 7, 11, 10, 1, 0, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 0010; v 1,5,6,7		(case 11c)
	//{1, 3, 8, 1, 8, 4, 10, 1, 4, 4, 7, 10, 11, 10, 7, -1},		// 1110 0011; v -2,-3,-4	(case -6)
	{-6, 4, 7, 11, 10, 1, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1},		// 1110 0011; v -2,-3,-4	(case -6)
	{-9, 11, 2, 1, 9, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 0100; v 2,5,6,7		(case 9b)
	{-7, 9, 4, 7, 11, 2, 1, 8, 0, 3, -1, -1, -1, -1, -1, -1},	// 1110 0101; v -1,-3,-4	(case -7)
	{-5, 0, 4, 7, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 0110; v -0,-3,-4	(case -5b)
	{-3, 4, 7, 11, 2, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 0111; v -3,-4		(case -3)
	{-14, 4, 7, 3, 2, 10, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1000; v 3,5,6,7		(case 14c)
	//{10, 9, 4, 10, 4, 7, 2, 10, 7, 7, 8, 2, 0, 2, 8, -1},		// 1110 1001; v -1,-2,-4	(case -6)
	{-6, 7, 8, 0, 2, 10, 9, 4, -1, -1, -1, -1, -1, -1, -1, -1},		// 1110 1001; v -1,-2,-4	(case -6)
	//{7, 3, 2, 7, 2, 10, 4, 7, 10, 10, 1, 4, 0, 4, 1, -1},		// 1110 1010; v -0,-2,-4	(case -6)
	{-6, 10, 1, 0, 4, 7, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1},		// 1110 1010; v -0,-2,-4	(case -6)
	{10, 1, 2, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1011; v -2,-4		(case -4)
	{-5, 3, 1, 9, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1100; v -0,-1,-4	(case -5b)
	{-3, 1, 9, 4, 7, 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1101; v -1,-4		(case -3)
	{4, 3, 0, 4, 7, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1110; v -0,-4		(case -2)
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1110 1111; v -4		(case -1)
	{10, 9, 8, 11, 10, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0000; v 4,5,6,7		(case 8)
	{-5, 10, 9, 0, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0001; v -1,-2,-3	(case -5b)
	{-5, 11, 10, 1, 0, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0010; v -0,-2,-3	(case -5b)
	{1, 3, 10, 3, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0011; v -2,-3		(case -2)
	{-5, 8, 11, 2, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0100; v -0,-1,-3	(case -5b)
	{-3, 9, 0, 3, 11, 2, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0101; v -1,-3		(case -3)
	{2, 0, 11, 0, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0110; v -0,-3		(case -2)
	{2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 0111; v -3		(case -1)
	{-5, 9, 8, 3, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1000; v -0,-1,-2	(case -5b)
	{10, 9, 2, 9, 0, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1001; v -1,-2		(case -2)
	{-3, 3, 2, 10, 1, 0, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1010; v -0,-2		(case -3)
	{10, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1011; v -2		(case -1)
	{3, 1, 8, 1, 9, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1100; v -0,-1		(case -2)
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1101; v -1		(case -1)
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	// 1111 1110; v -0		(case -1)
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}// 1111 1111; all		(case 0)
};


template <class T> TilingNonobtuseMC<T>::TilingNonobtuseMC()
:CNonobtuseMC<T>::CNonobtuseMC(), cubesProperty(NULL), existingVerticesOnEdge(NULL), nextUnusedVertexID(0), assoTriList(NULL),
numCubeMultiPatch(0), numPatchIgnored(0), numFaceStitching(0), numNonManifoldFix(0) {
}


template <class T> TilingNonobtuseMC<T>::~TilingNonobtuseMC()
{
	this->DeleteSurface();
}

// clean up all members
template <class T> void TilingNonobtuseMC<T>::DeleteSurface()
{
	CNonobtuseMC<T>::DeleteSurface();
	
	if (cubesProperty != NULL)
		delete[] cubesProperty;
	cubesProperty = NULL;

	if (existingVerticesOnEdge != NULL)
		delete[] existingVerticesOnEdge;
	existingVerticesOnEdge = NULL;

	vertexInfo.clear();
	nextUnusedVertexID = 0;

	if (assoTriList != NULL)
		delete[] assoTriList;
	assoTriList = NULL;

	numCubeMultiPatch = 0;
	numPatchIgnored = 0;
	numFaceStitching = 0;
	numNonManifoldFix = 0;
}

// Generates the isosurface from the source mesh
// Modified from the base class to give nonobtuse marching cube without using a scalarfield
// surfaceType = MIDPOINT
//
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
// tIsoLevel (in): isolevel
// nCellsX (in): number of cells in x-dim
// nCellsY (in): number of cells in y-dim
// nCellsZ (in): number of cells in z-dim
// fCellLengthX (in): cell length in x-dim
// fCellLengthY (in): cell length in y-dim
// fCellLengthZ (in): cell length in z-dim
// return value (out): time taken
template <class T> time_t TilingNonobtuseMC<T>::GenerateSurface(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ)
{
	// sanity checks
	if (verticesList == NULL ||
		numVertices == 0 ||
		polygonsList == NULL ||
		numPolygons == 0)
		return 0;

	// delete old data
	if (this->m_bValidSurface)
		this->DeleteSurface();

	// update members
	this->m_tIsoLevel = tIsoLevel;
	this->m_nCellsX = nCellsX;
	this->m_nCellsY = nCellsY;
	this->m_nCellsZ = nCellsZ;
	this->m_fCellLengthX = fCellLengthX;
	this->m_fCellLengthY = fCellLengthY;
	this->m_fCellLengthZ = fCellLengthZ;

	time_t b4Time = time(NULL);
	time_t afterTime = time(NULL);

	// computing mesh centroid
	double meshCentroid[3] = {0,0,0};
	for (int i = 0; i < numVertices; ++i)
		for (int j = 0; j < 3; ++j)
			meshCentroid[j] += verticesList[3*i+j];
	
	for (unsigned int i = 0; i < 3 && numVertices > 0; i++)
		meshCentroid[i] /= numVertices;

	float mcBoxLength[3] = {(nCellsX*fCellLengthX),(nCellsY*fCellLengthY),(nCellsZ*fCellLengthZ)};
	this->m_offsets[0] = meshCentroid[0] - (mcBoxLength[0] / 2.0);
	this->m_offsets[1] = meshCentroid[1] - (mcBoxLength[1] / 2.0);
	this->m_offsets[2] = meshCentroid[2] - (mcBoxLength[2] / 2.0);

	// ------- STAGE 1: determine which cubes are intersected by which triangle -------
	markIntersectedCubes(verticesList, numVertices, polygonsList, numPolygons);

	// ------- STAGE 2: for each intersected cube, determine the signs and triangulate -------

	// initialize existingVerticesOnEdge
	// 10 edges per cell: 0,3,8,12,13,14,15,16,17,18,19
	existingVerticesOnEdge = new vector<unsigned int>[11*(nCellsX+1)*(nCellsY+1)*(nCellsZ+1)];

	cout << "Start triangulating ... " << endl;

	// triangulate each cube
	for (unsigned int z = 0; z < nCellsZ; ++z) {
		for (unsigned int y = 0; y < nCellsY; ++y) {
			for (unsigned int x = 0; x < nCellsX; ++x) {
				//cout << "Triangulating cube(" << x << ", " << y << ", " << z << ")" << endl;
				triangulate(x,y,z, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
				checkStitching(x,y,z, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
			}
		}
	}

	// rename the vertex ids
	RenameVerticesAndTriangles();
	this->m_bValidSurface = true;

	afterTime = time(NULL);
	time_t totalTime = afterTime - b4Time;
	cout << "Total processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of cubes with multiple patches: " << numCubeMultiPatch << endl;
	cout << "Total number of patches ignored: " << numPatchIgnored << endl;
	cout << "Total number of faces needed for stitching: " << numFaceStitching << endl;
	cout << "Total number of non-manifold fix: " << numNonManifoldFix << endl;

	return totalTime;
}

// triangulate the cube(x,y,z) based on the intersecting triangles and the sorted groups
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::triangulate(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList)
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	if (cubesProperty[cubeIdx].isTriangulated)
		return;

	sortIntersectingTriangle(cubeIdx, polygonNeighbourList);	// make sure we sort the trianlges into patches if not already sorted

	vector<list<unsigned int> > &triGroups = cubesProperty[cubeIdx].triGroups;

	// triangulate patch-wise
	unsigned int numGroups = triGroups.size();
	if (numGroups > 1)
		++numCubeMultiPatch;

	for (unsigned int i = 0; i < numGroups; ++i)
	{
		// find all edge intersections
		vector<Intersection> edgeInt[12];
		findEdgeIntersection(x, y, z, i, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList, edgeInt);
		
		// assign signs to corners
		if (!assignSigns(edgeInt, x, y, z, i))
		{
			++numPatchIgnored;
			continue;
		}
		
		// triangulate patch
		//if ((x == 8 && y == 47 && z == 32) || (x == 9 && y == 47 && z == 32))
		triangulatePatch(x, y, z, i, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
	}

	cubesProperty[cubeIdx].isTriangulated = true;
}

// check the triangulated cube(x,y,z) if it needs any stitching, if so, stitch it
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::checkStitching(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList)
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	vector<EdgeOnFace> (&edgesOnFaces)[6] = cubesProperty[cubeIdx].edgesOnFaces;		// stores the edges that are introduced in the current cube; used for stitching

	// setting up quick indexing of adjacent cube
	int adjacentCube[6][3] = {{x,y,z+1}, {x-1,y,z}, {x,y,z-1}, {x+1,y,z}, {x,y+1,z}, {x,y-1,z}};

	// -------- stitching phase --------

	// ---- find neighbour patch for all edges ----
	vector<int> nbrPatch1NoList[6];		// neighbour patch for v1 of edge
	vector<int> nbrPatch2NoList[6];		// neighbour patch for v2 of edge
	for (unsigned int i = 0; i < 6; ++i)
	{
		// skip boundary cases
		if (i == 0 && z == this->m_nCellsZ - 1)
			continue;
		if (i == 1 && x == 0)
			continue;
		if (i == 2 && z == 0)
			continue;
		if (i == 3 && x == this->m_nCellsX - 1)
			continue;
		if (i == 4 && y == this->m_nCellsY - 1)
			continue;
		if (i == 5 && y == 0)
			continue;

		// no need to check for stitching if too few edges on face
		if (edgesOnFaces[i].size() < 2)
			continue;

		// find neighbour patch for all edges in current face
		for (vector<EdgeOnFace>::iterator iter1 = edgesOnFaces[i].begin(); iter1 != edgesOnFaces[i].end(); iter1++)
		{
			// find the associated intersecting triangle for the end points
			int intTriIdx1 = -1;
			if (!vertexInfo[iter1->v1ID].intTriIdx.empty())
				intTriIdx1 = vertexInfo[iter1->v1ID].intTriIdx[0];
			int intTriIdx2 = -1;
			if (!vertexInfo[iter1->v2ID].intTriIdx.empty())
				intTriIdx2 = vertexInfo[iter1->v2ID].intTriIdx[0];

			// find the connecting patch in the adjacent cube
			unsigned int nbrCubeIdx = adjacentCube[i][2]*(this->m_nCellsY)*(this->m_nCellsX) + adjacentCube[i][1]*(this->m_nCellsX) + adjacentCube[i][0];
			sortIntersectingTriangle(nbrCubeIdx, polygonNeighbourList);	// make sure we sort the trianlges into patches if not already sorted
			vector<IntersectingTri> &intersectedTriangle = cubesProperty[nbrCubeIdx].intersectedTriangle;
			
			IntersectingTri assoTri;
			assoTri.intTriIdx = intTriIdx1;
			vector<IntersectingTri>::iterator nbrAssoTri = find(intersectedTriangle.begin(), intersectedTriangle.end(), assoTri);
			if (intTriIdx1 < 0 || nbrAssoTri == intersectedTriangle.end())	// cannot find neighbour associated triangle
			{
				nbrPatch1NoList[i].push_back(-1);
			}
			
			unsigned int nbrPatchNo = nbrAssoTri->groupIdx;
			nbrPatch1NoList[i].push_back(nbrPatchNo);

			// for neighbour patch for v2
			assoTri.intTriIdx = intTriIdx2;
			nbrAssoTri = find(intersectedTriangle.begin(), intersectedTriangle.end(), assoTri);
			if (intTriIdx2 < 0 || nbrAssoTri == intersectedTriangle.end())	// endpoints of edge correspond to different neighbouring patch
			{
				//cout << "WARNING: TilingNonobtuseMC::triangulate() - endpoints of edge correspond to different neighbouring patches" << endl;
				nbrPatch2NoList[i].push_back(-1);
				continue;
			}
// 			else if (nbrAssoTri->groupIdx != nbrPatchNo)
// 			{
// 				cout << "WARNING: TilingNonobtuseMC::triangulate() - endpoints of edge correspond to different neighbouring patches" << endl;
// 			}
			nbrPatchNo = nbrAssoTri->groupIdx;
			nbrPatch2NoList[i].push_back(nbrPatchNo);
		}
	}

	vector<EdgeOnFace> openEdgesList;				// store the list of open edges after all stitching has been processed
	vector<unsigned int> faceOpenEdgesLieOnList;	// store the face the open edges are on in 'openEdgesList'
	bool hasStitch(false);							// indicate if face has been stitched
	for (unsigned int i = 0; i < 6; ++i)
	{
		hasStitch = false;

		vector<EdgeOnFace> eofs = edgesOnFaces[i];
		if (eofs.size() < 2)
		{
			for (unsigned int j = 0; j < eofs.size(); ++j) {
				openEdgesList.push_back(eofs[j]);
				faceOpenEdgesLieOnList.push_back(i);
			}
			continue;
		}	

		if (nbrPatch1NoList[i].empty() || nbrPatch2NoList[i].empty()) {
			continue;
		}

		unsigned int nbrCubeIdx = adjacentCube[i][2]*(this->m_nCellsY)*(this->m_nCellsX) + adjacentCube[i][1]*(this->m_nCellsX) + adjacentCube[i][0];
		
		// triangulate neighbour cube if not already done so; used for checking open edges and setting edgesOnFaces correspondingly
		triangulate(adjacentCube[i][0], adjacentCube[i][1], adjacentCube[i][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

		// find corresponding pair to stitch
		for (unsigned int j = 0; j < eofs.size(); ++j)
		{
			int nbrPatch1No = nbrPatch1NoList[i][j];
			int nbrPatch2No = nbrPatch2NoList[i][j];
			// check if edge j needs to be considered stitching
			if (nbrPatch1No == -1 || nbrPatch2No == -1 || !canStitch(eofs[j], i, nbrCubeIdx))
			{
				// save all open edges for further processing
				if (nbrPatch1No == -1 || nbrPatch2No == -1) {
					openEdgesList.push_back(eofs[j]);
					faceOpenEdgesLieOnList.push_back(i);
				}

				nbrPatch1NoList[i].erase(nbrPatch1NoList[i].begin() + j);
				nbrPatch2NoList[i].erase(nbrPatch2NoList[i].begin() + j);
				eofs.erase(eofs.begin() + j);
				--j;
				continue;
			}

			for (unsigned int k = j+1; k < eofs.size(); ++k)
			{
				// check if edge k needs to be considered stitching
				if (nbrPatch1NoList[i][k] == -1 || nbrPatch2NoList[i][k] == -1 || !canStitch(eofs[k], i, nbrCubeIdx))
				{
					// save all open edges for further processing
					if (nbrPatch1NoList[i][k] == -1 || nbrPatch2NoList[i][k] == -1) {
						openEdgesList.push_back(eofs[k]);
						faceOpenEdgesLieOnList.push_back(i);
					}

					nbrPatch1NoList[i].erase(nbrPatch1NoList[i].begin() + k);
					nbrPatch2NoList[i].erase(nbrPatch2NoList[i].begin() + k);
					eofs.erase(eofs.begin() + k);
					--k;
					continue;
				}

				// check if the 2 edges share one common neighbour patch
 				if ((nbrPatch1No == nbrPatch1NoList[i][k] || nbrPatch2No == nbrPatch2NoList[i][k]) ||
 					(nbrPatch1No == nbrPatch2NoList[i][k] || nbrPatch2No == nbrPatch1NoList[i][k]))
				{
					// stitch the two edges
					stitch(eofs[j], eofs[k], i, x, y, z, nbrPatch1No, nbrPatch2No, nbrPatch1NoList[i][k], nbrPatch2NoList[i][k], eofs, nbrPatch1NoList, nbrPatch2NoList);
					hasStitch = true;

					nbrPatch1NoList[i].erase(nbrPatch1NoList[i].begin() + k);
					nbrPatch2NoList[i].erase(nbrPatch2NoList[i].begin() + k);
					eofs.erase(eofs.begin() + k);
					nbrPatch1NoList[i].erase(nbrPatch1NoList[i].begin() + j);
					nbrPatch2NoList[i].erase(nbrPatch2NoList[i].begin() + j);
					eofs.erase(eofs.begin() + j);
					
					--j;
					break;
				}
			}
		}

		// the rest of the open edges did not get stitch
		for (unsigned int j = 0; j < eofs.size(); ++j) {
			openEdgesList.push_back(eofs[j]);
			faceOpenEdgesLieOnList.push_back(i);
		}

		if (hasStitch)
			++numFaceStitching;
	}

	// now check for non-manifold vertices
	checkNonManifoldVertices(x, y, z, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList, openEdgesList, faceOpenEdgesLieOnList);
}

// check the triangulated cube(x,y,z) if it generates any non-manifold vertices; if so, fix it
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
// openEdgesList (in/out): store the list of open edges after all stitching has been processed
// faceOpenEdgesLieOnList (in/out): store the face the open edges are on in 'openEdgesList'
template <class T> void TilingNonobtuseMC<T>::checkNonManifoldVertices(unsigned int x, unsigned int y, unsigned int z, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, vector<EdgeOnFace> &openEdgesList, vector<unsigned int> &faceOpenEdgesLieOnList)
{
	// for quick indexing
	int adjacentCube[6][3] = {{x,y,z+1}, {x-1,y,z}, {x,y,z-1}, {x+1,y,z}, {x,y+1,z}, {x,y-1,z}};
	int diagonalCube[12][3] = {{x-1,y,z-1}, {x,y+1,z-1}, {x+1,y,z-1}, {x,y-1,z-1}, {x-1,y,z+1}, {x,y+1,z+1}, {x+1,y,z+1}, {x,y-1,z+1}, {x-1,y-1,z}, {x-1,y+1,z}, {x+1,y+1,z}, {x+1,y-1,z}};
	int diagonalEdge[12] = {6,7,4,5,2,3,0,1,10,11,8,9};
	unsigned int nbrCubeEdge[6][12] = {{0,0,0,0,0,1,2,3,0,0,0,0}, 
										{2,0,0,0,6,0,0,0,11,10,0,0},
										{4,5,6,7,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,4,0,0,0,9,8},
										{0,3,0,0,0,7,0,0,0,8,11,0},
										{0,0,0,1,0,0,0,5,9,0,0,10}};

	unsigned int cubeIdx = z*(this->m_nCellsY)*(this->m_nCellsX) + y*(this->m_nCellsX) + x;

	// ------------- check non-manifold vertices ------------------
	// this may occur when the sampling grid is coarse near open patches
	// this occur when 2 triangles share a vertex where the 2 triangles lie in diagonal neighbour cubes

	// used to transform face indices of the current cube into face indices in the neighbour cube
	unsigned int transformedToNbrFaceIdx[6] = {2, 3, 0, 1, 5, 4};
	for (unsigned int i = 0; i < openEdgesList.size(); ++i)
	{
		// make sure the edge is open
		unsigned int nbr1CubeIdx = adjacentCube[faceOpenEdgesLieOnList[i]][2]*(this->m_nCellsY)*(this->m_nCellsX) + adjacentCube[faceOpenEdgesLieOnList[i]][1]*(this->m_nCellsX) + adjacentCube[faceOpenEdgesLieOnList[i]][0];
		// triangulate adjacent cube if not already done so
		triangulate(adjacentCube[faceOpenEdgesLieOnList[i]][0], adjacentCube[faceOpenEdgesLieOnList[i]][1], adjacentCube[faceOpenEdgesLieOnList[i]][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
		if (!canStitch(openEdgesList[i], faceOpenEdgesLieOnList[i], nbr1CubeIdx)) {
			openEdgesList.erase(openEdgesList.begin() + i);
			faceOpenEdgesLieOnList.erase(faceOpenEdgesLieOnList.begin() + i);
			--i;
			continue;
		}

		for (unsigned int j = i+1; j < openEdgesList.size(); ++j)
		{
			// make sure the edge is open
			unsigned int nbr2CubeIdx = adjacentCube[faceOpenEdgesLieOnList[j]][2]*(this->m_nCellsY)*(this->m_nCellsX) + adjacentCube[faceOpenEdgesLieOnList[j]][1]*(this->m_nCellsX) + adjacentCube[faceOpenEdgesLieOnList[j]][0];
			// triangulate adjacent cube if not already done so
			triangulate(adjacentCube[faceOpenEdgesLieOnList[j]][0], adjacentCube[faceOpenEdgesLieOnList[j]][1], adjacentCube[faceOpenEdgesLieOnList[j]][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
			if (!canStitch(openEdgesList[j], faceOpenEdgesLieOnList[j], nbr2CubeIdx)) {
				openEdgesList.erase(openEdgesList.begin() + j);
				faceOpenEdgesLieOnList.erase(faceOpenEdgesLieOnList.begin() + j);
				--j;
				continue;
			}

			// the two pair of edges must be on different faces (sanity check)
			if (faceOpenEdgesLieOnList[i] == faceOpenEdgesLieOnList[j])
				continue;

			// check if there's a pair of edges that share some common vertex
			unsigned int checkEdge(0);				// edge index where non-manifold vertex lies on
			unsigned int nonManifoldVertex(0);		// vertex ID of non-manifold vertex
			unsigned int nbrEdge1(0);				// edge index where neighbour 1, in cube(x,y,z), of nonManifoldVertex lies on
			unsigned int nbrEdge2(0);				// edge index where neighbour 2, in cube(x,y,z), of nonManifoldVertex lies on
			unsigned int nbrID1(0);					// vertex ID of neighbour 1, in cube(x,y,z), of nonManifoldVertex
			unsigned int nbrID2(0);					// vertex ID of neighbour 2, in cube(x,y,z), of nonManifoldVertex
			if (openEdgesList[j].v1ID == openEdgesList[i].v1ID) {
				checkEdge = openEdgesList[j].v1Edge;
				nonManifoldVertex = openEdgesList[j].v1ID;
				nbrEdge1 = openEdgesList[i].v2Edge;
				nbrEdge2 = openEdgesList[j].v2Edge;
				nbrID1 = openEdgesList[i].v2ID;
				nbrID2 = openEdgesList[j].v2ID;
			}
			else if (openEdgesList[j].v1ID == openEdgesList[i].v2ID) {
				checkEdge = openEdgesList[j].v1Edge;
				nonManifoldVertex = openEdgesList[j].v1ID;
				nbrEdge1 = openEdgesList[i].v1Edge;
				nbrEdge2 = openEdgesList[j].v2Edge;
				nbrID1 = openEdgesList[i].v1ID;
				nbrID2 = openEdgesList[j].v2ID;
			}
			else if (openEdgesList[j].v2ID == openEdgesList[i].v1ID) {
				checkEdge = openEdgesList[j].v2Edge;
				nonManifoldVertex = openEdgesList[j].v2ID;
				nbrEdge1 = openEdgesList[i].v2Edge;
				nbrEdge2 = openEdgesList[j].v1Edge;
				nbrID1 = openEdgesList[i].v2ID;
				nbrID2 = openEdgesList[j].v1ID;
			}
			else if (openEdgesList[j].v2ID == openEdgesList[i].v2ID) {
				checkEdge = openEdgesList[j].v2Edge;
				nonManifoldVertex = openEdgesList[j].v2ID;
				nbrEdge1 = openEdgesList[i].v1Edge;
				nbrEdge2 = openEdgesList[j].v1Edge;
				nbrID1 = openEdgesList[i].v1ID;
				nbrID2 = openEdgesList[j].v1ID;
			}
			else
				continue;

			// stores the two (open) edge indices that needs to be covered up in order to resolve non-manifold vertices
			unsigned int toFixEdge1(0);			// edge index of nbr, in cube(x,y,z) of non-manifold vertex where the fix occur
			unsigned int toFixEdge2(0);			// edge index of nbr, in diagonal cube, of non-manifold vertex where the fix occur
			// vertex ids for the two vertices that will be used to cover up
			unsigned int toTriangulateV1(0);	// in cube(x,y,z)
			unsigned int toTriangulateV2(0);	// in diagonal cube
			// nbr cube index where the new triangle is placed
			unsigned int toTriangulateNbrCubeX(0);	// cube x-coord of where the new triangle lies within
			unsigned int toTriangulateNbrCubeY(0);	// cube y-coord of where the new triangle lies within
			unsigned int toTriangulateNbrCubeZ(0);	// cube z-coord of where the new triangle lies within
			unsigned int toTriangulateNbrCubeIdx(0);	// cube Idx of where the new triangle lies within
			// face idx of where new edges are placed
			unsigned int toTriangulateFace1Idx(0);		// in toTriangulateNbrCubeIdx
			unsigned int toTriangulateFace2Idx(0);		// in toTriangulateNbrCubeIdx
			unsigned int triOrientation(0);		// 0 - {commonEdge, toFixEdge1, toFixEdge2}, 1 - {commonEdge, toFixEdge2, toFixEdge1}

			// ---- now, check if the diagonal neighbour cube use such vertex -----
			bool needStitch(false);
			// triangulate diagonal cube if not already done so
			triangulate(diagonalCube[checkEdge][0], diagonalCube[checkEdge][1], diagonalCube[checkEdge][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
			
			unsigned int diaCubeIdx = diagonalCube[checkEdge][2]*(this->m_nCellsY)*(this->m_nCellsX) + diagonalCube[checkEdge][1]*(this->m_nCellsX) + diagonalCube[checkEdge][0];
			vector<EdgeOnFace> (&eoDNbrFaces)[6] = cubesProperty[diaCubeIdx].edgesOnFaces;

			unsigned int facesToCheck[2] = {transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]],	transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]]};
			for (unsigned int k = 0; k < 2; ++k)
			{
				for (unsigned int m = 0; m < eoDNbrFaces[facesToCheck[k]].size(); ++m)
				{
					// check if edge is open
					if (k == 0) {
						triangulate(adjacentCube[faceOpenEdgesLieOnList[i]][0], adjacentCube[faceOpenEdgesLieOnList[i]][1], adjacentCube[faceOpenEdgesLieOnList[i]][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
						if (!canStitch(eoDNbrFaces[facesToCheck[k]][m], facesToCheck[k], nbr1CubeIdx))
							continue;
					}	
					else {
						triangulate(adjacentCube[faceOpenEdgesLieOnList[j]][0], adjacentCube[faceOpenEdgesLieOnList[j]][1], adjacentCube[faceOpenEdgesLieOnList[j]][2], verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
						if (!canStitch(eoDNbrFaces[facesToCheck[k]][m], facesToCheck[k], nbr2CubeIdx))
							continue;
					}

					// found non-manifold vertex
					if (eoDNbrFaces[facesToCheck[k]][m].v1ID == nonManifoldVertex) {
						toFixEdge1 = nbrEdge1;
						toTriangulateV1 = nbrID1;
						toTriangulateNbrCubeIdx = nbr1CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[i]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[i]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[i]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]];
						toTriangulateFace2Idx = faceOpenEdgesLieOnList[j];
						if (k == 1) {
							toFixEdge1 = nbrEdge2;
							toTriangulateV1 = nbrID2;
							toTriangulateNbrCubeIdx = nbr2CubeIdx;
							toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[j]][0];
							toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[j]][1];
							toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[j]][2];
							toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]];
							toTriangulateFace2Idx = faceOpenEdgesLieOnList[i];
						}
						toFixEdge2 = eoDNbrFaces[facesToCheck[k]][m].v2Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck[k]][m].v2ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck[k]][m].triID].pointID[p] == eoDNbrFaces[facesToCheck[k]][m].v1ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck[k]][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck[k]][m].v2ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
					else if (eoDNbrFaces[facesToCheck[k]][m].v2ID == nonManifoldVertex) {
						toFixEdge1 = nbrEdge1;
						toTriangulateV1 = nbrID1;
						toTriangulateNbrCubeIdx = nbr1CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[i]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[i]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[i]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]];
						toTriangulateFace2Idx = faceOpenEdgesLieOnList[j];
						if (k == 1) {
							toFixEdge1 = nbrEdge2;
							toTriangulateV1 = nbrID2;
							toTriangulateNbrCubeIdx = nbr2CubeIdx;
							toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[j]][0];
							toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[j]][1];
							toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[j]][2];
							toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]];
							toTriangulateFace2Idx = faceOpenEdgesLieOnList[i];
						}
						toFixEdge2 = eoDNbrFaces[facesToCheck[k]][m].v1Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck[k]][m].v1ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck[k]][m].triID].pointID[p] == eoDNbrFaces[facesToCheck[k]][m].v2ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck[k]][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck[k]][m].v1ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
				}
				if (needStitch)
					break;
			}

			if (!needStitch)
			{
				// ---- now, check if the adjacent neighbour cube (share with edge i) use such vertex -----
				vector<EdgeOnFace> (&eoDNbrFaces)[6] = cubesProperty[nbr1CubeIdx].edgesOnFaces;
	
				unsigned int facesToCheck = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]];
				for (unsigned int m = 0; m < eoDNbrFaces[facesToCheck].size(); ++m)
				{
					// check if edge is open
					if (!canStitch(eoDNbrFaces[facesToCheck][m], facesToCheck, cubeIdx))
						continue;
					
					// found non-manifold vertex
					if (eoDNbrFaces[facesToCheck][m].v1ID == nonManifoldVertex) {
						// check if we should let stitching fix this
						bool letStitchingHandle(false);
						for (unsigned int s = 0; s < eoDNbrFaces[facesToCheck].size(); ++s) {
							if ((eoDNbrFaces[facesToCheck][s].v1Edge == nbrCubeEdge[faceOpenEdgesLieOnList[i]][nbrEdge1] &&
								eoDNbrFaces[facesToCheck][s].v2Edge == eoDNbrFaces[facesToCheck][m].v2Edge) ||
								(eoDNbrFaces[facesToCheck][s].v2Edge == nbrCubeEdge[faceOpenEdgesLieOnList[i]][nbrEdge1] &&
								eoDNbrFaces[facesToCheck][s].v1Edge == eoDNbrFaces[facesToCheck][m].v2Edge)) {
								letStitchingHandle = true;
								break;
							}
						}
						if (letStitchingHandle)
							continue;

						toFixEdge1 = nbrEdge1;
						toTriangulateV1 = nbrID1;
						toTriangulateNbrCubeIdx = nbr1CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[i]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[i]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[i]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]];
						toTriangulateFace2Idx = toTriangulateFace1Idx;
						toFixEdge2 = eoDNbrFaces[facesToCheck][m].v2Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck][m].v2ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[p] == eoDNbrFaces[facesToCheck][m].v1ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck][m].v2ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
					else if (eoDNbrFaces[facesToCheck][m].v2ID == nonManifoldVertex) {
						// check if we should let stitching fix this
						bool letStitchingHandle(false);
						for (unsigned int s = 0; s < eoDNbrFaces[facesToCheck].size(); ++s) {
							if ((eoDNbrFaces[facesToCheck][s].v1Edge == nbrCubeEdge[faceOpenEdgesLieOnList[i]][nbrEdge1] &&
								eoDNbrFaces[facesToCheck][s].v2Edge == eoDNbrFaces[facesToCheck][m].v1Edge) ||
								(eoDNbrFaces[facesToCheck][s].v2Edge == nbrCubeEdge[faceOpenEdgesLieOnList[i]][nbrEdge1] &&
								eoDNbrFaces[facesToCheck][s].v1Edge == eoDNbrFaces[facesToCheck][m].v1Edge)) {
								letStitchingHandle = true;
								break;
							}
						}
						if (letStitchingHandle)
							continue;

						toFixEdge1 = nbrEdge1;
						toTriangulateV1 = nbrID1;
						toTriangulateNbrCubeIdx = nbr1CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[i]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[i]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[i]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[i]];
						toTriangulateFace2Idx = toTriangulateFace1Idx;
						toFixEdge2 = eoDNbrFaces[facesToCheck][m].v1Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck][m].v1ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[p] == eoDNbrFaces[facesToCheck][m].v2ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck][m].v1ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
				}
			}

			if (!needStitch)
			{
				// ---- now, check if the adjacent neighbour cube (share with edge j) use such vertex -----
				vector<EdgeOnFace> (&eoDNbrFaces)[6] = cubesProperty[nbr2CubeIdx].edgesOnFaces;
	
				unsigned int facesToCheck = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]];
				for (unsigned int m = 0; m < eoDNbrFaces[facesToCheck].size(); ++m)
				{
					// check if edge is open
					if (!canStitch(eoDNbrFaces[facesToCheck][m], facesToCheck, cubeIdx))
						continue;
					
					// found non-manifold vertex
					if (eoDNbrFaces[facesToCheck][m].v1ID == nonManifoldVertex) {
						// check if we should let stitching fix this
						bool letStitchingHandle(false);
						for (unsigned int s = 0; s < eoDNbrFaces[facesToCheck].size(); ++s) {
							if ((eoDNbrFaces[facesToCheck][s].v1Edge == nbrCubeEdge[faceOpenEdgesLieOnList[j]][nbrEdge2] &&
								eoDNbrFaces[facesToCheck][s].v2Edge == eoDNbrFaces[facesToCheck][m].v2Edge) ||
								(eoDNbrFaces[facesToCheck][s].v2Edge == nbrCubeEdge[faceOpenEdgesLieOnList[j]][nbrEdge2] &&
								eoDNbrFaces[facesToCheck][s].v1Edge == eoDNbrFaces[facesToCheck][m].v2Edge)) {
								letStitchingHandle = true;
								break;
							}
						}
						if (letStitchingHandle)
							continue;

						toFixEdge1 = nbrEdge2;
						toTriangulateV1 = nbrID2;
						toTriangulateNbrCubeIdx = nbr2CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[j]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[j]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[j]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]];
						toTriangulateFace2Idx = toTriangulateFace1Idx;
						toFixEdge2 = eoDNbrFaces[facesToCheck][m].v2Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck][m].v2ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[p] == eoDNbrFaces[facesToCheck][m].v1ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck][m].v2ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
					else if (eoDNbrFaces[facesToCheck][m].v2ID == nonManifoldVertex) {
						// check if we should let stitching fix this
						bool letStitchingHandle(false);
						for (unsigned int s = 0; s < eoDNbrFaces[facesToCheck].size(); ++s) {
							if ((eoDNbrFaces[facesToCheck][s].v1Edge == nbrCubeEdge[faceOpenEdgesLieOnList[j]][nbrEdge2] &&
								eoDNbrFaces[facesToCheck][s].v2Edge == eoDNbrFaces[facesToCheck][m].v1Edge) ||
								(eoDNbrFaces[facesToCheck][s].v2Edge == nbrCubeEdge[faceOpenEdgesLieOnList[j]][nbrEdge2] &&
								eoDNbrFaces[facesToCheck][s].v1Edge == eoDNbrFaces[facesToCheck][m].v1Edge)) {
								letStitchingHandle = true;
								break;
							}
						}
						if (letStitchingHandle)
							continue;

						toFixEdge1 = nbrEdge2;
						toTriangulateV1 = nbrID2;
						toTriangulateNbrCubeIdx = nbr2CubeIdx;
						toTriangulateNbrCubeX = adjacentCube[faceOpenEdgesLieOnList[j]][0];
						toTriangulateNbrCubeY = adjacentCube[faceOpenEdgesLieOnList[j]][1];
						toTriangulateNbrCubeZ = adjacentCube[faceOpenEdgesLieOnList[j]][2];
						toTriangulateFace1Idx = transformedToNbrFaceIdx[faceOpenEdgesLieOnList[j]];
						toTriangulateFace2Idx = toTriangulateFace1Idx;
						toFixEdge2 = eoDNbrFaces[facesToCheck][m].v1Edge;
						toTriangulateV2 = eoDNbrFaces[facesToCheck][m].v1ID;

						// check orientation
						for (unsigned int p = 0; p < 3; ++p) {
							if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[p] == eoDNbrFaces[facesToCheck][m].v2ID) {
								if (this->m_trivecTriangles[eoDNbrFaces[facesToCheck][m].triID].pointID[(p+1)%3] != eoDNbrFaces[facesToCheck][m].v1ID) {
									triOrientation = 1;
								}
								break;
							}
						}

						needStitch = true;
						break;
					}
				}
			}

			if (!needStitch)
				continue;

			++numNonManifoldFix;

			// stitch 
			// check if it's need to be stitch with two triangles
			if (isSpecialNonmanifoldCase(checkEdge, toFixEdge1, toFixEdge2))
			{
				unsigned int vertexID = nextUnusedVertexID;
				++nextUnusedVertexID;

				POINT3DID pta = this->computeVertexLocation(toTriangulateNbrCubeX, toTriangulateNbrCubeY, toTriangulateNbrCubeZ, toFixEdge1);
				POINT3DID ptb = this->computeVertexLocation(toTriangulateNbrCubeX, toTriangulateNbrCubeY, toTriangulateNbrCubeZ, diagonalEdge[toFixEdge1]);
				
				// computing midpoint m
				double m[3] = {0,0,0};
				midpoint(m, pta.x, pta.y, pta.z, ptb.x, ptb.y, ptb.z);
				POINT3DID ptm;
				ptm.newID = 0;
				ptm.x = m[0];
				ptm.y = m[1];
				ptm.z = m[2];

				// insert new vertex into list
				this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, ptm));

				// add vertex info
				VertexInfo vInfo;
				vInfo.x = toTriangulateNbrCubeX;
				vInfo.y = toTriangulateNbrCubeY;
				vInfo.z = toTriangulateNbrCubeZ;
				vInfo.patchNo = 0;
				vInfo.edgeID = 19;
				vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
				
				// 1st triangle
				TRIANGLE triangle;
				triangle.pointID[0] = nonManifoldVertex;
				triangle.pointID[1] = toTriangulateV1;
				triangle.pointID[2] = vertexID;
				if (triOrientation == 1)
					swap(triangle.pointID[1], triangle.pointID[2]);

				// add edgesOnFaces reference
				EdgeOnFace eof;
				eof.v1ID = nonManifoldVertex;
				eof.v2ID = toTriangulateV1;
				eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][checkEdge];
				eof.v2Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][toFixEdge1];
				eof.triID = this->m_trivecTriangles.size();
				cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace1Idx].push_back(eof);
	
				// add triangle reference to vertices
				for (unsigned int k = 0; k < 3; ++k)
					vertexInfo[triangle.pointID[k]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// 2nd triangle
				triangle.pointID[0] = nonManifoldVertex;
				triangle.pointID[1] = vertexID;
				triangle.pointID[2] = toTriangulateV2;
				if (triOrientation == 1)
					swap(triangle.pointID[1], triangle.pointID[2]);

				eof.v1ID = nonManifoldVertex;
				eof.v2ID = toTriangulateV2;
				eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace2Idx]][checkEdge];
				eof.v2Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace2Idx]][toFixEdge2];
				eof.triID = this->m_trivecTriangles.size();
				cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace2Idx].push_back(eof);

				// add triangle reference to vertices
				for (unsigned int k = 0; k < 3; ++k)
					vertexInfo[triangle.pointID[k]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
			else
			{
				TRIANGLE triangle;
				triangle.pointID[0] = nonManifoldVertex;
				triangle.pointID[1] = toTriangulateV1;
				triangle.pointID[2] = toTriangulateV2;
				if (triOrientation == 1)
					swap(triangle.pointID[1], triangle.pointID[2]);

				// add edgesOnFaces reference
				EdgeOnFace eof;
				if (toTriangulateFace1Idx == toTriangulateFace2Idx) {
					// for nbr cube
					eof.v1ID = nonManifoldVertex;
					eof.v2ID = toTriangulateV1;
					eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][checkEdge];
					eof.v2Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][toFixEdge1];
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace1Idx].push_back(eof);
					
					eof.v1ID = toTriangulateV1;
					eof.v2ID = toTriangulateV2;
					eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][toFixEdge1];
					eof.v2Edge = toFixEdge2;
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace1Idx].push_back(eof);

					// for cube(x,y,z)
					eof.v1ID = nonManifoldVertex;
					eof.v2ID = toTriangulateV2;
					eof.v1Edge = checkEdge;
					eof.v2Edge = nbrCubeEdge[toTriangulateFace1Idx][toFixEdge2];
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[cubeIdx].edgesOnFaces[transformedToNbrFaceIdx[toTriangulateFace1Idx]].push_back(eof);
					
					eof.v1ID = toTriangulateV1;
					eof.v2ID = toTriangulateV2;
					eof.v1Edge = toFixEdge1;
					eof.v2Edge = nbrCubeEdge[toTriangulateFace1Idx][toFixEdge2];
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[cubeIdx].edgesOnFaces[transformedToNbrFaceIdx[toTriangulateFace1Idx]].push_back(eof);
				}
				else {
					eof.v1ID = nonManifoldVertex;
					eof.v2ID = toTriangulateV1;
					eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][checkEdge];
					eof.v2Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace1Idx]][toFixEdge1];
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace1Idx].push_back(eof);
	
					eof.v1ID = nonManifoldVertex;
					eof.v2ID = toTriangulateV2;
					eof.v1Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace2Idx]][checkEdge];
					eof.v2Edge = nbrCubeEdge[transformedToNbrFaceIdx[toTriangulateFace2Idx]][toFixEdge2];
					eof.triID = this->m_trivecTriangles.size();
					cubesProperty[toTriangulateNbrCubeIdx].edgesOnFaces[toTriangulateFace2Idx].push_back(eof);
				}
				

				// add triangle reference to vertices
				for (unsigned int k = 0; k < 3; ++k)
					vertexInfo[triangle.pointID[k]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
	}
}

// triangulate patch on cube(x,y,z) based on the signs given in 'signs'
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// signs (in): array of signs for all 8 corners of the cube; 1=inside, -1=outside
// patchNo (in): patch number we are triangulating
// edgeInt (in): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::triangulatePatch(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, const vector<Intersection> (&edgeInt)[12], const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList)
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	vector<IntersectingTri> &intersectedTriangle = cubesProperty[cubeIdx].intersectedTriangle;
	vector<int> &signs = cubesProperty[cubeIdx].signs[patchNo];
	vector<EdgeOnFace> (&edgesOnFaces)[6] = cubesProperty[cubeIdx].edgesOnFaces;

	// setting up quick indexing of adjacent cube
	int adjacentCube[12][3][3] = {
									{{x,y,z-1}, {x-1,y,z-1}, {x-1,y,z}},			// edge 0
									{{x,y,z-1}, {x,y+1,z-1}, {x,y+1,z}},			// edge 1
									{{x,y,z-1}, {x+1,y,z-1}, {x+1,y,z}},			// edge 2
									{{x,y,z-1}, {x,y-1,z-1}, {x,y-1,z}},			// edge 3
									{{x,y,z+1}, {x-1,y,z+1}, {x-1,y,z}},			// edge 4
									{{x,y,z+1}, {x,y+1,z+1}, {x,y+1,z}},			// edge 5
									{{x,y,z+1}, {x+1,y,z+1}, {x+1,y,z}},			// edge 6
									{{x,y,z+1}, {x,y-1,z+1}, {x,y-1,z}},			// edge 7
									{{x-1,y,z}, {x-1,y-1,z}, {x,y-1,z}},			// edge 8
									{{x-1,y,z}, {x-1,y+1,z}, {x,y+1,z}},			// edge 9
									{{x+1,y,z}, {x+1,y+1,z}, {x,y+1,z}},			// edge 10
									{{x+1,y,z}, {x+1,y-1,z}, {x,y-1,z}}				// edge 11
								};

	int edgeOnFaceLookup[12][12] = {
									{-1, 2, 2, 2, 1, -1, -1, -1, 1, 1, -1, -1},
									{2, -1, 2, 2, -1, 4, -1, -1, -1, 4, 4, -1},
									{2, 2, -1, 2, -1, -1, 3, -1, -1, -1, 3, 3},
									{2, 2, 2, -1, -1, -1, -1, 5, 5, -1, -1, 5},
									{1, -1, -1, -1, -1, 0, 0, 0, 1, 1, -1, -1},
									{-1, 4, -1, -1, 0, -1, 0, 0, -1, 4, 4, -1},
									{-1, -1, 3, -1, 0, 0, -1, 0, -1, -1, 3, 3},
									{-1, -1, -1, 5, 0, 0, 0, -1, 5, -1, -1, 5},
									{1, -1, -1, 5, 1, -1, -1, 5, -1, 1, -1, 5},
									{1, 4, -1, -1, 1, 4, -1, -1, 1, -1, 4, -1},
									{-1, 4, 3, -1, -1, 4, 3, -1, -1, 4, -1, 3},
									{-1, -1, 3, 5, -1, -1, 3, 5, 5, -1, 3, -1},
									};

	// Calculate table lookup index from those
	// vertices which are below the isolevel.
	unsigned int tableIndex = 0;
	//    1----2
	//   /|  / |
	// 5----6  |
	// | /0-|--3
	// |/   |/
	// 4----7
	for (unsigned int i = 0; i < 8; ++i)
	{
		if (signs[i] > this->m_tIsoLevel)
			tableIndex |= (unsigned int)pow(2,i);
	}	
		
	// Now create a triangulation of the isosurface in this
	// cell.
	if (this->m_edgeTable[tableIndex] != 0)
	{
		if  (this->m_triTable[tableIndex][0] == -3)
		{
			// -3:	Special Case -3
			// {-3, a, b, c, d, e, f, -1, ... }
			// where a,b,c,d forms a quad, and d,e,f,a forms the other quad
			// The right hand ordering determines the normals
			// A new point m should be added to the midpoint of a,d
			// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID pta = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][1]);
			POINT3DID ptd = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][4]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 18);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo mInfo;
			mInfo.x = x;
			mInfo.y = y;
			mInfo.z = z;
			mInfo.patchNo = patchNo;
			mInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, mInfo));
			
			// create triangles
			for (unsigned int i = 1; i <= 6; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%6 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;
				
				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				eof.triID = this->m_trivecTriangles.size();
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);

				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -5)
		{
			// -5:  Special Case 5b
			// {-5, a, b, c, d, e, -1, ... }
			// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
			// The right hand ordering determins the normals
			// A new point m should be added to the midpoint of b,e
			// Then, the 5 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID ptb = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][2]);
			POINT3DID pte = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][5]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 12);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 5; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%5 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.triID = this->m_trivecTriangles.size();
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%5 + 1];
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);
				
				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if  (this->m_triTable[tableIndex][0] == -6)
		{
			// -6:	Special Case -6
			// {-3, a, b, c, d, e, f, g, -1, ... }
			// where a,b,c,d,e,f,g forms a seven-agon
			// The right hand ordering determines the normals
			// A new point m should be added to the center of cube
			// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, g, m, g, a, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID pta = this->computeVertexLocation(x, y, z, 0);
			POINT3DID ptd = this->computeVertexLocation(x, y, z, 6);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 19);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo mInfo;
			mInfo.x = x;
			mInfo.y = y;
			mInfo.z = z;
			mInfo.patchNo = patchNo;
			mInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, mInfo));
			
			// create triangles
			for (unsigned int i = 1; i <= 7; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%7 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;
				
				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%7 + 1];
				eof.triID = this->m_trivecTriangles.size();
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);

				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -7)
		{
			// -7:  Special Case -7
			// {-7, a, b, c, d, e, f, i, j, k, -1, ... }
			// where a,b,c,d,e,f forms a hexagon and i,j,k forms a triangle
			// The right hand ordering determines the normals
			// A new point m should be added to the centroid of a,b,c,d,e,f
			// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, i, j, k, -1}
			
			// ---- Add midpoint m first ----
			POINT3DID ptb = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][2]);
			POINT3DID pte = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][5]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 17);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 7; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 3; ++j)
				{
					if (i != 7 && j == 2)	// only the 7th triangle use 3rd vertex that lies on edge, other use midpoint m as the 3rd pt
						break;

					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (i == 7)
						edgeNo = this->m_triTable[tableIndex][i+j];
					else if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%6 + 1];
					
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint, except for the 7th triangle
				if (i != 7)
				{
					triangle.pointID[2] = mID;

					// add EdgeOnFace reference
					EdgeOnFace eof;
					eof.v1ID = triangle.pointID[0];
					eof.v2ID = triangle.pointID[1];
					eof.triID = this->m_trivecTriangles.size();
					eof.v1Edge = this->m_triTable[tableIndex][i];
					eof.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
					int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
					if (faceIdx != -1)
						edgesOnFaces[faceIdx].push_back(eof);
				}	
				else
				{
					// add EdgeOnFace reference
					EdgeOnFace eof;
					eof.triID = this->m_trivecTriangles.size();
					for (unsigned int k = 0; k < 3; ++k)
					{
						eof.v1ID = triangle.pointID[k];
						eof.v2ID = triangle.pointID[(k+1)%3];
						eof.v1Edge = this->m_triTable[tableIndex][i+k];
						eof.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
						int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
						if (faceIdx != -1)
							edgesOnFaces[faceIdx].push_back(eof);
					}
				}

				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());
				
				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -9)
		{
			// -9:  Special Case 9b
			// {-9, a, b, c, d, e, f, -1, ... }
			// where a,b,c,d,e,f forms a hexagon.
			// The right hand ordering determines the normals
			// A new point m should be added to the centroid of a,b,c,d,e,f
			// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID ptb = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][2]);
			POINT3DID pte = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][5]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 13);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 6; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%6 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.triID = this->m_trivecTriangles.size();
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);
				
				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -11)	
		{
			// -11: Special Case 11c
			// {-11, a, b, c, d, e, f, -1, ... ]
			// where a,b,c,d,e,f forms a bent hexagon.
			//       new point m is added in the center of the cube
			//       b,c,m and e,f,m are right isoscele triangles
			//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
			// The right hand ordering determines the normals
			// A new point m should be added to the center of cube (ie. midpoint of a and d)
			// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID pta = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][1]);
			POINT3DID ptd = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][4]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 14);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 6; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%6 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.triID = this->m_trivecTriangles.size();
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);
				
				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -12)
		{
			// -12: Special Case 12b
			// {-12, a, b, c, d, e, i, j, k, -1, ... }
			// where a, b, e forms a triangle and b, c, d, e forms a trapezoid (both connected)
			//       and i, j, k forms a seperate triangle
			// The right hand ordering determines the normals
			// A new point m should be added to the midpoint of b,e
			// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, a, m, i, j, k, -1}

			// ---- Add midpoint m first ----
			POINT3DID ptb = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][2]);
			POINT3DID pte = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][5]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 15);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 6; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 3; ++j)
				{
					if (i != 6 && j == 2)	// only the 6th triangle use 3rd vertex that lies on edge, other use midpoint m as the 3rd pt
						break;

					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (i == 6)
						edgeNo = this->m_triTable[tableIndex][i+j];
					else if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%5 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint, except for the 6th triangle
				if (i != 6)
				{
					triangle.pointID[2] = mID;

					// add EdgeOnFace reference
					EdgeOnFace eof;
					eof.v1ID = triangle.pointID[0];
					eof.v2ID = triangle.pointID[1];
					eof.triID = this->m_trivecTriangles.size();
					eof.v1Edge = this->m_triTable[tableIndex][i];
					eof.v2Edge = this->m_triTable[tableIndex][i%5 + 1];
					int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
					if (faceIdx != -1)
						edgesOnFaces[faceIdx].push_back(eof);
				}
				else
				{
					// add EdgeOnFace reference
					EdgeOnFace eof;
					eof.triID = this->m_trivecTriangles.size();
					for (unsigned int k = 0; k < 3; ++k)
					{
						eof.v1ID = triangle.pointID[k];
						eof.v2ID = triangle.pointID[(k+1)%3];
						eof.v1Edge = this->m_triTable[tableIndex][i+k];
						eof.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
						int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
						if (faceIdx != -1)
							edgesOnFaces[faceIdx].push_back(eof);
					}
				}

				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else if (this->m_triTable[tableIndex][0] == -14)
		{
			// -14: Special Case 14c
			// {-14, a, b, c, d, e, f, -1, ... ]
			// where a,b,c,d,e,f forms a bent hexagon.
			//       new point m is added in the center of the cube
			//       b,c,m and e,f,m are right isoscele triangles
			//       a,b,m and c,d,m and d,e,m and f,a,m are equilateral triangles
			// The right hand ordering determines the normals
			// A new point m should be added to the center of cube (ie. midpoint of a and d)
			// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}

			// ---- Add midpoint m first ----
			POINT3DID pta = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][1]);
			POINT3DID ptd = this->computeVertexLocation(x, y, z, this->m_triTable[tableIndex][4]);
			
			// computing midpoint m
			double m[3] = {0,0,0};
			midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
			POINT3DID ptm;
			ptm.newID = 0;
			ptm.x = m[0];
			ptm.y = m[1];
			ptm.z = m[2];

			// add m into list
			unsigned int mID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(mID, ptm));

			// add new vertex to corresponding edge for reference
			unsigned int edgeID = this->GetEdgeID(x, y, z, 16);
			existingVerticesOnEdge[edgeID].push_back(mID);

			// add vertex info
			VertexInfo vInfo;
			vInfo.x = x;
			vInfo.y = y;
			vInfo.z = z;
			vInfo.patchNo = patchNo;
			vInfo.edgeID = edgeID;
			vertexInfo.insert(ID2VERTEXINFO::value_type(mID, vInfo));

			// create triangles
			for (unsigned int i = 1; i <= 6; ++i) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 2; ++j)
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i];
					if (j == 1)
						edgeNo = this->m_triTable[tableIndex][i%6 + 1];
					// index of edge we are to put a vertex
					edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}
					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.v1ID = triangle.pointID[0];
				eof.v2ID = triangle.pointID[1];
				eof.triID = this->m_trivecTriangles.size();
				eof.v1Edge = this->m_triTable[tableIndex][i];
				eof.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
				if (faceIdx != -1)
					edgesOnFaces[faceIdx].push_back(eof);
				
				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
		else
		{
			for (unsigned int i = 0; this->m_triTable[tableIndex][i] != -1; i += 3) 
			{
				TRIANGLE triangle;
				for (unsigned int j = 0; j < 3; ++j) 
				{
					// edge number we are putting a vertex
					unsigned int edgeNo = this->m_triTable[tableIndex][i+j];
					// index of edge we are to put a vertex
					unsigned int edgeID = this->GetEdgeID(x, y, z, edgeNo);
			
					bool needNewVID(true);
					unsigned int vertexID;
					
					// check for existing vertices that we can use
					unsigned int numExistingVertices = existingVerticesOnEdge[edgeID].size();
					vector<unsigned int> potentialCandidates;	// store existing vertices on edge that has associated triangle as an intersected triangle in current patch
					vector<int> pc_intTriIdx;			// store the intTriIdx of the potential candidate
					for (unsigned int k = 0; k < numExistingVertices; ++k)
					{
						unsigned int vID = existingVerticesOnEdge[edgeID][k];

						for (unsigned int n = 0; n < vertexInfo[vID].intTriIdx.size(); ++n)
						{
							// special case check:
							// check for vertex that got introduced but no edge intersection exist; this occurs at open boundaries
							if (edgeInt[edgeNo].empty())
							{
								if (vertexInfo[vID].intTriIdx[n] == -1)
								{
									vertexID = vID;
									needNewVID = false;
									break;
								}
								else
									continue;
							}

							IntersectingTri exV_IntTri;
							exV_IntTri.intTriIdx = vertexInfo[vID].intTriIdx[n];
							vector<IntersectingTri>::iterator iter = find(intersectedTriangle.begin(), intersectedTriangle.end(), exV_IntTri);
							if (iter == intersectedTriangle.end())
								continue;
							else if (iter->groupIdx == patchNo)
							{
								vertexID = vID;
								needNewVID = false;
								break;
							}
							else
							{
								potentialCandidates.push_back(vID);
								pc_intTriIdx.push_back(vertexInfo[vID].intTriIdx[n]);
							}
						}

						if (!needNewVID)
							break;
					}

					// cannot find an existing vertex on edge that has an associated intTri in our patch, okay, relax a bit, and try to find an existing vertex on edge that has an associated inTri in another patch but are connected to the current patch by a common neighbour patch.
					if (needNewVID)
					{
						// associated triangle for the new vertex if a new one is needed
						int associatedTriIdx = getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);

						// check all potential candidates to see if it can be used
						for (unsigned int k = 0; k < potentialCandidates.size() && needNewVID; ++k)
						{
							// check against all 3 neighbour cubes to see if there's a common patch that connects the potential candidate with the new vertex
							for (unsigned int n = 0; n < 3; ++n)
							{
								unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][n][2] + (this->m_nCellsX)*adjacentCube[edgeNo][n][1] + adjacentCube[edgeNo][n][0];

								IntersectingTri testTri;
								testTri.intTriIdx = associatedTriIdx;
								vector<IntersectingTri>::iterator findAssoIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);
								if (findAssoIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								testTri.intTriIdx = pc_intTriIdx[k];
								vector<IntersectingTri>::iterator findPCIter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), testTri);

								if (findPCIter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())
									continue;

								if (findAssoIter->groupIdx == findPCIter->groupIdx)
								{
									vertexID = potentialCandidates[k];
									needNewVID = false;
									break;
								}
								
							}
						}
					}

					
					// if no existing vertex can be used, add a new one
					if (needNewVID)
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						// add new vertex to corresponding edge for reference
						existingVerticesOnEdge[edgeID].push_back(vertexID);

						// add vertex info
						VertexInfo vInfo;
						vInfo.x = x;
						vInfo.y = y;
						vInfo.z = z;
						vInfo.patchNo = patchNo;
						vInfo.intTriIdx.push_back(getAssociatedTriIdx(x, y, z, patchNo, edgeNo, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList));
						vInfo.edgeID = edgeID;
						vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, vInfo));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// add EdgeOnFace reference
				EdgeOnFace eof;
				eof.triID = this->m_trivecTriangles.size();
				for (unsigned int k = 0; k < 3; ++k)
				{
					eof.v1ID = triangle.pointID[k];
					eof.v2ID = triangle.pointID[(k+1)%3];
					eof.v1Edge = this->m_triTable[tableIndex][i+k];
					eof.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
					int faceIdx = edgeOnFaceLookup[eof.v1Edge][eof.v2Edge];
					if (faceIdx != -1)
						edgesOnFaces[faceIdx].push_back(eof);
				}
				
				// add triangle reference to vertices
				for (unsigned int j = 0; j < 3; ++j)
					vertexInfo[triangle.pointID[j]].triNbrs.push_back(this->m_trivecTriangles.size());

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);
			}
		}
	}
}

// determine if patch on cube(x,y,z) will introduce new triangles
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// patchNo (in): patch number we are triangulating
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
// return value (out): true if patch will introduce new triangles
template <class T> bool TilingNonobtuseMC<T>::willTriangulate(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList)
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;

	// assign signs to corners
	if (!cubesProperty[cubeIdx].isSignSet[patchNo])
	{
		vector<Intersection> edgeInt[12];
		findEdgeIntersection(x, y, z, patchNo, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList, edgeInt);

		if (!assignSigns(edgeInt, x, y, z, patchNo)) {
			return false;
		}
	}
	else if (cubesProperty[cubeIdx].signs[patchNo][0] == 0 || cubesProperty[cubeIdx].signs[patchNo][1] == 0 || cubesProperty[cubeIdx].signs[patchNo][2] == 0 || cubesProperty[cubeIdx].signs[patchNo][3] == 0 || cubesProperty[cubeIdx].signs[patchNo][4] == 0 || cubesProperty[cubeIdx].signs[patchNo][5] == 0 || cubesProperty[cubeIdx].signs[patchNo][6] == 0 || cubesProperty[cubeIdx].signs[patchNo][7] == 0)
	{
		return false;
	}

	vector<int> &signs = cubesProperty[cubeIdx].signs[patchNo];
	unsigned int tableIndex = 0;
	//    1----2
	//   /|  / |
	// 5----6  |
	// | /0-|--3
	// |/   |/
	// 4----7
	for (unsigned int i = 0; i < 8; ++i)
	{
		if (signs[i] > this->m_tIsoLevel)
			tableIndex |= (unsigned int)pow(2,i);
	}	
		
	// Now create a triangulation of the isosurface in this
	// cell.
	if (this->m_edgeTable[tableIndex] != 0)
		return true;

	return false;
}

// check if e1 can be stitched against neighbour cube 'nbrCubeIdx'
//
// e1 (in): edge 1
// faceIdx (in): face that e1 and e2 lies on
// nbrCubeIdx (in): index of neighbour cube
// return value (in): true if edge can be stitched
template <class T> bool TilingNonobtuseMC<T>::canStitch(EdgeOnFace e1, unsigned int faceIdx, unsigned int nbrCubeIdx)
{
	// used to transform face indices of the current cube into face indices in the neighbour cube
	unsigned int transformedToNbrFaceIdx[6] = {2, 3, 0, 1, 5, 4};

	vector<EdgeOnFace> (&edgesOnFaces)[6] = cubesProperty[nbrCubeIdx].edgesOnFaces;
	for (vector<EdgeOnFace>::iterator iter = edgesOnFaces[transformedToNbrFaceIdx[faceIdx]].begin(); iter != edgesOnFaces[transformedToNbrFaceIdx[faceIdx]].end(); iter++)
	{
		if ((iter->v1ID == e1.v1ID && iter->v2ID == e1.v2ID) || (iter->v1ID == e1.v2ID && iter->v2ID == e1.v1ID))
			return false;
	}

	return true;
}

// stitch the two edges e1 and e2 together
//
// e1 (in): edge 1
// e2 (in): edge 2
// faceIdx (in): index of face where the stitching occurs
// x (in): x-coord of cube where stitching occurs
// y (in): y-coord of cube where stitching occurs
// z (in): z-coord of cube where stitching occurs
// e1nbrPatch1No (in): neighbour patch number that connects e1.v1
// e1nbrPatch2No (in): neighbour patch number that connects e1.v2
// e2nbrPatch1No (in): neighbour patch number that connects e2.v1
// e2nbrPatch2No (in): neighbour patch number that connects e2.v2
// eofs (out): list of edges on the face 'faceIdx' of the cube; will be modified to update the new vIDs
// nbrPatch1NoList (out): stores the neighbour patch for v1 of edge stored in edgesOnFaces
// nbrPatch2NoList (out): stores the neighbour patch for v2 of edge stored in edgesOnFaces
template <class T> void TilingNonobtuseMC<T>::stitch(EdgeOnFace e1, EdgeOnFace e2, unsigned int faceIdx, unsigned int x, unsigned int y, unsigned int z, unsigned int e1nbrPatch1No, unsigned int e1nbrPatch2No, unsigned int e2nbrPatch1No, unsigned int e2nbrPatch2No, vector<EdgeOnFace> &eofs, vector<int> (&nbrPatch1NoList)[6], vector<int> (&nbrPatch2NoList)[6])
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;

	int edgeToFaceNbrIdx[12][8][4] = {
										{{x,y,z,1}, {x,y,z,2}, {x-1,y,z,3}, {x-1,y,z,2}, {x-1,y,z-1,0}, {x-1,y,z-1,3}, {x,y,z-1,0}, {x,y,z-1,1}},
										{{x,y,z,2}, {x,y,z,4}, {x,y,z-1,0}, {x,y,z-1,4}, {x,y+1,z-1,0}, {x,y+1,z-1,5}, {x,y+1,z,5}, {x,y+1,z,2}},
										{{x,y,z,2}, {x,y,z,3}, {x+1,y,z,1}, {x+1,y,z,2}, {x+1,y,z-1,1}, {x+1,y,z-1,0}, {x,y,z-1,0}, {x,y,z-1,3}},
										{{x,y,z,2}, {x,y,z,5}, {x,y-1,z,4}, {x,y-1,z,2}, {x,y-1,z-1,0}, {x,y-1,z-1,4}, {x,y,z-1,5}, {x,y,z-1,0}},
										{{x,y,z,1}, {x,y,z,0}, {x-1,y,z,3}, {x-1,y,z,0}, {x-1,y,z+1,2}, {x-1,y,z+1,3}, {x,y,z+1,2}, {x,y,z+1,1}},
										{{x,y,z,0}, {x,y,z,4}, {x,y,z+1,2}, {x,y,z+1,4}, {x,y+1,z+1,2}, {x,y+1,z+1,5}, {x,y+1,z,5}, {x,y+1,z,0}},
										{{x,y,z,0}, {x,y,z,3}, {x+1,y,z,1}, {x+1,y,z,0}, {x+1,y,z+1,1}, {x+1,y,z+1,2}, {x,y,z+1,2}, {x,y,z+1,3}},
										{{x,y,z,0}, {x,y,z,5}, {x,y-1,z,4}, {x,y-1,z,0}, {x,y-1,z+1,2}, {x,y-1,z+1,4}, {x,y,z+1,5}, {x,y,z+1,2}},
										{{x,y,z,1}, {x,y,z,5}, {x-1,y,z,3}, {x-1,y,z,5}, {x-1,y-1,z,3}, {x-1,y-1,z,4}, {x,y-1,z,1}, {x,y-1,z,4}},
										{{x,y,z,1}, {x,y,z,4}, {x-1,y,z,3}, {x-1,y,z,4}, {x-1,y+1,z,3}, {x-1,y+1,z,5}, {x,y+1,z,1}, {x,y+1,z,5}},
										{{x,y,z,3}, {x,y,z,4}, {x+1,y,z,1}, {x+1,y,z,4}, {x+1,y+1,z,1}, {x+1,y+1,z,5}, {x,y+1,z,3}, {x,y+1,z,5}},
										{{x,y,z,3}, {x,y,z,5}, {x+1,y,z,1}, {x+1,y,z,5}, {x+1,y-1,z,1}, {x+1,y-1,z,4}, {x,y-1,z,3}, {x,y-1,z,4}}
									};

	// use to transform v1Edge and v2Edge into edge index in 2D
	//
	//  _ 1 _
	// |     |
	// 0     2
	// |_   _|
	//    3
	int transformEdgeIdx[6][12] = {{-1,-1,-1,-1,0,1,2,3,-1,-1,-1,-1},
									{0,-1,-1,-1,2,-1,-1,-1,3,1,-1,-1},
									{2,1,0,3,-1,-1,-1,-1,-1,-1,-1,-1},
									{-1,-1,2,-1,-1,-1,0,-1,-1,-1,1,3},
									{-1,1,-1,-1,-1,3,-1,-1,-1,0,2,-1},
									{-1,-1,-1,3,-1,-1,-1,1,0,-1,-1,2}};

	int e1v1 = transformEdgeIdx[faceIdx][e1.v1Edge];
	int e1v2 = transformEdgeIdx[faceIdx][e1.v2Edge];
	int e2v1 = transformEdgeIdx[faceIdx][e2.v1Edge];
	int e2v2 = transformEdgeIdx[faceIdx][e2.v2Edge];

	// sanity check
	if (e1v1 == -1 || e1v2 == -1 || e2v1 == -1 || e2v2 == -1)
	{
		cout << "ERROR: TilingNonobtuseMC::stitch() - conflict in edge indices" << endl;
		return;
	}

	if (e1v1 == e2v2 && e1v2 == e2v1)	// edges have same orientation; stitch both ends
	{
		if (e1.v1ID != e2.v2ID)
		{
			POINT3DID pt = this->m_i2pt3idVertices[e1.v1ID];
			VertexInfo v1Info = vertexInfo[e1.v1ID];
			VertexInfo v2Info = vertexInfo[e2.v2ID];
	
			// erase e1.v1
			this->m_i2pt3idVertices.erase(e1.v1ID);
			vertexInfo.erase(e1.v1ID);
	
			// erase e2.v2
			this->m_i2pt3idVertices.erase(e2.v2ID);
			vertexInfo.erase(e2.v2ID);
			
			// add merged vertex with new vID
			unsigned int vertexID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));
	
			// update vIDs in edgesOnFace
			for (unsigned int i = 0; i < 8; ++i) {
				if (edgeToFaceNbrIdx[e1.v1Edge][i][0] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][0] >= (int)this->m_nCellsX || edgeToFaceNbrIdx[e1.v1Edge][i][1] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][1] >= (int)this->m_nCellsY || edgeToFaceNbrIdx[e1.v1Edge][i][2] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][2] >= (int)this->m_nCellsZ)
					continue;

				unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*edgeToFaceNbrIdx[e1.v1Edge][i][2] + (this->m_nCellsX)*edgeToFaceNbrIdx[e1.v1Edge][i][1] + edgeToFaceNbrIdx[e1.v1Edge][i][0];
				vector<EdgeOnFace> &edgesOnCurrentFace = cubesProperty[nbrCubeIdx].edgesOnFaces[edgeToFaceNbrIdx[e1.v1Edge][i][3]];
				for (vector<EdgeOnFace>::iterator iter = edgesOnCurrentFace.begin(); iter != edgesOnCurrentFace.end(); iter++) {
					if (iter->v1ID == e1.v1ID || iter->v1ID == e2.v2ID)
						iter->v1ID = vertexID;
					if (iter->v2ID == e1.v1ID || iter->v2ID == e2.v2ID)
						iter->v2ID = vertexID;
				}
			}
			
			// rename e1.v1 and e2.v2 reference in existingVerticesOnEdge
			bool renamedV1(false);
			bool renamedV2(false);
			for (vector<unsigned int>::iterator iter = existingVerticesOnEdge[v1Info.edgeID].begin(); iter != existingVerticesOnEdge[v1Info.edgeID].end(); iter++) {
				if (!renamedV1 && *iter == e1.v1ID) {
					*iter = vertexID;
					renamedV1 = true;
				}
				if (!renamedV2 && *iter == e2.v2ID) {
					*iter = vertexID;
					renamedV2 = true;
				}
				if (renamedV1 && renamedV2)
					break;
			}
	
			VertexInfo newVInfo;
			newVInfo.x = 0;
			newVInfo.y = 0;
			newVInfo.z = 0;
			newVInfo.patchNo = 0;
			// update intTriIdx for the merged vertex
			newVInfo.intTriIdx = v1Info.intTriIdx;
			for (vector<int>::iterator iter = v2Info.intTriIdx.begin(); iter != v2Info.intTriIdx.end(); iter++)
			{
				if (find(newVInfo.intTriIdx.begin(), newVInfo.intTriIdx.end(), *iter) == newVInfo.intTriIdx.end())
					newVInfo.intTriIdx.push_back(*iter);
			}

			newVInfo.edgeID = v1Info.edgeID;
	
			
			// rename e1.v1 reference in triangle
			for (vector<unsigned int>::iterator iter = v1Info.triNbrs.begin(); iter != v1Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e1.v1ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
			// rename e2.v2 reference in triangle
			for (vector<unsigned int>::iterator iter = v2Info.triNbrs.begin(); iter != v2Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				if (find(newVInfo.triNbrs.begin(), newVInfo.triNbrs.end(), triID) == newVInfo.triNbrs.end())
					newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e2.v2ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
	
			// add merged vertex info with new vID
			vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, newVInfo));
		}

		if (e1.v2ID != e2.v1ID)
		{
			POINT3DID pt = this->m_i2pt3idVertices[e1.v2ID];
			VertexInfo v1Info = vertexInfo[e1.v2ID];
			VertexInfo v2Info = vertexInfo[e2.v1ID];
	
			// erase e1.v2
			this->m_i2pt3idVertices.erase(e1.v2ID);
			vertexInfo.erase(e1.v2ID);
			
			// erase e2.v1
			this->m_i2pt3idVertices.erase(e2.v1ID);
			vertexInfo.erase(e2.v1ID);
			
			// add merged vertex with new vID
			unsigned int vertexID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));
	
			// update vIDs in edgesOnFace
			for (unsigned int i = 0; i < 8; ++i) {
				if (edgeToFaceNbrIdx[e1.v2Edge][i][0] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][0] >= (int)this->m_nCellsX || edgeToFaceNbrIdx[e1.v2Edge][i][1] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][1] >= (int)this->m_nCellsY || edgeToFaceNbrIdx[e1.v2Edge][i][2] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][2] >= (int)this->m_nCellsZ)
					continue;

				unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*edgeToFaceNbrIdx[e1.v2Edge][i][2] + (this->m_nCellsX)*edgeToFaceNbrIdx[e1.v2Edge][i][1] + edgeToFaceNbrIdx[e1.v2Edge][i][0];
				vector<EdgeOnFace> &edgesOnCurrentFace = cubesProperty[nbrCubeIdx].edgesOnFaces[edgeToFaceNbrIdx[e1.v2Edge][i][3]];
				for (vector<EdgeOnFace>::iterator iter = edgesOnCurrentFace.begin(); iter != edgesOnCurrentFace.end(); iter++) {
					if (iter->v1ID == e1.v2ID || iter->v1ID == e2.v1ID)
						iter->v1ID = vertexID;
					if (iter->v2ID == e1.v2ID || iter->v2ID == e2.v1ID)
						iter->v2ID = vertexID;
				}
			}
					
			// rename e1.v2 and e2.v1 reference in existingVerticesOnEdge
			bool renamedV1 = false;
			bool renamedV2 = false;
			for (vector<unsigned int>::iterator iter = existingVerticesOnEdge[v1Info.edgeID].begin(); iter != existingVerticesOnEdge[v1Info.edgeID].end(); iter++) {
				if (!renamedV1 && *iter == e1.v2ID) {
					*iter = vertexID;
					renamedV1 = true;
				}
				if (!renamedV2 && *iter == e2.v1ID) {
					*iter = vertexID;
					renamedV2 = true;
				}
				if (renamedV1 && renamedV2)
					break;
			}
	
			VertexInfo newVInfo;
			newVInfo.x = 0;
			newVInfo.y = 0;
			newVInfo.z = 0;
			newVInfo.patchNo = 0;
			// update intTriIdx for the merged vertex
			newVInfo.intTriIdx = v1Info.intTriIdx;
			for (vector<int>::iterator iter = v2Info.intTriIdx.begin(); iter != v2Info.intTriIdx.end(); iter++)
			{
				if (find(newVInfo.intTriIdx.begin(), newVInfo.intTriIdx.end(), *iter) == newVInfo.intTriIdx.end())
					newVInfo.intTriIdx.push_back(*iter);
			}
			newVInfo.edgeID = v1Info.edgeID;
	
			
			// rename e1.v2 reference in triangle
			for (vector<unsigned int>::iterator iter = v1Info.triNbrs.begin(); iter != v1Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e1.v2ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
			// rename e2.v1 reference in triangle
			for (vector<unsigned int>::iterator iter = v2Info.triNbrs.begin(); iter != v2Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				if (find(newVInfo.triNbrs.begin(), newVInfo.triNbrs.end(), triID) == newVInfo.triNbrs.end())
					newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e2.v1ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
	
			// add merged vertex info with new vID
			vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, newVInfo));
		}
	}
	else if (e1v1 == e2v2)				// one end is stitched, then triangulate
	{
		unsigned int vertexID;
		if (e1.v1ID != e2.v2ID)
		{
			POINT3DID pt = this->m_i2pt3idVertices[e1.v1ID];
			VertexInfo v1Info = vertexInfo[e1.v1ID];
			VertexInfo v2Info = vertexInfo[e2.v2ID];
	
			// erase e1.v1
			this->m_i2pt3idVertices.erase(e1.v1ID);
			vertexInfo.erase(e1.v1ID);
			
			// erase e2.v2
			this->m_i2pt3idVertices.erase(e2.v2ID);
			vertexInfo.erase(e2.v2ID);
			
			// add merged vertex with new vID
			vertexID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));
			
			// update vIDs in edgesOnFace
			for (unsigned int i = 0; i < 8; ++i) {
				if (edgeToFaceNbrIdx[e1.v1Edge][i][0] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][0] >= (int)this->m_nCellsX || edgeToFaceNbrIdx[e1.v1Edge][i][1] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][1] >= (int)this->m_nCellsY || edgeToFaceNbrIdx[e1.v1Edge][i][2] < 0 || edgeToFaceNbrIdx[e1.v1Edge][i][2] >= (int)this->m_nCellsZ)
					continue;

				unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*edgeToFaceNbrIdx[e1.v1Edge][i][2] + (this->m_nCellsX)*edgeToFaceNbrIdx[e1.v1Edge][i][1] + edgeToFaceNbrIdx[e1.v1Edge][i][0];
				vector<EdgeOnFace> &edgesOnCurrentFace = cubesProperty[nbrCubeIdx].edgesOnFaces[edgeToFaceNbrIdx[e1.v1Edge][i][3]];
				for (vector<EdgeOnFace>::iterator iter = edgesOnCurrentFace.begin(); iter != edgesOnCurrentFace.end(); iter++) {
					if (iter->v1ID == e1.v1ID || iter->v1ID == e2.v2ID)
						iter->v1ID = vertexID;
					if (iter->v2ID == e1.v1ID || iter->v2ID == e2.v2ID)
						iter->v2ID = vertexID;
				}
			}

			// rename e1.v1 and e2.v2 reference in existingVerticesOnEdge
			bool renamedV1(false);
			bool renamedV2(false);
			for (vector<unsigned int>::iterator iter = existingVerticesOnEdge[v1Info.edgeID].begin(); iter != existingVerticesOnEdge[v1Info.edgeID].end(); iter++) {
				if (!renamedV1 && *iter == e1.v1ID) {
					*iter = vertexID;
					renamedV1 = true;
				}
				if (!renamedV2 && *iter == e2.v2ID) {
					*iter = vertexID;
					renamedV2 = true;
				}
				if (renamedV1 && renamedV2)
					break;
			}
	
			VertexInfo newVInfo;
			newVInfo.x = 0;
			newVInfo.y = 0;
			newVInfo.z = 0;
			newVInfo.patchNo = 0;
			// update intTriIdx for the merged vertex
			newVInfo.intTriIdx = v1Info.intTriIdx;
			for (vector<int>::iterator iter = v2Info.intTriIdx.begin(); iter != v2Info.intTriIdx.end(); iter++)
			{
				if (find(newVInfo.intTriIdx.begin(), newVInfo.intTriIdx.end(), *iter) == newVInfo.intTriIdx.end())
					newVInfo.intTriIdx.push_back(*iter);
			}
			newVInfo.edgeID = v1Info.edgeID;
	
			
			// rename e1.v1 reference in triangle
			for (vector<unsigned int>::iterator iter = v1Info.triNbrs.begin(); iter != v1Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e1.v1ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
			// rename e2.v2 reference in triangle
			for (vector<unsigned int>::iterator iter = v2Info.triNbrs.begin(); iter != v2Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				if (find(newVInfo.triNbrs.begin(), newVInfo.triNbrs.end(), triID) == newVInfo.triNbrs.end())
					newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e2.v2ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
	
			// add merged vertex info with new vID
			vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, newVInfo));
		}
		else
			vertexID = e1.v1ID;

		// create triangle to cover hole
		TRIANGLE triangle;
		triangle.pointID[0] = vertexID;
		triangle.pointID[1] = e2.v1ID;
		triangle.pointID[2] = e1.v2ID;
		this->m_trivecTriangles.push_back(triangle);

		// add triangle reference into vertices
		unsigned int newTriID = this->m_trivecTriangles.size()-1;
		vertexInfo[triangle.pointID[0]].triNbrs.push_back(newTriID);
		vertexInfo[triangle.pointID[1]].triNbrs.push_back(newTriID);
		vertexInfo[triangle.pointID[2]].triNbrs.push_back(newTriID);

		// add new edge to edgesOnFaces
		EdgeOnFace eof;
		eof.v1ID = e2.v1ID;
		eof.v2ID = e1.v2ID;
		eof.triID = newTriID;
		eof.v1Edge = e2.v1Edge;
		eof.v2Edge = e1.v2Edge;
		cubesProperty[cubeIdx].edgesOnFaces[faceIdx].push_back(eof);
		
		// for checkStitching()
		eofs.push_back(eof);
		nbrPatch1NoList[faceIdx].push_back(e2nbrPatch1No);
		nbrPatch2NoList[faceIdx].push_back(e1nbrPatch2No);
	}
	else if (e1v2 == e2v1)				// the other end is stitched, then triangulate
	{	
		unsigned int vertexID;
		if (e1.v2ID != e2.v1ID)
		{
			POINT3DID pt = this->m_i2pt3idVertices[e1.v2ID];
			VertexInfo v1Info = vertexInfo[e1.v2ID];
			VertexInfo v2Info = vertexInfo[e2.v1ID];
	
			// erase e1.v2
			this->m_i2pt3idVertices.erase(e1.v2ID);
			vertexInfo.erase(e1.v2ID);
			
			// erase e2.v1
			this->m_i2pt3idVertices.erase(e2.v1ID);
			vertexInfo.erase(e2.v1ID);
			
			// add merged vertex with new vID
			vertexID = nextUnusedVertexID;
			++nextUnusedVertexID;
			this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));
			
			// update vIDs in edgesOnFace
			for (unsigned int i = 0; i < 8; ++i) {
				if (edgeToFaceNbrIdx[e1.v2Edge][i][0] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][0] >= (int)this->m_nCellsX || edgeToFaceNbrIdx[e1.v2Edge][i][1] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][1] >= (int)this->m_nCellsY || edgeToFaceNbrIdx[e1.v2Edge][i][2] < 0 || edgeToFaceNbrIdx[e1.v2Edge][i][2] >= (int)this->m_nCellsZ)
					continue;

				unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*edgeToFaceNbrIdx[e1.v2Edge][i][2] + (this->m_nCellsX)*edgeToFaceNbrIdx[e1.v2Edge][i][1] + edgeToFaceNbrIdx[e1.v2Edge][i][0];
				vector<EdgeOnFace> &edgesOnCurrentFace = cubesProperty[nbrCubeIdx].edgesOnFaces[edgeToFaceNbrIdx[e1.v2Edge][i][3]];
				for (vector<EdgeOnFace>::iterator iter = edgesOnCurrentFace.begin(); iter != edgesOnCurrentFace.end(); iter++) {
					if (iter->v1ID == e1.v2ID || iter->v1ID == e2.v1ID)
						iter->v1ID = vertexID;
					if (iter->v2ID == e1.v2ID || iter->v2ID == e2.v1ID)
						iter->v2ID = vertexID;
				}
			}

			// rename e1.v2 and e2.v1 reference in existingVerticesOnEdge
			bool renamedV1 = false;
			bool renamedV2 = false;
			for (vector<unsigned int>::iterator iter = existingVerticesOnEdge[v1Info.edgeID].begin(); iter != existingVerticesOnEdge[v1Info.edgeID].end(); iter++) {
				if (!renamedV1 && *iter == e1.v2ID) {
					*iter = vertexID;
					renamedV1 = true;
				}
				if (!renamedV2 && *iter == e2.v1ID) {
					*iter = vertexID;
					renamedV2 = true;
				}
				if (renamedV1 && renamedV2)
					break;
			}
	
			VertexInfo newVInfo;
			newVInfo.x = 0;
			newVInfo.y = 0;
			newVInfo.z = 0;
			newVInfo.patchNo = 0;
			// update intTriIdx for the merged vertex
			newVInfo.intTriIdx = v1Info.intTriIdx;
			for (vector<int>::iterator iter = v2Info.intTriIdx.begin(); iter != v2Info.intTriIdx.end(); iter++)
			{
				if (find(newVInfo.intTriIdx.begin(), newVInfo.intTriIdx.end(), *iter) == newVInfo.intTriIdx.end())
					newVInfo.intTriIdx.push_back(*iter);
			}
			newVInfo.edgeID = v1Info.edgeID;
	
			
			// rename e1.v2 reference in triangle
			for (vector<unsigned int>::iterator iter = v1Info.triNbrs.begin(); iter != v1Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e1.v2ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
			// rename e2.v1 reference in triangle
			for (vector<unsigned int>::iterator iter = v2Info.triNbrs.begin(); iter != v2Info.triNbrs.end(); iter++)
			{
				unsigned int triID = *iter;
				if (find(newVInfo.triNbrs.begin(), newVInfo.triNbrs.end(), triID) == newVInfo.triNbrs.end())
					newVInfo.triNbrs.push_back(triID);
				for (unsigned int i = 0; i < 3; ++i) {
					if (this->m_trivecTriangles[triID].pointID[i] == e2.v1ID) {
						this->m_trivecTriangles[triID].pointID[i] = vertexID;
						break;
					}
				}
			}
	
			// add merged vertex info with new vID
			vertexInfo.insert(ID2VERTEXINFO::value_type(vertexID, newVInfo));
		}
		else
			vertexID = e1.v2ID;

		// create triangle to cover hole
		TRIANGLE triangle;
		triangle.pointID[0] = vertexID;
		triangle.pointID[1] = e1.v1ID;
		triangle.pointID[2] = e2.v2ID;
		this->m_trivecTriangles.push_back(triangle);

		// add triangle reference into vertices
		unsigned int newTriID = this->m_trivecTriangles.size()-1;
		vertexInfo[triangle.pointID[0]].triNbrs.push_back(newTriID);
		vertexInfo[triangle.pointID[1]].triNbrs.push_back(newTriID);
		vertexInfo[triangle.pointID[2]].triNbrs.push_back(newTriID);

		// add new edge to edgesOnFaces
		EdgeOnFace eof;
		eof.v1ID = e1.v1ID;
		eof.v2ID = e2.v2ID;
		eof.triID = newTriID;
		eof.v1Edge = e1.v1Edge;
		eof.v2Edge = e2.v2Edge;
		cubesProperty[cubeIdx].edgesOnFaces[faceIdx].push_back(eof);
		
		// for checkStitching()
		eofs.push_back(eof);
		nbrPatch1NoList[faceIdx].push_back(e1nbrPatch1No);
		nbrPatch2NoList[faceIdx].push_back(e2nbrPatch2No);
	}
	else if (e1v1 != e2v1 && e1v1 != e2v2 && e1v2 != e2v1 && e1v2 != e2v2)	// edges are parallel; triangulate
	{
		if (((e1v1 + e2v1)%2 == 0) && ((e1v2 + e2v2)%2 == 0))	// to check for consistent facing
		{
			// create triangle to cover hole
			TRIANGLE triangle;
			triangle.pointID[0] = e1.v2ID;
			triangle.pointID[1] = e1.v1ID;
			triangle.pointID[2] = e2.v1ID;
			this->m_trivecTriangles.push_back(triangle);

			// add triangle reference into vertices
			unsigned int newTriID = this->m_trivecTriangles.size()-1;
			vertexInfo[triangle.pointID[0]].triNbrs.push_back(newTriID);
			vertexInfo[triangle.pointID[1]].triNbrs.push_back(newTriID);
			vertexInfo[triangle.pointID[2]].triNbrs.push_back(newTriID);
			
			triangle.pointID[0] = e2.v2ID;
			triangle.pointID[1] = e2.v1ID;
			triangle.pointID[2] = e1.v1ID;
			this->m_trivecTriangles.push_back(triangle);

			// add triangle reference into vertices
			newTriID = this->m_trivecTriangles.size()-1;
			vertexInfo[triangle.pointID[0]].triNbrs.push_back(newTriID);
			vertexInfo[triangle.pointID[1]].triNbrs.push_back(newTriID);
			vertexInfo[triangle.pointID[2]].triNbrs.push_back(newTriID);

			// add new edge to edgesOnFaces
			EdgeOnFace eof;
			eof.v1ID = e1.v1ID;
			eof.v2ID = e2.v1ID;
			eof.triID = newTriID;
			eof.v1Edge = e1.v1Edge;
			eof.v2Edge = e2.v1Edge;
			cubesProperty[cubeIdx].edgesOnFaces[faceIdx].push_back(eof);

			eof.v1ID = e2.v1ID;
			eof.v2ID = e1.v2ID;
			eof.triID = newTriID;
			eof.v1Edge = e2.v1Edge;
			eof.v2Edge = e1.v2Edge;
			cubesProperty[cubeIdx].edgesOnFaces[faceIdx].push_back(eof);

			eof.v1ID = e1.v1ID;
			eof.v2ID = e2.v2ID;
			eof.triID = newTriID;
			eof.v1Edge = e1.v1Edge;
			eof.v2Edge = e2.v2Edge;
			cubesProperty[cubeIdx].edgesOnFaces[faceIdx].push_back(eof);
		}
		else
			cout << "WARNING: TilingNonobtuseMC::stitch() - degenerate cases in stitching" << endl;
	}
	else		// degenerate cases
	{
		cout << "WARNING: TilingNonobtuseMC::stitch() - degenerate cases in stitching" << endl;
	}
}

// based on edge intersections, determine all signs for the cube
// array of signs for all 8 corners of the cube; 1=outside, -1=inside
//
// edgeInt (in): array of 12 slots; stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// patchNo (in): patch number we are triangulating
// return value (out): true if all signs can be assigned without conflict
template <class T> bool TilingNonobtuseMC<T>::assignSigns(const vector<Intersection> (&edgeInt)[12], unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo)
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	vector<int> &signs = cubesProperty[cubeIdx].signs[patchNo];
	
	if (cubesProperty[cubeIdx].isSignSet[patchNo])
	{
		if (signs[0] == 0 || signs[1] == 0 || signs[2] == 0 || signs[3] == 0 || signs[4] == 0 || signs[5] == 0 || signs[6] == 0 || signs[7] == 0)
			return false;

		return true;
	}

	cubesProperty[cubeIdx].isSignSet[patchNo] = true;

	// if no edge intersection, return an empty cube
	if (edgeInt[0].empty() && edgeInt[1].empty() && edgeInt[2].empty() && edgeInt[3].empty() && edgeInt[4].empty() && edgeInt[5].empty() && edgeInt[6].empty() && edgeInt[7].empty() && edgeInt[8].empty() && edgeInt[9].empty() && edgeInt[10].empty() && edgeInt[11].empty()) {
		for (unsigned int i = 0; i < 8; ++i)
			signs[i] = 1;
		return true;
	}

	unsigned int nbrEdgeIdx[8][3] = {{3,0,8},{1,0,9},{1,2,10},{3,2,11},{7,4,8},{5,4,9},{5,6,10},{7,6,11}};
	unsigned int closestIntIdx[8][3] = {{0,0,0},
										{0,edgeInt[0].size()-1,0},
										{edgeInt[1].size()-1,edgeInt[2].size()-1,0},
										{edgeInt[3].size()-1,0,0},
										{0,0,edgeInt[8].size()-1},
										{0,edgeInt[4].size()-1,edgeInt[9].size()-1},
										{edgeInt[5].size()-1,edgeInt[6].size()-1,edgeInt[10].size()-1},
										{edgeInt[7].size()-1,0,edgeInt[11].size()-1}};
	bool flipFacingPos[8][3] = {{false,false,false},
								{false,true,false},
								{true,true,false},
								{true,false,false},
								{false,false,true},
								{false,true,true},
								{true,true,true},
								{true,false,true}};
	unsigned int nbrIdx[8][3] = {{3,1,4},{2,0,5},{1,3,6},{0,2,7},{7,5,0},{6,4,1},{5,7,2},{4,6,3}};
	//    1----2
	//   /|  / |
	// 5----6  |
	// | /0-|--3
	// |/   |/
	// 4----7
	// assign signs to all 8 corners of the cube
	vector<unsigned int> unassignedCorners;
	for (unsigned int i = 0; i < 8; ++i)
	{
		// assign signs and make sure it doesn't conflict among the 3 neighbour edges
		signs[i] = 0;
		for (int j = 0; j < 3; ++j) {
			if (signs[i] == 0) {
				if (!edgeInt[nbrEdgeIdx[i][j]].empty()) {
					if (flipFacingPos[i][j] != edgeInt[nbrEdgeIdx[i][j]][closestIntIdx[i][j]].isFacingPos)
						signs[i] = -1;
					else
						signs[i] = 1;

					j = -1;	// make sure we check for consistency on all edges
					continue;
				}
			}
			else if (!edgeInt[nbrEdgeIdx[i][j]].empty()) {
				if ((signs[i] == -1 && flipFacingPos[i][j] == edgeInt[nbrEdgeIdx[i][j]][closestIntIdx[i][j]].isFacingPos) ||
					(signs[i] == 1 && flipFacingPos[i][j] != edgeInt[nbrEdgeIdx[i][j]][closestIntIdx[i][j]].isFacingPos))
				{
					//cout << "Cannot assign signs for cube(" << x << ", " << y << ", " << z << ") patch " << patchNo << endl;
					signs[0] = 0;
					return false;
				}
			}
			else if (signs[nbrIdx[i][j]] != 0 && signs[i] != signs[nbrIdx[i][j]]) {
				//cout << "Cannot assign signs for cube(" << x << ", " << y << ", " << z << ") patch " << patchNo << endl;
				signs[0] = 0;
				return false;
			}
		}

		if (signs[i] == 0)
			unassignedCorners.push_back(i);
	}

	// for all unmarked corner, we assign it by examining the closest neighbour
	vector<unsigned int>::iterator iter = unassignedCorners.begin();
	while (!unassignedCorners.empty())
	{
		unsigned int cornerIdx = *iter;
		// assign signs based on neighbour and check for consistency
		for (int j = 0; j < 3; ++j) {
			if (signs[cornerIdx] == 0) {
				if (signs[nbrIdx[cornerIdx][j]] != 0) {
					signs[cornerIdx] = signs[nbrIdx[cornerIdx][j]];
					unassignedCorners.erase(iter);
					j = -1;
					continue;
				}
			}
			else if (signs[nbrIdx[cornerIdx][j]] != 0 && signs[cornerIdx] != signs[nbrIdx[cornerIdx][j]])
			{
				//cout << "Cannot assign signs for cube(" << x << ", " << y << ", " << z << ") patch " << patchNo << endl;
				signs[0] = 0;
				return false;
			}
		}

		if (signs[cornerIdx] == 0)
			iter++;
		if (iter == unassignedCorners.end())
			iter = unassignedCorners.begin();
	}

	return true;
}

// find edge intersection of groupIdx on cube(x,y,z)
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// groupIdx (in): group index to check
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
// edgeInt (out): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
template <class T> void TilingNonobtuseMC<T>::findEdgeIntersection(unsigned int x, unsigned int y, unsigned int z, unsigned int groupIdx, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, vector<Intersection> (&edgeInt)[12])
{
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	list<unsigned int> &group = cubesProperty[cubeIdx].triGroups[groupIdx];
	vector<IntersectingTri> &intersectedTriangle = cubesProperty[cubeIdx].intersectedTriangle;

	// find intersection on all 12 edges of cube(x,y,z)
	for (unsigned int i = 0; i < 12; ++i)
	{
		unsigned int v1x = x;
		unsigned int v1y = y;
		unsigned int v1z = z;
		unsigned int v2x = x;
		unsigned int v2y = y+1;
		unsigned int v2z = z;
		if (i == 1)	{
			v1y += 1;
			v2x += 1;
		} else if (i == 2) {
			v1x += 1;
			v2x += 1;
		} else if (i == 3) {
			v2x += 1;
			v2y -= 1;
		} else if (i == 4) {
			v1z += 1;
			v2z += 1;
		} else if (i == 5) {
			v1y += 1;
			v1z += 1;
			v2x += 1;
			v2z += 1;
		} else if (i == 6) {
			v1x += 1;
			v1z += 1;
			v2x += 1;
			v2z += 1;
		} else if (i == 7) {
			v1z += 1;
			v2x += 1;
			v2y -= 1;
			v2z += 1;
		} else if (i == 8) {
			v2y -= 1;
			v2z += 1;
		} else if (i == 9) {
			v1y += 1;
			v2z += 1;
		} else if (i == 10) {
			v1x += 1;
			v1y += 1;
			v2x += 1;
			v2z += 1;
		} else if (i == 11) {
			v1x += 1;
			v2x += 1;
			v2y -= 1;
			v2z += 1;
		}
		double linePt1[3] = {v1x*(this->m_fCellLengthX) + (this->m_offsets[0]), v1y*(this->m_fCellLengthY) + (this->m_offsets[1]), v1z*(this->m_fCellLengthZ) + (this->m_offsets[2])};
		double linePt2[3] = {v2x*(this->m_fCellLengthX) + (this->m_offsets[0]), v2y*(this->m_fCellLengthY) + (this->m_offsets[1]), v2z*(this->m_fCellLengthZ) + (this->m_offsets[2])};

		vector<int> ignoreTri;		// stores the triangle to ignore; used for special cases such as edge intersection

		// check all the intersected triangles
		for (list<unsigned int>::iterator iter = group.begin(); iter != group.end(); iter++)
		{
			IntersectingTri &intTri = intersectedTriangle[*iter];

			// skip triangle if it should be ignored due to edge intersection of neighbouring triangle
			if (find(ignoreTri.begin(), ignoreTri.end(), intTri.intTriIdx) != ignoreTri.end())
				continue;

			// skip triangles that are completely inside cube
			if (intTri.insideCube)
				continue;

			// sanity checks
			if (intTri.intTriIdx >= numPolygons) {
				cout << "Error: TilingNonobtuseMC<T>::findEdgeIntersection() - Array Index Overflow for polygonsList" << endl;
				return;
			}

			double triPt1[3] = {verticesList[3*(polygonsList[intTri.intTriIdx][0])], verticesList[3*(polygonsList[intTri.intTriIdx][0])+1], verticesList[3*(polygonsList[intTri.intTriIdx][0])+2]};
			double triPt2[3] = {verticesList[3*(polygonsList[intTri.intTriIdx][1])], verticesList[3*(polygonsList[intTri.intTriIdx][1])+1], verticesList[3*(polygonsList[intTri.intTriIdx][1])+2]};
			double triPt3[3] = {verticesList[3*(polygonsList[intTri.intTriIdx][2])], verticesList[3*(polygonsList[intTri.intTriIdx][2])+1], verticesList[3*(polygonsList[intTri.intTriIdx][2])+2]};

			Intersection edgeIntPt;
			edgeIntPt.triIdx = intTri.intTriIdx;
			int intType = line_triangle_intersection(edgeIntPt.intPt, linePt1, linePt2, triPt1, triPt2, triPt3);
			if (intType == 0 || intType >= 4)	// no intersection or intersects in a coplanar manner
				continue;

			if (!segment_pt_intersection(edgeIntPt.intPt, linePt1, linePt2))	// no intersection
				continue;

			if (abs(intType) == 3)		// special case where ray hits triangle's edge
			{
				// check intersection for the neighbouring triangle
				unsigned int intEdge = 0;
				if (segment_pt_intersection(edgeIntPt.intPt, triPt2, triPt3))
					intEdge = 1;
				else if (segment_pt_intersection(edgeIntPt.intPt, triPt3, triPt1))
					intEdge = 2;

				// find neighbouring triangle
				bool nbrFound(false);
				unsigned int nbrTriIdx = 0;
				for (vector<int>::const_iterator nbrIter = polygonNeighbourList[intTri.intTriIdx].begin(); nbrIter != polygonNeighbourList[intTri.intTriIdx].end(); nbrIter++)
				{
					if (((polygonsList[*nbrIter][0] == polygonsList[intTri.intTriIdx][intEdge]) ||
						(polygonsList[*nbrIter][1] == polygonsList[intTri.intTriIdx][intEdge]) ||
						(polygonsList[*nbrIter][2] == polygonsList[intTri.intTriIdx][intEdge]))
						&&
						((polygonsList[*nbrIter][0] == polygonsList[intTri.intTriIdx][(intEdge+1)%3]) ||
						(polygonsList[*nbrIter][1] == polygonsList[intTri.intTriIdx][(intEdge+1)%3]) ||
						(polygonsList[*nbrIter][2] == polygonsList[intTri.intTriIdx][(intEdge+1)%3]))) 
					{
						nbrTriIdx = *nbrIter;
						nbrFound = true;
						break;
					}
				}

				if (nbrFound)	// check the type of edge intersection if it should be counted
				{
					double line[3] = {linePt2[0]-linePt1[0], linePt2[1]-linePt1[1], linePt2[2]-linePt1[2]};
					double normal[3] = {0,0,0};
					double nbrTriPt1[3] = {verticesList[3*(polygonsList[nbrTriIdx][0])],
										verticesList[3*(polygonsList[nbrTriIdx][0])+1],
										verticesList[3*(polygonsList[nbrTriIdx][0])+2]};
					double nbrTriPt2[3] = {verticesList[3*(polygonsList[nbrTriIdx][1])],
										verticesList[3*(polygonsList[nbrTriIdx][1])+1],
										verticesList[3*(polygonsList[nbrTriIdx][1])+2]};
					double nbrTriPt3[3] = {verticesList[3*(polygonsList[nbrTriIdx][2])],
										verticesList[3*(polygonsList[nbrTriIdx][2])+1],
										verticesList[3*(polygonsList[nbrTriIdx][2])+2]};

					computeFaceNormal(nbrTriPt1[0], nbrTriPt1[1], nbrTriPt1[2], nbrTriPt2[0], nbrTriPt2[1], nbrTriPt2[2], nbrTriPt3[0], nbrTriPt3[1], nbrTriPt3[2], normal);
					double dp = dotProduct(normal, line);
					
					if ((intType > 0) != (dp < 0))	// ray intersects edge but did not get in/out of the surface
					{
						// if nbr triangle has already been checked and added, remove it
						for (vector<Intersection>::iterator intIter = edgeInt[i].begin(); intIter != edgeInt[i].end(); intIter++) {
							if (intIter->triIdx == nbrTriIdx) {
								edgeInt[i].erase(intIter);
								break;
							}
						}

						// make sure we don't check the nbr triangle again for this edge
						ignoreTri.push_back(nbrTriIdx);
						
						// continue checking with next triangle
						continue;
					}
				}

				// no neighbour found means the current intersected triangle is at boundary, so we count the intersection as valid
			}

			edgeIntPt.isFacingPos = (intType < 0);	// ray hits triangle's backside

			// ---- insert intPt into array ----
			unsigned int sortIdx = 0;
			if (i == 0 || i == 2 || i == 4 || i == 6)
				sortIdx = 1;
			else if (i == 8 || i == 9 || i == 10 || i == 11)
				sortIdx = 2;
			
			bool isIntPtAdded(false);
			for (vector<Intersection>::iterator intIter = edgeInt[i].begin(); intIter != edgeInt[i].end(); intIter++) {
				if (intIter->intPt[sortIdx] >= edgeIntPt.intPt[sortIdx]) {
					edgeInt[i].insert(intIter, edgeIntPt);
					isIntPtAdded = true;
					break;
				} 
			}
			if (!isIntPtAdded)
				edgeInt[i].push_back(edgeIntPt);
		}
	}
}

// determine which cubes are intersected by which triangle in source mesh
//
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
template <class T> void TilingNonobtuseMC<T>::markIntersectedCubes(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons)
{
	// sanity checks
	if (verticesList == NULL ||
		numVertices == 0 ||
		polygonsList == NULL ||
		numPolygons == 0)
		return;

	if (cubesProperty != NULL)
		delete[] cubesProperty;

	// initialize cubes property list
	cubesProperty = new IntersectedCube[(this->m_nCellsX)*(this->m_nCellsY)*(this->m_nCellsZ)];

	cout << "Marking Intersected Cubes... " << endl;

	// go through all triangles and see which cubes it intersects
	for (int i = 0; i < numPolygons; ++i)
	{
		//cout << "Processing polygon " << i << endl;
		markIntersectedCubes(i, 
							verticesList[3*polygonsList[i][0]], 
							verticesList[3*polygonsList[i][0]+1], 
							verticesList[3*polygonsList[i][0]+2],
							verticesList[3*polygonsList[i][1]], 
							verticesList[3*polygonsList[i][1]+1], 
							verticesList[3*polygonsList[i][1]+2],
							verticesList[3*polygonsList[i][2]], 
							verticesList[3*polygonsList[i][2]+1], 
							verticesList[3*polygonsList[i][2]+2]);
	}
}

// determine which cubes are intersected by the given triangle
//
// triangleIdx (in): index of triangle
// pt1x (in): x-coord of vertex 1 of triangle
// pt1y (in): y-coord of vertex 1 of triangle
// pt1z (in): z-coord of vertex 1 of triangle
// pt2x (in): x-coord of vertex 2 of triangle
// pt2y (in): y-coord of vertex 2 of triangle
// pt2z (in): z-coord of vertex 2 of triangle
// pt3x (in): x-coord of vertex 3 of triangle
// pt3y (in): y-coord of vertex 3 of triangle
// pt3z (in): z-coord of vertex 3 of triangle
template <class T> void TilingNonobtuseMC<T>::markIntersectedCubes(int triangleIdx, double pt1x, double pt1y, double pt1z, double pt2x, double pt2y, double pt2z, double pt3x, double pt3y, double pt3z)
{
	// first find the coordinate to scan-slice
	double normal[3] = {0,0,0};
	computeFaceNormal(pt1x, pt1y, pt1z, pt2x, pt2y, pt2z, pt3x, pt3y, pt3z, normal);
	
	unsigned int largestProjCoord = 0;
	if (normal[largestProjCoord] < normal[1]) {
		largestProjCoord = 1;
	}
	if (normal[largestProjCoord] < normal[2]) {
		largestProjCoord = 2;
	}
	unsigned int scanSliceCoord = (largestProjCoord+1)%3;
	unsigned int scanLineCoord = (largestProjCoord+2)%3;

	double triPts[3][3] = {{pt1x, pt1y, pt1z},{pt2x, pt2y, pt2z},{pt3x, pt3y, pt3z}};	// for easy indexing

	// find the min and max
	double minSlice = triPts[0][scanSliceCoord];
	if (minSlice > triPts[1][scanSliceCoord])
		minSlice = triPts[1][scanSliceCoord];
	if (minSlice > triPts[2][scanSliceCoord])
		minSlice = triPts[2][scanSliceCoord];
	double maxSlice = triPts[0][scanSliceCoord];
	if (maxSlice < triPts[1][scanSliceCoord])
		maxSlice = triPts[1][scanSliceCoord];
	if (maxSlice < triPts[2][scanSliceCoord])
		maxSlice = triPts[2][scanSliceCoord];
	double minLine = triPts[0][scanLineCoord];
	if (minLine > triPts[1][scanLineCoord])
		minLine = triPts[1][scanLineCoord];
	if (minLine > triPts[2][scanLineCoord])
		minLine = triPts[2][scanLineCoord];
	double maxLine = triPts[0][scanLineCoord];
	if (maxLine < triPts[1][scanLineCoord])
		maxLine = triPts[1][scanLineCoord];
	if (maxLine < triPts[2][scanLineCoord])
		maxLine = triPts[2][scanLineCoord];
	double minProj = triPts[0][largestProjCoord];
	if (minProj > triPts[1][largestProjCoord])
		minProj = triPts[1][largestProjCoord];
	if (minProj > triPts[2][largestProjCoord])
		minProj = triPts[2][largestProjCoord];
	double maxProj = triPts[0][largestProjCoord];
	if (maxProj < triPts[1][largestProjCoord])
		maxProj = triPts[1][largestProjCoord];
	if (maxProj < triPts[2][largestProjCoord])
		maxProj = triPts[2][largestProjCoord];

	// make sure the slice coordinate has largest variation
	if (maxSlice - minSlice < maxLine - minLine)
	{
		swap(minSlice, minLine);
		swap(maxSlice, maxLine);
		swap(scanSliceCoord, scanLineCoord);
	}

	// sanity check
	/*
	if (normal[scanSliceCoord] == 1.0 || normal[scanSliceCoord] == -1.0)	// check if triangle is coplanar with scanSliceCoord
	{
		cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: triangle is coplanar with scanSliceCoord" << endl;
		return;
	}
	*/

	// check which coordinate is the x-coord
	// 0 - ProjCoord
	// 1 - ScanSliceCoord
	// 2 - ScanLineCoord
	unsigned int xCoord = 0;
	if (scanLineCoord == 0)
		xCoord = 2;
	else if (largestProjCoord != 0)
		xCoord = 1;

	unsigned int yCoord = 0;
	if (scanLineCoord == 1)
		yCoord = 2;
	else if (largestProjCoord != 1)
		yCoord = 1;

	unsigned int zCoord = 0;
	if (scanLineCoord == 2)
		zCoord = 2;
	else if (largestProjCoord != 2)
		zCoord = 1;

	// set CellLength and nCells in terms of Proj,Slice,Line coordinate
	double cellLength_PSLCoord[3] = {0,0,0};
	cellLength_PSLCoord[xCoord] = this->m_fCellLengthX;
	cellLength_PSLCoord[yCoord] = this->m_fCellLengthY;
	cellLength_PSLCoord[zCoord] = this->m_fCellLengthZ;
	unsigned int nCells_PSLCoord[3] = {0,0,0};
	nCells_PSLCoord[xCoord] = this->m_nCellsX;
	nCells_PSLCoord[yCoord] = this->m_nCellsY;
	nCells_PSLCoord[zCoord] = this->m_nCellsZ;

	// find the min cube and max cube to start testing each slice
	double cubeCoord = (minSlice - this->m_offsets[scanSliceCoord]) / cellLength_PSLCoord[1];
	int minSliceCube = (int)floor(cubeCoord);
	cubeCoord = (maxSlice - this->m_offsets[scanSliceCoord]) / cellLength_PSLCoord[1];
 	int maxSliceCube = (int)ceil(cubeCoord);

	// find the min cube and max cube along scan-line-coord and proj-coord that needs to be covered by the slices
	cubeCoord = (minLine - this->m_offsets[scanLineCoord]) / cellLength_PSLCoord[2];
 	int minLineCube = (int)floor(cubeCoord);
	cubeCoord = (maxLine - this->m_offsets[scanLineCoord]) / cellLength_PSLCoord[2];
 	int maxLineCube = (int)ceil(cubeCoord);

	cubeCoord = (minProj - this->m_offsets[largestProjCoord]) / cellLength_PSLCoord[0];
 	int minProjCube = (int)floor(cubeCoord);
	cubeCoord = (maxProj - this->m_offsets[largestProjCoord]) / cellLength_PSLCoord[0];
	int maxProjCube = (int)ceil(cubeCoord);

	if ((minSliceCube+1) == maxSliceCube &&		// triangle is completely within the cube
		(minProjCube+1) == maxProjCube &&
		(minLineCube+1) == maxLineCube)
	{
		// mark the cube
		unsigned int cubeIdx[3] = {minProjCube, minSliceCube, minLineCube};
		if (cubeIdx[xCoord] >= 0 && cubeIdx[xCoord] < this->m_nCellsX &&	// make sure the cube index is valid
			cubeIdx[yCoord] >= 0 && cubeIdx[yCoord] < this->m_nCellsY &&
			cubeIdx[zCoord] >= 0 && cubeIdx[zCoord] < this->m_nCellsZ)
		{	
			addIntersectingTriangle(triangleIdx, (this->m_nCellsY)*(this->m_nCellsX)*cubeIdx[zCoord] + (this->m_nCellsX)*cubeIdx[yCoord] + cubeIdx[xCoord], true);
		}
	}
	else
	{
		// check all slices in all x-y-z axis
		markIntersectedCubes(triangleIdx, triPts, minSliceCube, maxSliceCube, largestProjCoord, scanSliceCoord, scanLineCoord, cellLength_PSLCoord, nCells_PSLCoord);
		
		double updated_CellLen_PSL[3] = {cellLength_PSLCoord[1],cellLength_PSLCoord[2],cellLength_PSLCoord[0]};
		unsigned int updated_nCells_PSL[3] = {nCells_PSLCoord[1],nCells_PSLCoord[2],nCells_PSLCoord[0]};
		markIntersectedCubes(triangleIdx, triPts, minLineCube, maxLineCube, scanSliceCoord, scanLineCoord, largestProjCoord, updated_CellLen_PSL, updated_nCells_PSL);
		
		updated_CellLen_PSL[0] = cellLength_PSLCoord[2];
		updated_CellLen_PSL[1] = cellLength_PSLCoord[0];
		updated_CellLen_PSL[2] = cellLength_PSLCoord[1];
		updated_nCells_PSL[0] = nCells_PSLCoord[2];
		updated_nCells_PSL[1] = nCells_PSLCoord[0];
		updated_nCells_PSL[2] = nCells_PSLCoord[1];
		markIntersectedCubes(triangleIdx, triPts, minProjCube, maxProjCube, scanLineCoord, largestProjCoord, scanSliceCoord, updated_CellLen_PSL, updated_nCells_PSL);
	}
}

// determine which cubes between the minSliceCube and maxSliceCube that are intersected by the given triangle
//
// triangleIdx (in): index of triangle
// triPts (in): vertex positions of the triangle
// minSliceCube (in:) starting slice index
// maxSliceCube (in:) ending slice index
// largestProjCoord (in): determines which coordinate has the largest projection area
// scanSliceCoord (in): determines which coordinate scan slice takes place along
// scanLineCoord (in): determines which coordinate scan line takes place along
// cellLength_PSLCoord (in): CellLenth in terms of Proj,Slice,Line coordinate
// nCells_PSLCoord (in): number of Cells in terms of Proj,Slice,Line coordinate
template <class T> void TilingNonobtuseMC<T>::markIntersectedCubes(int triangleIdx, double triPts[3][3], int minSliceCube, int maxSliceCube, unsigned int largestProjCoord, unsigned int scanSliceCoord, unsigned int scanLineCoord, double cellLength_PSLCoord[3], unsigned int nCells_PSLCoord[3])
{
	for (int sliceIdx = minSliceCube; sliceIdx <= maxSliceCube && sliceIdx < (int)nCells_PSLCoord[1]; ++sliceIdx)
	{
		// make sure we skip all the cubes that are not valid
		if (sliceIdx < 0) {
			sliceIdx = -1;
			continue;
		}

		// check where the 3 vertices are located relative to the plane
		int vLocFlags[3] = {0,0,0};		// 0-pt is on plane; 1-pt is on front side; -1-pt is on back side
		for (unsigned int j = 0; j < 3; ++j)
		{
			double planeNormal[3] = {0,0,0};
			planeNormal[scanSliceCoord] = 1;
			double testVector[3] = {triPts[j][0],triPts[j][1],triPts[j][2]};
			testVector[scanSliceCoord] -= (sliceIdx*cellLength_PSLCoord[1] + this->m_offsets[scanSliceCoord]);
			double testResult = dotProduct(planeNormal, testVector);		// n dot (x - p0) = 0
			if (testResult < 0)
				vLocFlags[j] = -1;
			else if (testResult > 0)
				vLocFlags[j] = 1;
			else
				vLocFlags[j] = 0;
		}

		double pt1ScanLineCoord = 0;
		double pt1ProjCoord = 0;
		double pt2ScanLineCoord = 0;
		double pt2ProjCoord = 0;
		// check all the cases that may occur when a plane intersects a triangle
		if (vLocFlags[0] == 0 && vLocFlags[1] == 0 && vLocFlags[2] == 0)	// intersects pt1,pt2,pt3; Special Case, ignore it
			continue;
		else if ((vLocFlags[0] == 1 && vLocFlags[1] == 1 && vLocFlags[2] == 1) ||	// all 3 pts are on one side
			(vLocFlags[0] == -1 && vLocFlags[1] == -1 && vLocFlags[2] == -1))
			continue;	// no intersection
		else if (vLocFlags[0] == 0 && vLocFlags[1] != 0 && vLocFlags[2] != 0)	// intersects pt1 only
		{
			pt1ScanLineCoord = triPts[0][scanLineCoord];
			pt1ProjCoord = triPts[0][largestProjCoord];
			pt2ScanLineCoord = triPts[0][scanLineCoord];
			pt2ProjCoord = triPts[0][largestProjCoord];
		}
		else if (vLocFlags[0] != 0 && vLocFlags[1] == 0 && vLocFlags[2] != 0)	// intersects pt2 only
		{
			pt1ScanLineCoord = triPts[1][scanLineCoord];
			pt1ProjCoord = triPts[1][largestProjCoord];
			pt2ScanLineCoord = triPts[1][scanLineCoord];
			pt2ProjCoord = triPts[1][largestProjCoord];
		}
		else if (vLocFlags[0] != 0 && vLocFlags[1] != 0 && vLocFlags[2] == 0)	// intersects pt3 only
		{
			pt1ScanLineCoord = triPts[2][scanLineCoord];
			pt1ProjCoord = triPts[2][largestProjCoord];
			pt2ScanLineCoord = triPts[2][scanLineCoord];
			pt2ProjCoord = triPts[2][largestProjCoord];
		}
		else if (vLocFlags[0] == 0 && vLocFlags[1] == 0 && vLocFlags[2] != 0)	// intersects pt1 & pt2 only
		{
			pt1ScanLineCoord = triPts[0][scanLineCoord];
			pt1ProjCoord = triPts[0][largestProjCoord];
			pt2ScanLineCoord = triPts[1][scanLineCoord];
			pt2ProjCoord = triPts[1][largestProjCoord];
		}
		else if (vLocFlags[0] == 0 && vLocFlags[1] != 0 && vLocFlags[2] == 0)	// intersects pt1 & pt3 only
		{
			pt1ScanLineCoord = triPts[0][scanLineCoord];
			pt1ProjCoord = triPts[0][largestProjCoord];
			pt2ScanLineCoord = triPts[2][scanLineCoord];
			pt2ProjCoord = triPts[2][largestProjCoord];
		}
		else if (vLocFlags[0] != 0 && vLocFlags[1] == 0 && vLocFlags[2] == 0)	// intersects pt2 & pt3 only
		{
			pt1ScanLineCoord = triPts[1][scanLineCoord];
			pt1ProjCoord = triPts[1][largestProjCoord];
			pt2ScanLineCoord = triPts[2][scanLineCoord];
			pt2ProjCoord = triPts[2][largestProjCoord];
		}
		else if ((vLocFlags[0] == 1 && vLocFlags[1] == -1 && vLocFlags[2] == -1) ||	// pt1 is alone on one side
				(vLocFlags[0] == -1 && vLocFlags[1] == 1 && vLocFlags[2] == 1))
		{
			double intPt[2][3] = {{0,0,0},{0,0,0}};
			double planeNormal[3] = {0,0,0};
			planeNormal[scanSliceCoord] = 1;
			double ptOnPlane[3] = {0,0,0};
			ptOnPlane[scanSliceCoord] = (sliceIdx*cellLength_PSLCoord[1] + this->m_offsets[scanSliceCoord]);

			if (segment_plane_intersection(intPt[0], planeNormal, ptOnPlane, triPts[0], triPts[1]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt1,pt2) did not intersect plane at a point" << endl;
				return;
			}
			
			if (segment_plane_intersection(intPt[1], planeNormal, ptOnPlane, triPts[0], triPts[2]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt1,pt3) did not intersect plane at a point" << endl;
				return;
			}

			pt1ScanLineCoord = intPt[0][scanLineCoord];
			pt1ProjCoord = intPt[0][largestProjCoord];
			pt2ScanLineCoord = intPt[1][scanLineCoord];
			pt2ProjCoord = intPt[1][largestProjCoord];
		}
		else if ((vLocFlags[0] == -1 && vLocFlags[1] == 1 && vLocFlags[2] == -1) ||	// pt2 is alone on one side
				(vLocFlags[0] == 1 && vLocFlags[1] == -1 && vLocFlags[2] == 1))
		{
			double intPt[2][3] = {{0,0,0},{0,0,0}};
			double planeNormal[3] = {0,0,0};
			planeNormal[scanSliceCoord] = 1;
			double ptOnPlane[3] = {0,0,0};
			ptOnPlane[scanSliceCoord] = (sliceIdx*cellLength_PSLCoord[1] + this->m_offsets[scanSliceCoord]);

			if (segment_plane_intersection(intPt[0], planeNormal, ptOnPlane, triPts[1], triPts[0]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt2,pt1) did not intersect plane at a point" << endl;
				return;
			}
			
			if (segment_plane_intersection(intPt[1], planeNormal, ptOnPlane, triPts[1], triPts[2]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt2,pt3) did not intersect plane at a point" << endl;
				return;
			}

			pt1ScanLineCoord = intPt[0][scanLineCoord];
			pt1ProjCoord = intPt[0][largestProjCoord];
			pt2ScanLineCoord = intPt[1][scanLineCoord];
			pt2ProjCoord = intPt[1][largestProjCoord];
		}
		else if ((vLocFlags[0] == -1 && vLocFlags[1] == -1 && vLocFlags[2] == 1) ||	// pt3 is alone on one side
				(vLocFlags[0] == 1 && vLocFlags[1] == 1 && vLocFlags[2] == -1))
		{
			double intPt[2][3] = {{0,0,0},{0,0,0}};
			double planeNormal[3] = {0,0,0};
			planeNormal[scanSliceCoord] = 1;
			double ptOnPlane[3] = {0,0,0};
			ptOnPlane[scanSliceCoord] = (sliceIdx*cellLength_PSLCoord[1] + this->m_offsets[scanSliceCoord]);

			if (segment_plane_intersection(intPt[0], planeNormal, ptOnPlane, triPts[2], triPts[0]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt3,pt1) did not intersect plane at a point" << endl;
				return;
			}
			
			if (segment_plane_intersection(intPt[1], planeNormal, ptOnPlane, triPts[2], triPts[1]) != 1)
			{
				cout << "Error: TilingNonobtuseMC<T>::markIntersectedCubes: edge(pt3,pt2) did not intersect plane at a point" << endl;
				return;
			}

			pt1ScanLineCoord = intPt[0][scanLineCoord];
			pt1ProjCoord = intPt[0][largestProjCoord];
			pt2ScanLineCoord = intPt[1][scanLineCoord];
			pt2ProjCoord = intPt[1][largestProjCoord];
		}

		// mark all cubes by scan line
		markIntersectedCubes(triangleIdx, pt1ScanLineCoord, pt1ProjCoord, pt2ScanLineCoord, pt2ProjCoord, sliceIdx, largestProjCoord, scanLineCoord);
	}
}

// determine which cubes are intersected by the given line segment
//
// triangleIdx (in): index of triangle
// pt1ScanLineCoord (in): scan-line coord. of vertex 1 of line segment
// pt1ProjCoord (in): projection coord. of vertex 1 of line segment
// pt2ScanLineCoord (in): scan-line coord. of vertex 2 of line segment
// pt2ProjCoord (in): projection coord. of vertex 2 of line segment
// scanSliceCubeIdx (in): slice cube index; determines which slice the line is at
// projCoord (in): determines which coordinate has the largest projection area
// scanLineCoord (in): determines which coordinate scan long takes place along
template <class T> void TilingNonobtuseMC<T>::markIntersectedCubes(int triangleIdx, double pt1ScanLineCoord, double pt1ProjCoord, double pt2ScanLineCoord, double pt2ProjCoord, unsigned int scanSliceCubeIdx, unsigned int projCoord, unsigned int scanLineCoord)
{
	// make sure pt1 has min along scanLineCoord
	if (pt1ScanLineCoord > pt2ScanLineCoord)
	{
		swap(pt1ScanLineCoord, pt2ScanLineCoord);
		swap(pt1ProjCoord, pt2ProjCoord);
	}
	else if (pt1ScanLineCoord == pt2ScanLineCoord)
	{
		if (pt1ProjCoord > pt2ProjCoord)
			swap(pt1ProjCoord, pt2ProjCoord);
	}

	// check which coordinate is the x-coord
	// 0 - ProjCoord
	// 1 - ScanSliceCoord
	// 2 - ScanLineCoord
	unsigned int xCoord = 0;
	if (scanLineCoord == 0)
		xCoord = 2;
	else if (projCoord != 0)
		xCoord = 1;

	unsigned int yCoord = 0;
	if (scanLineCoord == 1)
		yCoord = 2;
	else if (projCoord != 1)
		yCoord = 1;

	unsigned int zCoord = 0;
	if (scanLineCoord == 2)
		zCoord = 2;
	else if (projCoord != 2)
		zCoord = 1;

	// set CellLength, nCells, offsets in terms of Proj,Slice,Line coordinate
	double cellLength_PSLCoord[3] = {0,0,0};
	cellLength_PSLCoord[xCoord] = this->m_fCellLengthX;
	cellLength_PSLCoord[yCoord] = this->m_fCellLengthY;
	cellLength_PSLCoord[zCoord] = this->m_fCellLengthZ;
	double offsets_PSLCoord[3] = {0,0,0};
	offsets_PSLCoord[xCoord] = this->m_offsets[0];
	offsets_PSLCoord[yCoord] = this->m_offsets[1];
	offsets_PSLCoord[zCoord] = this->m_offsets[2];

	// calculate slope of line pt1-pt2
	double slopeDenominator = pt2ScanLineCoord - pt1ScanLineCoord;
	double slopeNominator = pt2ProjCoord - pt1ProjCoord;
	double slope = 0;
	if (slopeDenominator != 0)
		slope = slopeNominator/slopeDenominator;

	bool pt1_lies_LineCoord(false);
	bool pt1_lies_ProjCoord(false);
	bool pt2_lies_LineCoord(false);
	bool pt2_lies_ProjCoord(false);
	bool pt1_closeTo_LineCoord(false);
	bool pt1_closeTo_FloorProjCoord(false);
	bool pt1_closeTo_CeilProjCoord(false);

	// -----------------------------------------------
	// -------- find which box pt1 resides in --------
	// -----------------------------------------------
	double cubeCoord = (pt1ScanLineCoord - this->m_offsets[scanLineCoord]) / cellLength_PSLCoord[2];
	int currentLineBox = (int)(floor(cubeCoord));		
	if (cubeCoord == ceil(cubeCoord))						// pt1 intersects at edge along the scan-line-coord
	{	
		pt1_lies_LineCoord = true;
		currentLineBox = (int)ceil(cubeCoord);			
	}
 	else if (equal(cubeCoord, floor(cubeCoord), 0.00001)) 	// pt1 is very close at edge along the scan-line-coord
	{
		currentLineBox = (int)(floor(cubeCoord)-1);			// make sure we examine more thoroughly to avoid roundoff errors
		pt1_closeTo_LineCoord = true;
	}

	cubeCoord = (pt1ProjCoord - this->m_offsets[projCoord]) / cellLength_PSLCoord[0];
	int currentProjBox = (int)floor(cubeCoord);
 	if (cubeCoord == ceil(cubeCoord))						// pt1 intersects at edge along the proj-coord
	{
		pt1_lies_ProjCoord = true;
		currentProjBox = (int)ceil(cubeCoord);		
	}
	else if (equal(cubeCoord, ceil(cubeCoord), 0.00001))	// pt1 is very close at edge along the proj-coord
	{
		currentProjBox = (int)ceil(cubeCoord);				// make sure we examine more thoroughly to avoid roundoff errors
		pt1_closeTo_CeilProjCoord = true;
	}
	else if (equal(cubeCoord, floor(cubeCoord), 0.00001))	// pt1 is very close at edge along the proj-coord
	{
		currentProjBox = (int)(floor(cubeCoord)-1);				// make sure we examine more thoroughly to avoid roundoff errors
		pt1_closeTo_FloorProjCoord = true;
	}

	if (pt1_lies_LineCoord && pt1_lies_ProjCoord)	// pt1 lies at grid corner
	{
		if (slope < 0)
			--currentProjBox;
	}
	else if (pt1_lies_ProjCoord)	// pt1 lies along the projCoord grid
	{
		if (slope < 0)
			--currentProjBox;
	}

	// -----------------------------------------------
	// -------- find which box pt2 resides in --------
	// -----------------------------------------------
	cubeCoord = (pt2ScanLineCoord - this->m_offsets[scanLineCoord]) / cellLength_PSLCoord[2];
	int lastLineBox = (int)floor(cubeCoord);
	if (cubeCoord == ceil(cubeCoord))						// pt2 intersects at edge along the scan-line-coord
	{
		pt2_lies_LineCoord = true;
		lastLineBox = (int)ceil(cubeCoord);					
	}
	else if (equal(cubeCoord, ceil(cubeCoord), 0.00001))	// pt2 is very close at edge along the scan-line-coord
		lastLineBox = (int)ceil(cubeCoord);					// make sure we examine more thoroughly to avoid roundoff errors

	cubeCoord = (pt2ProjCoord - this->m_offsets[projCoord]) / cellLength_PSLCoord[0];
	int lastProjBox = (int)floor(cubeCoord);
 	if (cubeCoord == ceil(cubeCoord))						// pt2 intersects at edge along the proj-coord
	{
		pt2_lies_ProjCoord = true;
		lastProjBox = (int)ceil(cubeCoord);					
	}
	else if (equal(cubeCoord, ceil(cubeCoord), 0.00001))	// pt2 is very close at edge along the proj-coord
		lastProjBox = (int)ceil(cubeCoord);					// make sure we examine more thoroughly to avoid roundoff errors
	else if (equal(cubeCoord, floor(cubeCoord), 0.00001))	// pt2 is very close at edge along the proj-coord
		lastProjBox = (int)(floor(cubeCoord)-1);					// make sure we examine more thoroughly to avoid roundoff errors

	if (pt2_lies_LineCoord && pt2_lies_ProjCoord)	// pt2 lies at grid corner
	{
		if (slopeDenominator == 0)
			--lastProjBox;
		else if (slope < 0)
			--lastLineBox;
		else if (slope > 0)
		{
			--lastLineBox;
			--lastProjBox;
		}
		else 
			--lastLineBox;
	}
	else if (pt2_lies_ProjCoord)	// pt2 lies along the projCoord grid
	{
		if (slopeDenominator == 0)
			--lastProjBox;
		else if (slope > 0)
			--lastProjBox;
	}
	else if (pt2_lies_LineCoord)	// pt2 lies along the lineCoord grid
	{
		if (slopeDenominator != 0)
			--lastLineBox;
	}


	// -------------------------------------------------------
	// -------- traverse boxes along the line pt1-pt2 --------
	// -------------------------------------------------------
	bool addLastCube(true);
	int cubeIdx[3] = {currentProjBox, scanSliceCubeIdx, currentLineBox};
	while ((currentLineBox <= lastLineBox) && 
			((slopeDenominator == 0 && currentProjBox <= lastProjBox) ||
			(slope >= 0 && currentProjBox <= lastProjBox) ||
			(slope < 0 && currentProjBox >= lastProjBox)))
	{
		// mark current cube as intersected
		cubeIdx[0] = currentProjBox;
		cubeIdx[1] = scanSliceCubeIdx;
		cubeIdx[2] = currentLineBox;
		for (unsigned int i = 0; i < 2; ++i)
		{
			if (cubeIdx[xCoord] >= 0 && cubeIdx[xCoord] < (int)this->m_nCellsX &&	// make sure the cube index is valid
				cubeIdx[yCoord] >= 0 && cubeIdx[yCoord] < (int)this->m_nCellsY &&
				cubeIdx[zCoord] >= 0 && cubeIdx[zCoord] < (int)this->m_nCellsZ)
			{	
				addIntersectingTriangle(triangleIdx, (this->m_nCellsY)*(this->m_nCellsX)*cubeIdx[zCoord] + (this->m_nCellsX)*cubeIdx[yCoord] + cubeIdx[xCoord], false);
			}
			cubeIdx[1] = scanSliceCubeIdx - 1;	// make sure we add the cubes for previous slice in case last slice missed any cubes
		}

		// find which side of the box intersects
		if (slopeDenominator == 0)	// line is vertical; slope infinite
			++currentProjBox;
		else if (slope == 0)		// line is horizontal; slope 0
			++currentLineBox;
		else if (slope > 0)			// line is going upward; slope > 0
		{
			double seg1[3] = {currentLineBox*cellLength_PSLCoord[2] + offsets_PSLCoord[2],
								(currentProjBox+1)*cellLength_PSLCoord[0] + offsets_PSLCoord[0],
								0};
			double seg2[3] = {(currentLineBox+1)*cellLength_PSLCoord[2] + offsets_PSLCoord[2], seg1[1], seg1[2]};
			double intPt[3] = {0,0,0};
			double pt1[3] = {pt1ScanLineCoord, pt1ProjCoord, 0};
			double pt2[3] = {pt2ScanLineCoord, pt2ProjCoord, 0};

			// test intersecting edge with top edge
			int res1 = segment_line_intersection(intPt, pt1, pt2, seg1, seg2);

			seg1[0] = seg2[0];
			seg1[1] = currentProjBox*cellLength_PSLCoord[0] + offsets_PSLCoord[0];
			// test intersecting edge with right edge
			int res2 = segment_line_intersection(intPt, pt1, pt2, seg1, seg2);

			if (res1 == 1 && res2 == 1)		// intersected top-right corner
			{
				++currentProjBox;
				++currentLineBox;
			}
			else if (res1 == 1 && res2 == 0)	// intersected top edge
			{
				++currentProjBox;
			}
			else if (res1 == 0 && res2 == 1)	// intersected right edge
			{
				++currentLineBox;
			}
			else if (res1 == 0 && res2 == 0)	// line stops at current cube
			{
				if (pt1_closeTo_LineCoord)				// extra check for roundoff error fail, so increment by one along lineCoord
					++currentLineBox;
				else if (pt1_closeTo_FloorProjCoord)	// extra check for roundoff error fail, so increment by one along projCoord
					++currentProjBox;
				else if (pt1_closeTo_CeilProjCoord)		// extra check for roundoff error fail, so decrement by one along projCoord
					--currentProjBox;
				else
				{
					addLastCube = false;
					break;
				}
			}
			else							// special cases?! THIS SHOULDN'T HAPPEN
			{
				// ignore it
			}
		}
		else						// line is going downward; slope < 0
		{
			double seg1[3] = {currentLineBox*cellLength_PSLCoord[2] + offsets_PSLCoord[2],
								currentProjBox*cellLength_PSLCoord[0] + offsets_PSLCoord[0],
								0};
			double seg2[3] = {(currentLineBox+1)*cellLength_PSLCoord[2] + offsets_PSLCoord[2], seg1[1], seg1[2]};
			double intPt[3] = {0,0,0};
			double pt1[3] = {pt1ScanLineCoord, pt1ProjCoord, 0};
			double pt2[3] = {pt2ScanLineCoord, pt2ProjCoord, 0};

			// test intersecting edge with bottom edge
			int res1 = segment_line_intersection(intPt, pt1, pt2, seg1, seg2);

			seg1[0] = seg2[0];
			seg1[1] = (currentProjBox+1)*cellLength_PSLCoord[0] + offsets_PSLCoord[0];
			// test intersecting edge with right edge
			int res2 = segment_line_intersection(intPt, pt1, pt2, seg1, seg2);

			if (res1 == 1 && res2 == 1)		// intersected bottom-right corner
			{
				--currentProjBox;
				++currentLineBox;
			}
			else if (res1 == 1 && res2 == 0)	// intersected bottom edge
			{
				--currentProjBox;
			}
			else if (res1 == 0 && res2 == 1)	// intersected right edge
			{
				++currentLineBox;
			}
			else if (res1 == 0 && res2 == 0)	// line stops at current cube
			{
				if (pt1_closeTo_LineCoord)				// extra check for roundoff error fail, so increment by one along lineCoord
					++currentLineBox;
				else if (pt1_closeTo_FloorProjCoord)	// extra check for roundoff error fail, so increment by one along projCoord
					++currentProjBox;
				else if (pt1_closeTo_CeilProjCoord)		// extra check for roundoff error fail, so decrement by one along projCoord
					--currentProjBox;
				else
				{
					addLastCube = false;
					break;
				}
			}
			else							// special cases?! THIS SHOULDN'T HAPPEN
			{
				// ignore it
			}
		}		
	}
}

// add triangle 'triIdx' to be intersecting cube 'cubeIdx'
//
// triIdx (in): index of intersecting triangle
// cubeIdx (in): cube index
// insideCube(in): true if triangle is completely inside the cube
template <class T> void TilingNonobtuseMC<T>::addIntersectingTriangle(unsigned int triangleIdx, unsigned int cubeIdx, bool insideCube)
{
	vector<IntersectingTri> &intTriList = cubesProperty[cubeIdx].intersectedTriangle;

	IntersectingTri intTri;
	intTri.intTriIdx = triangleIdx;
	intTri.insideCube = insideCube;

	if (find(intTriList.begin(), intTriList.end(), intTri) == intTriList.end())
		intTriList.push_back(intTri);
}

// sort the intersected triangles of cubeIdx into connected components
//
// cubeIdx (in): index of cube to be sorted
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::sortIntersectingTriangle(unsigned int cubeIdx, const vector<int>* polygonNeighbourList)
{
	if (cubesProperty[cubeIdx].isSorted)
		return;
	
	unsigned int numIntTri = cubesProperty[cubeIdx].intersectedTriangle.size();
	if (numIntTri == 0)
		return;

	vector<unsigned int> needSortedList;
	for (unsigned int i = 0; i < numIntTri; ++i)
		needSortedList.push_back(i);

	vector<list<unsigned int> > &triGroups = cubesProperty[cubeIdx].triGroups;
	// continue to add new group when there are still unsorted triangles
	while (!needSortedList.empty())
	{
		triGroups.resize(triGroups.size()+1);
		expandGroup(cubeIdx, polygonNeighbourList, triGroups.size()-1, 0, needSortedList);
	}

	cubesProperty[cubeIdx].isSorted = true;
	
	for (unsigned int i = 0; i < triGroups.size(); ++i) {
		cubesProperty[cubeIdx].isSignSet.push_back(false);
		vector<int> unsetSigns;
		for (unsigned int j = 0; j < 8; ++j)
			unsetSigns.push_back(0);
		cubesProperty[cubeIdx].signs.push_back(unsetSigns);
	}
}

// expand a connected component group by adding triIdx into groupIdx and expand it
//
// cubeIdx (in): index of cube to be sorted
// polygonNeighbourList (in): polygon neighbour information for source mesh
// groupIdx (in): index of group to expand
// triIdx (in): index of intersecting triangle in the needSortedList array to add to the group and expand
// needSortedList (in/out): index of intersecting triangles in intersectedTriangle need to be sorted
template <class T> void TilingNonobtuseMC<T>::expandGroup(unsigned int cubeIdx, const vector<int>* polygonNeighbourList, unsigned int groupIdx, unsigned int triIdx, vector<unsigned int> &needSortedList)
{
	vector<IntersectingTri> &intTriList = cubesProperty[cubeIdx].intersectedTriangle;
	vector<list<unsigned int> > &triGroups = cubesProperty[cubeIdx].triGroups;

	unsigned int triangleAdded = intTriList[needSortedList[triIdx]].intTriIdx;

	// if triIdx is in group, then no need to check
	if (find(triGroups[groupIdx].begin(), triGroups[groupIdx].end(), needSortedList[triIdx]) != triGroups[groupIdx].end())
		return;
	
	// add triIdx to group first if not in group already
	triGroups[groupIdx].push_back(needSortedList[triIdx]);
	intTriList[needSortedList[triIdx]].groupIdx = groupIdx;
		
	// remove triIdx from needSortedList
	needSortedList.erase(needSortedList.begin() + triIdx);

	// expand group by examining the neighbour of triangle just added
	for (vector<int>::const_iterator iter = polygonNeighbourList[triangleAdded].begin(); iter != polygonNeighbourList[triangleAdded].end(); iter++)
	{
		for (unsigned int i = 0; i < needSortedList.size(); ++i) {
			if (intTriList[needSortedList[i]].intTriIdx == *iter) {
				expandGroup(cubeIdx, polygonNeighbourList, groupIdx, i, needSortedList);
				break;
			}
		}
	}
}

// compute the intersecting point on the specified edge of the cube
//
// nX (in): x-coord of cube
// nY (in): y-coord of cube
// nZ (in): z-coord of cube
// nEdgeNo (in): edge number the vertex is to be put on
template <class T> POINT3DID TilingNonobtuseMC<T>::computeVertexLocation(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo)
{
	float x1, y1, z1, x2, y2, z2;
	unsigned int v1x = nX, v1y = nY, v1z = nZ;
	unsigned int v2x = nX, v2y = nY, v2z = nZ;
	
	switch (nEdgeNo)
	{
	case 0:
		v2y += 1;
		break;
	case 1:
		v1y += 1;
		v2x += 1;
		v2y += 1;
		break;
	case 2:
		v1x += 1;
		v1y += 1;
		v2x += 1;
		break;
	case 3:
		v1x += 1;
		break;
	case 4:
		v1z += 1;
		v2y += 1;
		v2z += 1;
		break;
	case 5:
		v1y += 1;
		v1z += 1;
		v2x += 1;
		v2y += 1;
		v2z += 1;
		break;
	case 6:
		v1x += 1;
		v1y += 1;
		v1z += 1;
		v2x += 1;
		v2z += 1;
		break;
	case 7:
		v1x += 1;
		v1z += 1;
		v2z += 1;
		break;
	case 8:
		v2z += 1;
		break;
	case 9:
		v1y += 1;
		v2y += 1;
		v2z += 1;
		break;
	case 10:
		v1x += 1;
		v1y += 1;
		v2x += 1;
		v2y += 1;
		v2z += 1;
		break;
	case 11:
		v1x += 1;
		v2x += 1;
		v2z += 1;
		break;
	}

	x1 = v1x*(this->m_fCellLengthX) + (this->m_offsets[0]);
	y1 = v1y*(this->m_fCellLengthY) + (this->m_offsets[1]);
	z1 = v1z*(this->m_fCellLengthZ) + (this->m_offsets[2]);
	x2 = v2x*(this->m_fCellLengthX) + (this->m_offsets[0]);
	y2 = v2y*(this->m_fCellLengthY) + (this->m_offsets[1]);
	z2 = v2z*(this->m_fCellLengthZ) + (this->m_offsets[2]);

	POINT3DID intersection = this->Interpolate(x1, y1, z1, x2, y2, z2, -1, 1);
	
	return intersection;
}

// find the associated triangle index to use for the new vertex at edgeNo for patchNo of cube(x,y,z)
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// patchNo (in): patch number 
// edgeNo (in): edge number the vertex is to be put on
// edgeInt (in): stores the edge intersection for all 12 edges of cube(x,y,z); sorted from smallest to largest depending on which edge
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// polygonNeighbourList (in): polygon neighbour information for source mesh
// return value (out): index of associated triangle
template <class T> int TilingNonobtuseMC<T>::getAssociatedTriIdx(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, unsigned int edgeNo, const vector<Intersection> (&edgeInt)[12], const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList)
{
	// setting up quick indexing of adjacent cube
	int adjacentCube[12][3][3] = {
									{{x,y,z-1}, {x-1,y,z-1}, {x-1,y,z}},			// edge 0
									{{x,y,z-1}, {x,y+1,z-1}, {x,y+1,z}},			// edge 1
									{{x,y,z-1}, {x+1,y,z-1}, {x+1,y,z}},			// edge 2
									{{x,y,z-1}, {x,y-1,z-1}, {x,y-1,z}},			// edge 3
									{{x,y,z+1}, {x-1,y,z+1}, {x-1,y,z}},			// edge 4
									{{x,y,z+1}, {x,y+1,z+1}, {x,y+1,z}},			// edge 5
									{{x,y,z+1}, {x+1,y,z+1}, {x+1,y,z}},			// edge 6
									{{x,y,z+1}, {x,y-1,z+1}, {x,y-1,z}},			// edge 7
									{{x-1,y,z}, {x-1,y-1,z}, {x,y-1,z}},			// edge 8
									{{x-1,y,z}, {x-1,y+1,z}, {x,y+1,z}},			// edge 9
									{{x+1,y,z}, {x+1,y+1,z}, {x,y+1,z}},			// edge 10
									{{x+1,y,z}, {x+1,y-1,z}, {x,y-1,z}}				// edge 11
								};

	for (unsigned int i = 0; i < edgeInt[edgeNo].size(); ++i)
	{
		int assoTriIdx = edgeInt[edgeNo][i].triIdx;
		// check if current intersection triangle is consistent with the neighbour cubes
		for (unsigned int j = 0; j < 3; ++j)	// check all 3 neighbour cubes
		{
			// sanity checks
			if (adjacentCube[edgeNo][j][0] < 0 || adjacentCube[edgeNo][j][0] >= (int)this->m_nCellsX ||
				adjacentCube[edgeNo][j][1] < 0 || adjacentCube[edgeNo][j][1] >= (int)this->m_nCellsY ||
				adjacentCube[edgeNo][j][2] < 0 || adjacentCube[edgeNo][j][2] >= (int)this->m_nCellsZ)
				continue;

			unsigned int nbrCubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*adjacentCube[edgeNo][j][2] + (this->m_nCellsX)*adjacentCube[edgeNo][j][1] + adjacentCube[edgeNo][j][0];
			sortIntersectingTriangle(nbrCubeIdx, polygonNeighbourList);	// make sure we sort the trianlges into patches if not already sorted
			
			IntersectingTri intTri;
			intTri.intTriIdx = edgeInt[edgeNo][i].triIdx;
			vector<IntersectingTri>::iterator iter = find(cubesProperty[nbrCubeIdx].intersectedTriangle.begin(), cubesProperty[nbrCubeIdx].intersectedTriangle.end(), intTri);
			if (iter == cubesProperty[nbrCubeIdx].intersectedTriangle.end())	// can't find current intersection triangle in neighbour cube
			{
				assoTriIdx = -1;
				break;
			}

			if (!willTriangulate(adjacentCube[edgeNo][j][0], adjacentCube[edgeNo][j][1], adjacentCube[edgeNo][j][2], iter->groupIdx, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList))		// neighbour patch doesn't introduce any new triangles
			{
				assoTriIdx = -1;
				break;
			}
		}

		if (assoTriIdx != -1)
			return assoTriIdx;
	}

	if (edgeInt[edgeNo].empty())
		return -1;

	return edgeInt[edgeNo][0].triIdx;
}

// unused vertices are moved to end of array;
template <class T> void TilingNonobtuseMC<T>::RenameVerticesAndTriangles()
{
	unsigned int nextID = 0;
	ID2POINT3DID::iterator mapIterator = this->m_i2pt3idVertices.begin();

	// Rename vertices.
	this->m_nVertices = this->m_i2pt3idVertices.size();
	this->m_ppt3dVertices = new POINT3D[this->m_nVertices];
	assoTriList = new int[this->m_nVertices];
	while (mapIterator != this->m_i2pt3idVertices.end()) {
		this->m_ppt3dVertices[nextID][0] = (*mapIterator).second.x;
		this->m_ppt3dVertices[nextID][1] = (*mapIterator).second.y;
		this->m_ppt3dVertices[nextID][2] = (*mapIterator).second.z;

		(*mapIterator).second.newID = nextID;

		assoTriList[(*mapIterator).first] = -1;
		if (!vertexInfo[(*mapIterator).first].intTriIdx.empty())
			assoTriList[(*mapIterator).first] = vertexInfo[(*mapIterator).first].intTriIdx[0];
		
		if (assoTriList[(*mapIterator).first] == -1)	// no intersected triangles, then use an arbitrary one from the patch
		{
			unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*vertexInfo[(*mapIterator).first].z + (this->m_nCellsX)*vertexInfo[(*mapIterator).first].y + vertexInfo[(*mapIterator).first].x;
			
			vector<IntersectingTri> &intersectedTriangle = cubesProperty[cubeIdx].intersectedTriangle;
			list<unsigned int> &group = cubesProperty[cubeIdx].triGroups[vertexInfo[(*mapIterator).first].patchNo];
			
			assoTriList[(*mapIterator).first] = intersectedTriangle[group.front()].intTriIdx;
		}
			
		nextID++;
		mapIterator++;
	}

	// Now rename triangles.
	this->m_nTriangles = this->m_trivecTriangles.size();
	this->m_piTriangleIndices = new unsigned int[(this->m_nTriangles)*3]; 
	for (unsigned int i = 0; i < this->m_nTriangles; ++i)
	{
		this->m_piTriangleIndices[3*i] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[0]].newID;
		this->m_piTriangleIndices[3*i+1] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[1]].newID;
		this->m_piTriangleIndices[3*i+2] = this->m_i2pt3idVertices[this->m_trivecTriangles[i].pointID[2]].newID;
	}

	this->m_i2pt3idVertices.clear();
	this->m_trivecTriangles.clear();
}

// returns the list of associated triangle for each vertex; -1 if no associated triangle
//
// assoTriList (out): array of associated triangles to return
// return value (out): number of slots in array
template <class T> const int* TilingNonobtuseMC<T>::getAssociatedTri() const
{
	return assoTriList;
}

// returns true if the nonmanifold vertices case is a special one that needs 2 triangles to triangulate
//
// commonEdge (in): the edge index where the non-manifold vertex lies on
// edge1 (in): the (open) edge index where a neighbour of the non-manifold vertex lies on; index relative to commonEdge
// edge2 (in): the (open) edge index where a neighbour of the non-manifold vertex lies on; index relative to diagonal cube
template <class T> bool TilingNonobtuseMC<T>::isSpecialNonmanifoldCase(unsigned int commonEdge, unsigned int edge1, unsigned int edge2) const
{
	if ((commonEdge == 0 && edge1 == 8 && edge2 == 5) ||
		(commonEdge == 0 && edge1 == 9 && edge2 == 7) ||
		(commonEdge == 0 && edge1 == 1 && edge2 == 11) ||
		(commonEdge == 0 && edge1 == 3 && edge2 == 10) ||
		(commonEdge == 1 && edge1 == 0 && edge2 == 11) ||
		(commonEdge == 1 && edge1 == 2 && edge2 == 8) ||
		(commonEdge == 1 && edge1 == 9 && edge2 == 6) ||
		(commonEdge == 1 && edge1 == 10 && edge2 == 4) ||
		(commonEdge == 2 && edge1 == 11 && edge2 == 5) ||
		(commonEdge == 2 && edge1 == 10 && edge2 == 7) ||
		(commonEdge == 2 && edge1 == 1 && edge2 == 8) ||
		(commonEdge == 2 && edge1 == 3 && edge2 == 9) ||
		(commonEdge == 3 && edge1 == 0 && edge2 == 10) ||
		(commonEdge == 3 && edge1 == 2 && edge2 == 9) ||
		(commonEdge == 3 && edge1 == 8 && edge2 == 6) ||
		(commonEdge == 3 && edge1 == 11 && edge2 == 4) ||
		(commonEdge == 4 && edge1 == 5 && edge2 == 11) ||
		(commonEdge == 4 && edge1 == 7 && edge2 == 10) ||
		(commonEdge == 4 && edge1 == 8 && edge2 == 1) ||
		(commonEdge == 4 && edge1 == 9 && edge2 == 3) ||
		(commonEdge == 5 && edge1 == 9 && edge2 == 2) ||
		(commonEdge == 5 && edge1 == 10 && edge2 == 0) ||
		(commonEdge == 5 && edge1 == 4 && edge2 == 11) ||
		(commonEdge == 5 && edge1 == 6 && edge2 == 8) ||
		(commonEdge == 6 && edge1 == 5 && edge2 == 8) ||
		(commonEdge == 6 && edge1 == 7 && edge2 == 9) ||
		(commonEdge == 6 && edge1 == 11 && edge2 == 1) ||
		(commonEdge == 6 && edge1 == 10 && edge2 == 3) ||
		(commonEdge == 7 && edge1 == 11 && edge2 == 0) ||
		(commonEdge == 7 && edge1 == 8 && edge2 == 2) ||
		(commonEdge == 7 && edge1 == 6 && edge2 == 9) ||
		(commonEdge == 7 && edge1 == 4 && edge2 == 10) ||
		(commonEdge == 8 && edge1 == 0 && edge2 == 5) ||
		(commonEdge == 8 && edge1 == 4 && edge2 == 1) ||
		(commonEdge == 8 && edge1 == 3 && edge2 == 6) ||
		(commonEdge == 8 && edge1 == 7 && edge2 == 2) ||
		(commonEdge == 9 && edge1 == 1 && edge2 == 6) ||
		(commonEdge == 9 && edge1 == 5 && edge2 == 2) ||
		(commonEdge == 9 && edge1 == 0 && edge2 == 7) ||
		(commonEdge == 9 && edge1 == 4 && edge2 == 3) ||
		(commonEdge == 10 && edge1 == 6 && edge2 == 3) ||
		(commonEdge == 10 && edge1 == 2 && edge2 == 7) ||
		(commonEdge == 10 && edge1 == 5 && edge2 == 0) ||
		(commonEdge == 10 && edge1 == 1 && edge2 == 4) ||
		(commonEdge == 11 && edge1 == 7 && edge2 == 0) ||
		(commonEdge == 11 && edge1 == 3 && edge2 == 4) ||
		(commonEdge == 11 && edge1 == 6 && edge2 == 1) ||
		(commonEdge == 11 && edge1 == 2 && edge2 == 5))
		return true;

	return false;
}

// modified from base class
// edge 12: (b,e) in special case 5b
// edge 13: (b,e) in special case 9b
// edge 14: (b,f) in special case 11b ***OBSOLETE***
// edge 14: (a,d) in special case 11c
// edge 15: (b,e) in special case 12b
// edge 16: (b,f) in special case 14b ***OBSOLETE***
// edge 16: (a,d) in special case 14c
// edge 17: (b,e) in special case -7
// edge 18: (a,d) in special case -3
// edge 19: center of cube in special case -6
template <class T> unsigned int TilingNonobtuseMC<T>::GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo)
{
	switch (nEdgeNo) {
	case 0:
		return this->GetVertexID(nX, nY, nZ) + 1;
	case 1:
		return this->GetVertexID(nX, nY + 1, nZ);
	case 2:
		return this->GetVertexID(nX + 1, nY, nZ) + 1;
	case 3:
		return this->GetVertexID(nX, nY, nZ);
	case 4:
		return this->GetVertexID(nX, nY, nZ + 1) + 1;
	case 5:
		return this->GetVertexID(nX, nY + 1, nZ + 1);
	case 6:
		return this->GetVertexID(nX + 1, nY, nZ + 1) + 1;
	case 7:
		return this->GetVertexID(nX, nY, nZ + 1);
	case 8:
		return this->GetVertexID(nX, nY, nZ) + 2;
	case 9:
		return this->GetVertexID(nX, nY + 1, nZ) + 2;
	case 10:
		return this->GetVertexID(nX + 1, nY + 1, nZ) + 2;
	case 11:
		return this->GetVertexID(nX + 1, nY, nZ) + 2;
	case 12:
		return this->GetVertexID(nX, nY, nZ) + 3;
	case 13:
		return this->GetVertexID(nX, nY, nZ) + 4;
	case 14:                           
		return this->GetVertexID(nX, nY, nZ) + 5;
	case 15:
		return this->GetVertexID(nX, nY, nZ) + 6;
	case 16:
		return this->GetVertexID(nX, nY, nZ) + 7;
	case 17:
		return this->GetVertexID(nX, nY, nZ) + 8;
	case 18:
		return this->GetVertexID(nX, nY, nZ) + 9;
	case 19:
		return this->GetVertexID(nX, nY, nZ) + 10;
	default:
		// Invalid edge no.
		return -1;
	}
}


template class TilingNonobtuseMC<short>;
template class TilingNonobtuseMC<unsigned short>;
template class TilingNonobtuseMC<float>;
