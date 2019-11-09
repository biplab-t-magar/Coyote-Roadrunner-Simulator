#pragma once
#ifndef AGENT_H
#define AGENT_H

#include "SimulationBoard.h"
#include <string>
#include <stdlib.h>
#include <time.h>


/**
An abstract class representing an entity at play in the simulation. It is the base class of the Coyote and Roadrunner classes.
*/
class Agent {
protected:
	/**Implementation function that returns a random adjacent location out of all four adjacent locations (up, down, left, right) that satisfy the given criteria.
	If no such location is found, then an uninitialized location is returned
	\param currentPosition The position in relation to which the random adjacent location is to be calculated
	\criteria The criteria that must be met by the randomly selected location-to-be-returned
	\return The randomly selected adjacent location that also fulfills the criteria. If no location found, an uninitialized location is returned
	*/
	Coordinates findRandomViableLocation(Coordinates currentPosition, BoardOccupantTypes criteria);
	///flag to know whether the agent is alive
	bool alive;
	///The interval of time after which the objects of the classes that inherit from Agent will breed
	unsigned int breedCountdown;
	///The current location of the object in the board
	Coordinates location;
	///The board in which the object resides
	SimulationBoard* board;
	/**
	Pure virtual function to be overriden by child class.
	It executes the breed mechanism of the object.
	\return Returns a pointer to a brand new object spawned by this object. If breeding does not take place, a NULL pointer is returned
	*/
	virtual Agent* breed() = 0;
	/**The default implementation of the die() function for any class that inherits from Agent. Simply sets alive flag to false
	*/
	virtual void die();
	/**
	Pure virtual function to be overriden by child class.
	It executes the move mechanism of the object.
	*/
	virtual void move() = 0;
public:
	/**
		The constructor for the Agent class
		\param board The board on which the objects of the classes that inherit from Agent will reside
		\param location The location in which the objects of the classes that inherit from Agent will reside
		\param breedCountdown The interval of time after which the objects of the classes that inherit from Agent will breed
		\param type The type of the child class being instantiated (could be coyote or roadrunner)
	*/
	Agent(SimulationBoard* board, Coordinates location, unsigned int breedCountdown, BoardOccupantTypes type);
	/**Pure virtual function to be overriden by child class.
		It executes the actions that the object will make in its given turn. This action includes move, breed, and die
		\return If the object breeds and creates a child object, then it returns a pointer to the child object. Else, it returns NULL
	*/
	virtual Agent* act() = 0;
	/**Checks if the object is alive.
	\return True if alive, false if dead
	*/
	bool isAlive();
};




#endif
