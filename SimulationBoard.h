#pragma once
#ifndef SIMULATIONBOARD_H
#define SIMULATIONBOARD_H

#include <vector>
#include <iostream>


/**
	An enum class that describes the types of objects that can occupy the board.
*/
enum class BoardOccupantTypes {
	unoccupied, ///< if no object resides in a location in the board, then unoccupied is used instead
	coyote, ///< if a Coyote object resides a location in the board, then enum type coyote is used
	roadrunner ///< if a Roadrunner object resides a location in the board, then enum type roadrunner is used
};

/**
	A class that is used to store coordinates to describe a location in a board.
*/
class Coordinates {
public:
	/**
		Default contructor. The Coordinates object is set to uninitialized
	*/
	Coordinates() {
		locationInitialized = false;
	}
	/**
		Overloaded contructor. The Coordinates object has its row and column value set to the arguments provided
		\param row The row attribute to describe the location in the board
		\param column The column attribute to describe the location in the board
	*/
	Coordinates(unsigned int row, unsigned int column) {
		this->row = row;
		this->column = column;
		locationInitialized = true;
	}

	/**
		Sets the Coordinates object's row and column attribute to new values
		\param row The row attribute to describe the location in the board
		\param column The column attribute to describe the location in the board
	*/
	void setCoordinates(unsigned int row, unsigned int column) {
		this->row = row;
		this->column = column;
		locationInitialized = true;
	}

	/**
		Uninitializes the Coordinates object by setting locationInitialized flag to false
	*/
	void setToNULL() {
		locationInitialized = false;
	}

	/**
		Checks to see if the Coordinates object is "initialized".
		\return False if not initialized. Else, true
	*/
	bool initialized() {
		return locationInitialized;
	}

	/**
		Used to get the row attribute
		\return the row value
	*/
	unsigned int getRow() {
		return row;
	}
	/**
		Used to get the column attribute
		\return the column value
	*/
	unsigned int getColumn() {
		return column;
	}
private:
	///The row aspect of the location that the Coordinates object describes
	unsigned int row;
	///The column aspect of the location that the Coordinates object describes
	unsigned int column;
	///A flag that informs whether a row and column value have been set.
	/**This flag is useful when functions that search for certain locations cannot find the location, so they set the Coordinates object to null (set locationInitialized to false) and
	return this object, informing the calling function that the desired location was not found
	*/
	bool locationInitialized;
};

/**
The singleton class that defines the virtual board on which the Coyote-Roadrunner simulation is run
*/
class SimulationBoard {
private:
	///The number of rows in the board
	unsigned int numOfRows;
	///The number of columns in the board
	unsigned int numOfColumns;
	///The two-dimensional vector that holds the list of all the objects that occupy the board
	std::vector<std::vector<BoardOccupantTypes>> board;

	/**
		Constructor for SimulationBoard. Set to private to implement a singleton design pattern.
		\param numOfRows The number of rows in the board
		\param numOfColumns The number of columns in the board
	*/
	SimulationBoard(unsigned int numOfRows, unsigned int numOfColumns);
	/**Default contructor for SimulationBoard. Not implemented and set to private to implement singleton design pattern
	*/
	SimulationBoard();
	/**Copy contructor for SimulationBoard. Set to private to implement singleton design pattern
	*/
	SimulationBoard(SimulationBoard const&);
	/**Overloaded assignment operator for SimulationBoard. Set to private to implement singleton design pattern
	*/
	SimulationBoard& operator=(SimulationBoard const&);
	///A static instance of  SimulationBoard. This is the only instance of SimulationBoard available to clients
	static SimulationBoard* boardInstance;
public:

	/**Used to get the static instance of the SimulationBoard class
	\param numOfRows The number of rows in the board
	\param numOfColumns The number of columns in the board
	\return The static instance of the SimulationBoard
	*/
	static SimulationBoard* get_instance(unsigned int numOfRows, unsigned int numOfColumns);

	/**Adds an agent of the specified type to the SimulationBoard at the specified location
	\param agentType The type of the agent to be added to the board
	\param location The location to which the agent is to be added
	*/
	void addAgent(BoardOccupantTypes agentType, Coordinates location);

	/**Removes an agent from the SimulationBoard at the specified location, rendering the location unoccupied
	\param location The location from which the agent is to be removed
	*/
	void removeAgent(Coordinates location);

	/**Moves an agent in the SimulationBoard from one location to another. If the destination is same as the source, then does nothing
	\param sourceLocation The location at which the agent currently resides
	\param destinationLocation The location to which the agent is to be moved to
	*/
	void moveAgent(Coordinates sourceLocation, Coordinates destinationLocation);

	/**Checks if the given location is out of bounds, i.e. checks if the given does not location exist in the board
	\param location The locatoin whose validity is to be checked
	\return true if the location does not exist, false if the location does exist
	*/
	bool outOfBounds(Coordinates location);

	/**First, makes sure given location is not out of bounds and then checks to see if the given type resides in that location
	\param type The type that is to be checked for in the location
	\param location The location in which the type to be checked
	\return true If the location exists and the location is occupied by the given type, false otherwise
	*/
	bool existsHere(BoardOccupantTypes type, Coordinates location);
	/**
	Returns the total number of roadrunners currently residing in the board
	\return The total number of roadrunners in the board
	*/
	unsigned int numOfRoadRunners();

	/**
	Returns the total number of coyotes currently residing in the board
	\return The total number of coyotes in the board
	*/
	unsigned int numOfCoyotes();

	/**
	Checks to see if the board is board is empty
	\return True if board is empty, false otherwise
	*/
	bool boardIsEmpty();

	/**
	Prints the contents of the board
	*/
	void printBoard();
};

#endif
