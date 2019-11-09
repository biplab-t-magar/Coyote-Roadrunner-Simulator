simulator: Agent.o Coyote.o Roadrunner.o SimulationBoard.o source.o
	g++ -std=c++0x Agent.o Coyote.o Roadrunner.o SimulationBoard.o source.o -o simulator

Agent.o: Agent.cpp Agent.h
	g++ -c -std=c++0x Agent.cpp

Coyote.o: Coyote.cpp Coyote.h
	g++ -c -std=c++0x Coyote.cpp

Roadrunner.o: Roadrunner.cpp Roadrunner.h
	g++ -c -std=c++0x Roadrunner.cpp

SimulationBoard.o: SimulationBoard.cpp SimulationBoard.h
	g++ -c -std=c++0x SimulationBoard.cpp

source.o: source.cpp 
	g++ -c -std=c++0x source.cpp

clean:
	rm *.o simulator
