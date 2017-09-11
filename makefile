APP=knights
CXXFLAGS=-Wall -std=c++11 -O3 -g
LDFLAGS=-lpthread -lSDL

src=$(wildcard *.cpp)
obj=$(src:.cpp=.o)

build_all: $(obj)
	$(CXX) $^ -o $(APP) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(APP)
