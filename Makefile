CC = /usr/bin/g++
FILE_SUFFIXES = '.*\.\(c\|h\)' 
FORMAT_TARGET = clang-format -i -style=Mozilla

#PATHS
SRC_PATH = src
LIB_FILES = posthandler.c 
EXE_FILE = april.cc
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
LIB_PATH = $(BUILD_PATH)/lib

# Target names
EXETARGET = run
LIBTARGET = libposthandler.so

# extensions
SRC_EXT = c

# SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
SOURCES = $(addprefix $(SRC_PATH)/, $(LIB_FILES))
EXESOURCE = $(SRC_PATH)/$(EXE_FILE)

OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJECTS:.o=.d)

CFLAGS = -ansi -Wall -Wextra -std=gnu++11 -O3 -fpic -Werror -g -Wno-unused-parameter 
CLIBFLAGS = -shared
INCLUDES = -I include -I /usr/local/include -I /usr/include/opencv4
LIBS = -lcurl -lapriltag
LIBS := $(LIBS) $(shell pkg-config --libs opencv4)

release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating build directory"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)
	@mkdir -p $(LIB_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(EXETARGET) symlink"
	@$(RM) $(EXETARGET)
	@$(RM) $(LIBTARGET)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)
	@$(RM) -r $(LIB_PATH)
	@$(RM) *.pnm
	@$(RM) *.log
	@$(RM) *.ps
	@$(RM) ./src/*.pnm
	@$(RM) ./src/*.ps


.PHONY: all
all: $(BIN_PATH)/$(EXETARGET) $(LIB_PATH)/$(LIBTARGET)
	@echo $(SOURCES)
	@echo "Making symlink: $(EXETARGET) -> $<"
	@$(RM) $(EXETARGET)
	@ln -s $(BIN_PATH)/$(EXETARGET) $(EXETARGET)

.PHONY: format
format:
	@find ./src -regex $(FILE_SUFFIXES) | xargs $(FORMAT_TARGET)
	@find ./include -regex $(FILE_SUFFIXES) | xargs $(FORMAT_TARGET)


$(LIB_PATH)/$(LIBTARGET): $(OBJECTS)
	@echo "Creating libraries out of $@"
	$(CC) $(OBJECTS) $(CLIBFLAGS) -o $@
	# @mv $(LIBTARGET) $@

$(BIN_PATH)/$(EXETARGET): $(OBJECTS) 
	@echo "Linking: $@"
	$(CC) $(OBJECTS) $(INCLUDES) $(EXESOURCE) $(LIBS) -o $@

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -MP -MMD -c $< -o $@
