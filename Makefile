BUILD_DIR = build
BINARY = scop
NUM_CORES = $(shell nproc 2>/dev/null || echo 4)

.PHONY: all build run clean re help

all: build

build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; \
		ln -sf ./build/compile_commands.json .; \
	fi
	cmake --build $(BUILD_DIR) -j $(NUM_CORES)

r run: build
	@./$(BUILD_DIR)/$(BINARY)

c clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		cmake --build $(BUILD_DIR) --target clean; \
	fi

fclean:
	rm -rf $(BUILD_DIR)

re: clean build
