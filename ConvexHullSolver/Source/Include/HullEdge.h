/**
 * \file HullEdge.h
 * \author Douglas W. Paul
 *
 * Declares the HullEdge class and associated typedefs
 */

#pragma once

#include <Set.hpp>
#include "HullPoint.h"

using namespace peek;

/**
 * Represents an edge on the hull
 */
class HullEdge {
public:

	/** Constructor */
	HullEdge(HullPoint p0, HullPoint p1);

	/** Gets the first point defining the edge */
	inline HullPoint getP0() const { return this->p0; }

	/** Gets the second point defining the edge */
	inline HullPoint getP1() const { return this->p1; }

	/** Equality operator */
	inline bool operator==(const HullEdge &rArg) const {
		return ((this->p0 == rArg.p0) && (this->p1 == rArg.p1)
			||  (this->p0 == rArg.p1) && (this->p1 == rArg.p0));
	}

private:

	/** The points that define the edge */
	HullPoint p0, p1;

};

typedef vector<HullEdge> HullEdgeList;
typedef Set<HullEdge> HullEdgeSet;