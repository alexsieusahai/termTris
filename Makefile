terminalTetris: main.cpp frontier.cpp frontier.h
	g++ main.cpp frontier.cpp frontier.h -o terminalTetris -Wall; ./terminalTetris

clean:
	rm terminalTetris

run: terminalTetris
	./terminalTetris
