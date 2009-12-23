/**
 * \file Application.h
 * \author Douglas W. Paul
 *
 * Declares the Application class
 */

#pragma once

#include <Geometry.hpp>
#include <PerspectiveCamera.hpp>
#include <FixedTargetCameraRigging.hpp>
#include <Drawable.hpp>
#include <CustomEventHandler.hpp>
#include <MouseButtonEventHandler.hpp>
#include <MouseMotionEventHandler.hpp>
#include <ResizeEventHandler.hpp>
#include <Engine.hpp>
#include <GlWrappers.hpp>
#include "ConvexHullSolver.h"
#include "HullFace.h"
#include "UiHandler_pre.h"

using namespace peek;

static const unsigned int numPoints = 250;
static const double boundingRadius = 3.0;

/**
 * The application
 */
class Application : public Drawable {
public:

	/** Constructor */
	Application();

	/** Destructor */
	~Application();

	void run();

	/** Draws the scene */
	virtual void draw();

	/** Resets the solver */
	void reset();

	/** Iterates the solver once */
	void iterate();

	/** Iterates the solver to completion */
	void iterateToCompletion();

	/** Provides access to the screen width */
	inline int getScreenWidth() { return this->screenWidth; }

	/** Provides access to the screen height */
	inline int getScreenHeight() { return this->screenHeight; }

	/** Provides access to the camera */
	inline Camera *getCamera() { return this->cameraRigging->getCamera(); }

	/** Provides access to the camera rigging */
	inline FixedTargetCameraRigging *getCameraRigging() { return this->cameraRigging.get(); }

	inline Engine *getEngine() { return &engine; }

protected:

	/** The engine */
	Engine engine;

	auto_ptr<UiHandler> uiHandler;

	/** The input points */
	Point3dSet inputPoints;

	/** The convex hull solver */
	auto_ptr<ConvexHullSolver> solver;

	/** The current hull */
	HullFaceSet hull;

	/** The camera rigging */
	FixedTargetCameraRigging::handle cameraRigging;

	/** Initializes OpenGL state */
	void initGL();

	/** The screen width */
	int screenWidth;

	/** The screen height */
	int screenHeight;

	/** Draws a set of axes at the origin */
	void drawAxes() const;

	/** Draws the input points */
	void drawInputPoints() const;

	/** Draws the hull */
	void drawHull() const;

	/** Draws a face of the hull */
	void drawHullFace(HullFace face) const;

};

// Custom ConvexHullSolver events...
static const int CHSE_NONE = 0;
static const int CHSE_EXIT = 1;
static const int CHSE_DECREASE_LATITUDE = 2;
static const int CHSE_INCREASE_LATITUDE = 3;
static const int CHSE_DECREASE_LONGITUDE = 4;
static const int CHSE_INCREASE_LONGITUDE = 5;
static const int CHSE_RESET = 6;
static const int CHSE_ITERATE = 7;
static const int CHSE_ITERATE_TO_COMPLETION = 8;
