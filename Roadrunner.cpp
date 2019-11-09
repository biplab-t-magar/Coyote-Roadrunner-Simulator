#include "Roadrunner.h"

Roadrunner::Roadrunner(SimulationBoard* board, Coordinates location) : Agent(board, location, 3, BoardOccupantTypes::roadrunner) {

}

int Roadrunner::numOfAdjacentCoyotes(Coordinates inLocation) {
	int counter = 0;
	Coordinates tempLocation;

	//look to see there is a coyote below
	tempLocation.setCoordinates(inLocation.getRow() + 1, inLocation.getColumn());
	if (board->existsHere(BoardOccupantTypes::coyote, tempLocation)) {
		counter++;
	}

	//look to see there is a coyote above
	tempLocation.setCoordinates(inLocation.getRow() - 1, inLocation.getColumn());
	if (board->existsHere(BoardOccupantTypes::coyote, tempLocation)) {
		counter++;
	}

	//look to see there is a coyote to the right
	tempLocation.setCoordinates(inLocation.getRow(), inLocation.getColumn() + 1);
	if (board->existsHere(BoardOccupantTypes::coyote, tempLocation)) {
		counter++;
	}

	//look to see there is a coyote to the left
	tempLocation.setCoordinates(inLocation.getRow(), inLocation.getColumn() - 1);
	if (board->existsHere(BoardOccupantTypes::coyote, tempLocation)) {
		counter++;
	}
	return counter;
}

void Roadrunner::move() {
	Coordinates newLocation;
	//if no adjacent coyotes in the current location, then just move in a random unoccupied adjacent location
	if (numOfAdjacentCoyotes(location) == 0) {
		newLocation = findRandomViableLocation(location, BoardOccupantTypes::unoccupied);
	} else {
		//else, look for a location (within 1 or 2 steps) with the least number of adjacent coyotes
		unsigned int numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(location);
		newLocation = moveImplementation(location, numOfCoyoteesInNewLocation, 2);
	}

	if (newLocation.initialized()) {
		board->moveAgent(location, newLocation);
		location = newLocation;
	}
}

Coordinates Roadrunner::moveImplementation(Coordinates currLocation, unsigned int &numOfCoyoteesInNewLocation, unsigned int stepsAllowed) {
	//return the current location if the roadrunner has run out of steps or if the current location is the ideal location
	//note: ideal location = location with 0 adjacent coyotes
	if (stepsAllowed == 0 || numOfAdjacentCoyotes(currLocation) == 0) {
		numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(currLocation);
		return currLocation;
	}
	Coordinates foundLocation;
	//this vector lists the directions the roadrunner can move.
	//if one direction is not viable, then that direction is removed from the vector and the other directions are checked
	std::vector<char> locationToMoveTo = { 'u', 'd', 'l', 'r' };
	int index;
	while (locationToMoveTo.size() > 0) { //loop until all locations are checked (unless an ideal location is found before that)
		index = rand() % locationToMoveTo.size();
		switch (locationToMoveTo[index]) {
		case 'u':

			foundLocation.setCoordinates(currLocation.getRow() - 1, currLocation.getColumn());
			//check if up is a viable location (it is unoccupied)
			if (board->existsHere(BoardOccupantTypes::unoccupied, foundLocation)) {
				//if the number of coyotes adjacent to the up location is less than the number of coyotees in our current new location,
				//then set our current new location to up
				if (numOfCoyoteesInNewLocation > numOfAdjacentCoyotes(foundLocation)) {
					numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(foundLocation);
					currLocation = foundLocation;
				}
				//now, repeat the process, but with up as the new reference location
				//this is the recursive part where the logic of the roadrunner being able to take 2 steps comes into play
				currLocation = moveImplementation(currLocation, numOfCoyoteesInNewLocation, stepsAllowed - 1);
			}
			//since this location has been checked already, remove it from the vector
			locationToMoveTo.erase(locationToMoveTo.begin() + index);
			break;
			//repeat the same process for the other three directions as well
		case 'd':
			foundLocation.setCoordinates(currLocation.getRow() + 1, currLocation.getColumn());
			if (board->existsHere(BoardOccupantTypes::unoccupied, foundLocation)) {
				if (numOfCoyoteesInNewLocation > numOfAdjacentCoyotes(foundLocation)) {
					numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(foundLocation);
					currLocation = foundLocation;
				}
				currLocation = moveImplementation(currLocation, numOfCoyoteesInNewLocation, stepsAllowed - 1);
			}
			locationToMoveTo.erase(locationToMoveTo.begin() + index);
			break;
		case 'l':
			foundLocation.setCoordinates(currLocation.getRow(), currLocation.getColumn() - 1);
			if (board->existsHere(BoardOccupantTypes::unoccupied, foundLocation)) {
				if (numOfCoyoteesInNewLocation > numOfAdjacentCoyotes(foundLocation)) {
					numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(foundLocation);
					currLocation = foundLocation;
				}
				currLocation = moveImplementation(currLocation, numOfCoyoteesInNewLocation, stepsAllowed - 1);
			}
			locationToMoveTo.erase(locationToMoveTo.begin() + index);
			break;
		case 'r':
			foundLocation.setCoordinates(currLocation.getRow(), currLocation.getColumn() + 1);
			if (board->existsHere(BoardOccupantTypes::unoccupied, foundLocation)) {
				if (numOfCoyoteesInNewLocation > numOfAdjacentCoyotes(foundLocation)) {
					numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(foundLocation);
					currLocation = foundLocation;
				}
				currLocation = moveImplementation(currLocation, numOfCoyoteesInNewLocation, stepsAllowed - 1);
			}
			locationToMoveTo.erase(locationToMoveTo.begin() + index);
			break;
		}
		//if for  any new location, the number of adjacent coyotes is 0, then return that location
		if (numOfAdjacentCoyotes(currLocation) == 0) {
			numOfCoyoteesInNewLocation = numOfAdjacentCoyotes(currLocation);
			return currLocation;
		}
	}

	//this value could also be the same as the current location of the roadrunner, which means that staying still is the best
	//possible move for the roadrunner
	return currLocation;
}




Agent* Roadrunner::act() {
	if(!isAlive()) {
		return NULL;
	}
	//First check if the roadrunner has been eaten by a coyote. If it has, then execute die()
	if (!(board->existsHere(BoardOccupantTypes::roadrunner, location))) {
		die();
		return NULL;
	}
	move();
	//decrease time until next breed
	if (breedCountdown > 0) {
		breedCountdown--;
	}

	Agent* babyRoadrunner = NULL;
	//if time to breed, return pointer to new baby roadrunner, else return NULL
	if (breedCountdown == 0) {
		babyRoadrunner = breed();
	}
	return babyRoadrunner;
}

Agent* Roadrunner::breed() {
	Roadrunner* babyRoadrunner = NULL;
	//find a random adjacent location that is unoccupied
	Coordinates breedLocation = findRandomViableLocation(location, BoardOccupantTypes::unoccupied);
	//if such a location is found, then spawn a new baby there
	if (breedLocation.initialized()) {
		breedCountdown = 3;
		babyRoadrunner = new Roadrunner(board, breedLocation);
	}
	return babyRoadrunner;
}
