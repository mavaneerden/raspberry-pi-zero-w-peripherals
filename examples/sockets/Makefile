CXX = g++
CXXFLAGS = -O2 -Wall -Wextra

LDFLAGS = -lpthread

SRCDIR = .
OBJECTS = sockets_example.o socket_client.o socket_server.o

%.o : $(SRCDIR)/%.cpp ../../src/sockets/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

%.o : ../../src/sockets/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sockets: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXEC)