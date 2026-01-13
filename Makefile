NAME        = scop
BUILD_DIR   = build
BUILD_TYPE  ?= Debug
NUM_CORES   = $(shell nproc 2>/dev/null || echo 4)

.PHONY: all build run clean fclean re

all: build

MAKEFLAGS += --no-print-directory

build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -Wno-dev > /dev/null; \
	fi
	@cmake --build $(BUILD_DIR) -j $(NUM_CORES) -- --no-print-directory -s 2>&1 | grep -v "Built target" || true
	@ln -sf $(BUILD_DIR)/compile_commands.json .

run: build
	@./$(BUILD_DIR)/bin/$(NAME)

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		cmake --build $(BUILD_DIR) --target clean -- --no-print-directory -s; \
	fi

fclean:
	@rm -rf $(BUILD_DIR)
	@rm -f compile_commands.json

re: fclean all
