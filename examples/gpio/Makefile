CXX = g++
CXXFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lgpiodcxx

INCDIR = ../../src/gpio/include
DEPS = $(INCDIR)/gpio_pin.hpp

SRCDIR = ../../src/gpio
OBJECTS = blink.o gpio_pin.o

%.o : $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

blink: blink.o gpio_pin.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXEC)