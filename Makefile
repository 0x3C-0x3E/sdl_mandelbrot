PROJECTNAME = project
OUTPUT_DIR = build
OBJ_DIR = $(OUTPUT_DIR)/obj

CXX = gcc
CXXFLAGS = -Wall -g3 -ggdb
INCLUDE_DIRS = -Iinclude/SDL2 $(addprefix -I, $(wildcard src/*/)) -Iinclude/imgui -Isrc
LIB_DIRS = -Llib
LIBS = -lmingw32 -lws2_32 -limgui -lSDL2main -lSDL2

# Gather all source files and corresponding object files
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Default target
default: $(OUTPUT_DIR)/$(PROJECTNAME)

# Link object files into the final executable
$(OUTPUT_DIR)/$(PROJECTNAME): $(OBJ)
	@mkdir -p $(OUTPUT_DIR)
	@$(CXX) $(OBJ) -o $@ $(LIB_DIRS) $(LIBS) $(CXXFLAGS)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDE_DIRS)
	
# Clean target - remove only object files and the final executable
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR)  # Remove all object files
	@rm -f $(OUTPUT_DIR)/$(PROJECTNAME)  # Remove the final executable
	@find $(OUTPUT_DIR) -type f -name '*.o' -delete  # Delete any leftover .o files
	@find $(OUTPUT_DIR) -type f ! -name '*.dll' -delete  # Keep .dll files
	@find $(OUTPUT_DIR) -type d -empty -delete  # Remove empty folders

.PHONY: default clean
