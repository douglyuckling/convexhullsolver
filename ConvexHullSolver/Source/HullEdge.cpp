/**
 * \file HullEdge.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior for the HullEdge class
 */

#include "HullEdge.h"

using namespace peek;


/**
 * \param p0 The first point defining the edge
 * \param p1 The second point defining the edge
 */
HullEdge::HullEdge(HullPoint p0, HullPoint p1) {
	this->p0 = p0;
	this->p1 = p1;
}