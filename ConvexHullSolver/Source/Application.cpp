/**
 * \file Application.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior of the Application class
 */

#include "Application.h"
#include <Peek_base.hpp>
#include <Color.hpp>
#include "UiHandler.h"
#include "RandomPointGenerator.h"
#include "ConvexHullSolver.h"
#include <ctime>

using namespace peek;

Application::Application() {
	Camera::handle camera = Camera::handle(new PerspectiveCamera());
	this->cameraRigging.reset(new FixedTargetCameraRigging(camera, 45.0, 45.0, 10.0));
}

Application::~Application() {}

void Application::run() {
	this->uiHandler.reset(new UiHandler(this));
	this->uiHandler->bindKeys();

	this->engine.setDrawable(this);
	this->engine.setCustomEventHandler(this->uiHandler.get());
	this->engine.setMouseButtonEventHandler(this->uiHandler.get());
	this->engine.setMouseMotionEventHandler(this->uiHandler.get());
	this->engine.setResizeEventHandler(this->uiHandler.get());

	srand((unsigned int) time(0));
	reset();
	this->engine.run();
}

void Application::reset() {
	this->inputPoints.clear();

	for (unsigned int i = 0; i < numPoints; i++) {
		this->inputPoints.add(RandomPointGenerator::get3dPointInRadius(boundingRadius));
	}

	this->solver.reset(new ConvexHullSolver(this->inputPoints));
	this->hull = this->solver->getHull();
}

void Application::iterate() {
	this->solver->iterate();
	this->hull = this->solver->getHull();
}

void Application::iterateToCompletion() {
	this->solver->iterateToCompletion();
	this->hull = this->solver->getHull();
}

void Application::draw() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->cameraRigging->getCamera()->setProjectionMatrix();
	this->cameraRigging->initModelviewMatrix();

	drawAxes();
	drawInputPoints();
	drawHull();

	glFlush();
}

void Application::drawInputPoints() const {
	glDisable(GL_LIGHTING);
	pkGlColor(Color::orange);
	glBegin(GL_POINTS);

	for (Point3dSet::const_iterator i = this->inputPoints.begin(); i != this->inputPoints.end(); i++) {
		pkGlVertex(*i);
	}

	glEnd();
}

void Application::drawHull() const {
	glDisable(GL_LIGHTING);

	Color hullColor = this->solver->isComplete() ? Color::sky : Color::pink;
	pkGlColor(hullColor);

	for (HullFaceSet::const_iterator i = this->hull.begin(); i != this->hull.end(); i++) {
		drawHullFace(*i);
	}
}

/**
 * \param face The HullFace to draw
 */
void Application::drawHullFace(HullFace face) const {
	glBegin(GL_LINE_LOOP);
	pkGlVertex(face.getP0());
	pkGlVertex(face.getP1());
	pkGlVertex(face.getP2());
	glEnd();
}

void Application::drawAxes() const {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	pkGlColor(Color::red);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0);

	pkGlColor(Color::green);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 1.0, 0.0);

	pkGlColor(Color::blue);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 1.0);

	glEnd();
}

void Application::initGL() {
	pkGlClearColor(Color::black, 0.0);
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
}
