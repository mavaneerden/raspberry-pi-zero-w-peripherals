CXX = g++
CXXFLAGS = -O2 -Wall -Wextra

INCDIR = include
DEPS = $(INCDIR)/ssd1306.hpp

SRCDIR = .
OBJECTS = oled_example.o ssd1306.o i2c_bus.o i2c_device.o i2c_exception.o i2c.o

%.o : $(SRCDIR)/%.cpp ../../src/i2c/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

%.o : ../../src/i2c/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

i2c.o : ../../lib/libi2c/i2c.c
	gcc -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

oled: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXEC)