############################################################
# PARAMETERS

CC := g++

CFLAGS := -g -Wall -Wextra 
CFLAGS += -std=c++11 # std=c++17 necessary for std::optional

INCLUDES := -Iinclude
LIBS := -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system

TARGET_FOLDER := bin
TARGET := $(TARGET_FOLDER)/main
TARGET_TEST := $(TARGET_FOLDER)/test

# Folders in which there are .cpp files
MODULES := . 
MODULES += test
MODULES += utils geometry
MODULES += engine
MODULES += engine/bullTricker engine/checkers engine/loot 
MODULES += graphics
MODULES += graphics/bullTricker graphics/checkers graphics/loot

.DEFAULT_GOAL := all

############################################################
# MECANISM

SOURCES = $(foreach module, $(MODULES), $(wildcard $(addprefix src/, $(module)/*.cpp)))
OBJDIRS = $(foreach module, $(MODULES), obj/$(module))
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))

$(TARGET_FOLDER) :
	mkdir -p $(TARGET_FOLDER)
	cp -r resources $(TARGET_FOLDER)

$(TARGET): $(OBJDIRS) $(TARGET_FOLDER) $(OBJS)
	rm obj/main.o
	$(CC) -c src/main.cpp -o obj/main.o $(CFLAGS) $(INCLUDES) $(LIBS) -DTEST=0
	$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) -o $@

$(TARGET_TEST): $(OBJDIRS) $(TARGET_FOLDER) $(OBJS)
	rm obj/main.o
	$(CC) -c src/main.cpp -o obj/main.o $(CFLAGS) $(INCLUDES) $(LIBS) -DTEST=1
	
	$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) -o $@


$(OBJDIRS):
	mkdir -p $@

obj/%.o: src/%.cpp | $(OBJDIRS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@

############################################################
# COMMANDS

.PHONY: all clean run

all: $(TARGET) $(TARGET_TEST)

run: $(TARGET)
	./$<

test: $(TARGET_TEST)
	./$<

clean:
	rm -rf $(TARGET_FOLDER)
	rm -rf obj
