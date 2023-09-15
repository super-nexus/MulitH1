# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Targets
all: MovieRate RatingDistance

MovieRate: MovieRate.cpp
	$(CXX) $(CXXFLAGS) MovieRate.cpp -o MovieRate

RatingDistance: RatingDistance.cpp
	$(CXX) $(CXXFLAGS) RatingDistance.cpp -o RatingDistance

clean:
	rm -f MovieRate RatingDistance
