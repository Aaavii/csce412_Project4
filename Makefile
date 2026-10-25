.RECIPEPREFIX = >

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = loadbalancer

SRCS = main.cpp RequestQueue.cpp WebServer.cpp LoadBalancer.cpp Firewall.cpp Config.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.h)

all: $(TARGET)

$(TARGET): $(OBJS)
> $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp $(HEADERS)
> $(CXX) $(CXXFLAGS) -c $<

clean:
> rm -f $(TARGET) $(TARGET).exe $(OBJS)

.PHONY: all clean