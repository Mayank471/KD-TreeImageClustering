CC = g++
# Use a broadly supported standard for Windows MinGW; adjust if your compiler is newer
CXXFLAGS = -std=gnu++17 -O2 -Wall

all: project.exe histogram.exe

project.exe: project.cpp KdTree.hpp json.hpp
	$(CC) $(CXXFLAGS) project.cpp -o project.exe

histogram.exe: histogram.cpp json.hpp stb_image.h
	$(CC) $(CXXFLAGS) histogram.cpp -o histogram.exe

.PHONY: clean
clean:
	-del /Q *.o 2> NUL
	-del /Q *.exe 2> NUL