/**
 * \file HullFace.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior of the HullFace class
 */

#include "HullFace.h"
#include <Peek_base.hpp>

using namespace peek;


/**
 * A HullFace generated with this default constructor is invalid.  Since
 * all of the points are at the origin, the face is degenerate.
 */
HullFace::HullFace() {}

/**
 * \param p0 The first point defining the face
 * \param p1 The second point defining the face
 * \param p2 The third point defining the face
 */
HullFace::HullFace(HullPoint p0, HullPoint p1, HullPoint p2) {
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

/**
 * \param p The point to associate with the face
 */
void HullFace::associate(HullPoint p) {
	this->associatedPoints.add(p);
}

/**
 * \return The list of associated points
 */
HullPointSet HullFace::getAssociatedPoints() {
	return this->associatedPoints;
}

/**
 * \return The number of associated points
 */
size_t HullFace::getNumAssociatedPoints() {
	return this->associatedPoints.size();
}

/**
 * \return The face's edges
 */
HullEdgeSet HullFace::getEdges() const {
	HullEdgeSet edges;

	edges.add(HullEdge(this->p0, this->p1));
	edges.add(HullEdge(this->p1, this->p2));
	edges.add(HullEdge(this->p2, this->p0));

	return edges;
}

HullPoint HullFace::getFurthestPoint() const {
	double maxD = 0.0;
	HullPointSet::const_iterator farthestPoint = associatedPoints.begin();

	for (HullPointSet::const_iterator i = associatedPoints.begin() + 1;
		i != associatedPoints.end(); i++) {
		
		// The determinant d is not actually the distance, but it is directly
		// proportional to the distance.
		double d = determinant(this->p2, this->p1, this->p0, *i);
		
		if (d >= maxD) {
			farthestPoint = i;
			maxD = d;
		}
	}

	return *farthestPoint;
}