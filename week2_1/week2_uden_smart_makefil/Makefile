CXX = g++-14
HEADER = first_text.h second_text.h third_text.h

greeting.out: greeting.o first_text.o second_text.o third_text.o
	$(CXX) $^ -o $@

%.o: %.cpp $(HEADER)
	$(CXX) -c $< -o $@
