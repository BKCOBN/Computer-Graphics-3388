# Makefile for Water Shader Project

CXX = g++
CXXFLAGS = -std=c++17 -g -Wall

INCLUDES = -I.
LIBS = -lglfw -lGLEW -lGL
SRC = A6-Water.cpp
OBJ = $(SRC:.cpp=.o)

UTILS = PlaneMesh.hpp ShaderUtils.hpp BMPReader.hpp GlobeCamera.hpp

BIN = water

all: $(BIN)

$(BIN): $(SRC) $(UTILS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(INCLUDES) $(LIBS)

clean:
	rm -f $(BIN) *.o
