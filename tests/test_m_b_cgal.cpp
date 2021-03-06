/**
 * @file cgal_tool.hpp
 *
 * @brief Test the utilities to ease (and wrap) use of CGAL
 *
 * @author Anthony Truchet <anthony.truchet@logilab.fr>
 *
 */


#include <cstdlib>
#include <deque>

#include "gtest/gtest.h"

#include "Tympan/models/common/3d.h"
#include "Tympan/models/common/plan.h"
#include "Tympan/models/business/geometry/TYPolygon.h"
#include "Tympan/models/common/cgal_tools.h"
#include "Tympan/models/business/cgal_bridge.h"

using std::cout;
using std::cerr;
using std::endl;

TEST(cgal_tools, basic_types)
{

    using namespace tympan;

    OPoint3D op(1, 2, 3);
    EXPECT_EQ(op, from_cgal(to_cgal(op)));
    CGAL_Point3 cp(4, 5, 6);
    EXPECT_EQ(cp, to_cgal(from_cgal(cp)));
    OVector3D ov(1, 2, 3);
    EXPECT_EQ(ov, from_cgal(to_cgal(ov)));
    CGAL_Vector3 cv(4, 5, 6);
    EXPECT_EQ(cv, to_cgal(from_cgal(cv)));

}

TEST(cgal_tools, build_box)
{
    using namespace tympan;
    // start with a simple one (can be guessed knowing the third point of the plane: (0,1,0))
    std::deque<CGAL_Point3> vertices = build_box(2, 4, CGAL_Point3(0, 0, 0),
            CGAL_Point3(10, 0, 0));
    EXPECT_EQ(vertices[0], CGAL_Point3(0, -1, -2));
    EXPECT_EQ(vertices[1], CGAL_Point3(10, -1, -2));
    EXPECT_EQ(vertices[2], CGAL_Point3(0, 1, -2));
    EXPECT_EQ(vertices[3], CGAL_Point3(0, -1, 2));
    // check box dimensions (check third plane point is recomputed in build_box())
    vertices = build_box(2, 4, CGAL_Point3(9, 1, 0),  CGAL_Point3(0, 1, 0));
    EXPECT_NEAR(9, sqrt(CGAL_Vector3(vertices[0], vertices[1]).squared_length()), 1e-10);
    EXPECT_NEAR(2, sqrt(CGAL_Vector3(vertices[0], vertices[2]).squared_length()), 1e-10);
    EXPECT_NEAR(4, sqrt(CGAL_Vector3(vertices[0], vertices[3]).squared_length()), 1e-10);
    // same thing, with a weird rotated box
    vertices = build_box(1, 5, CGAL_Point3(1, 2, 3),  CGAL_Point3(50, 35, 100));
    EXPECT_NEAR(CGAL_Vector3(CGAL_Point3(1, 2, 3), CGAL_Point3(50, 35, 100)).squared_length(),
            CGAL_Vector3(vertices[0], vertices[1]).squared_length(), 1e-10);
    EXPECT_NEAR(1, sqrt(CGAL_Vector3(vertices[0], vertices[2]).squared_length()), 1e-10);
    EXPECT_NEAR(5, sqrt(CGAL_Vector3(vertices[0], vertices[3]).squared_length()), 1e-10);
}

// To factorize with the two others geometrical tests
TEST(geometry_tools, plane)
{
    using namespace tympan;

    OVector3D v1(1, 1.5 , 2);
    OPoint3D A(2, 3, 4);
    OPlan P(A, v1);

    // Test for the explicit representation
    EXPECT_EQ(A, P.rframe._origin);
    v1.normalize();
    EXPECT_DOUBLE_EQ(0.0, (v1 - P.rframe._vecK).norme());

    // Test for conversion to CGAL
    CGAL_Plane CP(to_cgal(P));
    CGAL_Point3 cgal_origin(2, 3, 4);
    CGAL_Point3 proj(CP.projection(cgal_origin));
    EXPECT_NEAR(0, (proj - cgal_origin).squared_length(), 1e-20);
}

TEST(geometry_tools, triangulator)
{
    using namespace tympan;

    // This is a dart-like concave quadrangle
    CGAL_Point2 points[] = { CGAL_Point2(0, 0), CGAL_Point2(5, 0), CGAL_Point2(1, 1), CGAL_Point2(0.5, 6)};
    CGAL_Polygon poly(points, points + 4);

    PolygonTriangulator triangulator(poly);

    EXPECT_EQ(4, triangulator.triangulation().number_of_vertices()); // Finite vetices
    EXPECT_EQ(3, triangulator.triangulation().number_of_faces()); // Finite faces
    std::deque<PolygonTriangulator::Triangle> triangles;
    triangulator.exportTriangles(triangles);
    EXPECT_EQ(2, triangles.size());

    std::deque<PolygonTriangulator::Tri_indices> tri_indices;
    triangulator.exportTrianglesIndices(tri_indices);
    EXPECT_EQ(2, tri_indices.size());

    // Now assert consistency on first triangle
    PolygonTriangulator::Triangle tri(triangles.front());
    for (unsigned i = 0; i < 3; ++i)
    {
        unsigned pt_idx = tri_indices.front()[i];
        EXPECT_EQ(poly[pt_idx], tri[i]);
    }
}


