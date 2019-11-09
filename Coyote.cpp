#include "Coyote.h"


Coyote::Coyote(SimulationBoard* board, Coordinates location) : Agent(board, location, 8, BoardOccupantTypes::coyote) {
	timeSinceLastMeal = 0;
}

void Coyote::move() {
	//if there is an adjacent roadrunner, then move to that location in the board
	Coordinates newLocation = findRandomViableLocation(location, BoardOccupantTypes::roadrunner);
	if (newLocation.initialized()) {
		board->moveAgent(location, newLocation);
		location = newLocation;
		timeSinceLastMeal = 0;
	} else {
		//if there is no adjacent roadrunner, then move to a random adjacent unoccpied location
		newLocation = findRandomViableLocation(location, BoardOccupantTypes::unoccupied);
		if (newLocation.initialized()) {
			board->moveAgent(location, newLocation);
			location = newLocation;
		}

	}
}

Agent* Coyote::breed() {
	Coyote* babyCoyote = NULL;
	//if there is an unoccupied adjacent location, then birth a baby coyote there
	Coordinates breedLocation = findRandomViableLocation(location, BoardOccupantTypes::unoccupied);
	if (breedLocation.initialized()) {
		breedCountdown = 8;
		babyCoyote = new Coyote(board, breedLocation);
	}
	return babyCoyote;
}


void Coyote::die() {
	board->removeAgent(location);
	alive = false;
}



Agent* Coyote::act() {
	if(!isAlive()) {
		return NULL;
	}
	move();
	timeSinceLastMeal++;
	if (breedCountdown > 0) {
		breedCountdown--;
	}
	Agent* babyCoyote = NULL;
	if (breedCountdown == 0) {
		babyCoyote = breed();
	}
	if (timeSinceLastMeal >= 4) {
		die();
	}

	return babyCoyote;
}
