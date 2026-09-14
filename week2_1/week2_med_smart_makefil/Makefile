CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lgreeting
TARGET = greeting
HEADER = first_text.h second_text.h third_text.h

all: $(TARGET).out

$(TARGET).out: $(TARGET).o
	$(CXX) $(CXXFLAGS) $@

%.o: %.cpp $(HEADER)
	$(CXX) -c $< -o $@

clean:
	rm -f *.o $(TARGET).out
