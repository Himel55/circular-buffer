# Source, Executable, Includes, Library Defines
INC_DIR 	= inc/
SRC_DIR 	= src/
TEST_DIR  = test/
DEMO_DIR  = demo/
CATCH_DIR = thirdparty/catch2/
BIN_DIR   = bin/
OBJ_DIR 	= $(BIN_DIR)obj/
OUTPUT		= CircularBuffer

SRC_FILES 			= $(wildcard $(SRC_DIR)*.c)
SRC_FILES_TEST 	= $(wildcard $(TEST_DIR)*.cpp)
SRC_FILES_DEMO  = $(wildcard $(DEMO_DIR)*.c)

OBJ_FILES_LIB 	= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES)) 
OBJ_FILES_TEST 	= $(patsubst $(TEST_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES_TEST))
OBJ_FILES_DEMO  = $(patsubst $(DEMO_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES_DEMO))

TEST_EXE 			= $(BIN_DIR)Test_$(OUTPUT).exe
DEMO_EXE      = $(BIN_DIR)Demo.exe
LIB_OUTPUT 		= $(BIN_DIR)lib$(OUTPUT).a

# Make Directories 
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Compiler, Linker Includes
CC      	= g++
INCLUDES  = -I$(INC_DIR) -I$(CATCH_DIR)
CFLAGS  	= -c -Wall -g $(INCLUDES)
LDFLAGS = -static-libgcc -static-libstdc++
LDFLAGS_DEMO = -I$(INC_DIR) -L$(BIN_DIR) -l$(OUTPUT)

all: test lib demo
demo: $(DEMO_EXE)
test: $(TEST_EXE)
lib: $(LIB_OUTPUT)

# Compile C Source Files into Object Files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
			$(CC) $(CFLAGS) $< -o $@

# Compile C Source Files into Object Files
$(OBJ_DIR)%.o: $(DEMO_DIR)%.c
			$(CC) $(CFLAGS) $< -o $@

# Compile CPP Source Files into Object Files
$(OBJ_DIR)%.o: $(TEST_DIR)%.cpp
			$(CC) $(CFLAGS) $< -o $@

# Link all Object Files into a Test Binary
$(TEST_EXE): $(OBJ_FILES_LIB) $(OBJ_FILES_TEST)
			$(CC) $(OBJ_FILES_LIB) $(OBJ_FILES_TEST) $(LDFLAGS) -o $(TEST_EXE)

# Archive into a Static Library
$(LIB_OUTPUT): $(OBJ_FILES_LIB)
			$(AR) rcs $(LIB_OUTPUT) $(OBJ_FILES_LIB)

# Link Object Files and Lib into a Demo Binary
$(DEMO_EXE): $(LIB_OUTPUT) $(OBJ_FILES_DEMO)
				$(CC) $(OBJ_FILES_DEMO) $(LDFLAGS_DEMO) $(LDFLAGS) -o $(DEMO_EXE)

# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
			rm -rf $(BIN_DIR)