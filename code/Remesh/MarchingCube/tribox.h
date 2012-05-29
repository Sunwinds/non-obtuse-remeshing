#ifndef DUT_TRIBOX_H
#define DUT_TRIBOX_H

#include <opencv2/opencv.hpp>

#define X 0
#define Y 1
#define Z 2

namespace DUT
{
    class TriBox
    {
    private:
        TriBox() {}
        ~TriBox() {}

    public:
        static void findMinMax(double x0, double x1, double x2, double& min, double& max);
        /*======================== X-tests ========================*/
        static void axisX01(
            double a, double b, double fa, double fb,
            const cv::Point3d& v0, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
            double& p0, double& p2, double& min, double& max, double& rad);
        static void axisX2(
            double a, double b, double fa, double fb,
            const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
            double& p0, double& p1, double& min, double& max, double& rad);
        /*======================== Y-tests ========================*/
        static void axisY02(
            double a, double b, double fa, double fb,
            const cv::Point3d& v0, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
            double& p0, double& p2, double& min, double& max, double& rad);
        static void axisY1(
            double a, double b, double fa, double fb,
            const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
            double& p0, double& p1, double& min, double& max, double& rad);
        /*======================== Z-tests ========================*/
        static void axisZ12(
            double a, double b, double fa, double fb,
            const cv::Point3d& v1, const cv::Point3d& v2, const cv::Point3d& boxHalfSize,
            double& p1, double& p2, double& min, double& max, double& rad);
        static void axisZ0(
            double a, double b, double fa, double fb,
            const cv::Point3d& v0, const cv::Point3d& v1, const cv::Point3d& boxHalfSize,
            double& p0, double& p1, double& min, double& max, double& rad);
        static int planeBoxOverlap(const cv::Point3d& normal, const cv::Point3d& vertex, const cv::Point3d& maxBox);
        static int triBoxOverlap(const cv::Point3d& boxCenter, const cv::Point3d& boxHalfSize, const cv::Point3d triVerts[3]);
    };
}

#endif // DUT_TRIBOX_H
