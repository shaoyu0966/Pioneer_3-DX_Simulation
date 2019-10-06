CXX = g++
CFLAGS = -fPIC -g -Wall
ARIA_INCLUDE = -I/usr/local/Aria/include
ARIA_LINK = -L/usr/local/Aria/lib -lAria -lpthread -ldl -lrt

all: part_c part_e bonus

part_c: part_c.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) part_c.cpp -o part_c $(ARIA_LINK)

part_e: part_e.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) part_e.cpp -o part_e $(ARIA_LINK)

bonus: bonus.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) bonus.cpp -o bonus $(ARIA_LINK)

# $<: current dependencies
# $@: current target
# $*: current dependencies, without file extensions

# clean:
# 	@rm -rf example
