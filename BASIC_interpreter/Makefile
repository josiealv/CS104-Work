all: hw4

hw4: FORCE interpreter/booleanExpression.o interpreter/command.o interpreter/interpreter.o interpreter/arithmetic.o
	g++ -g -Wall -std=c++11 interpreter/booleanExpression.o interpreter/arithmetic.o interpreter/command.o interpreter/interpreter.o main.cpp -o hw4

command.o: interpreter/command.h interpreter/command.cpp
	g++ -g -Wall -std=c++11 -c command.cpp -o command.o

interpreter.o: interpreter/interpreter.h interpreter/interpreter.cpp
	g++ -g -Wall -std=c++11 -c interpreter.cpp -o interpreter.o

arithmetic.o: interpreter/arithmetic.h interpreter/arithmetic.cpp
	g++ -g -Wall -std=c++11 -c arithmetic.cpp -o arithmetic.o	

booleanExpression.o: interpreter/booleanExpression.h interpreter/booleanExpression.cpp
	g++ -g -Wall -std=c++11 -c booleanExpression.cpp -o booleanExpression.o

FORCE: ;

.phony: clean hw4

clean:
	-@rm -rf interpreter/*.o	
