CXX = g++
CXXFLAGS = -Wall

TGT = main

SRCDIR = src

SRC = $(SRCDIR)/main.cpp \
      $(SRCDIR)/MMAPGPIO.cpp
OBJ = $(SRC:.cpp=.o)
HDR = inc

BUILD = build
BIN = bin
BIN_PATH = $(BUILD)/$(BIN)

all: $(TGT).out

install:
	@echo 'Creating build directory $(BIN_PATH)...'
	@mkdir -p $(BUILD)
	@mkdir -p $(BIN_PATH)
	@echo 'Moving binary file(s) to $(BIN_PATH)...'
	@mv $(TGT).out $(BIN_PATH)
	@echo 'Done.'

%.out: $(OBJ)

	$(CXX) $(CXXFLAGS) -I$(HDR) $^ -o $@

%.o: %.cpp

	$(CXX) $(CXXFLAGS) -I$(HDR) -c $^ -o $@

clean:
	@echo 'Deleting build files and build directory "$(BIN_PATH)"...'
	@rm -f $(BIN_PATH)/*.out
	@rm -r $(BUILD)
