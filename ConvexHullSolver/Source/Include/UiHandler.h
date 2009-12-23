/**
 * \file UiHandler.h
 * \author Douglas W. Paul
 *
 * Declares the UiHandler class
 */

#pragma once

#include "Application.h"

/**
 * Handles UI events for the application
 */
class UiHandler : public CustomEventHandler, public MouseButtonEventHandler, public MouseMotionEventHandler, public ResizeEventHandler {
public:

	/** Constructor */
	UiHandler(Application *app);

	/** Destructor */
	~UiHandler();

	/** Handles a custom event */
	void handleCustomEvent(int customEvent);

	/** Handles a mouse button event */
	virtual void handleMouseButtonEvent(const SDL_MouseButtonEvent &buttonEvent);

	/** Handles a mouse motion event */
	virtual void handleMouseMotionEvent(const SDL_MouseMotionEvent &motionEvent);

	/** Handles a window-resize event */
	virtual void handleResizeEvent(int sizeX, int sizeY);

	void bindKeys();

protected:

	/** The application */
	Application *app;

	/** The last x-position of the mouse cursor */
	int lastMouseX;

	/** The last y-position of the mouse cursor */
	int lastMouseY;

	/** A saved x-position of the mouse cursor */
	int savedMouseX;

	/** A saved y-position of the mouse cursor */
	int savedMouseY;

	/** Whether the left mouse button is depressed */
	bool leftMouseButtonDown;

	/** Whether the right mouse button is depressed */
	bool rightMouseButtonDown;

	/** Amount to change the camera's longitude */
	double longitudeStep;

	/** Amount to change the camera's latitude */
	double latitudeStep;

	/** Minimum amount to change the distance of the camera from its target */
	double minDistanceStep;

	/** The fraction of the current distance by which the distance should change */
	double distanceRatio;

	/** Sensitivity of the mouse for mouselook */
	double mouseSensitivity;

	/** Calculates the distanceStep */
	double getDistanceStep();

};
