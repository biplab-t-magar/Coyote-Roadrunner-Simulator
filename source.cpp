#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "Roadrunner.h"
#include "Coyote.h"
#include "SimulationBoard.h"
#include <fstream>
#include <string>
#include <limits>
#include <climits>
#include <stdexcept>
#include <exception>

using namespace std;

/**returns a random value to use in std::random_shuffle()
*/
int myRandom(int i) {
	return std::rand() % i;
}

/**
	Shuffles the order of the agents in a specified portion of the agentList vector
	\param agentList The vector to shuffle
	\param indexToStartShuffleFrom The index of the vector after which the vector is shuffled. All elements of the vector before this index remain unchanged
*/
void shuffleAgents(vector<Agent*> &agentList, unsigned int indexToStartShuffleFrom) {
	std::random_shuffle(agentList.begin() + indexToStartShuffleFrom, agentList.end(), myRandom);
}

/**Goes through the agentList vector and looks at each agent to see which one is marked dead. If the agent is marked dead, then it
	removes the agent from the vector. Deallocates all pointers of the dead agents as well
	\param agentList the vector that is to be cleaned up
*/
void cleanAgentList(vector<Agent*> &agentList) {
	vector<Agent*> tempVector = agentList;
	agentList.clear();
	for (unsigned int i = 0; i < tempVector.size(); i++) {
		if (!(tempVector[i]->isAlive())) {
			delete tempVector[i];
		}
		else {
			agentList.push_back(tempVector[i]);
		}
	}
}

/**Prints the menu for the options available to the user of the simulation
*/

void printOptions() {
	cout << endl;
	cout << "Enter:" << endl;
	cout << "		(1) Immediate next step:" << endl;
	cout << "		(2) Next time step:" << endl;
	cout << "		(3) Skip 10 time steps:" << endl;
	cout << "		(4) Skip 20 time steps:" << endl;
	cout << "		(5) Skip 50 time steps:" << endl;
	cout << "		(6) Skip 100 time steps:" << endl;
	cout << "		(7) Skip 500 time steps:" << endl;
	cout << "		(-) Enter any other character to end the simulation: " << endl;
	cout << endl;
}


/**An implementation function that read's the user's input and produces a value to change the logic accordingly
	\param prompt The user's chosen menu option
	\return The timeStepsCountdown value corresponding to the chosen option
*/
int menuLogic(char prompt) {
	if(prompt == '1') {
		return 0;
	} else if(prompt == '2') {
		return 1;
	} else if(prompt == '3') {
		return 10;
	} else if(prompt == '4') {
		return 20;
	} else if(prompt == '5') {
		return 50;
	} else if(prompt == '6') {
		return 100;
	} else if(prompt == '7') {
		return 500;
	} else {
		return -2;
	}
}


/**An implementation function used when parsing the config file. It takes a line from the file and extracts needed values
from the line
\param type The string representation of the category for which the function is extracting the value
\param line The line from which the value is to be extracted
*/
unsigned int extractValueFromLine(string type, string line) {
	unsigned int value;
	//remove the name of the category from the line (for example, make the string "row: 45" into the string ": 45")
	line = line.substr(type.length());
	//remove extraneous white space and colon character that occur before value
	while(line.length() != 0 && (line[0] == ':' || line[0] == ' ')) {
		line = line.substr(1);
	}
	//if not value, throw exception
	if(line.length() == 0) {
		throw invalid_argument("bad argument");
	}
	try {
		//extract value
		value = stoul(line);
	} catch (invalid_argument) {
		throw invalid_argument("bad argument");
	} catch (out_of_range) {
		throw out_of_range("out of range");
	}
	return value;
}

/**A function used to parse the config file, extract required values, and assign them to corresponding variables
	\param argv The command line argument containing the name of the file
	\param numOfRows The number of rows in the Simulation Board
	\param numOfColumns The number of columns in the Simulation Board
	\param initialNumOfRoadrunners The initial number of roadrunners in the Simulation Board
	\param initialNumOfCoyotes The initial number of coyotes in the Simulation Board
	\return true if parse was successful, false if unsuccessful
*/
bool parseFile(char** argv, unsigned int &numOfRows, unsigned int &numOfColumns, unsigned int &initialNumOfRoadrunners, unsigned int &initialNumOfCoyotes) {
	string fileContent = "";
  string nextLine;
  ifstream myFile(argv[1]);
  if(myFile.is_open()) {
    while(getline(myFile, nextLine)) {
      if(nextLine == "") {
				continue;
			}
			//check to see if a given line contains information for each attribute
			if(nextLine.substr(0, 4) == "rows") {
				try {
					//extract value from line and assign to corresponding attribute
					numOfRows = extractValueFromLine("rows", nextLine);
				} catch (invalid_argument) {
					throw invalid_argument("Invalid row value.");
				} catch (out_of_range) {
					throw out_of_range("Invalid row value.");
				}
			} else if(nextLine.substr(0, 7) == "columns") {
				try {
					numOfColumns = extractValueFromLine("columns", nextLine);
				} catch (invalid_argument) {
					throw invalid_argument("Invalid column value.");
				} catch (out_of_range) {
					throw out_of_range("Invalid column value.");
				}
			} else if(nextLine.substr(0, 11) == "roadrunners") {
				try {
					initialNumOfRoadrunners = extractValueFromLine("roadrunners", nextLine);
				} catch (invalid_argument) {
					throw invalid_argument("Invalid roadrunners value");
				} catch (out_of_range) {
					throw out_of_range("Invalid roadrunners value");
				}
			} else if(nextLine.substr(0, 7) == "coyotes") {
				try {
					initialNumOfCoyotes = extractValueFromLine("coyotes", nextLine);
				} catch (invalid_argument) {
					throw invalid_argument("Invalid coyote value");
				} catch (out_of_range) {
					throw out_of_range("Invalid coyote value");
				}
			} else {
				//if the line contains information irrelevant to any of the four attributes, declare file to be invalid and return false
				cout << "Invalid file" << endl;
				return false;
			}
    }
    myFile.close();
  } else {
    return false;
  }
	return true;
}



int main(int argc, char** argv) {
	unsigned int numOfRows;
	unsigned int numOfColumns;
	unsigned int initialNumOfRoadrunners;
	unsigned int initialNumOfCoyotes;

	if(argc <= 1) {
  	cout << "File name missing. Assigning default values" << endl;
		numOfRows = 30;
		numOfColumns = 30;
		initialNumOfRoadrunners = 50;
		initialNumOfCoyotes = 50;
  } else {
		try {
			if(!parseFile(argv, numOfRows, numOfColumns, initialNumOfRoadrunners, initialNumOfCoyotes)) {
				cout << "File is invalid or not able to be opened. Aborting." << endl;
				return 0;
			}
		} catch(invalid_argument &e) {
			cout << e.what() << endl;
			return 0;
		} catch(out_of_range &e) {
			cout << e.what() << endl;
			return 0;
		} catch(exception &e) {
			cout << e.what() << endl;
			return 0;
		}
	}

	//check to see if the roadrunners and coyotes can fit in the board
	if(initialNumOfCoyotes + initialNumOfRoadrunners > numOfRows * numOfColumns) {
		cout << "Too many roadrunners and coyotes to fit in the board." << endl;
		return 0;
	}

	vector<Agent*> agentList;
	SimulationBoard* myBoard = SimulationBoard::get_instance(numOfRows, numOfColumns);
	Coordinates tempLocation;
	unsigned int randomNum;
	srand(time(NULL));

	//add roadrunners to the agentList vector in random unoccupied locations
	for (int i = 0; i < initialNumOfRoadrunners; i++) {
		do {
			tempLocation.setCoordinates(rand() % numOfRows, rand() % numOfColumns);
		} while (!myBoard->existsHere(BoardOccupantTypes::unoccupied, tempLocation));
		agentList.push_back(new Roadrunner(myBoard, tempLocation));
	}

	//add coyotes to the agenList vector in random unoccupied locations
	for (int i = 0; i < initialNumOfCoyotes; i++) {
		do {
			tempLocation.setCoordinates(rand() % numOfRows, rand() % numOfColumns);
		} while (!myBoard->existsHere(BoardOccupantTypes::unoccupied, tempLocation));
		agentList.push_back(new Coyote(myBoard, tempLocation));
	}

	cout << "Initial state of board: " << endl;
	myBoard->printBoard();
	Agent* temp = NULL;
	char prompt;
	int timeStepsCountdown = 0;

	printOptions();
	cin >> prompt;
	timeStepsCountdown = menuLogic(prompt);

	//Run until no agents left or until only roadrunners left
	while (agentList.size() != 0 && myBoard->numOfRoadRunners() < numOfRows * numOfColumns) {

		//used to keep track of the number of time steps to be jumped (as chosen by the user)
		timeStepsCountdown--;
		//shuffle agents each time step
		shuffleAgents(agentList, 0);
		for (int i = 0; i < agentList.size(); i++) {
			//each agent's act() function is called
			temp = (agentList[i])->act();
			//if the agent returns a baby agent, the baby is added to agentList
			if (temp != NULL) {
				agentList.push_back(temp);
				shuffleAgents(agentList, i + 1);
			}
			//depending on whether the user has chosen to skip or not skip viewing this step in the simulation, the board is printed
			//and the menu options are printed
			if(timeStepsCountdown == 0 || timeStepsCountdown == -1) {
				myBoard->printBoard();
				printOptions();
				cin >> prompt;
				timeStepsCountdown = menuLogic(prompt);
			}
		}
		//if the user chose to end the simulation, we break out of the loop
		if(timeStepsCountdown < -1) {
			break;
		}
		//clear list of dead agents
		cleanAgentList(agentList);
	}
	myBoard->printBoard();
	cout << endl << "End of simulation." << endl << endl;
	//deallocate pointers
	for(int i = 0; i < agentList.size(); i++) {
		delete agentList[i];
	}
	return 0;
}
