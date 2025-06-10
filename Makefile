CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRC = src/filter.cpp src/main.cpp src/io.cpp src/schema.cpp src/select.cpp
TARGET = megatron

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)

