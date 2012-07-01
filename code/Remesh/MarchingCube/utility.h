#ifndef DUT_UTILITY_H
#define DUT_UTILITY_H

#include "datadef.h"
#include <math.h>
#include <opencv2/opencv.hpp>

namespace DUT
{
    class Utility
    {
    private:
        Utility() {}
        ~Utility() {}

    public:
        // compare two double values with tolerence eps
        // a (in): vector a
        // b (in): vector b
        // eps (in): epsilon
        static bool equal(double a, double b, double eps);
        // angle between two vectors in radian
        // a (in): vector a
        // b (in): vector b
        // return val (out): angle between a,b in radian
        static double getAngle(const cv::Point3d& a, const cv::Point3d& b);
        // angle between two vectors in radian
        // anglePt (in): location of the point coincides with the angle
        // edgeEndPt1 (in): location of the point that makes up an edge for the angle at anglePt
        // edgeEndPt2 (in): location of the point that makes up an edge for the angle at anglePt
        // return val (out): angle between two vectors in radian
        static double getAngle(const cv::Point3d& anglePt, const cv::Point3d& edgeEndPt1, const cv::Point3d& edgeEndPt2);
        // normalize a vector
        // a (in/out): vector to be normalize
        static void normalize(cv::Point3d& a);
        // returns the face normal of triangle (pt1, pt2, pt3)
        static void computeFaceNormal(const cv::Point3d& pt1, const cv::Point3d& pt2, const cv::Point3d& pt3, cv::Point3d& normal);
        // check if norm is pointing the same direction as the normal of the plane created by triPt1, triPt2, triPt3
        // triPt1 (in): point of triangle
        // triPt2 (in): point of triangle
        // triPt3 (in): point of triangle
        // norm (in): normal we are checking against
        static bool checkSameClockDir(const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, const cv::Point3d& norm);
        // returns the shortest distance from pt to segment (segPt1, segPt2)
        // pt (in): point
        // segPt1 (in): point 1 of segment
        // segPt2 (in): point 2 of segment
        // closestPt (out): closest point from pt to segment
        // return val (out): shortest distance from pt to segment (segPt1, segPt2)
        static double pointSegmentDistance(const cv::Point3d& pt, const cv::Point3d& segPt1, const cv::Point3d& segPt2, cv::Point3d& closestPt);
        // returns the shortest distance from pt to triangle (tript1, tript2, tript3)
        // pt (in): point
        // triPt1 (in): point 1 of triangle
        // triPt2 (in): point 2 of triangle
        // triPt3 (in): point 3 of triangle
        // closestPt (out): closest point from pt to segment
        // closestDist (out): shortest distance from pt to triangle (triPt1, triPt2, triPt3)
        // return val (out):  0 - closest pt is inside triangle, 
        //                    1 - closest pt is tript1,
        //                    2 - closest pt is triPt2,
        //                    3 - closest pt is triPt3,
        //                   -1 - closest pt is on edge (triPt1, triPt2),
        //                   -2 - closest pt is on edge (triPt2, triPt3),
        //                   -3 - closest pt is on edge (triPt3, triPt1),
        static int pointTriangleDistance(const cv::Point3d& pt, const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, cv::Point3d& closestPt, double& closestDist);
        // returns the shortest distance from pt to triangle (triPt1, triPt2, triPt3)
        // pt (in): point
        // tript1 (in): point 1 of triangle
        // tript2 (in): point 2 of triangle
        // tript3 (in): point 3 of triangle
        // closestPt (out): closest point from pt to segment
        // return val (out): shortest distance from pt to triangle (triPt1, triPt2, triPt3)
        static double pointTriangleDistance(const cv::Point3d& pt, const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, cv::Point3d& closestPt);
    };
}

#endif // DUT_UTILITY_H
