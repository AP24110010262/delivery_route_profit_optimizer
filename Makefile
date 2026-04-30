CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
TARGET   = DeliveryOptimizer
SRCS     = src/main.cpp src/Order.cpp src/RoutePlanner.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
