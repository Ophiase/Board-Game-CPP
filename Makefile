CC = g++
CFLAGS = -g -Wall -Wextra -std=c++11
INCLUDES = -Iinclude
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = main
SOURCES = $(wildcard src/*.cpp) $(wildcard src/engine/*.cpp) $(wildcard src/graphics/*.cpp)

OBJDIR = bin
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

.PHONY: all clean run

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $(OBJ) $(LIBS)
	cp -r resources $(OBJDIR)

run: $(TARGET)
	./$(OBJDIR)/$(TARGET)

clean:
	rm -rf $(OBJDIR)