CXX = g++
CXXFLAGS = -O2 -Wall -Wextra

INCDIR = include
DEPS = $(INCDIR)/ssd1306_spi.hpp

SRCDIR = .
OBJECTS = oled_spi_example.o ssd1306_spi.o spi_driver.o spi.o

%.o : $(SRCDIR)/%.cpp ../../src/spi/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

%.o : ../../src/spi/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

spi.o : ../../lib/azorg_spi/spi.c
	gcc -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

oled: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXEC)