LAB_N=lab8

compile:
	g++ -o $(LAB_N) main.cpp ScopedTimer.cpp File.cpp OHLCAggregator.cpp \
	-I. \
	-l pthread \
	-l boost_system

run:
	./$(LAB_N)

all: compile run
