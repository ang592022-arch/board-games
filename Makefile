CXX ?= g++
CPPFLAGS += -Iinclude
CXXFLAGS ?= -O2
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

ifeq ($(OS),Windows_NT)
EXEEXT := .exe
define ensure_dir
@if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
endef
define run_binary
"$(subst /,\,$1)"
endef
define remove_build
@if exist "$(subst /,\,$(BUILD_DIR))" rmdir /S /Q "$(subst /,\,$(BUILD_DIR))"
endef
else
EXEEXT :=
define ensure_dir
@mkdir -p "$1"
endef
define run_binary
./$1
endef
define remove_build
@rm -rf "$(BUILD_DIR)"
endef
endif

LIB_SOURCES := \
	src/core/board.cpp \
	src/core/input.cpp \
	src/core/text.cpp \
	src/exercises/menu.cpp \
	src/exercises/stones.cpp \
	src/games/gomoku.cpp \
	src/games/n_queen_duel.cpp \
	src/algorithms/n_queen_alg.cpp \
	src/cli/cli.cpp

LIB_OBJECTS := $(LIB_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
APP_OBJECT := $(OBJ_DIR)/app/main.o
TEST_OBJECT := $(OBJ_DIR)/tests/test_runner.o
DEMO_OBJECT := $(OBJ_DIR)/examples/board_demo.o
ALL_OBJECTS := $(LIB_OBJECTS) $(APP_OBJECT) $(TEST_OBJECT) $(DEMO_OBJECT)

APP_BIN := $(BIN_DIR)/board-games$(EXEEXT)
TEST_BIN := $(BIN_DIR)/board-games-tests$(EXEEXT)
DEMO_BIN := $(BIN_DIR)/board-demo$(EXEEXT)

.PHONY: all app tests test run demo clean

all: app tests

app: $(APP_BIN)

tests: $(TEST_BIN)

test: $(TEST_BIN)
	$(call run_binary,$(TEST_BIN))

run: $(APP_BIN)
	$(call run_binary,$(APP_BIN))

demo: $(DEMO_BIN)

$(APP_BIN): $(LIB_OBJECTS) $(APP_OBJECT)
	$(call ensure_dir,$(@D))
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_BIN): $(LIB_OBJECTS) $(TEST_OBJECT)
	$(call ensure_dir,$(@D))
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DEMO_BIN): $(LIB_OBJECTS) $(DEMO_OBJECT)
	$(call ensure_dir,$(@D))
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(call ensure_dir,$(@D))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(call remove_build)

-include $(ALL_OBJECTS:.o=.d)
