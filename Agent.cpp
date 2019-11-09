#include "Agent.h"

Agent::Agent(SimulationBoard* board, Coordinates location, unsigned int breedCountdown, BoardOccupantTypes type) {
	this->breedCountdown = breedCountdown;
	this->location = location;
	alive = true;
	this->board = board;
	this->board->addAgent(type, location);
}

bool Agent::isAlive() {
	return alive;
}

void Agent::die() {
	alive = false;
}
Coordinates Agent::findRandomViableLocation(Coordinates currentPosition, BoardOccupantTypes criteria) {
	Coordinates foundLocation;
	std::vector<char> locationToBreed = { 'u', 'd', 'l', 'r' };
	int index;

	//In this loop, a random location is determined (up, down, left, or right). If the randomly selected location does not fulfill
	//the criteria, the location is removed from the locationToBreed vector and the process is repeated with the remaining locations in the
	//vector
	while (locationToBreed.size() > 0) {

		//choosing a random location from the vector
		index = rand() % locationToBreed.size();

		switch (locationToBreed[index]) { //randomly selected location is checked to see if it satisfies criteria
		case 'u':
			foundLocation.setCoordinates(currentPosition.getRow() - 1, currentPosition.getColumn()); //this is the randomly selected location
			if (board->existsHere(criteria, foundLocation)) { //check to see if the location meets the criteria and is within the bounds of the board
				return foundLocation; //if location satisfies the criteria, it is returned
			}
			else {
				//if the above check fails, the location is erased from the available options and the process is repeated
				//with the other locations
				locationToBreed.erase(locationToBreed.begin() + index);
			}
			break;
			//Rest of the cases follow the same formula
		case 'd':
			foundLocation.setCoordinates(currentPosition.getRow() + 1, currentPosition.getColumn());
			if (board->existsHere(criteria, foundLocation)) {
				return foundLocation;
			}
			else {
				locationToBreed.erase(locationToBreed.begin() + index);
			}
			break;
		case 'l':
			foundLocation.setCoordinates(currentPosition.getRow(), currentPosition.getColumn() - 1);
			if (board->existsHere(criteria, foundLocation)) {
				return foundLocation;
			}
			else {
				locationToBreed.erase(locationToBreed.begin() + index);
			}
			break;
		case 'r':
			foundLocation.setCoordinates(currentPosition.getRow(), currentPosition.getColumn() + 1);
			if (board->existsHere(criteria, foundLocation)) {
				return foundLocation;
			}
			else {
				locationToBreed.erase(locationToBreed.begin() + index);
			}
			break;
		}
	}

	foundLocation.setToNULL(); //if no location meets the criteria, an uninitialized location is returned
	return foundLocation;
}
