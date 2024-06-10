#ifndef TP_PLANE_H
#define TP_PLANE_H

#include "tp_vector_operations.h"
#include "tp_scurve.h"

// Function to create a plane from three points
struct sc_plane create_plane_from_points(const struct sc_pnt *p1, const struct sc_pnt *p2, const struct sc_pnt *p3) {
    struct sc_plane plane;

    // Calculate the direction cosines of the plane
    struct sc_pnt vec1, vec2;
    cross_product(p1, p2, p3, &vec1);
    cross-Product(p2, p3, p1, &vec2);

    // Normalize the direction cosines
    normalize(&vec1);
    normalize(&vec2);

    // Set the axis point as one of the given points
    plane.ax = p1->x;
    plane.ay = p1->y;
    plane.az = p1->z;

    // Set the direction cosines
    plane.dx = vec1.x;
    plane.dy = vec1.y;
    plane.dz = vec1.z;

    return plane;
}

// Function to align the plane with global XYZ axes and position it at the origin
struct sc_plane align_plane_to_origin(const struct sc_plane originalPlane) {
    struct sc_plane rotatedPlane = originalPlane;

    // Normalize the direction cosines
    double magnitude = sqrt(rotatedPlane.dx * rotatedPlane.dx + rotatedPlane.dy * rotatedPlane.dy + rotatedPlane.dz * rotatedPlane.dz);
    rotatedPlane.dx /= magnitude;
    rotatedPlane.dy /= magnitude;
    rotatedPlane.dz /= magnitude;

    // Set the axis point of the rotated plane to the origin
    rotatedPlane.ax = 0.0;
    rotatedPlane.ay = 0.0;
    rotatedPlane.az = 0.0;

    return rotatedPlane;
}

void plane_test(){

    struct sc_cavavierline l0;
    l0.p0={0,0,0};
    l0.p1={50,50,50};
    l0.bulge=0;

    struct sc_cavavierline l1;
    l1.p0={50,50,50};
    l1.p1={100,0,0};
    l1.bulge=0;

    struct sc_plane original_plane=create_plane_from_points(l0.p0,l0.p1,l1.p1);
    struct sc_plane zero_plane=align_plane_to_origin(original_plane);

    struct sc_cavavierline cl0=l0;
    struct sc_cavavierline cl1=l1;

}

#endif // TP_PLANE_H













