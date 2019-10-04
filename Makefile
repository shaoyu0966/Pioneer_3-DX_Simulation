CXX = g++
CFLAGS = -fPIC -g -Wall
ARIA_INCLUDE = -I/usr/local/Aria/include
ARIA_LINK = -L/usr/local/Aria/lib -lAria -lpthread -ldl -lrt

main: *.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) $< -o $@ $(ARIA_LINK)
	# $<: current dependencies
	# $@: current target
	# $*: current dependencies, without file extensions

# clean:
# 	@rm -rf example
