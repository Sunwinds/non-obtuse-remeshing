#ifndef DUT_DATADEF_H
#define DUT_DATADEF_H

#include <opencv2/opencv.hpp>

namespace DUT
{
    typedef unsigned int uint;

    struct Edge
    {
        Edge() : iV1Idx(-1), iV2Idx(-1) {}
        uint iV1Idx;
        uint iV2Idx;
    };

    struct LineMeshIntType
    {
        int iIntType; // type of intersection with ray
        // 0  - no intersection
        // 1  - intersection is inside the triangle (ray hits frontside)
        // -1 - intersection is inside the triangle (ray hits backside)
        // 2  - intersection is at a vertex (ray hits frontside)
        // -2 - intersection is at a vertex (ray hits backside)
        // 3  - intersection is on an edge (ray hits frontside)
        // -3 - intersection is on an edge (ray hits backside)
        // 4  - intersection contains an edge (coplanar)
        // 5  - intersection through 2 edges (coplanar)
        // 6  - intersection through one vertex and an edge (coplanar)
        // 7  - intersection is at a vertex (coplanar)
        cv::Point3d iPt; // intersecting point
    };
}

#endif // DUT_DATADEF_H
