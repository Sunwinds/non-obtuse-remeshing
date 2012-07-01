#include "utility.h"

namespace DUT
{
    bool Utility::equal(double a, double b, double eps)
    {
        return std::abs(a - b) <= eps;
    }

    double Utility::getAngle(const cv::Point3d& a, const cv::Point3d& b)
    {
        return acos(std::min<double>(1, std::max<double>(-1, a.dot(b) / (cv::norm(a) * cv::norm(b)))));
    }

    double Utility::getAngle(const cv::Point3d& anglePt, const cv::Point3d& edgeEndPt1, const cv::Point3d& edgeEndPt2)
    {
        cv::Point3d edge1 = edgeEndPt1 - anglePt;
        cv::Point3d edge2 = edgeEndPt2 - anglePt;
        return Utility::getAngle(edge1, edge2);
    }

    void Utility::normalize(cv::Point3d& a)
    {
        double len = cv::norm(a);
        if(len > 0)
        {
            a.x /= len;
            a.y /= len;
            a.z /= len;
        }
        else
        {
            a.x = 0;
            a.y = 0;
            a.z = 0;
        }
    }

    void Utility::computeFaceNormal(const cv::Point3d& pt1, const cv::Point3d& pt2, const cv::Point3d& pt3, cv::Point3d& normal)
    {
        cv::Point3d edge1 = pt2 - pt1;
        cv::Point3d edge2 = pt3 - pt1;
        normal = edge1.cross(edge2);
        Utility::normalize(normal);
    }

    bool Utility::checkSameClockDir(const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, const cv::Point3d& norm)
    {
        // edge 1 of triangle
        cv::Point3d edge1 = triPt2 - triPt1;
        // edge 2 of triangle
        cv::Point3d edge2 = triPt3 - triPt1;
        // vector normal of triangle
        cv::Point3d triNormal = edge1 - edge2;
        // dot product with norm
        return norm.dot(triNormal) > 0;
    }
    
    double Utility::pointSegmentDistance(const cv::Point3d& pt, const cv::Point3d& segPt1, const cv::Point3d& segPt2, cv::Point3d& closestPt)
    {
        cv::Point3d segPt1_to_pt = pt - segPt1;
        cv::Point3d segPt1_to_segPt2 =  segPt2 - segPt1;
        double vecLength = cv::norm(segPt1_to_segPt2);
        Utility::normalize(segPt1_to_segPt2);
        // u = (p3-pt).(p2-p1) / (p2-p1).(p2-p1)
        double u = segPt1_to_pt.dot(segPt1_to_segPt2);
        u = std::min(std::max(u, 0.0), vecLength);
        // p = pt1 + u * (p2-p1)
        closestPt = segPt1 + segPt1_to_segPt2 * u;
        cv::Point3d pt_to_closestPt = closestPt - pt;
        return cv::norm(pt_to_closestPt);
    }
    
    int Utility::pointTriangleDistance(const cv::Point3d& pt, const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, cv::Point3d& closestPt, double& closestDist)
    {
        // project pt to plane of triangle
        cv::Point3d triPt1_to_triPt2 = triPt2 - triPt1;
        cv::Point3d triPt1_to_triPt3 = triPt3 - triPt1;
        cv::Point3d triangleNorm = triPt1_to_triPt2.cross(triPt1_to_triPt3);
        Utility::normalize(triangleNorm);
        cv::Point3d triPt1_to_pt = pt - triPt1;
        double alpha = triPt1_to_pt.dot(triangleNorm);
        cv::Point3d projectedPt = pt - triangleNorm * alpha;
        int returnCode = 0;
        // check if projected pt is inside triangle
        if(Utility::checkSameClockDir(triPt1, triPt2, projectedPt, triangleNorm) &&
            Utility::checkSameClockDir(triPt2, triPt3, projectedPt, triangleNorm) &&
            Utility::checkSameClockDir(triPt3, triPt1, projectedPt, triangleNorm))
        {
            closestPt = projectedPt;
        }
        else
        {
            // pt is not inside triangle
            // check closest pt to the 3 edges
            cv::Point3d closestPt_seg;
            returnCode = -1;
            double minDist = Utility::pointSegmentDistance(projectedPt, triPt1, triPt2, closestPt);
            double dist = Utility::pointSegmentDistance(projectedPt, triPt2, triPt3, closestPt_seg);
            if(std::abs(dist - minDist) < 0.0000001)
            {
                returnCode = 2;
            }
            else if(dist < minDist)
            {	
                minDist = dist;
                closestPt = closestPt_seg;
                returnCode = -2;
            }
            dist = Utility::pointSegmentDistance(projectedPt, triPt3, triPt1, closestPt_seg);
            if(std::abs(dist - minDist) < 0.0000001)
            {
                if(returnCode == -1)
                {
                    returnCode = 1;
                }
                else if(returnCode == -2)
                {
                    returnCode = 3;
                }
            }
            else if(dist < minDist)
            {	
                minDist = dist;
                closestPt = closestPt_seg;
                returnCode = -3;
            }
        }
        cv::Point3d pt_to_closestPt = closestPt - pt;
        closestDist = cv::norm(pt_to_closestPt);
        return returnCode;
    }
    
    double Utility::pointTriangleDistance(const cv::Point3d& pt, const cv::Point3d& triPt1, const cv::Point3d& triPt2, const cv::Point3d& triPt3, cv::Point3d& closestPt)
    {
        // ---- project pt to plane of triangle ----
        cv::Point3d triPt1_to_triPt2 = triPt2 - triPt1;
        cv::Point3d triPt1_to_triPt3 = triPt3 - triPt1;
        cv::Point3d triangleNorm = triPt1_to_triPt2.cross(triPt1_to_triPt3);
        Utility::normalize(triangleNorm);
        cv::Point3d triPt1_to_pt = pt - triPt1;
        double alpha = triPt1_to_pt.dot(triangleNorm);
        cv::Point3d projectedPt = pt - triangleNorm * alpha;

        // ---- check if projected pt is inside triangle ----
        if(Utility::checkSameClockDir(triPt1, triPt2, projectedPt, triangleNorm) &&
            Utility::checkSameClockDir(triPt2, triPt3, projectedPt, triangleNorm) &&
            Utility::checkSameClockDir(triPt3, triPt1, projectedPt, triangleNorm))
        {
            closestPt = projectedPt;
        }
        else
        {
            // ---- pt is not inside triangle       ----
            // ---- check closest pt to the 3 edges ----
            cv::Point3d closestPt_seg;
            double minDist = Utility::pointSegmentDistance(projectedPt, triPt1, triPt2, closestPt);
            double dist = Utility::pointSegmentDistance(projectedPt, triPt2, triPt3, closestPt_seg);
            if(dist < minDist)
            {    
                minDist = dist;
                closestPt = closestPt_seg;
            }
            dist = Utility::pointSegmentDistance(projectedPt, triPt3, triPt1, closestPt_seg);
            if(dist < minDist)
            {    
                minDist = dist;
                closestPt = closestPt_seg;
            }
        }
        cv::Point3d pt_to_closestPt = closestPt - pt;
        return cv::norm(pt_to_closestPt);
    }
}