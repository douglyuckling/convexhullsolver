/**
 * \file UiHandler.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior for the UiHandler class
 */

#include "UiHandler.h"

/**
 * \param app A pointer to the application to control
 */
UiHandler::UiHandler(Application *app) {
	this->app = app;
	this->leftMouseButtonDown = false;
	this->rightMouseButtonDown = false;
	this->lastMouseX = 0;
	this->lastMouseY = 0;
	this->longitudeStep = 2.0;
	this->latitudeStep = 2.0;
	this->minDistanceStep = 0.01;
	this->distanceRatio = 0.05;
	this->mouseSensitivity = 0.125;
}

UiHandler::~UiHandler() {}

void UiHandler::bindKeys() {
	Engine *engine = this->app->getEngine();

	engine->bindKey(SDLK_ESCAPE, CHSE_EXIT);
	engine->bindKey(SDLK_UP, CHSE_INCREASE_LATITUDE);
	engine->bindKey(SDLK_e, CHSE_INCREASE_LATITUDE);
	engine->bindKey(SDLK_DOWN, CHSE_DECREASE_LATITUDE);
	engine->bindKey(SDLK_d, CHSE_DECREASE_LATITUDE);
	engine->bindKey(SDLK_LEFT, CHSE_DECREASE_LONGITUDE);
	engine->bindKey(SDLK_s, CHSE_DECREASE_LONGITUDE);
	engine->bindKey(SDLK_RIGHT, CHSE_INCREASE_LONGITUDE);
	engine->bindKey(SDLK_f, CHSE_INCREASE_LONGITUDE);
	engine->bindKey(SDLK_r, CHSE_RESET);
	engine->bindKey(SDLK_i, CHSE_ITERATE);
	engine->bindKey(SDLK_c, CHSE_ITERATE_TO_COMPLETION);
}

/**
* @param action The action to take
*/
void UiHandler::handleCustomEvent(int customEvent) {
	switch (customEvent) {
		case CHSE_EXIT: // Quit (ESC)
			exit(0);
		case CHSE_INCREASE_LATITUDE:   // Increase camera's latitude
			this->app->getCameraRigging()->changeLatitude(this->latitudeStep);
			this->app->getEngine()->invalidate();
			break;
		case CHSE_DECREASE_LATITUDE:   // Decrease camera's latitude
			this->app->getCameraRigging()->changeLatitude(-this->latitudeStep);
			this->app->getEngine()->invalidate();
			break;
		case CHSE_DECREASE_LONGITUDE:   // Decrease camera's longitude
			this->app->getCameraRigging()->changeLongitude(-this->longitudeStep);
			this->app->getEngine()->invalidate();
			break;
		case CHSE_INCREASE_LONGITUDE:   // Increase camera's longitude
			this->app->getCameraRigging()->changeLongitude(this->longitudeStep);
			this->app->getEngine()->invalidate();
			break;
		case CHSE_RESET:   // Reset the solver
			this->app->reset();
			this->app->getEngine()->invalidate();
			break;
		case CHSE_ITERATE:   // Iterate the solver once
			this->app->iterate();
			this->app->getEngine()->invalidate();
			break;
		case CHSE_ITERATE_TO_COMPLETION:   // Iterate the solver to completion
			this->app->iterateToCompletion();
			this->app->getEngine()->invalidate();
			break;
		default: break;
	}
}

/*!
* @param buttonEvent The button event
*/
void UiHandler::handleMouseButtonEvent(const SDL_MouseButtonEvent &buttonEvent) {
	const int button = buttonEvent.button;
	const int state = buttonEvent.state;
	const int x = buttonEvent.x;
	const int y = buttonEvent.y;

	if (button == SDL_BUTTON_RIGHT) {
		if (state == SDL_PRESSED) {
			// Save the mouse coordinates and begin "dragging"
			this->savedMouseX = x;
			this->savedMouseY = y;
			this->rightMouseButtonDown = true;
			int centerX = this->app->getScreenWidth() / 2;
			int centerY = this->app->getScreenHeight() / 2;
			SDL_ShowCursor(SDL_DISABLE);
			SDL_WM_GrabInput(SDL_GRAB_ON);
			this->lastMouseX = centerX;
			this->lastMouseY = centerY;
		} else {
			// End dragging
			this->rightMouseButtonDown = false;
			SDL_ShowCursor(SDL_ENABLE);
			SDL_WM_GrabInput(SDL_GRAB_OFF);
		}
	} else if (button == SDL_BUTTON_WHEELUP) {
		// Move camera closer
		this->app->getCameraRigging()->changeDistance(-getDistanceStep());
		this->app->getEngine()->invalidate();
	} else if (button == SDL_BUTTON_WHEELDOWN) {
		// Move camera farther
		this->app->getCameraRigging()->changeDistance(getDistanceStep());
		this->app->getEngine()->invalidate();
	}
}

/*!
* @param motionEvent The motion event
*/
void UiHandler::handleMouseMotionEvent(const SDL_MouseMotionEvent &motionEvent) {
	int dx = motionEvent.xrel;
	int dy = motionEvent.yrel;

	if (this->rightMouseButtonDown == true) {
		int centerX = this->app->getScreenWidth() / 2;
		int centerY = this->app->getScreenHeight() / 2;
		this->lastMouseX = centerX;
		this->lastMouseY = centerY;

		double deltaLongitude = (double) dx * (-this->longitudeStep) * this->mouseSensitivity;
		double deltaLatitude = (double) dy * this->latitudeStep * this->mouseSensitivity;

		this->app->getCameraRigging()->changeLongitude(deltaLongitude);
		this->app->getCameraRigging()->changeLatitude(deltaLatitude);

		this->app->getEngine()->invalidate();
	}
}

/*!
* @param sizeX The new x-size of the window
* @param sizeY The new y-size of the window
*/
void UiHandler::handleResizeEvent(int sizeX, int sizeY) {
	this->app->getCamera()->setAspectRatio((double)sizeX/(double)sizeY);
	this->app->getEngine()->invalidate();
}

/**
 * We calculate the distance to move the camera toward or away from the target
 * based on the current distance from the camera to its target.
 * 
 * \return The increment to move toward or away from the object
 */
double UiHandler::getDistanceStep() {
	double distanceStep = this->app->getCameraRigging()->getDistance() * this->distanceRatio;

	return (distanceStep > this->minDistanceStep) ? distanceStep : this->minDistanceStep;
}
