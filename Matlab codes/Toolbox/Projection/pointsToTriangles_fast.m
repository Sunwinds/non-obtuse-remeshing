% function [closestIndex, closestDistances, closestPoints] = pointsToTriangles_fast(points, firstPointsOfTriangles, secondPointsOfTriangles, thirdPointsOfTriangles)
%Compute the closest distances and points from some points to some
%triangles based on AABB tree data structure of CGAL
%%Input:
%points---the corrdinates of the point, which is a n*3 vector
%firstPointsOfTriangles---the corrdinates of the first point of the
%m triangles, which is a m * 3 matrix
%secondPointsOfTriangles---the corrdinates of the second point of the
%m triangles, which is a m * 3 matrix
%thirdPointsOfTriangles---the corrdinates of the third point of the
%m triangles, which is a m * 3 matrix
%
%%Output:
%closestIndex--the indices of the closest triangles
%closestDistances--the closest distances, which is a n * 1 vector
%closestPoints--the closest points on the triangles, which is a n * 3
%matrix
%
%Hui Wang, August 10, 2012, wanghui19841109@gmail.com