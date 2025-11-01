# Makefile as a command runner
BUILD_DIR := build
CMAKE := cmake
CTEST := ctest
BUILD_TYPE ?= Debug

# Colors
RESET  := \033[0m
BOLD   := \033[1m
RED    := \033[31m
GREEN  := \033[32m
YELLOW := \033[33m
BLUE   := \033[34m
MAGENTA := \033[35m
CYAN   := \033[36m

# Default target
.PHONY: all
all: help

# Configure and build
.PHONY: build
build:
	@echo "$(CYAN)Configuring and building project... $(RESET)"
	@$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	@$(CMAKE) --build $(BUILD_DIR)
	@echo "$(GREEN)Build successful!$(RESET)" || \
	( echo "$(RED)Build failed!$(RESET)"; exit 1 )

# Run all tests
.PHONY: test
test: build
	@echo "$(MAGENTA)Running tests...$(RESET)"
	@cd $(BUILD_DIR) && $(CTEST) --output-on-failure

# Clean build directory
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@echo "$(YELLOW)Build directory cleaned.$(RESET)"

# Build docs - use VERBOSE=1 for loud
.PHONY: docs
docs: build
	@echo "$(BLUE)Building docs...$(RESET)"
ifeq ($(VERBOSE),1)
	@cmake --build $(BUILD_DIR) --target doc
else
	@cmake --build $(BUILD_DIR) --target doc > /dev/null 2>&1 && \
	echo "$(GREEN)Docs build successful!$(RESET)" || \
	( echo "$(RED)Docs build failed!$(RESET)\n  $(YELLOW)Try running with VERBOSE=1 to see full output.$(RESET)"; exit 1 )
endif

# Displays help command
.PHONY: help
help:
	@echo "$(BOLD)Available Commands:$(RESET)"
	@echo "  $(CYAN)make build$(RESET)     - Build the project"
	@echo "  $(MAGENTA)make test$(RESET)      - Run all tests"
	@echo "  $(YELLOW)make clean$(RESET)     - Remove the build directory"
	@echo "  $(BLUE)make docs$(RESET)      - Build docs"
	@echo "  $(GREEN)make help$(RESET)      - Show this help message"
