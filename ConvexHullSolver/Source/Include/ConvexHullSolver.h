/**
 * \file ConvexHullSolver.h
 * \author Douglas W. Paul
 *
 * Declares the ConvexHullSolver class
 */

#pragma once

#include <Geometry.hpp>
#include "HullFace.h"

using namespace peek;

/**
 * Solves the 3D convex hull problem
 */
class ConvexHullSolver {
public:

	/** Constructor */
	ConvexHullSolver(Point3dSet inputPoints);

	/** Iterates once */
	void iterate();

	/** Iterates until the hull is complete */
	void iterateToCompletion();

	/** Gets the current hull */
	inline HullFaceSet getHull() const { return this->hull; }

	/** Gets whether or not the hull is complete */
	inline bool isComplete() const { return this->complete; }

private:

	/** The hull */
	HullFaceSet hull;

	/** Whether or not the hull is complete */
	bool complete;

	/** Determines the next face to consider */
	auto_ptr<HullFace> determineNextFaceToConsider();

	/** Adds a point to the hull */
	void addPointToHull(HullPoint p);

	/** Adds faces to the hull */
	void addFaces(HullPoint p, HullEdgeSet edges, HullPointSet points);
};
