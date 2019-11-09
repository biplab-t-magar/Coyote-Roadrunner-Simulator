#include "SimulationBoard.h"
#include <stdexcept>

SimulationBoard* SimulationBoard::boardInstance;

SimulationBoard* SimulationBoard::get_instance(unsigned int numOfRows, unsigned int numOfColumns) {
	if(!boardInstance) {
		boardInstance = new SimulationBoard(numOfRows, numOfColumns);
	}
	return boardInstance;
}

SimulationBoard::SimulationBoard(unsigned int numOfRows, unsigned int numOfColumns) {
	this->numOfRows = numOfRows;
	this->numOfColumns = numOfColumns;
	for (unsigned int i = 0; i < numOfRows; i++) {
		board.push_back(std::vector<BoardOccupantTypes>());
		for (unsigned int j = 0; j < numOfColumns; j++) {
			board[i].push_back(BoardOccupantTypes::unoccupied);
		}
	}
}

void SimulationBoard::addAgent(BoardOccupantTypes agentType, Coordinates location) {
	if(outOfBounds(location)) {
		throw std::invalid_argument("Invalid location");
	}
	board[location.getRow()][location.getColumn()] = agentType;
}

void SimulationBoard::removeAgent(Coordinates location) {
	if(outOfBounds(location)) {
		throw std::invalid_argument("Invalid location");
	}
	board[location.getRow()][location.getColumn()] = BoardOccupantTypes::unoccupied;
}

void SimulationBoard::moveAgent(Coordinates sourceLocation, Coordinates destinationLocation) {
	//first, check to see if destination is the same as the source
	if ((sourceLocation.getRow() == destinationLocation.getRow()) && (sourceLocation.getColumn() == destinationLocation.getColumn())) {
		return;
	}
	board[destinationLocation.getRow()][destinationLocation.getColumn()] = board[sourceLocation.getRow()][sourceLocation.getColumn()];
	board[sourceLocation.getRow()][sourceLocation.getColumn()] = BoardOccupantTypes::unoccupied;
}

bool SimulationBoard::outOfBounds(Coordinates location) {
	return (location.getRow() < 0 || location.getRow() >= numOfRows || location.getColumn() < 0 || location.getColumn() >= numOfColumns);
}

bool SimulationBoard::existsHere(BoardOccupantTypes type, Coordinates location) {
	return (!(outOfBounds(location)) && board[location.getRow()][location.getColumn()] == type);
}

unsigned int SimulationBoard::numOfCoyotes() {
	unsigned int counter = 0;
	for (unsigned int i = 0; i < numOfRows; i++) {
		for (unsigned int j = 0; j < numOfColumns; j++) {
			if (board[i][j] == BoardOccupantTypes::coyote) {
				counter++;
			}
		}
	}
	return counter;
}

unsigned int SimulationBoard::numOfRoadRunners() {
	unsigned int counter = 0;
	for (unsigned int i = 0; i < numOfRows; i++) {
		for (unsigned int j = 0; j < numOfColumns; j++) {
			if (board[i][j] == BoardOccupantTypes::roadrunner) {
				counter++;
			}
		}
	}
	return counter;
}


bool SimulationBoard::boardIsEmpty() {
	for (unsigned int i = 0; i < numOfRows; i++) {
		for (unsigned int j = 0; j < numOfColumns; j++) {
			if (board[i][j] != BoardOccupantTypes::unoccupied) {
				return false;
			}
		}
	}
	return true;
}


void SimulationBoard::printBoard() {

	for (unsigned int i = 0; i < numOfRows; i++) {
		for (unsigned int j = 0; j < numOfColumns; j++) {
			switch (board[i][j]) {
			case BoardOccupantTypes::unoccupied:
				std::cout << "- ";
				break;
			case BoardOccupantTypes::coyote:
				std::cout << "C ";
				break;
			case BoardOccupantTypes::roadrunner:
				std::cout << "R ";
				break;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}
