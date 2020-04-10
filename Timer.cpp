/*
 *  Timer.cpp
 *  Tutorial
 *
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <SDL.h>
#include "Timer.h"

Timer::Timer() {
	startTicks = 0;	//Initialisierung
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start() {
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
}

void Timer::stop() {
	started = false;
	paused = false;
}

void Timer::pause() {

	if( started && !paused ) {
		paused = true;		//Timer anhalten
		pausedTicks = SDL_GetTicks() - startTicks;	//die Zeit, die bis zur Pause vergangen ist speichern
	}
}

void Timer::unpause() {

	if( started && paused ) {
		paused = false;		//Timer starten
		startTicks = SDL_GetTicks() - pausedTicks;	//die Zeit die seit dem pausieren vergangen ist, als neue Startzeit speichern
		pausedTicks = 0;	//Pausezeit zurücksetzen
	}
}

int Timer::getTicks() {

	if( started ) {
		if( paused ) {
			return pausedTicks;
		} else {
			return SDL_GetTicks() - startTicks; //Die aktuelle Zeit zurückgeben
		}
	}

	return 0; //Wenn der Timer nicht gestartet wurde gebe 0s zurück.
}

bool Timer::is_started() {
	return started;
}

bool Timer::is_paused() {
	return paused;
}
