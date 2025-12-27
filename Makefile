BUILD_DIR = build
BINARY_NAME = scop
BINARY_PATH = $(BUILD_DIR)/bin/$(BINARY_NAME)
NUM_CORES = $(shell nproc 2>/dev/null || echo 4)

.PHONY: all build run clean re

all: build

build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Configuring CMake..."; \
		cmake -S . -B $(BUILD_DIR); \
	fi
	@if [ -f "$(BUILD_DIR)/compile_commands.json" ]; then \
		ln -sf "$(BUILD_DIR)/compile_commands.json" .; \
	fi
	@cmake --build $(BUILD_DIR) -j $(NUM_CORES)

run: build
	@echo "Running $(BINARY_NAME)..."
	@./$(BINARY_PATH)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f compile_commands.json
	@echo "Cleaned build directory."

re: clean all
