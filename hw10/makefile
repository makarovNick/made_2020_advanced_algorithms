CC = g++
FLAGS = -O2
SRC_PACKAGE = src
INCLUDE_PACKAGE = include
TESTS_PACKAGE = test
BIN_NAME = parser

all : run

run : main.o parser.o lexer.o
	$(CC) $(FLAGS) -o $(BIN_NAME) main.o parser.o lexer.o

test : test.o parser.o lexer.o
	$(CC) $(FLAGS) -o tests test.o parser.o lexer.o

main.o: ${SRC_PACKAGE}/main.cpp $(INCLUDE_PACKAGE)/cxxopts.hpp
	$(CC) -c ${SRC_PACKAGE}/main.cpp $(INCLUDE_PACKAGE)/cxxopts.hpp

parser.o : ${SRC_PACKAGE}/parser.cpp $(INCLUDE_git PACKAGE)/parser.h
	$(CC) -c ${SRC_PACKAGE}/parser.cpp $(INCLUDE_PACKAGE)/parser.h

lexer.o : ${SRC_PACKAGE}/lexer.cpp $(INCLUDE_PACKAGE)/lexer.h
	$(CC) -c ${SRC_PACKAGE}/lexer.cpp $(INCLUDE_PACKAGE)/lexer.h

test.o : ${TESTS_PACKAGE}/test.cpp $(INCLUDE_PACKAGE)/catch.hpp
	$(CC) -c ${TESTS_PACKAGE}/test.cpp $(INCLUDE_PACKAGE)/catch.hpp

clean : 
	rm -f *.o parser tests
	rm -f ${INCLUDE_PACKAGE}/*.gch