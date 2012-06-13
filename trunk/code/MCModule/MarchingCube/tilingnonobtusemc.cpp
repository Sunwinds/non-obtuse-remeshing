/***************************************************************************
 *   Copyright (C) 2006 by John Li                                         *
 *   ysl@cs.sfu.ca                                                         *
 ***************************************************************************/
#include "tilingnonobtusemc.h"

bool operator==(IntersectTri a, IntersectTri b)
{
	return (a.triIdx == b.triIdx);
}

bool operator==(EdgePosInfo a, EdgePosInfo b)
{
	return (a.edgeIdx == b.edgeIdx);
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
	// -6:	Special Case -6
	// {-6, a, b, c, d, e, f, g, -1, ... }
	// where a,b,c,d,e,f,g forms a seven-agon
	// The right hand ordering determines the normals
	// A new point m should be added to the center of cube
	// Then, the 7 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, g, m, g, a, m, -1}
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
	{-12, 1, 3, 8, 4, 5, 6, 7, 11, -1, -1, -1, -1, -1, -1, -1},	// 1010 0011; v 0.1.5.7		(case 12b)
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
:CNonobtuseMC<T>::CNonobtuseMC(), cubesProperty(NULL), nextUnusedVertexID(0), nextUnusedEdgeID(0), closestTriList(NULL),
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

	nextUnusedVertexID = 0;
	nextUnusedEdgeID = 0;

	v_on_e.clear();
	e_on_f.clear();
	v_to_eOfCube.clear();
	e_to_face.clear();

	edgeList.clear();
	vpair_to_e.clear();

	v_to_e.clear();
	v_to_tri.clear();
	e_to_tri.clear();
	tri_to_patch.clear();
	tri_to_CubeID.clear();

	if (closestTriList != NULL)
		delete[] closestTriList;
	closestTriList = NULL;

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
template <class T> clock_t TilingNonobtuseMC<T>::GenerateSurface(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* polygonNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ)
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

	clock_t b4Time = clock();
	clock_t afterTime = clock();

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

	// make sure we have room to put out data in v_on_e and e_on_f
	// 10 edges per cell: 0,3,8,12,13,14,15,16,17,18,19
	v_on_e.resize(11*(nCellsX+1)*(nCellsY+1)*(nCellsZ+1));	
	e_on_f.resize(3*(nCellsX+1)*(nCellsY+1)*(nCellsZ+1));	// faces are index by cubeIdx + {0,1,2}

 	cout << "Start triangulating ... " << endl;
 
	// 	triangulate each cube
	for (unsigned int z = 0; z < nCellsZ; ++z) {
		for (unsigned int y = 0; y < nCellsY; ++y) {
			for (unsigned int x = 0; x < nCellsX; ++x) {
				//cout << "Triangulating cube(" << x << ", " << y << ", " << z << ")" << endl;
				triangulate(x,y,z, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
			}
		}
	}

	cout << "Start stitching ... " << endl;

	// post process stitching
	for (unsigned int z = 0; z <= nCellsZ; ++z) {
		for (unsigned int y = 0; y <= nCellsY; ++y) {
			for (unsigned int x = 0; x <= nCellsX; ++x) {
//    				cout << "Stitching cube(" << x << ", " << y << ", " << z << ")" << endl;
				checkStitching(x, y, z, polygonNeighbourList);
			}
		}
	}

	// rename the vertex ids
	RenameVerticesAndTriangles(verticesList, numVertices, polygonsList, numPolygons);
	this->m_bValidSurface = true;

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total processed time: " << totalTime << " seconds." << endl;
	cout << "Total number of cubes with multiple patches: " << numCubeMultiPatch << endl;
	cout << "Total number of patches ignored: " << numPatchIgnored << endl;
	cout << "Total number of faces needed for stitching: " << numFaceStitching << endl;
	cout << "Total number of non-manifold fix: " << numNonManifoldFix << endl;

	return totalTime;
}

// Generates the isosurface from the source mesh (using simple MC)
// surfaceType = MIDPOINT
//
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
// tIsoLevel (in): isolevel
// nCellsX (in): number of cells in x-dim
// nCellsY (in): number of cells in y-dim
// nCellsZ (in): number of cells in z-dim
// fCellLengthX (in): cell length in x-dim
// fCellLengthY (in): cell length in y-dim
// fCellLengthZ (in): cell length in z-dim
// return value (out): time taken
template <class T> clock_t TilingNonobtuseMC<T>::GenerateSurface_simpleMC(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList, T tIsoLevel, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float fCellLengthX, float fCellLengthY, float fCellLengthZ)
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

	clock_t b4Time = clock();
	clock_t afterTime = clock();

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

	// ------- STAGE 2: triangulate using simple MC -------
	triangulate_simpleMC(verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList);

	afterTime = clock();
	clock_t totalTime = (afterTime - b4Time) / CLOCKS_PER_SEC;
	cout << "Total processed time: " << totalTime << " seconds." << endl;

	return totalTime;
}

// triangulate the intersected cubes using simple MC
//
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::triangulate_simpleMC(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList)
{
	unsigned int surfaceType = MIDPOINT;
// 	this->allTableIndex = new unsigned int[this->m_nCellsX * this->m_nCellsY * this->m_nCellsZ];
	
	// stores scalarfield for each grid point; + outside, - inside
	unsigned int numScalars = (this->m_nCellsX+1) * (this->m_nCellsY+1) * (this->m_nCellsZ+1);
	T* sField = new T[numScalars];
	this->m_ptScalarField = sField;
	vector<bool> isValidScalar;
	isValidScalar.resize(numScalars, false);

	// quick indexing for locating where the cube is relative to edge
	unsigned int cubeLocRelToEdge[12] = {3, 2, 2, 3, 1, 0, 0, 1, 3, 1, 0, 2};	
	// stores vertex to tri relationship at the adjacent cubes 0,1,2,3
	vector<vector<vector<int> > > v_to_tri(3*numScalars, vector<vector<int> >(4));

	unsigned int nextExtraVID = GetEdgeID(this->m_nCellsX-1, this->m_nCellsY-1, this->m_nCellsZ-1, 19) + 1;

	// Generate isosurface.
	for (unsigned int z = 0; z < this->m_nCellsZ; z++)
		for (unsigned int y = 0; y < this->m_nCellsY; y++)
			for (unsigned int x = 0; x < this->m_nCellsX; x++) {
				unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;

				// only triangulate cubes that are intersected
// 				if (cubesProperty[cubeIdx].intersectedTriangle.empty())
// 					continue;
				if (!cubesProperty[cubeIdx].intersectedTriangle.empty()) {
				
					// Calculate table lookup index from those
					// vertices which are below the isolevel.
					unsigned int tableIndex = 0;
					//    1----2
					//   /|  / |
					// 5----6  |
					// | /0-|--3
					// |/   |/
					// 4----7
					
					unsigned int closestTriIdx(0);
					unsigned int gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + x;
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x, y, z, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 1;
					
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*(y+1) + x;
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x, y+1, z, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 2;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*(y+1) + (x+1);
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x+1, y+1, z, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 4;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + (x+1);
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x+1, y, z, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 8;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*y + x;
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x, y, z+1, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 16;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*(y+1) + x;
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x, y+1, z+1, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 32;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*(y+1) + (x+1);
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x+1, y+1, z+1, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 64;
	
					closestTriIdx = 0;
					gridIdx = (this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*y + (x+1);
					if (!isValidScalar[gridIdx]) {
						if (getScalar(x+1, y, z+1, closestTriIdx, verticesList, numVertices, polygonsList, numPolygons, vertexNeighbourList) >= 0)
							sField[gridIdx] = 1;
						else
							sField[gridIdx] = -1;
						isValidScalar[gridIdx] = true;
					}
					if (sField[gridIdx] >= this->m_tIsoLevel)
						tableIndex |= 128;
	
	// 				this->allTableIndex[z*(this->m_nCellsX)*(this->m_nCellsY) + y*(this->m_nCellsX) + x] = tableIndex;
						
					unsigned int nbrXIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + (x+1);
					unsigned int nbrYIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*(y+1) + x;
					unsigned int nbrZIdx = (this->m_nCellsX)*(this->m_nCellsY)*(z+1) + (this->m_nCellsX)*y + x;
					unsigned int nbrXYIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*(y+1) + (x+1);
					unsigned int nbrXZIdx = (this->m_nCellsX)*(this->m_nCellsY)*(z+1) + (this->m_nCellsX)*y + (x+1);
					unsigned int nbrYZIdx = (this->m_nCellsX)*(this->m_nCellsY)*(z+1) + (this->m_nCellsX)*(y+1) + x;
	
					// Now create a triangulation of the isosurface in this
					// cell.
					if (this->m_edgeTable[tableIndex] != 0)
					{
						if (this->m_edgeTable[tableIndex] & 8) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 3, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 3);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
						if (this->m_edgeTable[tableIndex] & 1) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 0, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 0);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
						if (this->m_edgeTable[tableIndex] & 256) {
							POINT3DID pt = this->CalculateIntersection(x, y, z, 8, surfaceType);
							pt.newID = 0;
							unsigned int id = GetEdgeID(x, y, z, 8);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
						}
	
						if (x == this->m_nCellsX - 1 || cubesProperty[nbrXIdx].intersectedTriangle.empty()) {
							if (this->m_edgeTable[tableIndex] & 4) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 2, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 2);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
							if (this->m_edgeTable[tableIndex] & 2048) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 11, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 11);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
						}
						if (y == this->m_nCellsY - 1 || cubesProperty[nbrYIdx].intersectedTriangle.empty()) {
							if (this->m_edgeTable[tableIndex] & 2) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 1, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 1);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
							if (this->m_edgeTable[tableIndex] & 512) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 9, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 9);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
						}
						if (z == this->m_nCellsZ - 1 || cubesProperty[nbrZIdx].intersectedTriangle.empty()) {
							if (this->m_edgeTable[tableIndex] & 16) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 4, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 4);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
							if (this->m_edgeTable[tableIndex] & 128) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 7, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 7);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
						}
						if (((x == this->m_nCellsX - 1) && (y == this->m_nCellsY - 1)) || cubesProperty[nbrXYIdx].intersectedTriangle.empty())
							if (this->m_edgeTable[tableIndex] & 1024) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 10, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 10);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
						if (((x == this->m_nCellsX - 1) && (z == this->m_nCellsZ - 1)) || cubesProperty[nbrXZIdx].intersectedTriangle.empty())
							if (this->m_edgeTable[tableIndex] & 64) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 6, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 6);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
						if (((y == this->m_nCellsY - 1) && (z == this->m_nCellsZ - 1)) || cubesProperty[nbrYZIdx].intersectedTriangle.empty())
							if (this->m_edgeTable[tableIndex] & 32) {
								POINT3DID pt = this->CalculateIntersection(x, y, z, 5, surfaceType);
								pt.newID = 0;
								unsigned int id = GetEdgeID(x, y, z, 5);
								this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, pt));
							}
	
						if  (this->m_triTable[tableIndex][0] == -3)
						{
							// -3:	Special Case -3
							// {-3, a, b, c, d, e, f, -1, ... }
							// where a,b,c,d forms a quad, and d,e,f,a forms the other quad
							// The right hand ordering determines the normals
							// A new point m should be added to the midpoint of a,d
							// Then, the 6 new triangles are {a, b, m, b, c, m, c, d, m, d, e, m, e, f, m, f, a, m, -1}
							
							// get point a, d
							POINT3DID pta;
							POINT3DID ptd;
							pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
							ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);
							
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 18);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
							
							// create triangles
							for (unsigned int i = 1; i <= 6; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								pointID2 = GetEdgeID(x, y, z, 18);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%6 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
								
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
	
							// get point b, e
							POINT3DID ptb;
							POINT3DID pte;
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
							
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 12);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
					
							// create triangles
							for (unsigned int i = 1; i <= 5; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%5 + 1]);
								pointID2 = GetEdgeID(x, y, z, 12);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%5 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%5 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
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
	
							// get point a, d
							POINT3DID ptb;
							POINT3DID pte;
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
							
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 19);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
							
							// create triangles
							for (unsigned int i = 1; i <= 7; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%7 + 1]);
								pointID2 = GetEdgeID(x, y, z, 19);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%7 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%7 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
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
							
							
							// get point b, e
							POINT3DID ptb;
							POINT3DID pte;
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
	
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 17);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
	
							// create triangles
							for (unsigned int i = 1; i <= 7; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								if (i == 7)
								{
									pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
									pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+1]);
									pointID2 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+2]);
								}
								else
								{
									pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
									pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
									pointID2 = GetEdgeID(x, y, z, 17);
								}
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								if (i == 7)
								{
									unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
									unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
									unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+1]);
									unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+1]];
									unsigned int edgeIdx3 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+2]);
									unsigned int cubePos3 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+2]];
									v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx3][cubePos3].push_back(this->m_trivecTriangles.size()-1);
								}
								else
								{
									unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
									unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
									unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
									unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%6 + 1]];
									v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
								}
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
	
							// get point b, e
							POINT3DID ptb;
							POINT3DID pte;
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
	
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 13);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
	
							// create triangles
							for (unsigned int i = 1; i <= 6; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								pointID2 = GetEdgeID(x, y, z, 13);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%6 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
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
	
							// get point a, d
							POINT3DID pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
							POINT3DID ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);
	
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 14);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
	
							// create triangles
							for (unsigned int i = 1; i <= 6; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								pointID2 = GetEdgeID(x, y, z, 14);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%6 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
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
	
							// get point b, e
							POINT3DID ptb;
							POINT3DID pte;
							ptb = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][2], surfaceType);
							pte = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][5], surfaceType);
	
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, ptb.x, ptb.y, ptb.z, pte.x, pte.y, pte.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 15);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
	
							// create triangles
							for (unsigned int i = 1; i <= 6; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
	
								unsigned int idx2 = this->m_triTable[tableIndex][i%5 + 1];
								unsigned int idx3 = 15;
								if (i == 6)
								{
									idx2 = this->m_triTable[tableIndex][i+1];
									idx3 = this->m_triTable[tableIndex][i+2];
								}
								
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, idx2);
								pointID2 = GetEdgeID(x, y, z, idx3);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								if (i == 6)
								{
									unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
									unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
									unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+1]);
									unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+1]];
									unsigned int edgeIdx3 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+2]);
									unsigned int cubePos3 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+2]];
									v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx3][cubePos3].push_back(this->m_trivecTriangles.size()-1);
								}
								else
								{
									unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
									unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
									unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%5 + 1]);
									unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%5 + 1]];
									v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
									v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
								}
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
	
							// get point a, d
							POINT3DID pta = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][1], surfaceType);
							POINT3DID ptd = this->CalculateIntersection(x, y, z, this->m_triTable[tableIndex][4], surfaceType);
	
							// calculate midpoint m
							double *m = new double[3];
							midpoint(m, pta.x, pta.y, pta.z, ptd.x, ptd.y, ptd.z);
							POINT3DID ptm;
							ptm.newID = 0;
							ptm.x = m[0];
							ptm.y = m[1];
							ptm.z = m[2];
							delete[] m;
	
							// add vertex m
							unsigned int id = GetEdgeID(x, y, z, 16);
							this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(id, ptm));
	
							// create triangles
							for (unsigned int i = 1; i <= 6; ++i) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								pointID2 = GetEdgeID(x, y, z, 16);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i%6 + 1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i%6 + 1]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
							}
						}
						else
						{
							for (unsigned int i = 0; this->m_triTable[tableIndex][i] != -1; i += 3) {
								TRIANGLE triangle;
								unsigned int pointID0, pointID1, pointID2;
								pointID0 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i]);
								pointID1 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+1]);
								pointID2 = GetEdgeID(x, y, z, this->m_triTable[tableIndex][i+2]);
								triangle.pointID[0] = pointID0;
								triangle.pointID[1] = pointID1;
								triangle.pointID[2] = pointID2;
								this->m_trivecTriangles.push_back(triangle);
	
								// store v-to-tri info
								unsigned int edgeIdx1 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i]);
								unsigned int cubePos1 = cubeLocRelToEdge[this->m_triTable[tableIndex][i]];
								unsigned int edgeIdx2 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+1]);
								unsigned int cubePos2 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+1]];
								unsigned int edgeIdx3 = edgeIndexing(x, y, z, this->m_triTable[tableIndex][i+2]);
								unsigned int cubePos3 = cubeLocRelToEdge[this->m_triTable[tableIndex][i+2]];
								v_to_tri[edgeIdx1][cubePos1].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx2][cubePos2].push_back(this->m_trivecTriangles.size()-1);
								v_to_tri[edgeIdx3][cubePos3].push_back(this->m_trivecTriangles.size()-1);
							}
						}
					}
				}

				// check for non-manifold vertices
				for (unsigned int i = 0; i < 3; ++i)
				{
					unsigned int edge = 3;
					if (i == 1)
						edge = 0;
					else if (i == 2)
						edge = 8;
					unsigned int edgeIdx = edgeIndexing(x, y, z, edge);

					// non-manifold vertices occur when diagonal cubes generate triangles where as the other diagonal cubes do not
					unsigned int indexToChange = 0;
					if (!v_to_tri[edgeIdx][0].empty() && v_to_tri[edgeIdx][1].empty() && v_to_tri[edgeIdx][2].empty() && !v_to_tri[edgeIdx][3].empty())
						indexToChange = 3;
					else if (v_to_tri[edgeIdx][0].empty() && !v_to_tri[edgeIdx][1].empty() && !v_to_tri[edgeIdx][2].empty() && v_to_tri[edgeIdx][3].empty())
						indexToChange = 2;
					else
						continue;

					// split the vertex into two
					unsigned int vertexID = GetEdgeID(x, y, z, edge);
					unsigned int newVID = nextExtraVID;
					POINT3DID ptm;
					ptm.newID = 0;
					ptm.x = this->m_i2pt3idVertices[vertexID].x;
					ptm.y = this->m_i2pt3idVertices[vertexID].y;
					ptm.z = this->m_i2pt3idVertices[vertexID].z;
					this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(newVID, ptm));
					++nextExtraVID;

					for (unsigned int j = 0; j < v_to_tri[edgeIdx][indexToChange].size(); ++j) {
						unsigned int triIdx = v_to_tri[edgeIdx][indexToChange][j];
						for (unsigned int k = 0; k < 3; ++k) {
							if (this->m_trivecTriangles[triIdx].pointID[k] == vertexID) {
								this->m_trivecTriangles[triIdx].pointID[k] = newVID;
								break;
							}
						}						
					}
				}

			}

	CIsoSurface<T>::RenameVerticesAndTriangles();
	this->CalculateNormals();
	this->m_bValidSurface = true;
	
	delete[] sField;
	this->m_ptScalarField = NULL;
// 	delete[] this->allTableIndex;
// 	this->allTableIndex = NULL;
}

// returns the edge index
//
// x (in): cube x-coord
// y (in): cube y-coord
// z (in): cube z-coord
// edgeIdx (in): edge index of cube (x,y,z)
// return value (out): edge index being query
template <class T> unsigned int TilingNonobtuseMC<T>::edgeIndexing(unsigned int x, unsigned int y, unsigned int z, unsigned int edgeIdx) const
{
	if (edgeIdx == 0)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + x) + 1);
	else if (edgeIdx == 1)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*(y+1) + x) + 0);
	else if (edgeIdx == 2)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + (x+1)) + 1);
	else if (edgeIdx == 3)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + x) + 0);
	else if (edgeIdx == 4)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*y + x) + 1);
	else if (edgeIdx == 5)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*(y+1) + x) + 0);
	else if (edgeIdx == 6)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*y + (x+1)) + 1);
	else if (edgeIdx == 7)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*(z+1) + (this->m_nCellsX+1)*y + x) + 0);
	else if (edgeIdx == 8)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + x) + 2);
	else if (edgeIdx == 9)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*(y+1) + x) + 2);
	else if (edgeIdx == 10)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*(y+1) + (x+1)) + 2);
	else if (edgeIdx == 11)
		return (3*((this->m_nCellsX+1)*(this->m_nCellsY+1)*z + (this->m_nCellsX+1)*y + (x+1)) + 2);

	return 0;
}

// post-processing for eliminating non-manifold vertices introduced during construction using simpleMC
//
template <class T> void TilingNonobtuseMC<T>::postProcess_simpleMC()
{
}

// returns the scalar of grid point at (x,y,z); also return the closest triangle in the original mesh
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// closestTriIdx (out): index of closest triangle in original mesh 
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
// vertexNeighbourList (in): vertex-to-polygon neighbour information for source mesh
// return val (out): scalar value of the grid point
template <class T> double TilingNonobtuseMC<T>::getScalar(unsigned int x, unsigned int y, unsigned int z, unsigned int &closestTriIdx, const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons, const vector<int>* vertexNeighbourList) const
{
	// geometry of grid point
	double pt[3] = {x*(this->m_fCellLengthX) + this->m_offsets[0], y*(this->m_fCellLengthY) + this->m_offsets[1], z*(this->m_fCellLengthZ) + this->m_offsets[2]};

	closestTriIdx = 0;
	double closestDist = -1;
	double closestPt[3] = {0,0,0};
	double closestPtType(0);			// determines if closest pt is inside triangle, on edge, or at a vertex
												//	0 - closest pt is inside triangle, 
												// 1 - closest pt is tript1,
												// 2 - closest pt is tript2,
												// 3 - closest pt is tript3,
												// -1 - closest pt is on edge (tript1, tript2),
												// -2 - closest pt is on edge (tript2, tript3),
												// -3 - closest pt is on edge (tript3, tript1),

	// offset for the cubes to check; 1st 8 are the cubes that share the grid point; others are direct neighbours of those 8 cubes
	int nbrCubesOffset[32][3] = {
									{0,0,0},
									{0,0,-1},
									{-1,0,-1},
									{-1,0,0},
									{0,-1,0},
									{0,-1,-1},
									{-1,-1,-1},
									{-1,-1,0},

									{1,0,0},
									{1,0,-1},
									{1,-1,0},
									{1,-1,-1},
									{-2,0,0},
									{-2,0,-1},
									{-2,-1,0},
									{-2,-1,-1},

									{0,1,0},
									{0,1,-1},
									{-1,1,0},
									{-1,1,-1},
									{0,-2,0},
									{0,-2,-1},
									{-1,-2,0},
									{-1,-2,-1},

									{0,0,1},
									{0,-1,1},
									{-1,0,1},
									{-1,-1,1},
									{0,0,-2},
									{0,-1,-2},
									{-1,0,-2},
									{-1,-1,-2},
								};

	for (unsigned int i = 0; i < 32; ++i)	// check all 8 + 24 neighbour cubes for closest triangle
	{
		// check if closest triangle lies in the adjacent 8 cubes
		if (i > 7 && closestDist < this->m_fCellLengthX && closestDist > 0)
			break;

		unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*(z+nbrCubesOffset[i][2]) + (this->m_nCellsX)*(y+nbrCubesOffset[i][1]) + (x+nbrCubesOffset[i][0]);

		// check cubes that have intersected triangles only
		if (cubesProperty[cubeIdx].intersectedTriangle.empty())
			continue;

		for (unsigned int j = 0; j < cubesProperty[cubeIdx].intersectedTriangle.size(); ++j)
		{
			unsigned int triIdx = cubesProperty[cubeIdx].intersectedTriangle[j].triIdx;
			double triPt1[3] = {verticesList[3*(polygonsList[triIdx][0])],
								verticesList[3*(polygonsList[triIdx][0])+1],
								verticesList[3*(polygonsList[triIdx][0])+2]};
			double triPt2[3] = {verticesList[3*(polygonsList[triIdx][1])],
								verticesList[3*(polygonsList[triIdx][1])+1],
								verticesList[3*(polygonsList[triIdx][1])+2]};
			double triPt3[3] = {verticesList[3*(polygonsList[triIdx][2])],
								verticesList[3*(polygonsList[triIdx][2])+1],
								verticesList[3*(polygonsList[triIdx][2])+2]};
			double closestPt_tri[3];
			double dist;
			int returnCode = pt_triangle_distance(pt, triPt1, triPt2, triPt3, closestPt_tri, dist);

			if (closestDist == -1 || dist < closestDist)
			{
				closestTriIdx = triIdx;
				closestDist = dist;
				closestPt[0] = closestPt_tri[0];
				closestPt[1] = closestPt_tri[1];
				closestPt[2] = closestPt_tri[2];
				closestPtType = returnCode;
			}
		}
	}

	// after finding closest triangle, now use it to compute scalar field

	if (closestDist == -1)	// can't find a closest triangle
	{
// 		cout << "this shouldn't happen!" << endl;
		return 0;
	}
	
	double normal[3] = {0,0,0};

	if (closestPtType == 0)		// closest pt is inside triangle
	{
		// get closest triangle's face normal
		double triPt1[3] = {verticesList[3*(polygonsList[closestTriIdx][0])],
									verticesList[3*(polygonsList[closestTriIdx][0])+1],
									verticesList[3*(polygonsList[closestTriIdx][0])+2]};
		double triPt2[3] = {verticesList[3*(polygonsList[closestTriIdx][1])],
									verticesList[3*(polygonsList[closestTriIdx][1])+1],
									verticesList[3*(polygonsList[closestTriIdx][1])+2]};
		double triPt3[3] = {verticesList[3*(polygonsList[closestTriIdx][2])],
									verticesList[3*(polygonsList[closestTriIdx][2])+1],
									verticesList[3*(polygonsList[closestTriIdx][2])+2]};
		computeFaceNormal(triPt1[0], triPt1[1], triPt1[2], triPt2[0], triPt2[1], triPt2[2], triPt3[0], triPt3[1], triPt3[2], normal);
	}
	else if (closestPtType > 0)	// closest pt is at a vertex
	{
		int vID = polygonsList[closestTriIdx][0];
		if (closestPtType == 2)
			vID = polygonsList[closestTriIdx][1];
		else if (closestPtType == 3)
			vID = polygonsList[closestTriIdx][2];

		double pt[3] = {verticesList[3*vID],
							verticesList[3*vID+1],
							verticesList[3*vID+2]};

		// get angle weighted vertex normal
		double totalAngle = 0;
		for (unsigned int i = 0; i < vertexNeighbourList[vID].size(); ++i)
		{
			unsigned int triIdx = vertexNeighbourList[vID][i];
			// get angle weights
			int prevIdx(0);
			int nextIdx(0);
			for (unsigned int j = 0; j < 3; ++j) {
				if (polygonsList[triIdx][j] == vID)
				{
					nextIdx = polygonsList[triIdx][(j+1)%3];
					prevIdx = polygonsList[triIdx][(j+2)%3];
					break;
				}
			}
			double nextpt[3] = {verticesList[3*nextIdx],
									verticesList[3*nextIdx+1],
									verticesList[3*nextIdx+2]};
			double prevpt[3] = {verticesList[3*prevIdx],
									verticesList[3*prevIdx+1],
									verticesList[3*prevIdx+2]};

			double angle = getAngle(pt, nextpt, prevpt);

			// get face normals
			double triNormal[3] = {0,0,0};
			computeFaceNormal(pt[0], pt[1], pt[2], nextpt[0], nextpt[1], nextpt[2], prevpt[0], prevpt[1], prevpt[2], triNormal);

			normal[0] += angle * triNormal[0];
			normal[1] += angle * triNormal[1];
			normal[2] += angle * triNormal[2];
			totalAngle += angle;
		}

		normal[0] /= totalAngle;
		normal[1] /= totalAngle;
		normal[2] /= totalAngle;
	}
	else									// closest pt is on an edge
	{
		// get face normal of closest triangle
		double triPt1[3] = {verticesList[3*(polygonsList[closestTriIdx][0])],
									verticesList[3*(polygonsList[closestTriIdx][0])+1],
									verticesList[3*(polygonsList[closestTriIdx][0])+2]};
		double triPt2[3] = {verticesList[3*(polygonsList[closestTriIdx][1])],
									verticesList[3*(polygonsList[closestTriIdx][1])+1],
									verticesList[3*(polygonsList[closestTriIdx][1])+2]};
		double triPt3[3] = {verticesList[3*(polygonsList[closestTriIdx][2])],
									verticesList[3*(polygonsList[closestTriIdx][2])+1],
									verticesList[3*(polygonsList[closestTriIdx][2])+2]};
		computeFaceNormal(triPt1[0], triPt1[1], triPt1[2], triPt2[0], triPt2[1], triPt2[2], triPt3[0], triPt3[1], triPt3[2], normal);

		// now get the face normal of the other triangle

		int vID = polygonsList[closestTriIdx][0];
		int nextVID = polygonsList[closestTriIdx][1];
		if (closestPtType == -2) {
			vID = polygonsList[closestTriIdx][1];
			nextVID = polygonsList[closestTriIdx][2];
		}
		else if (closestPtType == -3) {
			vID = polygonsList[closestTriIdx][2];
			nextVID = polygonsList[closestTriIdx][0];
		}

		// find adjacent triangle
		bool foundNbr = false;
		int nbrTriIdx;
		for (unsigned int i = 0; i < vertexNeighbourList[vID].size(); ++i)
		{
			unsigned int triIdx = vertexNeighbourList[vID][i];
			if (triIdx == closestTriIdx)
				continue;

			for (unsigned int j = 0; j < 3; ++j) {
				if (polygonsList[triIdx][j] == vID) {
					if (polygonsList[triIdx][(j+2)%3] == nextVID) {
						foundNbr = true;
						nbrTriIdx = triIdx;
					}
					break;
				}
			}

			if (foundNbr)
				break;
		}

		if (foundNbr)
		{
			double triPt1[3] = {verticesList[3*(polygonsList[nbrTriIdx][0])],
									verticesList[3*(polygonsList[nbrTriIdx][0])+1],
									verticesList[3*(polygonsList[nbrTriIdx][0])+2]};
			double triPt2[3] = {verticesList[3*(polygonsList[nbrTriIdx][1])],
									verticesList[3*(polygonsList[nbrTriIdx][1])+1],
									verticesList[3*(polygonsList[nbrTriIdx][1])+2]};
			double triPt3[3] = {verticesList[3*(polygonsList[nbrTriIdx][2])],
									verticesList[3*(polygonsList[nbrTriIdx][2])+1],
									verticesList[3*(polygonsList[nbrTriIdx][2])+2]};
			double triNormal[3] = {0,0,0};
			computeFaceNormal(triPt1[0], triPt1[1], triPt1[2], triPt2[0], triPt2[1], triPt2[2], triPt3[0], triPt3[1], triPt3[2], triNormal);
			
			normal[0] = (normal[0] + triNormal[0]) / 2.0;
			normal[1] = (normal[1] + triNormal[1]) / 2.0;
			normal[2] = (normal[2] + triNormal[2]) / 2.0;
		}
	}

	// closestPt-to-pt vector
	double v[3] = {pt[0]-closestPt[0], pt[1]-closestPt[1], pt[2]-closestPt[2]};
	normalize(v);
	
	if (dotProduct(normal, v) < 0)
		closestDist = -1 * closestDist;

	return closestDist;
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
	sortIntersectingTriangle(cubeIdx, polygonNeighbourList);	// make sure we sort the trianlges into patches if not already sorted

 	vector<vector<int> > &triGroups = cubesProperty[cubeIdx].triGroups;

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
			cubesProperty[cubeIdx].isConsistent[i] = false;
			cubesProperty[cubeIdx].isEmptyTriangulation[i] = true;
			++numPatchIgnored;
			continue;
		}

		cubesProperty[cubeIdx].isConsistent[i] = true;
		
		// triangulate patch
		//if ((x == 8 && y == 47 && z == 32) || (x == 9 && y == 47 && z == 32))
		triangulatePatch(x, y, z, i, edgeInt, verticesList, numVertices, polygonsList, numPolygons, polygonNeighbourList);
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
	vector<int> &signs = cubesProperty[cubeIdx].signs[patchNo];

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
			//tableIndex |= (unsigned int)pow(2,i);
			tableIndex |= (unsigned int)pow((double)2,(double)i);		// strange hack to get it compile at home
	}	
		
	// Now create a triangulation of the isosurface in this
	// cell.
	if (this->m_edgeTable[tableIndex] == 0)
		cubesProperty[cubeIdx].isEmptyTriangulation[patchNo] = true;
	else
	{
		cubesProperty[cubeIdx].isEmptyTriangulation[patchNo] = false;
		// stores the vertex ID introduced on an edge of the cube that were used during the triangulation
		int existingVertices[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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

					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}
			
					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;
				
				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%6 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}

					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%5 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%5 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));
			
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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}

					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%7 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%7 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;	

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}

					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint, except for the 7th triangle
				if (i != 7)
					triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				if (i != 7)
				{
					int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%6 + 1]];
					VPAIR vertexpair;
					vertexpair.first = triangle.pointID[0];
					vertexpair.second = triangle.pointID[1];

					EdgePosInfo eInfo;
					eInfo.v1Edge = this->m_triTable[tableIndex][i];
					eInfo.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
					eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
					eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
					eInfo.isV1First = true;

					if (vertexpair.first > vertexpair.second) {
						swap(vertexpair.first, vertexpair.second);
						swap(eInfo.v1Edge, eInfo.v2Edge);
						eInfo.isV1First = false;
					}
					if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
					{
						eInfo.edgeIdx = vpair_to_e[vertexpair];
						unsigned int faceID = GetFaceID(x,y,z,faceIdx);
						e_on_f[faceID].push_back(eInfo);
						e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
					}
				}
				else
				{	
					for (unsigned int k = 0; k < 3; ++k)
					{
						int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i+k]][this->m_triTable[tableIndex][i+((k+1)%3)]];
						VPAIR vertexpair;
						vertexpair.first = triangle.pointID[k];
						vertexpair.second = triangle.pointID[(k+1)%3];

						EdgePosInfo eInfo;
						eInfo.v1Edge = this->m_triTable[tableIndex][i+k];
						eInfo.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
						eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
						eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
						eInfo.isV1First = true;

						if (vertexpair.first > vertexpair.second) {
							swap(vertexpair.first, vertexpair.second);
							swap(eInfo.v1Edge, eInfo.v2Edge);
							eInfo.isV1First = false;
						}
						if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
						{
							eInfo.edgeIdx = vpair_to_e[vertexpair];
							unsigned int faceID = GetFaceID(x,y,z,faceIdx);
							e_on_f[faceID].push_back(eInfo);
							e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
						}
					}
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}
			
					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%6 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}
			
					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%6 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}

					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint, except for the 6th triangle
				if (i != 6)
					triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				if (i != 6)
				{
					// add edge-on-face reference
					int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%5 + 1]];
					VPAIR vertexpair;
					vertexpair.first = triangle.pointID[0];
					vertexpair.second = triangle.pointID[1];

					EdgePosInfo eInfo;
					eInfo.v1Edge = this->m_triTable[tableIndex][i];
					eInfo.v2Edge = this->m_triTable[tableIndex][i%5 + 1];
					eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
					eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
					eInfo.isV1First = true;

					if (vertexpair.first > vertexpair.second) {
						swap(vertexpair.first, vertexpair.second);
						swap(eInfo.v1Edge, eInfo.v2Edge);
						eInfo.isV1First = false;
					}
					if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
					{
						eInfo.edgeIdx = vpair_to_e[vertexpair];
						unsigned int faceID = GetFaceID(x,y,z,faceIdx);
						e_on_f[faceID].push_back(eInfo);
						e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
					}
				}
				else
				{
					for (unsigned int k = 0; k < 3; ++k)
					{	
						// add edge-on-face reference
						int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i+k]][this->m_triTable[tableIndex][i+((k+1)%3)]];
						VPAIR vertexpair;
						vertexpair.first = triangle.pointID[k];
						vertexpair.second = triangle.pointID[(k+1)%3];

						EdgePosInfo eInfo;
						eInfo.v1Edge = this->m_triTable[tableIndex][i+k];
						eInfo.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
						eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
						eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
						eInfo.isV1First = true;

						if (vertexpair.first > vertexpair.second) {
							swap(vertexpair.first, vertexpair.second);
							swap(eInfo.v1Edge, eInfo.v2Edge);
							eInfo.isV1First = false;
						}
						if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
						{
							eInfo.edgeIdx = vpair_to_e[vertexpair];
							unsigned int faceID = GetFaceID(x,y,z,faceIdx);
							e_on_f[faceID].push_back(eInfo);
							e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
						}
					}
				}
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
			v_on_e[edgeID].push_back(mID);
			v_to_eOfCube.insert(VTOEOFCUBE::value_type(mID, edgeID));

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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}
			
					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				// 3rd point of the triangle is midpoint
				triangle.pointID[2] = mID;

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i]][this->m_triTable[tableIndex][i%6 + 1]];
				VPAIR vertexpair;
				vertexpair.first = triangle.pointID[0];
				vertexpair.second = triangle.pointID[1];

				EdgePosInfo eInfo;
				eInfo.v1Edge = this->m_triTable[tableIndex][i];
				eInfo.v2Edge = this->m_triTable[tableIndex][i%6 + 1];
				eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
				eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
				eInfo.isV1First = true;

				if (vertexpair.first > vertexpair.second) {
					swap(vertexpair.first, vertexpair.second);
					swap(eInfo.v1Edge, eInfo.v2Edge);
					eInfo.isV1First = false;
				}
				if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
				{
					eInfo.edgeIdx = vpair_to_e[vertexpair];
					unsigned int faceID = GetFaceID(x,y,z,faceIdx);
					e_on_f[faceID].push_back(eInfo);
					e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
				}
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
			
					// sanity check
					if (edgeNo >= 12) {
						cout << "TilingNonobtuseMC<T>::triangulatePatch() - edgeNo is not with [0,11]" << endl;
						return;
					}
			
					unsigned int vertexID;
					if (existingVertices[edgeNo] > -1)
					{
						vertexID = existingVertices[edgeNo];
					}
					// if no existing vertex can be used, add a new one
					else
					{
						vertexID = nextUnusedVertexID;
						++nextUnusedVertexID;

						// insert new vertex into list
						POINT3DID pt = this->computeVertexLocation(x, y, z, edgeNo);
						this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

						existingVertices[edgeNo] = vertexID;

						// add new vertex to corresponding edge for reference
						v_on_e[edgeID].push_back(vertexID);
						v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
					}

					// add vertex reference to the triangle
					triangle.pointID[j] = vertexID;
				}

				for (unsigned int j = 0; j < 3; ++j)
				{
					// add triangle reference to vertices
					if (v_to_tri.find(triangle.pointID[j]) == v_to_tri.end())
					{
						vector<unsigned int> adjTriList(1, this->m_trivecTriangles.size());
						v_to_tri.insert(VTOTRI::value_type(triangle.pointID[j], adjTriList));
					}
					else
						v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

					Edge e;
					e.vertex1Idx = triangle.pointID[j];
					e.vertex2Idx = triangle.pointID[(j+1)%3];
					if (e.vertex1Idx > e.vertex2Idx)
						swap(e.vertex1Idx, e.vertex2Idx);
					
					// check if a new edge needs to be created
					unsigned int currentEdgeID;
					bool needNewEdge(true);
					
					VTOE::iterator iter = v_to_e.find(triangle.pointID[j]);
					if (iter != v_to_e.end())
					{
						vector<unsigned int> &adjEdges = v_to_e[triangle.pointID[j]];
						for (unsigned int i = 0; i < adjEdges.size(); ++i) {
							if (e.vertex1Idx == edgeList[adjEdges[i]].vertex1Idx && e.vertex2Idx == edgeList[adjEdges[i]].vertex2Idx) {
								currentEdgeID = adjEdges[i];
								needNewEdge = false;
								break;
							}
						}
					}
		
					// add edge to the edge list
					if (needNewEdge) {
						currentEdgeID = nextUnusedEdgeID;
						++nextUnusedEdgeID;
						edgeList.insert(ID2EDGE::value_type(currentEdgeID, e));
						
						VPAIR vertexpair;
						vertexpair.first = e.vertex1Idx;
						vertexpair.second = e.vertex2Idx;
						vpair_to_e.insert(VPAIRTOE::value_type(vertexpair, currentEdgeID));
					
						// add e-to-tri references
						vector<unsigned int> triIndices;
						triIndices.push_back(this->m_trivecTriangles.size());
						e_to_tri.insert(ETOTRI::value_type(currentEdgeID, triIndices));
					}
					else	// add e-to-tri references
						e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());

					// add v-to-e references
					if (iter == v_to_e.end())
					{
						vector<unsigned int> adjEdgeList;
						adjEdgeList.push_back(currentEdgeID);
						v_to_e.insert(VTOE::value_type(triangle.pointID[j], adjEdgeList));

						if (v_to_e.find(triangle.pointID[(j+1)%3]) == v_to_e.end())
							v_to_e.insert(VTOE::value_type(triangle.pointID[(j+1)%3], adjEdgeList));
						else
							v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
					else if (needNewEdge)
					{
						v_to_e[triangle.pointID[j]].push_back(currentEdgeID);
						v_to_e[triangle.pointID[(j+1)%3]].push_back(currentEdgeID);
					}
				}

				// add triangle to triangle list
				this->m_trivecTriangles.push_back(triangle);

				// add tri-to-cube reference
				vector<CubeID> cIDList(1, CubeID(x,y,z));
				vector<unsigned int> pNoList(1, patchNo);
				tri_to_CubeID.push_back(cIDList);
				tri_to_patch.push_back(pNoList);

				// add edge-on-face reference
				for (unsigned int k = 0; k < 3; ++k)
				{
					int faceIdx = edgeOnFaceLookup[this->m_triTable[tableIndex][i+k]][this->m_triTable[tableIndex][i+((k+1)%3)]];
					VPAIR vertexpair;
					vertexpair.first = triangle.pointID[k];
					vertexpair.second = triangle.pointID[(k+1)%3];

					EdgePosInfo eInfo;
					eInfo.v1Edge = this->m_triTable[tableIndex][i+k];
					eInfo.v2Edge = this->m_triTable[tableIndex][i+((k+1)%3)];
					eInfo.v1Edge = transformEdgeIdx[faceIdx][eInfo.v1Edge];
					eInfo.v2Edge = transformEdgeIdx[faceIdx][eInfo.v2Edge];
					eInfo.isV1First = true;

					if (vertexpair.first > vertexpair.second) {
						swap(vertexpair.first, vertexpair.second);
						swap(eInfo.v1Edge, eInfo.v2Edge);
						eInfo.isV1First = false;
					}
					if (vpair_to_e.find(vertexpair) != vpair_to_e.end())
					{
						eInfo.edgeIdx = vpair_to_e[vertexpair];
						int faceID = GetFaceID(x,y,z,faceIdx);
						if (faceID != -1)
						{
							e_on_f[faceID].push_back(eInfo);
							e_to_face.insert(ETOFACE::value_type(eInfo.edgeIdx, faceID));
						}
					}
				}
			}
		}
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
	vector<int> &group = cubesProperty[cubeIdx].triGroups[groupIdx];
	vector<IntersectTri> &intersectedTriangle = cubesProperty[cubeIdx].intersectedTriangle;

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
		for (vector<int>::iterator iter = group.begin(); iter != group.end(); iter++)
		{
			IntersectTri &intTri = intersectedTriangle[*iter];

			// skip triangle if it should be ignored due to edge intersection of neighbouring triangle
			if (find(ignoreTri.begin(), ignoreTri.end(), intTri.triIdx) != ignoreTri.end())
				continue;

			// skip triangles that are completely inside cube
			if (intTri.insideCube)
				continue;

			// sanity checks
			if (intTri.triIdx >= numPolygons) {
				cout << "Error: TilingNonobtuseMC<T>::findEdgeIntersection() - Array Index Overflow for polygonsList" << endl;
				return;
			}

			double triPt1[3] = {verticesList[3*(polygonsList[intTri.triIdx][0])], verticesList[3*(polygonsList[intTri.triIdx][0])+1], verticesList[3*(polygonsList[intTri.triIdx][0])+2]};
			double triPt2[3] = {verticesList[3*(polygonsList[intTri.triIdx][1])], verticesList[3*(polygonsList[intTri.triIdx][1])+1], verticesList[3*(polygonsList[intTri.triIdx][1])+2]};
			double triPt3[3] = {verticesList[3*(polygonsList[intTri.triIdx][2])], verticesList[3*(polygonsList[intTri.triIdx][2])+1], verticesList[3*(polygonsList[intTri.triIdx][2])+2]};

			Intersection edgeIntPt;
			edgeIntPt.triIdx = intTri.triIdx;
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
				for (vector<int>::const_iterator nbrIter = polygonNeighbourList[intTri.triIdx].begin(); nbrIter != polygonNeighbourList[intTri.triIdx].end(); nbrIter++)
				{
					if (((polygonsList[*nbrIter][0] == polygonsList[intTri.triIdx][intEdge]) ||
						(polygonsList[*nbrIter][1] == polygonsList[intTri.triIdx][intEdge]) ||
						(polygonsList[*nbrIter][2] == polygonsList[intTri.triIdx][intEdge]))
						&&
						((polygonsList[*nbrIter][0] == polygonsList[intTri.triIdx][(intEdge+1)%3]) ||
						(polygonsList[*nbrIter][1] == polygonsList[intTri.triIdx][(intEdge+1)%3]) ||
						(polygonsList[*nbrIter][2] == polygonsList[intTri.triIdx][(intEdge+1)%3]))) 
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
	vector<IntersectTri> &intTriList = cubesProperty[cubeIdx].intersectedTriangle;

	IntersectTri intTri;
	intTri.triIdx = triangleIdx;
	intTri.insideCube = insideCube;

	if (find(intTriList.begin(), intTriList.end(), intTri) == intTriList.end())
		intTriList.push_back(intTri);
}

// // sort the intersected triangles of cubeIdx into connected components
// //
// // cubeIdx (in): index of cube to be sorted
// // polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::sortIntersectingTriangle(unsigned int cubeIdx, const vector<int>* polygonNeighbourList)
{
	unsigned int numIntTri = cubesProperty[cubeIdx].intersectedTriangle.size();
	if (numIntTri == 0)
		return;

	vector<unsigned int> needSortedList;
	for (unsigned int i = 0; i < numIntTri; ++i)
		needSortedList.push_back(i);

	vector<vector<int> > &triGroups = cubesProperty[cubeIdx].triGroups;
	// continue to add new group when there are still unsorted triangles
	while (!needSortedList.empty())
	{
		triGroups.resize(triGroups.size()+1);
		expandGroup(cubeIdx, polygonNeighbourList, triGroups.size()-1, 0, needSortedList);
	}

	// create space for the signs generated by this patch
	for (unsigned int i = 0; i < triGroups.size(); ++i) {
		vector<int> unsetSigns(8,0);
		cubesProperty[cubeIdx].signs.push_back(unsetSigns);
		cubesProperty[cubeIdx].isConsistent.push_back(true);
		cubesProperty[cubeIdx].isEmptyTriangulation.push_back(false);
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
	vector<IntersectTri> &intTriList = cubesProperty[cubeIdx].intersectedTriangle;
	vector<vector<int> > &triGroups = cubesProperty[cubeIdx].triGroups;

	unsigned int triangleAdded = intTriList[needSortedList[triIdx]].triIdx;

	// if triIdx is in group, then no need to check
	if (find(triGroups[groupIdx].begin(), triGroups[groupIdx].end(), (int)needSortedList[triIdx]) != triGroups[groupIdx].end())
		return;
	
	// add triIdx to group first if not in group already
	triGroups[groupIdx].push_back(needSortedList[triIdx]);
	intTriList[needSortedList[triIdx]].patchNo = groupIdx;
		
	// remove triIdx from needSortedList
	needSortedList.erase(needSortedList.begin() + triIdx);

	// expand group by examining the neighbour of triangle just added
	for (vector<int>::const_iterator iter = polygonNeighbourList[triangleAdded].begin(); iter != polygonNeighbourList[triangleAdded].end(); iter++)
	{
		for (unsigned int i = 0; i < needSortedList.size(); ++i) {
			if (intTriList[needSortedList[i]].triIdx == *iter) {
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

// unused vertices are moved to end of array;
//
// verticesList (in): source mesh's vertices list
// numVertices (in): number of vertices in source mesh
// polygonsList (in): source mesh's polygons list
// numPolygons (in): number of polygons in source mesh
template <class T> void TilingNonobtuseMC<T>::RenameVerticesAndTriangles(const double* verticesList, int numVertices, const int* const* polygonsList, int numPolygons)
{
	unsigned int nextID = 0;
	ID2POINT3DID::iterator mapIterator = this->m_i2pt3idVertices.begin();

	// Rename vertices.
	this->m_nVertices = this->m_i2pt3idVertices.size();
	this->m_ppt3dVertices = new POINT3D[this->m_nVertices];
 	closestTriList = new int[this->m_nVertices];
	while (mapIterator != this->m_i2pt3idVertices.end()) {
		this->m_ppt3dVertices[nextID][0] = (*mapIterator).second.x;
		this->m_ppt3dVertices[nextID][1] = (*mapIterator).second.y;
		this->m_ppt3dVertices[nextID][2] = (*mapIterator).second.z;

		(*mapIterator).second.newID = nextID;

		closestTriList[nextID] = -1;
		if (!v_to_tri[(*mapIterator).first].empty())
		{
			CubeID c = tri_to_CubeID[v_to_tri[(*mapIterator).first][0]][0];
			unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*c.z + (this->m_nCellsX)*c.y + c.x;

			unsigned int pNo = tri_to_patch[v_to_tri[(*mapIterator).first][0]][0];

			vector<IntersectTri> &intersectedTriangle = cubesProperty[cubeIdx].intersectedTriangle;
			vector<int> &group = cubesProperty[cubeIdx].triGroups[pNo];

			int closestTriIdx(-1);
			double closestDist(-1.0);
			for (unsigned int i = 0; i < group.size(); ++i)
			{
				double pt[3] = {(*mapIterator).second.x, (*mapIterator).second.y, (*mapIterator).second.z};
				double triPt1[3] = {verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][0])],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][0])+1],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][0])+2]};
				double triPt2[3] = {verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][1])],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][1])+1],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][1])+2]};
				double triPt3[3] = {verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][2])],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][2])+1],
									verticesList[3*(polygonsList[intersectedTriangle[group[i]].triIdx][2])+2]};

				double closestPt_tri[3];
				double dist = pt_triangle_distance(pt, triPt1, triPt2, triPt3, closestPt_tri);
				if (closestTriIdx == -1 || dist < closestDist)
				{
					closestTriIdx = intersectedTriangle[group[i]].triIdx;
					closestDist = dist;
				}
			}

			closestTriList[nextID] = intersectedTriangle[group.front()].triIdx;
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
template <class T> unsigned int TilingNonobtuseMC<T>::GetEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo) const
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

// modified from base class
// this class: each cube has 11 vertices
// edge 12: (b,e) in special case 5b
// edge 13: (b,e) in special case 9b
// edge 14: (a,d) in special case 11c
// edge 15: (b,e) in special case 12b
// edge 16: (a,d) in special case 14c
// edge 17: (b,e) in special case -7
// edge 18: (a,d) in special case -3
// edge 19: center of cube in special case -6
template <class T> unsigned int TilingNonobtuseMC<T>::GetVertexID(unsigned int nX, unsigned int nY, unsigned int nZ) const
{
	return 11*(nZ*(this->m_nCellsY + 1)*(this->m_nCellsX + 1) + nY*(this->m_nCellsX + 1) + nX);
}

// Returns the face ID of cube.
template <class T> unsigned int TilingNonobtuseMC<T>::GetFaceID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nFaceNo) const
{
	switch (nFaceNo) {
	case 0:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*(nZ+1) + (this->m_nCellsX)*nY + nX) + 2;
	case 1:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*nZ + (this->m_nCellsX)*nY + nX);
	case 2:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*nZ + (this->m_nCellsX)*nY + nX) + 2;
	case 3:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*nZ + (this->m_nCellsX)*nY + (nX+1));
	case 4:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*nZ + (this->m_nCellsX)*(nY+1) + nX) + 1;
	case 5:
		return 3*((this->m_nCellsX)*(this->m_nCellsY)*nZ + (this->m_nCellsX)*nY + nX) + 1;
	default:
		// Invalid face no.
		return -1;
	}
}

// check the triangulated cube(x,y,z) if it needs any stitching, if so, stitch it
//
// x (in): x-coord of cube
// y (in): y-coord of cube
// z (in): z-coord of cube
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> void TilingNonobtuseMC<T>::checkStitching(unsigned int x, unsigned int y, unsigned int z, const vector<int>* polygonNeighbourList)
{
	int faceToConsider[3] = {1,2,5};

	// sanity check
	if (x > this->m_nCellsX || y > this->m_nCellsY || z > this->m_nCellsZ)
		return;

	for (unsigned int k = 0; k < 3; ++k)
	{
		if (x == this->m_nCellsX && y == this->m_nCellsY && z == this->m_nCellsZ)
			break;
		else if (x == this->m_nCellsX && y == this->m_nCellsY)
			break;
		else if (x == this->m_nCellsX && z == this->m_nCellsZ)
			break;
		else if (z == this->m_nCellsZ && y == this->m_nCellsY)
			break;
		else if (x == this->m_nCellsX)
		{
			if (k != 0)
				continue;
		}
		else if (y == this->m_nCellsY)
		{
			if (k != 2)
				continue;
		}
		else if (z == this->m_nCellsZ)
		{
			if (k != 1)
				continue;
		}

		bool needStitch = false;
		
		vector<EdgePosInfo> &edgesOnFace = e_on_f[GetFaceID(x, y, z, faceToConsider[k])];
	
		// pre-process the edges and stitch up all the edge pairs that uses the same set of vertices
		for (int i = 0; i < (int)edgesOnFace.size(); ++i)
		{
			// check if edge is open, if not, skip to next edge
			if (e_to_tri[edgesOnFace[i].edgeIdx].size() >= 2)
			{
				continue;
			}

			Edge e1 = edgeList[edgesOnFace[i].edgeIdx];
			for (unsigned int j = i+1; j < edgesOnFace.size(); ++j)
			{
				// check if edge is open, if not, skip to next edge
				if (e_to_tri[edgesOnFace[j].edgeIdx].size() >= 2)
				{
					continue;
				}

				Edge e2 = edgeList[edgesOnFace[j].edgeIdx];
				if ((e1.vertex1Idx == e2.vertex1Idx && e1.vertex2Idx == e2.vertex2Idx) || (e1.vertex1Idx == e2.vertex2Idx && e1.vertex2Idx == e2.vertex1Idx))
				{
					if (stitch(edgesOnFace[i], edgesOnFace[j], x, y, z, faceToConsider[k], polygonNeighbourList))
					{
						
					}
					else
						cout << "TilingNonobtuseMC<T>::checkStitching() - cannot stitch edges during pre-processing" << endl;
				}
			}
		}

		// try to pair up edges and stitch them
		for (int i = 0; i < (int)edgesOnFace.size(); ++i)
		{
			// check if edge is open, if not, skip to next edge
			if (e_to_tri[edgesOnFace[i].edgeIdx].size() >= 2)
			{
				continue;
			}

			for (unsigned int j = i+1; j < edgesOnFace.size(); ++j)
			{
				// check if edge is open, if not, skip to next edge
				if (e_to_tri[edgesOnFace[j].edgeIdx].size() >= 2)
				{
					continue;
				}

				if (shouldStitch(edgesOnFace[i].edgeIdx, edgesOnFace[j].edgeIdx, x, y, z, faceToConsider[k], polygonNeighbourList))
				{
					if (stitch(edgesOnFace[i], edgesOnFace[j], x, y, z, faceToConsider[k], polygonNeighbourList))
					{
						needStitch = true;
						i = -1;
						break;
					}
				}
			}
		}

		if (needStitch)
			++numFaceStitching;
	}
}

// returns true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
//
// edge1Idx (in): edge index 1
// edge2Idx (in): edge index 2
// x,y,z (in):	index of cube where stitching was invoked
// faceIdx (in): face index of where stitching occurs on cube
// polygonNeighbourList (in): polygon neighbour information for source mesh
// return value (out): true if edge 'edge1Idx' and 'edge2Idx' should be stitched up
template <class T> bool TilingNonobtuseMC<T>::shouldStitch(unsigned int edge1Idx, unsigned int edge2Idx, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList)
{
	// setting up quick indexing of adjacent cube
	int adjacentCube[6][3] = {{x,y,z+1}, {x-1,y,z}, {x,y,z-1}, {x+1,y,z}, {x,y+1,z}, {x,y-1,z}};

	// sanity check
	if (e_to_tri[edge1Idx].size() != 1 || e_to_tri[edge2Idx].size() != 1)
		return false;
	
	int tri1Idx = e_to_tri[edge1Idx][0];
	int tri2Idx = e_to_tri[edge2Idx][0];

// 	int patch1No = tri_to_patch[tri1Idx];
// 	int patch2No = tri_to_patch[tri2Idx];

	CubeID helperCube1(x,y,z);
	CubeID helperCube2(adjacentCube[faceIdx][0], adjacentCube[faceIdx][1], adjacentCube[faceIdx][2]);

	// SPECIAL CASE: two edges generated from the same patch within a cube. Only stitch it if the patch is connected to some black patch in the other cube
// 	if (tri_to_CubeID[tri1Idx].x == tri_to_CubeID[tri2Idx].x && tri_to_CubeID[tri1Idx].y == tri_to_CubeID[tri2Idx].y && tri_to_CubeID[tri1Idx].z == tri_to_CubeID[tri2Idx].z && patch1No == patch2No)
// 	{
// 		if (tri_to_CubeID[tri1Idx].x == x && tri_to_CubeID[tri1Idx].y == y && tri_to_CubeID[tri1Idx].z == z)
// 			return isConnected_samePatch(tri_to_CubeID[tri1Idx].x, tri_to_CubeID[tri1Idx].y, tri_to_CubeID[tri1Idx].z, patch1No, helperCube2, polygonNeighbourList);
// 		else
// 			return isConnected_samePatch(tri_to_CubeID[tri1Idx].x, tri_to_CubeID[tri1Idx].y, tri_to_CubeID[tri1Idx].z, patch1No, helperCube1, polygonNeighbourList);
// 	}
	vector<CubeID> &cID1List = tri_to_CubeID[tri1Idx];
	vector<unsigned int> &pNo1List = tri_to_patch[tri1Idx];
	vector<CubeID> &cID2List = tri_to_CubeID[tri2Idx];
	vector<unsigned int> &pNo2List = tri_to_patch[tri2Idx];
	for (unsigned int i = 0; i < cID1List.size(); ++i)
	{
		for (unsigned int j = 0; j < cID2List.size(); ++j)
		{
			// never stitch edges generated from same patch.... for now!
			if (cID1List[i].x == cID2List[j].x && cID1List[i].y == cID2List[j].y && cID1List[i].z == cID2List[j].z && pNo1List[i] == pNo2List[j])
			{
				// stitch edges generated from same patch if and only if there exist a black patch that connects the two edges
				// THIS MAY NOT BE CORRECT CONCEPTUALLY!!!
// 				if (isConnected_samePatch(cID1List[i].x, cID1List[i].y, cID1List[i].z, pNo1List[i], helperCube2, polygonNeighbourList))
// 					return true;
				return false;
			}
			
			if (isConnected(cID1List[i].x, cID1List[i].y, cID1List[i].z, pNo1List[i], 
							cID2List[j].x, cID2List[j].y, cID2List[j].z, pNo2List[j], 
							helperCube1, helperCube2, polygonNeighbourList))
				return true;
		}
	}

	return false;

// 	return isConnected(tri_to_CubeID[tri1Idx].x, tri_to_CubeID[tri1Idx].y, tri_to_CubeID[tri1Idx].z, patch1No, 
// 						tri_to_CubeID[tri2Idx].x, tri_to_CubeID[tri2Idx].y, tri_to_CubeID[tri2Idx].z, patch2No, 
// 						helperCube1, helperCube2, polygonNeighbourList);
}

// returns true if patch is connected with some nontriangulable patch in adjcent cube
// 
// x, y, z (in): index of cube where patch is in
// patchNo (in): patch number
// helperCube (in): index of helper cube
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> bool TilingNonobtuseMC<T>::isConnected_samePatch(unsigned int x, unsigned int y, unsigned int z, unsigned int patchNo, CubeID helperCube, const vector<int>* polygonNeighbourList)
{
	vector<unsigned int> triList;	// list of triangles that are in patchNo
	vector<unsigned int> helperTriList;	// list of triangles that are from 'non-triangulable' patches within the helper cube

	// add all triangles in patch 1
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z + (this->m_nCellsX)*y + x;
	unsigned int numTriInPatch = cubesProperty[cubeIdx].triGroups[patchNo].size();
	for (unsigned int i = 0; i < numTriInPatch; ++i)
	{
		unsigned int idx = cubesProperty[cubeIdx].triGroups[patchNo][i];
		triList.push_back(cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx);
	}

	// add all triangles in patch that does not generate any triangles in helper cube
	cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*helperCube.z + (this->m_nCellsX)*helperCube.y + helperCube.x;
	unsigned int numPatch = cubesProperty[cubeIdx].triGroups.size();
	for (unsigned int j = 0; j < numPatch; ++j)
	{
		//if (cubesProperty[cubeIdx].isConsistent[j] && cubesProperty[cubeIdx].isEmptyTriangulation[j])
		if (cubesProperty[cubeIdx].isEmptyTriangulation[j])
		{
			unsigned int numTriInPatch = cubesProperty[cubeIdx].triGroups[j].size();
			for (unsigned int i = 0; i < numTriInPatch; ++i)
			{
				unsigned int idx = cubesProperty[cubeIdx].triGroups[j][i];
				unsigned int triIdx = cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx;
				
				if (find(helperTriList.begin(), helperTriList.end(), triIdx) == helperTriList.end())
					helperTriList.push_back(triIdx);
			}
		}
	}
	
	unsigned int numHelperTri = helperTriList.size();
	if (numHelperTri == 0)
		return false;

	isOnePatch_aux(triList, helperTriList, polygonNeighbourList, 0, true);
	return (numHelperTri != helperTriList.size());
}

// returns true if patch 1 and patch 2 are connected by a sequence of untriangulable patches
// 
// x1, y1, z1 (in): index of cube where patch 1 is in
// patch1No (in): patch 1
// x2, y2, z2 (in): index of cube where patch 2 is in
// patch2No (in): patch 2
// helperCube1 (in): index of helper cube 1
// helperCube2 (in): index of helper cube 2
// polygonNeighbourList (in): polygon neighbour information for source mesh
template <class T> bool TilingNonobtuseMC<T>::isConnected(unsigned int x1, unsigned int y1, unsigned int z1, unsigned int patch1No, unsigned int x2, unsigned int y2, unsigned int z2, unsigned int patch2No, CubeID helperCube1, CubeID helperCube2, const vector<int>* polygonNeighbourList)
{
	vector<unsigned int> triList;	// list of triangles that are in patch1No or patch2No
	vector<unsigned int> helperTriList;	// list of triangles that are from 'non-triangulable' patches within the same cube

	// add all triangles in patch 1
	unsigned int cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z1 + (this->m_nCellsX)*y1 + x1;
	unsigned int numTriInPatch = cubesProperty[cubeIdx].triGroups[patch1No].size();
	for (unsigned int i = 0; i < numTriInPatch; ++i)
	{
		unsigned int idx = cubesProperty[cubeIdx].triGroups[patch1No][i];
		triList.push_back(cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx);
	}

	// add all triangles in patch 2
	cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*z2 + (this->m_nCellsX)*y2 + x2;
	numTriInPatch = cubesProperty[cubeIdx].triGroups[patch2No].size();
	for (unsigned int i = 0; i < numTriInPatch; ++i)
	{
		unsigned int idx = cubesProperty[cubeIdx].triGroups[patch2No][i];
		unsigned int triIdx = cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx;
		if (find(triList.begin(), triList.end(), triIdx) == triList.end())
			triList.push_back(triIdx);
	}

	// add all triangles in patch that does not generate any triangles in helper cube 1
	cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*helperCube1.z + (this->m_nCellsX)*helperCube1.y + helperCube1.x;
	unsigned int numPatch = cubesProperty[cubeIdx].triGroups.size();
	for (unsigned int j = 0; j < numPatch; ++j)
	{
// 		if (cubesProperty[cubeIdx].isConsistent[j] && cubesProperty[cubeIdx].isEmptyTriangulation[j])
		if (cubesProperty[cubeIdx].isEmptyTriangulation[j])
		{
			unsigned int numTriInPatch = cubesProperty[cubeIdx].triGroups[j].size();
			for (unsigned int i = 0; i < numTriInPatch; ++i)
			{
				unsigned int idx = cubesProperty[cubeIdx].triGroups[j][i];
				unsigned int triIdx = cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx;
				
				if (find(helperTriList.begin(), helperTriList.end(), triIdx) == helperTriList.end())
					helperTriList.push_back(triIdx);
			}
		}
	}

	// add all triangles in patch that does not generate any triangles in cube 2
	cubeIdx = (this->m_nCellsX)*(this->m_nCellsY)*helperCube2.z + (this->m_nCellsX)*helperCube2.y + helperCube2.x;
	numPatch = cubesProperty[cubeIdx].triGroups.size();
	for (unsigned int j = 0; j < numPatch; ++j)
	{
		//if (cubesProperty[cubeIdx].isConsistent[j] && cubesProperty[cubeIdx].isEmptyTriangulation[j])
		if (cubesProperty[cubeIdx].isEmptyTriangulation[j])
		{
			unsigned int numTriInPatch = cubesProperty[cubeIdx].triGroups[j].size();
			for (unsigned int i = 0; i < numTriInPatch; ++i)
			{
				unsigned int idx = cubesProperty[cubeIdx].triGroups[j][i];
				unsigned int triIdx = cubesProperty[cubeIdx].intersectedTriangle[idx].triIdx;
				
				if (find(helperTriList.begin(), helperTriList.end(), triIdx) == helperTriList.end())
					helperTriList.push_back(triIdx);
			}
		}
	}

	return (isOnePatch(triList, helperTriList, polygonNeighbourList));
}

// check if the triangles in checkTriList are all connected by the helper triangles in helperList
//
// checkTriList (in/out): index of triangles to see if one patch can be formed
// helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
// polygonNeighbourList (in): polygon neighbour information for source mesh
// triIdx (in): index of needSortedList array to add to the patch and expand
// return value (out): true if the triangles in needSortedList are all connected into one big patch
template <class T> bool TilingNonobtuseMC<T>::isOnePatch(vector<unsigned int> &checkTriList, vector<unsigned int> &helperList, const vector<int>* polygonNeighbourList)
{
	if (checkTriList.empty())
		return true;

	isOnePatch_aux(checkTriList, helperList, polygonNeighbourList, 0, true);
	return (checkTriList.empty());
}

// check if the triangles in needSortedList are all connected into one big patch
//
// checkTriList (in/out): index of triangles to see if one patch can be formed
// helperList (in/out): index of triangles from non-triangulable patches may help connect the patches
// polygonNeighbourList (in): polygon neighbour information for source mesh
// index (in): index of needSortedList array to add to the patch and expand
// indexFromCheckTriList (in): true if index is for checkTriList
// return value (out): true if the triangles in needSortedList are all connected into one big patch
template <class T> void TilingNonobtuseMC<T>::isOnePatch_aux(vector<unsigned int> &checkTriList, vector<unsigned int> &helperList, const vector<int>* polygonNeighbourList, unsigned int index, bool indexFromCheckTriList)
{
	unsigned int triangleAdded = checkTriList[index];
	if (!indexFromCheckTriList)
		triangleAdded = helperList[index];

	// remove index from checkTriList
	if (!indexFromCheckTriList)
		helperList.erase(helperList.begin() + index);
	else
		checkTriList.erase(checkTriList.begin() + index);

	// expand group by examining the neighbour of triangle just added
	for (vector<int>::const_iterator iter = polygonNeighbourList[triangleAdded].begin(); iter != polygonNeighbourList[triangleAdded].end(); iter++)
	{
		bool foundNbr(false);
		for (unsigned int i = 0; i < checkTriList.size(); ++i) {
			if ((int)checkTriList[i] == *iter) {
				isOnePatch_aux(checkTriList, helperList, polygonNeighbourList, i, true);
				foundNbr = true;
				break;
			}
		}

		if (!foundNbr)
		{
			for (unsigned int i = 0; i < helperList.size(); ++i) {
				if ((int)helperList[i] == *iter) {
					isOnePatch_aux(checkTriList, helperList, polygonNeighbourList, i, false);
					foundNbr = true;
					break;
				}
			}
		}
	}
}

// stitch the two edges e1 and e2 together
//
// e1Info (in): edge info 1
// e2Info (in): edge info 2
// x,y,z (in): index of cube we are stitching at
// faceIdx (in): face index of where stitching occurs
// polygonNeighbourList (in): polygon neighbour information for source mesh
// return value (out): true if stitching successful
template <class T> bool TilingNonobtuseMC<T>::stitch(EdgePosInfo e1Info, EdgePosInfo e2Info, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList)
{
	unsigned int transformEdgePos[6][4] = {{2,1,0,3},{2,1,0,3},{2,1,0,3},{2,1,0,3},{0,3,2,1},{0,3,2,1}};

	Edge e1 = edgeList[e1Info.edgeIdx];	
	Edge e2 = edgeList[e2Info.edgeIdx];

	if (!e1Info.isV1First) {
		swap(e1Info.v1Edge, e1Info.v2Edge);
		swap(e1.vertex1Idx, e1.vertex2Idx);
	}
	if (!e2Info.isV1First) {
		swap(e2Info.v1Edge, e2Info.v2Edge);
		swap(e2.vertex1Idx, e2.vertex2Idx);
	}

	int e1v1 = e1Info.v1Edge;
	int e1v2 = e1Info.v2Edge;
	int e2v1 = e2Info.v1Edge;
	int e2v2 = e2Info.v2Edge;

	// flip v1Edge and v2Edge if edge was recorded at the adjacent cube
	bool e1InCube(true);
	CubeID c = tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]][0];
	if (c.x != x || c.y != y || c.z != z)
	{
		e1v1 = transformEdgePos[faceIdx][e1v1];
		e1v2 = transformEdgePos[faceIdx][e1v2];
		e1InCube = false;
	}

	c = tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]][0];
	if (c.x != x || c.y != y || c.z != z)
	{
		e2v1 = transformEdgePos[faceIdx][e2v1];
		e2v2 = transformEdgePos[faceIdx][e2v2];
	}

	// sanity check
	if (e1v1 == -1 || e1v2 == -1 || e2v1 == -1 || e2v2 == -1)
	{
		cout << "ERROR: TilingNonobtuseMC::stitch() - conflict in edge indices" << endl;
		return false;
	}

	if (e1v1 == e2v2 && e1v2 == e2v1)	// edges have same orientation; stitch both ends
	{
		int newv1Idx = stitchVertices(e1.vertex1Idx, e2.vertex2Idx);
		if (newv1Idx == -1)
			return false;
		int newv2Idx = stitchVertices(e1.vertex2Idx, e2.vertex1Idx);
		if (newv2Idx == -1)
			return false;

		// remove the redundant edges
		edgeList.erase(e1Info.edgeIdx);
		edgeList.erase(e2Info.edgeIdx);
		VPAIR vp;
		vp.first = newv1Idx;
		vp.second = newv2Idx;
		if (vp.first > vp.second)
			swap(vp.first, vp.second);
		vpair_to_e.erase(vp);

		unsigned int tri1Idx = e_to_tri[e1Info.edgeIdx][0];
		unsigned int tri2Idx = e_to_tri[e2Info.edgeIdx][0];
		e_to_tri.erase(e1Info.edgeIdx);
		e_to_tri.erase(e2Info.edgeIdx);

		// remove e_on_f reference
		unsigned int onFaceIdx = e_to_face[e1Info.edgeIdx];
		vector<EdgePosInfo>::iterator iter = find(e_on_f[onFaceIdx].begin(), e_on_f[onFaceIdx].end(), e1Info);
		if (iter != e_on_f[onFaceIdx].end())
			e_on_f[onFaceIdx].erase(iter);
		e_to_face.erase(e1Info.edgeIdx);

		onFaceIdx = e_to_face[e2Info.edgeIdx];
		iter = find(e_on_f[onFaceIdx].begin(), e_on_f[onFaceIdx].end(), e2Info);
		if (iter != e_on_f[onFaceIdx].end())
			e_on_f[onFaceIdx].erase(iter);
		e_to_face.erase(e2Info.edgeIdx);

		// add new edge
		Edge newEdge;
		newEdge.vertex1Idx = newv1Idx;
		newEdge.vertex2Idx = newv2Idx;
		EdgePosInfo epi;
		epi.v1Edge = e1v1;
		epi.v2Edge = e1v2;
		// make sure v1Edge and v2Edge are recorded respect to the cube where it contains the triangle in the first entry of e_to_tri
		if (tri_to_CubeID[tri1Idx][0].x != x ||
			tri_to_CubeID[tri1Idx][0].y != y ||
			tri_to_CubeID[tri1Idx][0].z != z)
		{
			epi.v1Edge = transformEdgePos[faceIdx][e1v1];
			epi.v2Edge = transformEdgePos[faceIdx][e1v2];
		}

		epi.isV1First = true;
		if (!e1InCube)
		{
			swap(newEdge.vertex1Idx, newEdge.vertex2Idx);
			swap(epi.v1Edge, epi.v2Edge);
			epi.isV1First = false;
		}

		if (newEdge.vertex1Idx > newEdge.vertex2Idx) {
			swap(newEdge.vertex1Idx, newEdge.vertex2Idx);
			swap(epi.v1Edge, epi.v2Edge);
			epi.isV1First = false;
		}

		unsigned int newEdgeIdx = nextUnusedEdgeID;
		++nextUnusedEdgeID;
		epi.edgeIdx = newEdgeIdx;
		edgeList.insert(ID2EDGE::value_type(newEdgeIdx, newEdge));
		
		v_to_e[newEdge.vertex1Idx].push_back(newEdgeIdx);
		v_to_e[newEdge.vertex2Idx].push_back(newEdgeIdx);

		vp.first = newEdge.vertex1Idx;
		vp.second = newEdge.vertex2Idx;
		vpair_to_e.insert(VPAIRTOE::value_type(vp, newEdgeIdx));

		vector<unsigned int> adjTriList(2);
		adjTriList[0] =	tri1Idx;
		adjTriList[1] = tri2Idx;
		e_to_tri.insert(ETOTRI::value_type(newEdgeIdx, adjTriList));

		e_on_f[onFaceIdx].push_back(epi);
		e_to_face.insert(ETOFACE::value_type(newEdgeIdx, onFaceIdx));

	}
	else if (e1v1 == e2v2)				// one end is stitched, then triangulate
	{
		int vertexID = stitchVertices(e1.vertex1Idx, e2.vertex2Idx);
		if (vertexID == -1)
			return false;

		// create triangle to cover hole
		TRIANGLE triangle;
		triangle.pointID[0] = vertexID;
		triangle.pointID[1] = e2.vertex1Idx;
		triangle.pointID[2] = e1.vertex2Idx;

		// record info for the new triangle
		vector<CubeID> cIDList;
		vector<unsigned int> pNoList;
		for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
			cIDList.push_back(tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
			cIDList.push_back(tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
			pNoList.push_back(tri_to_patch[e_to_tri[e1Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
			pNoList.push_back(tri_to_patch[e_to_tri[e2Info.edgeIdx][0]][i]);
// 		CubeID cID = tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]];
// 		unsigned int pNo = tri_to_patch[e_to_tri[e1Info.edgeIdx][0]];
// 		CubeID cID2 = tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]];
// 		unsigned int pNo2 = tri_to_patch[e_to_tri[e2Info.edgeIdx][0]];

		for (unsigned int j = 0; j < 3; ++j)
		{
			// add triangle reference to vertices
			v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

			// add e-to-tri references
			VPAIR vp;
			vp.first = triangle.pointID[j];
			vp.second = triangle.pointID[(j+1)%3];
			if (vp.first > vp.second)
				swap(vp.first, vp.second);

			VPAIRTOE::iterator iter = vpair_to_e.find(vp);
			if (iter != vpair_to_e.end())
			{
				unsigned int currentEdgeID = iter->second;
				e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());
			}
		}

		// add the new edge
		Edge e;
		e.vertex1Idx = e2.vertex1Idx;
		e.vertex2Idx = e1.vertex2Idx;
		EdgePosInfo epi;
		epi.v1Edge = e2v1;
		epi.v2Edge = e1v2;
		// make sure v1Edge and v2Edge are recorded respect to the cube where it contains the triangle in the first entry of e_to_tri
		if (cIDList[0].x != x || cIDList[0].y != y || cIDList[0].z != z)
		{
			epi.v1Edge = transformEdgePos[faceIdx][e2v1];
			epi.v2Edge = transformEdgePos[faceIdx][e1v2];
		}

		epi.isV1First = true;
		if (e.vertex1Idx > e.vertex2Idx) {
			swap(e.vertex1Idx, e.vertex2Idx);
			swap(epi.v1Edge, epi.v2Edge);
			epi.isV1First = false;
		}

		unsigned int newEdgeIdx = nextUnusedEdgeID;
		++nextUnusedEdgeID;
		epi.edgeIdx = newEdgeIdx;
		edgeList.insert(ID2EDGE::value_type(newEdgeIdx, e));

		VPAIR vp;
		vp.first = e.vertex1Idx;
		vp.second = e.vertex2Idx;
		vpair_to_e.insert(VPAIRTOE::value_type(vp, newEdgeIdx));

		// add v_to_e references		
		v_to_e[e.vertex1Idx].push_back(newEdgeIdx);
		v_to_e[e.vertex2Idx].push_back(newEdgeIdx);

		// add e_to_tri references
		vector<unsigned int> adjTriList(1,this->m_trivecTriangles.size());
		e_to_tri.insert(ETOTRI::value_type(newEdgeIdx, adjTriList));

		// add e_to_tri references for the two existing edges
		vp.first = vertexID;
		vp.second = e2.vertex1Idx;
		if (vp.first > vp.second)
			swap(vp.first, vp.second);
		e_to_tri[vpair_to_e[vp]].push_back(this->m_trivecTriangles.size());

		vp.first = e1.vertex2Idx;
		vp.second = vertexID;
		if (vp.first > vp.second)
			swap(vp.first, vp.second);
		e_to_tri[vpair_to_e[vp]].push_back(this->m_trivecTriangles.size());

		// add e_on_f references for new edge
		unsigned int onFaceIdx = e_to_face[e1Info.edgeIdx];
		e_on_f[onFaceIdx].push_back(epi);
		e_to_face.insert(ETOFACE::value_type(newEdgeIdx, onFaceIdx));
		
		// add triangle to triangle list
		this->m_trivecTriangles.push_back(triangle);
		
		// add tri-to-cube reference
		tri_to_CubeID.push_back(cIDList);
		tri_to_patch.push_back(pNoList);

		// stitch new edge that may overlap with existing ones
		stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList);
// 		if (!stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList))
// 		{
// 			// if it doesn't work the first try, then try to associate the other patch to the new triangle, and try again
// 			vector<EdgePosInfo>::iterator iter = find(e_on_f[onFaceIdx].begin(), e_on_f[onFaceIdx].end(), epi);
// 			if (iter != e_on_f[onFaceIdx].end())
// 			{
// 				if (cID.x != x || cID.y != y || cID.z != z)
// 				{
// 					// convert coordinate back w.r.t current cube
// 					iter->v1Edge = transformEdgePos[faceIdx][iter->v1Edge];
// 					iter->v2Edge = transformEdgePos[faceIdx][iter->v2Edge];
// 				}
// 
// 				if (cID2.x != x || cID2.y != y || cID2.z != z)
// 				{
// 					// convert coordinate w.r.t associated cube
// 					iter->v1Edge = transformEdgePos[faceIdx][iter->v1Edge];
// 					iter->v2Edge = transformEdgePos[faceIdx][iter->v2Edge];
// 				}
// 
// 				tri_to_CubeID[tri_to_CubeID.size()-1] = cID2;
// 				tri_to_patch[tri_to_patch.size()-1] = pNo2;
// 				stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList);
// 			}
// 		}
	}
	else if (e1v2 == e2v1)				// the other end is stitched, then triangulate
	{	
		int vertexID = stitchVertices(e1.vertex2Idx, e2.vertex1Idx);
		if (vertexID == -1)
			return false;

		// record info for the new triangle
		vector<CubeID> cIDList;
		vector<unsigned int> pNoList;
		for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
			cIDList.push_back(tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
			cIDList.push_back(tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
			pNoList.push_back(tri_to_patch[e_to_tri[e1Info.edgeIdx][0]][i]);
		for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
			pNoList.push_back(tri_to_patch[e_to_tri[e2Info.edgeIdx][0]][i]);
// 		CubeID cID = tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]];
// 		unsigned int pNo = tri_to_patch[e_to_tri[e1Info.edgeIdx][0]];
// 		CubeID cID2 = tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]];
// 		unsigned int pNo2 = tri_to_patch[e_to_tri[e2Info.edgeIdx][0]];

		// create triangle to cover hole
		TRIANGLE triangle;
		triangle.pointID[0] = vertexID;
		triangle.pointID[1] = e1.vertex1Idx;
		triangle.pointID[2] = e2.vertex2Idx;

		for (unsigned int j = 0; j < 3; ++j)
		{
			// add triangle reference to vertices
			v_to_tri[triangle.pointID[j]].push_back(this->m_trivecTriangles.size());

			// add e-to-tri references
			VPAIR vp;
			vp.first = triangle.pointID[j];
			vp.second = triangle.pointID[(j+1)%3];
			if (vp.first > vp.second)
				swap(vp.first, vp.second);

			VPAIRTOE::iterator iter = vpair_to_e.find(vp);
			if (iter != vpair_to_e.end())
			{
				unsigned int currentEdgeID = iter->second;
				e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());
			}
		}

		// add the new edge
		Edge e;
		e.vertex1Idx = e1.vertex1Idx;
		e.vertex2Idx = e2.vertex2Idx;
		EdgePosInfo epi;
		epi.v1Edge = e1v1;
		epi.v2Edge = e2v2;
		// make sure v1Edge and v2Edge are recorded respect to the cube where it contains the triangle in the first entry of e_to_tri
		if (cIDList[0].x != x || cIDList[0].y != y || cIDList[0].z != z)
		{
			epi.v1Edge = transformEdgePos[faceIdx][e1v1];
			epi.v2Edge = transformEdgePos[faceIdx][e2v2];
		}
		epi.isV1First = true;
		if (e.vertex1Idx > e.vertex2Idx) {
			swap(e.vertex1Idx, e.vertex2Idx);
			swap(epi.v1Edge, epi.v2Edge);
			epi.isV1First = false;
		}

		unsigned int newEdgeIdx = nextUnusedEdgeID;
		++nextUnusedEdgeID;
		epi.edgeIdx = newEdgeIdx;
		edgeList.insert(ID2EDGE::value_type(newEdgeIdx, e));

		VPAIR vp;
		vp.first = e.vertex1Idx;
		vp.second = e.vertex2Idx;
		vpair_to_e.insert(VPAIRTOE::value_type(vp, newEdgeIdx));

		// add v_to_e references		
		v_to_e[e.vertex1Idx].push_back(newEdgeIdx);
		v_to_e[e.vertex2Idx].push_back(newEdgeIdx);

		// add e_to_tri references
		vector<unsigned int> adjTriList(1,this->m_trivecTriangles.size());
		e_to_tri.insert(ETOTRI::value_type(newEdgeIdx, adjTriList));

		vp.first = vertexID;
		vp.second = e1.vertex1Idx;
		if (vp.first > vp.second)
			swap(vp.first, vp.second);
		e_to_tri[vpair_to_e[vp]].push_back(this->m_trivecTriangles.size());

		vp.first = e2.vertex2Idx;
		vp.second = vertexID;
		if (vp.first > vp.second)
			swap(vp.first, vp.second);
		e_to_tri[vpair_to_e[vp]].push_back(this->m_trivecTriangles.size());

		// add e_on_f references
		unsigned int onFaceIdx = e_to_face[e1Info.edgeIdx];
		e_on_f[onFaceIdx].push_back(epi);
		e_to_face.insert(ETOFACE::value_type(newEdgeIdx, onFaceIdx));

		// add triangle to triangle list
		this->m_trivecTriangles.push_back(triangle);

		// add tri-to-cube reference
		tri_to_CubeID.push_back(cIDList);
		tri_to_patch.push_back(pNoList);

		// stitch new edge that may overlap with existing ones
		stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList);
// 		if (!stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList))
// 		{
// 			// if it doesn't work the first try, then try to associate the other patch to the new triangle, and try again
// 			vector<EdgePosInfo>::iterator iter = find(e_on_f[onFaceIdx].begin(), e_on_f[onFaceIdx].end(), epi);
// 			if (iter != e_on_f[onFaceIdx].end())
// 			{
// 				if (cID.x != x || cID.y != y || cID.z != z)
// 				{
// 					// convert coordinate back w.r.t current cube
// 					iter->v1Edge = transformEdgePos[faceIdx][iter->v1Edge];
// 					iter->v2Edge = transformEdgePos[faceIdx][iter->v2Edge];
// 				}
// 
// 				if (cID2.x != x || cID2.y != y || cID2.z != z)
// 				{
// 					// convert coordinate w.r.t associated cube
// 					iter->v1Edge = transformEdgePos[faceIdx][iter->v1Edge];
// 					iter->v2Edge = transformEdgePos[faceIdx][iter->v2Edge];
// 				}
// 
// 				tri_to_CubeID[tri_to_CubeID.size()-1] = cID2;
// 				tri_to_patch[tri_to_patch.size()-1] = pNo2;
// 				stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList);
// 			}
// 		}
	}
	else if (e1v1 != e2v1 && e1v1 != e2v2 && e1v2 != e2v1 && e1v2 != e2v2)	// edges are parallel; triangulate
	{
		if (((e1v1 + e2v1)%2 == 0) && ((e1v2 + e2v2)%2 == 0))	// to check for consistent facing
		{
			// record info for the new triangle
			vector<CubeID> cIDList;
			vector<unsigned int> pNoList;
			for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
				cIDList.push_back(tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]][i]);
			for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e1Info.edgeIdx][0]].size(); ++i)
				pNoList.push_back(tri_to_patch[e_to_tri[e1Info.edgeIdx][0]][i]);
			for (unsigned int i = 0; i < tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
				cIDList.push_back(tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]][i]);
			for (unsigned int i = 0; i < tri_to_patch[e_to_tri[e2Info.edgeIdx][0]].size(); ++i)
				pNoList.push_back(tri_to_patch[e_to_tri[e2Info.edgeIdx][0]][i]);
// 			CubeID cID = tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]];
// 			unsigned int pNo = tri_to_patch[e_to_tri[e1Info.edgeIdx][0]];

			// create triangle to cover hole
			TRIANGLE triangle1;
			triangle1.pointID[0] = e1.vertex2Idx;
			triangle1.pointID[1] = e1.vertex1Idx;
			triangle1.pointID[2] = e2.vertex1Idx;

			for (unsigned int j = 0; j < 3; ++j)
			{
				// add triangle reference to vertices
				v_to_tri[triangle1.pointID[j]].push_back(this->m_trivecTriangles.size());
	
				// add e-to-tri references
				VPAIR vp;
				vp.first = triangle1.pointID[j];
				vp.second = triangle1.pointID[(j+1)%3];
				if (vp.first > vp.second)
					swap(vp.first, vp.second);
	
				VPAIRTOE::iterator iter = vpair_to_e.find(vp);
				if (iter != vpair_to_e.end())
				{
					unsigned int currentEdgeID = iter->second;
					e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());
				}
			}
	
			// add the new edges
			EdgePosInfo tocheckOverlapStiching;
			for (unsigned int k = 0; k < 2; ++k)
			{
				unsigned int v1Idx = e1.vertex1Idx;
				unsigned int v2Idx = e2.vertex1Idx;
				unsigned int v1Edge = e1v1;
				unsigned int v2Edge = e2v1;
				if (k == 1)
				{
					v1Idx = e2.vertex1Idx;
					v2Idx = e1.vertex2Idx;
					v1Edge = e2v1;
					v2Edge = e1v2;
				}

				Edge e;
				e.vertex1Idx = v1Idx;
				e.vertex2Idx = v2Idx;
				EdgePosInfo epi;
				epi.v1Edge = v1Edge;
				epi.v2Edge = v2Edge;
				// make sure v1Edge and v2Edge are recorded respect to the cube where it contains the triangle in the first entry of e_to_tri
				if (cIDList[0].x != x || cIDList[0].y != y || cIDList[0].z != z)
				{
					epi.v1Edge = transformEdgePos[faceIdx][epi.v1Edge];
					epi.v2Edge = transformEdgePos[faceIdx][epi.v2Edge];
				}

				epi.isV1First = true;
				if (e.vertex1Idx > e.vertex2Idx) {
					swap(e.vertex1Idx, e.vertex2Idx);
					swap(epi.v1Edge, epi.v2Edge);
					epi.isV1First = false;
				}
		
				unsigned int newEdgeIdx = nextUnusedEdgeID;
				++nextUnusedEdgeID;
				epi.edgeIdx = newEdgeIdx;
				edgeList.insert(ID2EDGE::value_type(newEdgeIdx, e));
		
				VPAIR vp;
				vp.first = e.vertex1Idx;
				vp.second = e.vertex2Idx;
				vpair_to_e.insert(VPAIRTOE::value_type(vp, newEdgeIdx));
		
				// add v_to_e references		
				v_to_e[e.vertex1Idx].push_back(newEdgeIdx);
				v_to_e[e.vertex2Idx].push_back(newEdgeIdx);
		
				// add e_to_tri references
				vector<unsigned int> adjTriList(1,this->m_trivecTriangles.size());
				e_to_tri.insert(ETOTRI::value_type(newEdgeIdx, adjTriList));
		
				// add e_on_f references
				unsigned int onFaceIdx = e_to_face[e1Info.edgeIdx];
				e_on_f[onFaceIdx].push_back(epi);
				e_to_face.insert(ETOFACE::value_type(newEdgeIdx, onFaceIdx));

				if (k == 1)
					tocheckOverlapStiching = epi;
			}
	
			// add triangle to triangle list
			this->m_trivecTriangles.push_back(triangle1);

			// add tri-to-cube reference
			tri_to_CubeID.push_back(cIDList);
			tri_to_patch.push_back(pNoList);

			// save the face index for later use
			unsigned int saveOnFaceIdx = e_to_face[e1Info.edgeIdx];

			stitchOverlappingEdges(tocheckOverlapStiching, x, y, z, faceIdx, polygonNeighbourList);

			//----------- begin triangle 2---------------
			// ====================================================================================================================
			// make sure we get the most updated vIdx from the edges; note vIdx may changed due to stitchOverlappingEdges(), so don't use a copied version
			// ====================================================================================================================
			vector<EdgePosInfo>::iterator iter = find(e_on_f[saveOnFaceIdx].begin(), e_on_f[saveOnFaceIdx].end(), e1Info);
			if (iter != e_on_f[saveOnFaceIdx].end())
				e1Info = *iter;
			iter = find(e_on_f[saveOnFaceIdx].begin(), e_on_f[saveOnFaceIdx].end(), e2Info);
			if (iter != e_on_f[saveOnFaceIdx].end())
				e2Info = *iter;
			Edge e1 = edgeList[e1Info.edgeIdx];	
			Edge e2 = edgeList[e2Info.edgeIdx];
			if (!e1Info.isV1First) {
				swap(e1Info.v1Edge, e1Info.v2Edge);
				swap(e1.vertex1Idx, e1.vertex2Idx);
			}
			if (!e2Info.isV1First) {
				swap(e2Info.v1Edge, e2Info.v2Edge);
				swap(e2.vertex1Idx, e2.vertex2Idx);
			}
			e1v1 = e1Info.v1Edge;
			e1v2 = e1Info.v2Edge;
			e2v1 = e2Info.v1Edge;
			e2v2 = e2Info.v2Edge;
		
			// flip v1Edge and v2Edge if edge was recorded at the adjacent cube
			CubeID c = tri_to_CubeID[e_to_tri[e1Info.edgeIdx][0]][0];
			if (c.x != x || c.y != y || c.z != z)
			{
				e1v1 = transformEdgePos[faceIdx][e1v1];
				e1v2 = transformEdgePos[faceIdx][e1v2];
			}
			c = tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]][0];
			if (c.x != x || c.y != y || c.z != z)
			{
				e2v1 = transformEdgePos[faceIdx][e2v1];
				e2v2 = transformEdgePos[faceIdx][e2v2];
			}
			// ====================================================================================================================
			// ====================================================================================================================

			// record info for the new triangle
// 			cID = tri_to_CubeID[e_to_tri[e2Info.edgeIdx][0]];
// 			pNo = tri_to_patch[e_to_tri[e2Info.edgeIdx][0]];
			
			// add 2nd triangle
			TRIANGLE triangle2;
			triangle2.pointID[0] = e2.vertex2Idx;
			triangle2.pointID[1] = e2.vertex1Idx;
			triangle2.pointID[2] = e1.vertex1Idx;
			
			for (unsigned int j = 0; j < 3; ++j)
			{
				// add triangle reference to vertices
				v_to_tri[triangle2.pointID[j]].push_back(this->m_trivecTriangles.size());
	
				// add e-to-tri references
				VPAIR vp;
				vp.first = triangle2.pointID[j];
				vp.second = triangle2.pointID[(j+1)%3];
				if (vp.first > vp.second)
					swap(vp.first, vp.second);
	
				VPAIRTOE::iterator iter = vpair_to_e.find(vp);
				if (iter != vpair_to_e.end())
				{
					unsigned int currentEdgeID = iter->second;
					e_to_tri[currentEdgeID].push_back(this->m_trivecTriangles.size());
				}
			}
	
			// add the new edge
			unsigned int v1Idx = e1.vertex1Idx;
			unsigned int v2Idx = e2.vertex2Idx;
			unsigned int v1Edge = e1v1;
			unsigned int v2Edge = e2v2;

			Edge e;
			e.vertex1Idx = v1Idx;
			e.vertex2Idx = v2Idx;
			EdgePosInfo epi;
			epi.v1Edge = v1Edge;
			epi.v2Edge = v2Edge;
			// make sure v1Edge and v2Edge are recorded respect to the cube where it contains the triangle in the first entry of e_to_tri
			if (cIDList[0].x != x || cIDList[0].y != y || cIDList[0].z != z)
			{
				epi.v1Edge = transformEdgePos[faceIdx][epi.v1Edge];
				epi.v2Edge = transformEdgePos[faceIdx][epi.v2Edge];
			}

			epi.isV1First = true;
			if (e.vertex1Idx > e.vertex2Idx) {
				swap(e.vertex1Idx, e.vertex2Idx);
				swap(epi.v1Edge, epi.v2Edge);
				epi.isV1First = false;
			}
	
			unsigned int newEdgeIdx = nextUnusedEdgeID;
			++nextUnusedEdgeID;
			epi.edgeIdx = newEdgeIdx;
			edgeList.insert(ID2EDGE::value_type(newEdgeIdx, e));
	
			VPAIR vp;
			vp.first = e.vertex1Idx;
			vp.second = e.vertex2Idx;
			vpair_to_e.insert(VPAIRTOE::value_type(vp, newEdgeIdx));
	
			// add v_to_e references		
			v_to_e[e.vertex1Idx].push_back(newEdgeIdx);
			v_to_e[e.vertex2Idx].push_back(newEdgeIdx);
	
			// add e_to_tri references
			vector<unsigned int> adjTriList(1,this->m_trivecTriangles.size());
			e_to_tri.insert(ETOTRI::value_type(newEdgeIdx, adjTriList));
	
			// add e_on_f references
			unsigned int onFaceIdx = e_to_face[e1Info.edgeIdx];
			e_on_f[onFaceIdx].push_back(epi);
			e_to_face.insert(ETOFACE::value_type(newEdgeIdx, onFaceIdx));

			// add triangle to triangle list
			this->m_trivecTriangles.push_back(triangle2);

			// add tri-to-cube reference
			tri_to_CubeID.push_back(cIDList);
			tri_to_patch.push_back(pNoList);

			stitchOverlappingEdges(epi, x, y, z, faceIdx, polygonNeighbourList);
		}
 		else
		{
// 			cout << "WARNING: TilingNonobtuseMC::stitch() - degenerate cases in stitching" << endl;
			return false;
		}
	}
	else		// degenerate cases
	{
// 		cout << "WARNING: TilingNonobtuseMC::stitch() - degenerate cases in stitching" << endl;
		return false;
	}
	
	return true;
}

// Stitch overlapping edges; only call from stitch() to ensure we dont' introduce many overlapping vertices and run into infinite loops
//
// eInfo (in): info of edge that we want to stitch up with
// x,y,z (in):	index of cube where stitching was invoked
// faceIdx (in): face index of where stitching occurs on cube
// polygonNeighbourList (in): polygon neighbour information for source mesh
// return value (out): true if edge is stitched
template <class T> bool TilingNonobtuseMC<T>::stitchOverlappingEdges(EdgePosInfo eInfo, unsigned int x, unsigned int y, unsigned int z, unsigned int faceIdx, const vector<int>* polygonNeighbourList)
{
	unsigned int transformEdgePos[6][4] = {{2,1,0,3},{2,1,0,3},{2,1,0,3},{2,1,0,3},{0,3,2,1},{0,3,2,1}};
	vector<EdgePosInfo> &edgesOnFace = e_on_f[GetFaceID(x, y, z, faceIdx)];

	for (unsigned int j = 0; j < edgesOnFace.size() - 1; ++j)	// last one is eInfo, so no need to check
	{
		// check if edge is open, if not, skip to next edge
		if (e_to_tri[edgesOnFace[j].edgeIdx].size() >= 2)
		{
			continue;
		}

		int eiv1 = eInfo.v1Edge;
		int eiv2 = eInfo.v2Edge;
		if (!eInfo.isV1First)
			swap(eiv1,eiv2);
		int ejv1 = edgesOnFace[j].v1Edge;
		int ejv2 = edgesOnFace[j].v2Edge;
		if (!edgesOnFace[j].isV1First)
			swap(ejv1,ejv2);


// 		// flip v1Edge and v2Edge if edge was recorded at the adjacent cube
// 		CubeID c = tri_to_CubeID[e_to_tri[eInfo.edgeIdx][0]];
// 		if (c.x != x || c.y != y || c.z != z)
// 		{
// 			eiv1 = transformEdgePos[faceIdx][eiv1];
// 			eiv2 = transformEdgePos[faceIdx][eiv2];
// 		}
// 		c = tri_to_CubeID[e_to_tri[edgesOnFace[j].edgeIdx][0]];
// 		if (c.x != x || c.y != y || c.z != z)
// 		{
// 			ejv1 = transformEdgePos[faceIdx][ejv1];
// 			ejv2 = transformEdgePos[faceIdx][ejv2];
// 		}
// 		
// 		if (!(eiv1 == ejv2 && eiv2 == ejv1))	// only consider edges that are overlapping
// 			continue;

		vector<CubeID> &cID1List = tri_to_CubeID[e_to_tri[eInfo.edgeIdx][0]];
		vector<CubeID> &cID2List = tri_to_CubeID[e_to_tri[edgesOnFace[j].edgeIdx][0]];

		// both edges are generated from stitching, they shouldn't need to be stitched together
		if (cID1List.size() > 1 && cID2List.size() > 1)
			continue;

		for (unsigned int i = 0; i < cID1List.size(); ++i)
		{
			CubeID c1 = cID1List[i];
			if (c1.x != x || c1.y != y || c1.z != z)
			{
				eiv1 = transformEdgePos[faceIdx][eiv1];
				eiv2 = transformEdgePos[faceIdx][eiv2];
			}

			for (unsigned int k = 0; k < cID2List.size(); ++k)
			{
				CubeID c2 = cID2List[k];
				if (c2.x != x || c2.y != y || c2.z != z)
				{
					ejv1 = transformEdgePos[faceIdx][ejv1];
					ejv2 = transformEdgePos[faceIdx][ejv2];
				}

				if (eiv1 == ejv2 && eiv2 == ejv1)	// only consider edges that are overlapping
				{
					if (shouldStitch(eInfo.edgeIdx, edgesOnFace[j].edgeIdx, x, y, z, faceIdx, polygonNeighbourList))
					{
						if (stitch(eInfo, edgesOnFace[j], x, y, z, faceIdx, polygonNeighbourList))
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// join vertex v1Idx and v2Idx together
//
// v1Idx (in): index of vertex 1
// v2Idx (in): index of vertex 2
// return value (out): vertex ID of the joined vertex
template <class T> unsigned int TilingNonobtuseMC<T>::stitchVertices(unsigned int v1Idx, unsigned int v2Idx)
{
	unsigned int newvIdx = v1Idx;
	if (v1Idx != v2Idx)
	{
		if (!canVerticesJoin(v1Idx, v2Idx))
			return -1;

		POINT3DID pt = this->m_i2pt3idVertices[v1Idx];
		// erase v1
		this->m_i2pt3idVertices.erase(v1Idx);
		// erase v2
		this->m_i2pt3idVertices.erase(v2Idx);

		// add merged vertex with new vID
		unsigned int vertexID = nextUnusedVertexID;
		++nextUnusedVertexID;
		newvIdx = vertexID;
		this->m_i2pt3idVertices.insert(ID2POINT3DID::value_type(vertexID, pt));

		vector<unsigned int> adjTriList;
		// update v1 reference in triangle
		for (vector<unsigned int>::iterator iter = v_to_tri[v1Idx].begin(); iter != v_to_tri[v1Idx].end(); iter++)
		{
			unsigned int triID = *iter;
			adjTriList.push_back(triID);
			for (unsigned int i = 0; i < 3; ++i) {
				if (this->m_trivecTriangles[triID].pointID[i] == v1Idx) {
					this->m_trivecTriangles[triID].pointID[i] = vertexID;
					break;
				}
			}
		}
		v_to_tri.erase(v1Idx);

		// update v2 reference in triangle
		for (vector<unsigned int>::iterator iter = v_to_tri[v2Idx].begin(); iter != v_to_tri[v2Idx].end(); iter++)
		{
			unsigned int triID = *iter;
			if (find(adjTriList.begin(), adjTriList.end(), triID) == adjTriList.end())
				adjTriList.push_back(triID);
			for (unsigned int i = 0; i < 3; ++i) {
				if (this->m_trivecTriangles[triID].pointID[i] == v2Idx) {
					this->m_trivecTriangles[triID].pointID[i] = vertexID;
					break;
				}
			}
		}
		v_to_tri.erase(v2Idx);

		v_to_tri.insert(VTOTRI::value_type(vertexID, adjTriList));

		// update v1_to_e references
		vector<unsigned int> adjEdgeList;
		for (vector<unsigned int>::iterator iter = v_to_e[v1Idx].begin(); iter != v_to_e[v1Idx].end(); iter++)
		{
			// update edges
			Edge &e = edgeList[*iter];
			VPAIR vp;
			vp.first = e.vertex1Idx;
			vp.second = e.vertex2Idx;
			vpair_to_e.erase(vp);

			// do not add duplicate edges
			if (find(adjEdgeList.begin(), adjEdgeList.end(), *iter) == adjEdgeList.end())
				adjEdgeList.push_back(*iter);

			if (e.vertex1Idx == (int)v1Idx) 
				e.vertex1Idx = vertexID;
			else if (e.vertex2Idx == (int)v1Idx) 
				e.vertex2Idx = vertexID;
			
			EdgePosInfo eInfo;
			eInfo.edgeIdx = *iter;
			vector<EdgePosInfo>::iterator iter2 = find(e_on_f[e_to_face[*iter]].begin(), e_on_f[e_to_face[*iter]].end(), eInfo);
		
			if (e.vertex1Idx > e.vertex2Idx) {
				swap(e.vertex1Idx, e.vertex2Idx);
				swap(iter2->v1Edge, iter2->v2Edge);
				iter2->isV1First = !iter2->isV1First;
			}

			vp.first = e.vertex1Idx;
			vp.second = e.vertex2Idx;
			vpair_to_e.insert(VPAIRTOE::value_type(vp,*iter));
		}
		v_to_e.erase(v1Idx);

		// update v2_to_e references
		for (vector<unsigned int>::iterator iter = v_to_e[v2Idx].begin(); iter != v_to_e[v2Idx].end(); iter++)
		{
			// update edges
			Edge &e = edgeList[*iter];
			VPAIR vp;
			vp.first = e.vertex1Idx;
			vp.second = e.vertex2Idx;
			vpair_to_e.erase(vp);

			// do not add duplicate edges
			if (find(adjEdgeList.begin(), adjEdgeList.end(), *iter) == adjEdgeList.end())
				adjEdgeList.push_back(*iter);

			if (e.vertex1Idx == (int)v2Idx) 
				e.vertex1Idx = vertexID;
			else if (e.vertex2Idx == (int)v2Idx) 
				e.vertex2Idx = vertexID;
			
			EdgePosInfo eInfo;
			eInfo.edgeIdx = *iter;
			vector<EdgePosInfo>::iterator iter2 = find(e_on_f[e_to_face[*iter]].begin(), e_on_f[e_to_face[*iter]].end(), eInfo);
		
			if (e.vertex1Idx > e.vertex2Idx) {
				swap(e.vertex1Idx, e.vertex2Idx);
				swap(iter2->v1Edge, iter2->v2Edge);
				iter2->isV1First = !iter2->isV1First;
			}

			vp.first = e.vertex1Idx;
			vp.second = e.vertex2Idx;
			vpair_to_e.insert(VPAIRTOE::value_type(vp,*iter));
		}
		v_to_e.erase(v2Idx);
		v_to_e.insert(VTOE::value_type(vertexID, adjEdgeList));

		unsigned int edgeID = v_to_eOfCube[v1Idx];
		vector<int>::iterator iter = find(v_on_e[edgeID].begin(), v_on_e[edgeID].end(), (int)v1Idx);
		if (iter != v_on_e[edgeID].end())
			v_on_e[edgeID].erase(iter);
		iter = find(v_on_e[edgeID].begin(), v_on_e[edgeID].end(), (int)v2Idx);
		if (iter != v_on_e[edgeID].end())
			v_on_e[edgeID].erase(iter);

		v_to_eOfCube.erase(v1Idx);
		v_to_eOfCube.erase(v2Idx);

		v_on_e[edgeID].push_back(vertexID);
		v_to_eOfCube.insert(VTOEOFCUBE::value_type(vertexID, edgeID));
	}

	return newvIdx;
}

// check if vertex v1Idx and v2Idx can be joined together
//
// v1Idx (in): index of vertex 1
// v2Idx (in): index of vertex 2
// return value (out): true if can join
template <class T> unsigned int TilingNonobtuseMC<T>::canVerticesJoin(unsigned int v1Idx, unsigned int v2Idx)
{
	vector<unsigned int> &v1EdgeList = v_to_e[v1Idx];
	vector<unsigned int> &v2EdgeList = v_to_e[v2Idx];
	for (unsigned int i = 0; i < v1EdgeList.size(); ++i)
	{
		bool checkClosedEdges(false);
		if (e_to_tri[v1EdgeList[i]].size() < 2)
			checkClosedEdges = true;

		unsigned int u1Idx = edgeList[v1EdgeList[i]].vertex1Idx;
		if (u1Idx == v1Idx)
			u1Idx = edgeList[v1EdgeList[i]].vertex2Idx;

		// search if v2Idx has an edge connected v1Idx
		for (unsigned int j = 0; j < v2EdgeList.size(); ++j)
		{
			unsigned int u2Idx = edgeList[v2EdgeList[j]].vertex1Idx;
			if (u2Idx == v2Idx)
				u2Idx = edgeList[v2EdgeList[j]].vertex2Idx;

			if (u1Idx != u2Idx)
				continue;

			// u1Idx is connected to a closed edge, and v2Idx is connected to u1Idx, so joining v1Idx + v2Idx will result in nonmanifold edge
			if (!checkClosedEdges)
				return false;
			else if (e_to_tri[v2EdgeList[j]].size() >= 2)
				return false;
		}
	}

	return true;
}

// returns the list of closest triangle for each vertex; -1 if no closest triangle
//
// closestTriList (out): array of associated triangles to return
// return value (out): number of slots in array
template <class T> const int* TilingNonobtuseMC<T>::getClosestTri() const
{
	return closestTriList;
}

template class TilingNonobtuseMC<short>;
template class TilingNonobtuseMC<unsigned short>;
template class TilingNonobtuseMC<int>;
template class TilingNonobtuseMC<unsigned int>;
template class TilingNonobtuseMC<float>;
template class TilingNonobtuseMC<double>;
