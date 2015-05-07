PROJECT=solveChallenge

all:	$(PROJECT).cpp
	g++ `pkg-config --cflags --libs opencv` -o $(PROJECT) $(PROJECT).cpp `pkg-config --libs opencv`

clean:
	rm $(PROJECT)