CC = g++
CFLAGS = -g -Wall -Wextra -std=c++17 # pour type optional
INCLUDES = -Iinclude
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET_FOLDER = bin
TARGET = $(TARGET_FOLDER)/main

FOLDERS = . utils engine engine/bullTricker engine/checkers engine/loot graphics graphics/bullTricker graphics/checkers graphics/bullTricker
SOURCES = $(foreach folder, $(FOLDERS), $(wildcard $(addprefix src/, $(folder)/*.cpp)))

OBJDIR = $(foreach folder, $(FOLDERS), obj/$(folder))
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET_FOLDER):
	mkdir -p $@

#$(OBJDIR): :
#	mkdir -p $@

#$(OBJDIR)/%.o: src/%.cpp
#	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#$(TARGET): $(OBJDIR)
#	mkdir $(TARGET_FOLDER)
#	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $(LIBS)
#	cp -r resources $(TARGET_FOLDER)

$(TARGET): $(TARGET_FOLDER)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(SOURCES) -o $(TARGET)
	cp -r resources $(TARGET_FOLDER)

run: $(TARGET)
	./$<

clean:
	rm -rf $(TARGET_FOLDER)
	rm -rf obj