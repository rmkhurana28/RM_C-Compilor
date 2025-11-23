# RM Compiler Makefile
# Author: Ridham Khurana (RM)
# Date: November 23, 2025

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
LDFLAGS = -lm

# Target executable
TARGET = Main

# Source files
SRCS = Main.c \
       00_print.c \
       01_genTokens.c \
       02_genAST.c \
       03_semanticCheck.c \
       04_icg.c \
       05_optimization.c \
       06_tcg.c \
       06_01_tcg_real.c \
       database.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = database.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files to object files
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJS) $(TARGET)
	rm -f *.s *.asm output.s output.asm
	rm -f tester/*.s tester/*.asm tester/*.out tester/test
	@echo "Clean complete."

# Clean and rebuild
rebuild: clean all

# Run the compiler with default test file
run: $(TARGET)
	@echo "Running $(TARGET) with tester/test.c..."
	./$(TARGET) tester/test.c

# Run with custom file (usage: make test FILE=yourfile.c)
test: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make test FILE=yourfile.c"; \
		exit 1; \
	fi
	@echo "Running $(TARGET) with $(FILE)..."
	./$(TARGET) $(FILE)

# Install (optional - copies to /usr/local/bin)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete."

# Uninstall
uninstall:
	@echo "Uninstalling $(TARGET) from /usr/local/bin..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete."

# Show help
help:
	@echo "RM Compiler - Makefile Help"
	@echo "============================"
	@echo "Available targets:"
	@echo "  make              - Build the compiler (default)"
	@echo "  make all          - Build the compiler"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make run          - Build and run with default test file"
	@echo "  make test FILE=<file> - Build and run with custom file"
	@echo "  make install      - Install to /usr/local/bin (requires sudo)"
	@echo "  make uninstall    - Remove from /usr/local/bin (requires sudo)"
	@echo "  make help         - Show this help message"

# Phony targets (not actual files)
.PHONY: all clean rebuild run test install uninstall help
