CC = g++
CFLAGS = -g -Wall -Wextra -std=c++17
INCLUDES = -Iinclude
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET_FOLDER = bin
TARGET = $(TARGET_FOLDER)/main

MODULES = . utils engine engine/bullTricker engine/checkers engine/loot graphics graphics/bullTricker graphics/checkers graphics/loot
SOURCES = $(foreach module, $(MODULES), $(wildcard $(addprefix src/, $(module)/*.cpp)))
OBJDIRS = $(foreach module, $(MODULES), obj/$(module))
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET_FOLDER) :
	mkdir -p $(TARGET_FOLDER)

$(TARGET): $(OBJDIRS) $(TARGET_FOLDER) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(TARGET) $(OBJS)
	cp -r resources $(TARGET_FOLDER)

$(OBJDIRS):
	mkdir -p $@

obj/%.o: src/%.cpp | $(OBJDIRS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$<

clean:
	rm -rf $(TARGET_FOLDER)
	rm -rf obj
