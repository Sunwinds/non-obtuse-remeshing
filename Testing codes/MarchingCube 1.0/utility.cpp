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
		normalize(normal);
	}
}