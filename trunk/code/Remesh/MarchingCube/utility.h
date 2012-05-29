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
    };
}

#endif // DUT_UTILITY_H
