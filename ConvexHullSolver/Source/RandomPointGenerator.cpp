/**
 * \file RandomPointGenerator.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior of the RandomPointGenerator class
 */

#include "RandomPointGenerator.h"
#include <Geometry.hpp>

using namespace peek;


/**
 * \param radius The bounding radius for the random points
 * \return A random point in the XY plane within the given radius of the origin
 */
Point3d RandomPointGenerator::get2dPointInRadius(double radius) {
	// Generate the point in polar coordinates
	double r = uniformRand(0.0, radius);
	double t = uniformRand(0.0, (2.0 * PI));

	// Convert to Cartesian coordinates
	double x = r * cos(t);
	double y = r * sin(t);

	return Point3d(x, y, 0.0);
}

/**
 * \param radius The bounding radius for the random points
 * \return A random point in space within the given radius of the origin
 */
Point3d RandomPointGenerator::get3dPointInRadius(double radius) {
	// Generate the point in spherical coordinates
	double r = uniformRand(0.0, radius);
	double t = uniformRand(0.0, (2.0 * PI));
	double p = uniformRand(0.0, PI);

	// Convert to Cartesian coordinates
	double x = r * cos(t) * sin(p);
	double y = r * sin(t) * sin(p);
	double z = r * cos(p);

	return Point3d(x, y, z);
}
