#include "tribox.h"

namespace DUT
{
    void TriBox::findMinMax(double x0, double x1, double x2, double& min, double& max)
    {
        min = x0;
        max = x0;
        if(x1 < min) min = x1;
        if(x1 > max) max = x1;
        if(x2 < min) min = x2;
        if(x2 > max) max = x2;
    }

    void TriBox::axisX01(
        double a, double b, double fa, double fb,
        const cv::Point3d& v0, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
        double& p0, double& p2, double& min, double& max, double& rad)
    {
        p0 = a * v0.y - b * v0.z;
        p2 = a * v2.y - b * v2.z;
        if(p0 < p2)
        {
            min = p0;
            max = p2;
        }
        else
        {
            min = p2;
            max = p0;
        }
        rad = fa * boxHalfSize.y + fb * boxHalfSize.z;
        //if((min > rad) || (max < -rad)) return 0;
    }

    void TriBox::axisX2(
        double a, double b, double fa, double fb,
        const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
        double& p0, double& p1, double& min, double& max, double& rad)
    {
        p0 = a * v0.y - b * v0.z;
        p1 = a * v1.y - b * v1.z;
        if(p0 < p1)
        {
            min = p0;
            max = p1;
        }
        else
        {
            min = p1;
            max = p0;
        }
        rad = fa * boxHalfSize.y + fb * boxHalfSize.z;
        //if((min > rad) || (max < -rad)) return 0;
    }

    void TriBox::axisY02(
        double a, double b, double fa, double fb,
        const cv::Point3d& v0, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
        double& p0, double& p2, double& min, double& max, double& rad)
    {
        p0 = -a * v0.x + b * v0.z;
        p2 = -a * v2.x + b * v2.z;
        if(p0 < p2)
        {
            min = p0;
            max = p2;
        }
        else
        {
            min = p2;
            max = p0;
        }
        rad = fa * boxHalfSize.x + fb * boxHalfSize.z;
        //if((min > rad) || (max < -rad)) return 0;
    }

    void TriBox::axisY1(
        double a, double b, double fa, double fb,
        const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
        double& p0, double& p1, double& min, double& max, double& rad)
    {
        p0 = -a * v0.x + b * v0.z;
        p1 = -a * v1.x + b * v1.z;
        if(p0 < p1)
        {
            min = p0;
            max = p1;
        }
        else
        {
            min = p1;
            max = p0;
        }
        rad = fa * boxHalfSize.x + fb * boxHalfSize.z;
        //if((min > rad) || (max < -rad)) return 0;
    }

    void TriBox::axisZ12(
        double a, double b, double fa, double fb,
        const cv::Point3d& v1, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
        double& p1, double& p2, double& min, double& max, double& rad)
    {
        p1 = a * v1.x - b * v1.y;
        p2 = a * v2.x - b * v2.y;
        if(p2 < p1)
        {
            min = p2;
            max = p1;
        }
        else
        {
            min = p1;
            max = p2;
        }
        rad = fa * boxHalfSize.x + fb * boxHalfSize.y;
        //if((min > rad) || (max < -rad)) return 0;
    }

    void TriBox::axisZ0(
        double a, double b, double fa, double fb,
        const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
        double& p0, double& p1, double& min, double& max, double& rad)
    {
        p0 = a * v0.x - b * v0.y;
        p1 = a * v1.x - b * v1.y;
        if(p0 < p1)
        {
            min = p0;
            max = p1;
        }
        else
        {
            min = p1;
            max = p0;
        }
        rad = fa * boxHalfSize.x + fb * boxHalfSize.y;
        //if((min > rad) || (max < -rad)) return 0;
    }

    int TriBox::planeBoxOverlap(const cv::Point3d& normal, const cv::Point3d& vertex, const cv::Point3d& maxBox)
    {
        cv::Point3d vmin;
        cv::Point3d vmax;
        // x
        if(normal.x > 0.0f)
        {
            vmin.x = -maxBox.x - vertex.x;
            vmax.x = maxBox.x - vertex.x;
        }
        else
        {
            vmin.x = maxBox.x - vertex.x;
            vmax.x = -maxBox.x - vertex.x;
        }
        // y
        if(normal.y > 0.0f)
        {
            vmin.y = -maxBox.y - vertex.y;
            vmax.y = maxBox.y - vertex.y;
        }
        else
        {
            vmin.y = maxBox.y - vertex.y;
            vmax.y = -maxBox.y - vertex.y;
        }
        // z
        if(normal.z > 0.0f)
        {
            vmin.z = -maxBox.z - vertex.z;
            vmax.z = maxBox.z - vertex.z;
        }
        else
        {
            vmin.z = maxBox.z - vertex.z;
            vmax.z = -maxBox.z - vertex.z;
        }
        if(normal.dot(vmin) > 0.0f)
        {
            return 0;
        }
        if(normal.dot(vmax) >= 0.0f)
        {
            return 1;
        }
        return 0;
    }

    int TriBox::triBoxOverlap(const cv::Point3d& boxCenter, const cv::Point3d& boxHalfSize, const cv::Point3d triVerts[3])
    {
        // use separating axis theorem to test overlap between triangle and box
        // need to test for overlap in these directions:
        // 1) the {x,y,z}-directions (actually, since we use the AABB of the triangle we do not even need to test these)
        // 2) normal of the triangle
        // 3) crossproduct(edge from tri, {x,y,z}-directin) this gives 3x3=9 more tests

        // This is the fastest branch on Sun
        // move everything so that the boxcenter is in (0,0,0)
        cv::Point3d v0 = triVerts[0] - boxCenter;
        cv::Point3d v1 = triVerts[1] - boxCenter;
        cv::Point3d v2 = triVerts[2] - boxCenter;
        //cv::Point3d axis;
        // -NJMP- "d" local variable removed
        double min;
        double max;
        double p0;
        double p1;
        double p2;
        double rad;
        cv::Point3d fe;
        cv::Point3d normal;

        // compute triangle edges
        cv::Point3d e0 = v1 - v0; // tri edge 0
        cv::Point3d e1 = v2 - v1; // tri edge 1
        cv::Point3d e2 = v0 - v2; // tri edge 2

        // Bullet 3:
        // test the 9 tests first (this was faster)
        fe.x = std::abs(e0.x);
        fe.y = std::abs(e0.y);
        fe.z = std::abs(e0.z);
        TriBox::axisX01(e0.z, e0.y, fe.z, fe.y, v0, v2, boxHalfSize, p0, p2, min, max, rad);
        TriBox::axisY02(e0.z, e0.x, fe.z, fe.x, v0, v2, boxHalfSize, p0, p2, min, max, rad);
        TriBox::axisZ12(e0.y, e0.x, fe.y, fe.x, v1, v2, boxHalfSize, p1, p2, min, max, rad);

        fe.x = std::abs(e1.x);
        fe.y = std::abs(e1.y);
        fe.z = std::abs(e1.z);
        TriBox::axisX01(e1.z, e1.y, fe.z, fe.y, v0, v2, boxHalfSize, p0, p2, min, max, rad);
        TriBox::axisY02(e1.z, e1.x, fe.z, fe.x, v0, v2, boxHalfSize, p0, p2, min, max, rad);
        TriBox::axisZ0(e1.y, e1.x, fe.y, fe.x, v0, v1, boxHalfSize, p0, p1, min, max, rad);

        fe.x = std::abs(e2.x);
        fe.y = std::abs(e2.y);
        fe.z = std::abs(e2.z);
        TriBox::axisX2(e2.z, e2.y, fe.z, fe.y, v0, v1, boxHalfSize, p0, p1, min, max, rad);
        TriBox::axisY1(e2.z, e2.x, fe.z, fe.x, v0, v1, boxHalfSize, p0, p1, min, max, rad);
        TriBox::axisZ12(e2.y, e2.x, fe.y, fe.x, v1, v2, boxHalfSize, p1, p2, min, max, rad);

        // Bullet 1:
        // first test overlap in the {x,y,z}-directions
        // find min, max of the triangle each direction, and test for overlap in
        // that direction -- this is equivalent to testing a minimal AABB around
        // the triangle against the AABB

        // test in X-direction
        TriBox::findMinMax(v0.x, v1.x, v2.x, min, max);
        if((min > boxHalfSize.x) || (max < -boxHalfSize.x)) return 0;

        // test in Y-direction
        TriBox::findMinMax(v0.y, v1.y, v2.y, min, max);
        if((min > boxHalfSize.y) || (max < -boxHalfSize.y)) return 0;

        // test in Z-direction
        TriBox::findMinMax(v0.z, v1.z, v2.z, min, max);
        if((min > boxHalfSize.z) || (max < -boxHalfSize.z)) return 0;

        // Bullet 2:
        // test if the box intersects the plane of the triangle
        // compute plane equation of triangle: normal*x+d=0
        normal = e0.cross(e1);
        // -NJMP- (line removed here)
        if(!TriBox::planeBoxOverlap(normal, v0, boxHalfSize)) return 0;	// -NJMP-
        return 1; // box and triangle overlaps
    }
}
