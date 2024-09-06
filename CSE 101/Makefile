CXX = g++
CXXFLAGS = -Wall -gdwarf-4

OBJECTS = sixdegrees.o

sixdegrees: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o sixdegrees sixdegrees.o
	
sixdegrees.o: sixdegrees.cpp
	$(CXX) -g $(CXXFLAGS) -c sixdegrees.cpp

clean: 
	rm -f *.o
	rm sixdegrees
