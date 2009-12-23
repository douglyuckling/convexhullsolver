/**
 * \file ConvexHullSolver.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior of the ConvexHullSolver class
 */

#include "ConvexHullSolver.h"
#include "HullEdge.h"

using namespace peek;

/**
 * \param inputPoints The points whose convex hull to find
 */
ConvexHullSolver::ConvexHullSolver(Point3dSet inputPoints) {
	this->complete = false;

	// If there are fewer than three points, the hull has no faces.
	if (inputPoints.size() < 3) {
		this->complete = true;
		return;
	}

	// Grab three points from the list.
	Point3d p0 = inputPoints.popOne();
	Point3d p1 = inputPoints.popOne();
	Point3d p2 = inputPoints.popOne();

	// Construct two faces from the points we just grabbed.
	HullFace face1(p0, p1, p2);
	HullFace face2(p2, p1, p0);

	// Associate each of the remaining input points with one of the two faces.
	while (inputPoints.size() > 0) {
		Point3d p = inputPoints.popOne();
		double d = determinant(p2, p1, p0, p);

		// If the determinant is positive, the point is in front of face1.
		// If the determinant is negative, the point is in front of face2.
		if (d >= 0.0) {
			face1.associate(p);
		}
		else if (d < 0.0) {
			face2.associate(p);
		}
	}

	// Start the hull with the faces we just created.
	this->hull.add(face1);
	this->hull.add(face2);
}

void ConvexHullSolver::iterate() {
	if (isComplete()) {
		return;
	}

	auto_ptr<HullFace> potentialFace = determineNextFaceToConsider();

	// If there are no more faces to consider, then the hull is complete.
	if (potentialFace.get() == NULL) {
		this->complete = true;
		return;
	}

	// Find the point furthest from the face and add it to the hull.
	HullPoint furthestPoint = potentialFace->getFurthestPoint();
	addPointToHull(furthestPoint);
}

void ConvexHullSolver::iterateToCompletion() {
	while (!isComplete()) {
		iterate();
	}
}

/**
 * \return A pointer to the face to consider. (A NULL pointer if there is none.)
 * \todo Implement using Boost Library's "optional" template, instead of auto_ptr
 *
 * When the solver has run out of faces to consider, this function returns an uninitialized
 * auto_ptr.
 */
auto_ptr<HullFace> ConvexHullSolver::determineNextFaceToConsider() {
	// Try to find a face with associated points...
	for (HullFaceSet::iterator i = this->hull.begin(); i != this->hull.end(); i++) {
		if ((*i).getNumAssociatedPoints() > 0) {
			return auto_ptr<HullFace>(new HullFace(*i));
		}
	}

	return auto_ptr<HullFace>();
}

/**
 * \param p The point to add to the hull
 */
void ConvexHullSolver::addPointToHull(HullPoint p) {
	// Remove from the hull all faces that the point can "see" (is in front
	// of). Keep track of what edges are part of the hole we create in the
	// hull, and maintain a list of all of the "candidate" points that were
	// associated with the faces we're removing.

	HullPointSet points;
	HullEdgeSet edges;
	HullFaceSet facesToRemove;

	for (HullFaceSet::iterator i = this->hull.begin(); i != this->hull.end(); i++) {
		HullPoint p0 = (*i).getP0();
		HullPoint p1 = (*i).getP1();
		HullPoint p2 = (*i).getP2();
		HullPoint p3 = p;

		// If the determinant is greater than zero, the point can "see" this
		// face, so remove it.
		if (determinant(p2, p1, p0, p3) > 0.0) {
			// Add the face's associated points to our list of candidate
			// points...
			points.addAll((*i).getAssociatedPoints());

			// Get the new set of edges from the face to be removed.  If it
			// has any edges in common with our current list of edges, they
			// are removed because they were shared only by faces that were
			// removed.
			HullEdgeSet newEdges = (*i).getEdges();
			HullEdgeSet commonEdges = setIntersection(edges, newEdges);
			edges.addAll(newEdges);
			edges.removeAll(commonEdges);

			// Marke the face for removal
			facesToRemove.add(*i);
		}
	}

	this->hull.removeAll(facesToRemove);

	// Add new faces to the hull to fill the hole we created and encompass
	// the new point.
	addFaces(p, edges, points);
}
	
/**
 * Adds new faces to the hull between the given edges and the given point.
 * Tries associating each given point with one of the new faces.
 * 
 * \param p The point that all new faces will have in common
 * \param edges The edges to use in forming faces with point p
 * \param points The points to try associating with faces
 */
void ConvexHullSolver::addFaces(HullPoint p, HullEdgeSet edges, HullPointSet points) {
	// Make sure the point that the faces will have in common is not in the set
	// of points to be associated with the faces.
	points.remove(p);

	// For each of the free edges, add a face.
	for (HullEdgeSet::iterator i = edges.begin(); i != edges.end(); i++) {
		HullPoint p0 = (*i).getP0();
		HullPoint p1 = (*i).getP1();
		HullPoint p2 = p;
		HullFace face(p0, p1, p2);
		HullPointSet leftoverPoints;
	
		// Try associating all of the "free" points with the new face.
		for (HullPointSet::iterator j = points.begin(); j != points.end(); j++) {
			// If the determinant is greater than zero, associate the point
			// with the face.
			if (determinant(p2, p1, p0, *j) > 0.0) {
				face.associate(*j);
			}
			else {
				leftoverPoints.add(*j);
			}
		}

		points = leftoverPoints;

		this->hull.add(face);
	}

	// All unassociated candidate points must have been consumed by the hull.

}