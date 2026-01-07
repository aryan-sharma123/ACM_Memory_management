CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = src/main.cpp \
      src/allocator/allocator.cpp \
      src/buddy/buddy.cpp \
      src/cache/cache.cpp \
      src/virtual_memory/virtual_memory.cpp

OUT = memsim

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f memsim
