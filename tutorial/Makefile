CXX=g++ -m64
CXXFLAGS=-O3 -std=c++11 -Wall -g

APP_NAME=tutorial
DEMO=demo

default: $(APP_NAME)

.PHONY: dirs clean

clean:
	/bin/rm -rf $(APP_NAME) $(DEMO)

$(APP_NAME): clean
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp -lpthread

$(DEMO): clean
	$(CXX) $(CXXFLAGS) -o $@ $@.cxx