% function [intersectedTriangleIndex, intersectedPoints] = linesToTriangles_fast(points, directions, firstPointsOfTriangles, secondPointsOfTriangles, thirdPointsOfTriangles)
%Compute the closest hiting points to some triangles based on AABB tree
%data structure of CGAL
%%Input:
%points---the corrdinates of a point on the line, which is a n * 3 vector
%directions---the direction of the line, which is a n * 3 vector
%firstPointsOfTriangles---the corrdinates of the first point of the
%m triangles, which is m * 3 matrix
%secondPointsOfTriangles---the corrdinates of the second point of the
%m triangles, which is m * 3 matrix
%thirdPointsOfTriangles---the corrdinates of the third point of the
%m triangles, which is m * 3 matrix
%
%%Output:
%intersectedTriangleIndex---the indexes of the intersected triangles
%intersectedPoints---the closest hitting intersected points
%
%If there some no hitting, the intersectedTriangleIndex would be 0
%Hui Wang, August 10, 2012, wanghui19841109@gmail.com