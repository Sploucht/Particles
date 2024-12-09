CXX := g++
SOURCES := main.cpp Engine.cpp Particle.cpp Matrices.cpp
OBJECTS := $(SOURCES:.cpp=.o)
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -pthread
CPPFLAGS := -g -Wall -fpermissive -std=c++17 -pthread
TARGET := game.out

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)
