CC = gcc
CXX = g++

CPPFLAGS = -I..
CFLAGS = -g -std=gnu99 -Wall -Wno-unused-parameter -Wno-unused-function -O3
CXXFLAGS = -g -Wall -O3
LDFLAGS = -lpthread -lm

TARGETS := opencv_demo

.PHONY: all
all: opencv_demo

# apriltag_demo: apriltag_demo.o ../libapriltag.a
#         @echo "   [$@]"
#         @$(CC) -o $@ $^ $(LDFLAGS)

opencv_demo: opencv_demo.o ../libapriltag.a
	@echo "   [$@]"
	@$(CXX) -o $@ $^ $(LDFLAGS) `pkg-config --libs opencv`

%.o: %.c
	@echo "   $@"
	@$(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

%.o: %.cc
	@echo "   $@"
	@$(CXX) -o $@ -c $< $(CXXFLAGS) $(CPPFLAGS)

.PHONY: clean
clean:
	@rm -rf *.o $(TARGETS)
