#pragma once
#ifndef COYOTE_H
#define COYOTE_H

#include "Agent.h"


/**
	The class that defines the attributes and functionalities of the coyote in the Coyote-Roadrunner simulation
*/
class Coyote : public Agent {
private:
	///Keeps track of the amount of turns the coyote has gone without eating. After a certain amount is exceeded, the coyote dies
	unsigned int timeSinceLastMeal;
	/**
		This function executes the move mechanism of the coyote object and makes adequate changes on the board to reflect the move.
	*/
	void move();
	/**
	It executes the breed mechanism of the coyote object.
	\return Returns a pointer to a brand new coyote spawned by this coyote. If breeding does not take place, a NULL pointer is returned
	*/
	Agent* breed();
	/**
		Is called when timeSinceLastMeal reaches 4. Sets alive flag to false and removes itself from the board
	*/
	void die();
public:
	/**
		The constructor for the Coyote class
		\param board The board on which this coyote object will reside
		\param location The location in which this coyote object will reside
	*/
	Coyote(SimulationBoard* board, Coordinates location);
	/** Executes the actions that the coyote object will make in its given turn. This action includes move, breed, and die.
		\return If the coyote object breeds and creates a child object, then it returns a pointer to the child object. Else, it returns NULL
	*/
	Agent* act();
};

#endif
