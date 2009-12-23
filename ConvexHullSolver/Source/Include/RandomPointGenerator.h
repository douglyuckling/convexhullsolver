/**
 * \file RandomPointGenerator.h
 * \author Douglas W. Paul
 *
 * Declares the RandomPointGenerator class
 */

#pragma once

#include <Geometry.hpp>

using namespace peek;

/**
 * Container class for static methods that generate random points.
 */
class RandomPointGenerator {
public:

	/** Gets a random 2-D point within the given radius of the origin */
	static Point3d get2dPointInRadius(double radius);

	/** Gets a random 3-D point within the given radius of the origin */
	static Point3d get3dPointInRadius(double radius);

};
