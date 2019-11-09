#pragma once
#ifndef ROADRUNNER_H
#define ROADRUNNER_H

#include "Agent.h"


/**
	The class that defines the attributes and functionalities of the roadrunner in the Coyote-Roadrunner simulation
*/
class Roadrunner : public Agent {
private:
	/**
		Implementation function that is used by the move mechanism of the  Roadrunner to find out the number of adjacent coyotees in the
		given location
		\param inLocation The location in relation to which the number of adjacent coyotes is to be determined
		\return The number of adjacent coyotes relative to the given location
	*/
	int numOfAdjacentCoyotes(Coordinates inLocation);

	/**
		This function executes the move mechanism of the roadrunner object and makes adequate changes on the board to reflect the move.
	*/
	void move();

	/**
		This is an implementation function utilized by the move() function. It is called when there is a Coyote adjacent to this
		roadrunner object. This function returns a new location with the least adjacent number of coyotes. This location may be 1 or 2
		steps away from the roadrunner's current location, so it can check up to 16 locations to find the ideal location to return. It is
		recursive.
		\param currLocation The location in relation to which the new location is to be determined
		\param numOfCoyoteesInNewLocation The number of Coyotes in the new location
		\param stepsAllowed The total number of steps the roadrunner is allowed to move in a given turn
		\return The location that is 1 or 2 steps away from this roadrunner and that has the least number of adjacent Coyotes.
	*/
	Coordinates moveImplementation(Coordinates currLocation, unsigned int &numOfCoyoteesInNewLocation, unsigned int stepsAllowed);
	/**
	It executes the breed mechanism of the roadrunner object.
	\return Returns a pointer to a brand new roadrunner spawned by this roadrunner. If breeding does not take place, a NULL pointer is returned
	*/
	Agent* breed();
public:
	/**
		The constructor for the Roadrunner class
		\param board The board on which this roadrunner object will reside
		\param location The location in which this roadrunner object will reside
	*/
	Roadrunner(SimulationBoard* board, Coordinates location);
	/** Executes the actions that the roadrunner object will make in its given turn. This action includes move, breed, and die.
		\return If the roadrunner object breeds and creates a child object, then it returns a pointer to the child object. Else, it returns NULL
	*/
	Agent* act();
};

#endif
