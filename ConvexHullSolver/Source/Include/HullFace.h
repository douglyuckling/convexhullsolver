/**
 * \file HullFace.h
 * \author Douglas W. Paul
 *
 * Declares the HullFace class and associated typedefs
 */

#pragma once

#include <Set.hpp>
#include "HullPoint.h"
#include "HullEdge.h"

using namespace peek;

/**
 * Represents a face of a 3-D hull
 */
class HullFace {
public:

	/** No-arg constructor */
	HullFace();

	/** Constructor */
	HullFace(HullPoint p0, HullPoint p1, HullPoint p2);

	/** Associates a point with this face */
	void associate(HullPoint p);

	/** Gets the list of associated points */
	HullPointSet getAssociatedPoints();

	/** Gets the number of associated points */
	size_t getNumAssociatedPoints();

	/** Gets the first point defining the face */
	inline HullPoint getP0() const { return p0; }

	/** Gets the second point defining the face */
	inline HullPoint getP1() const { return p1; }

	/** Gets the third point defining the face */
	inline HullPoint getP2() const { return p2; }

	/** Equality operator */
	inline bool operator==(const HullFace &rArg) const {
		return ((this->p0 == rArg.p0) && (this->p1 == rArg.p1) && (this->p2 == rArg.p2)
			||  (this->p0 == rArg.p1) && (this->p1 == rArg.p2) && (this->p2 == rArg.p0)
			||  (this->p0 == rArg.p2) && (this->p1 == rArg.p0) && (this->p2 == rArg.p1));
	}

	/** Gets the face's edges */
	HullEdgeSet getEdges() const;

	/** Gets the furthest associated point */
	HullPoint getFurthestPoint() const;

protected:

	/** The points that define the face */
	HullPoint p0, p1, p2;

	/** Points that are associated with this face */
	HullPointSet associatedPoints;

};

typedef vector<HullFace> HullFaceList;
typedef Set<HullFace> HullFaceSet;