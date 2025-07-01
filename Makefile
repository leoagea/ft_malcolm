###############################################################################
# Colors
###############################################################################
GREEN  := \033[0;32m
YELLOW := \033[1;33m
BLUE   := \033[0;34m
NC     := \033[0m

###############################################################################
# Compiler & Flags
###############################################################################
CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -O2 -MMD -MP #-g -fsanitize=address
LDFLAGS  = -lm 
# -MMD and -MP tell the compiler to generate .d (dependency) files for each .c

DEBUG_FLAGS   := -g -fsanitize=address
BUILD_MODE_FILE = $(OBJ_DIR)/.build_mode

###############################################################################
# Project Settings
###############################################################################
TARGET   = ft_malcolm
SRC_DIR  = srcs
INC_DIR  = inc
OBJ_DIR  = build

# Libft info
LIBFT_DIR = libft
LIBFT_LIB = lib
LIBFT     = $(LIBFT_DIR)/$(LIBFT_LIB)/libft.a

###############################################################################
# Sources / Objects
###############################################################################
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
			
SRCS      := $(SRC_FILES)

OBJS      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

DEPS      = $(OBJS:.o=.d)

###############################################################################
# Default Rule
###############################################################################
all: $(TARGET)

###############################################################################
# Linking
###############################################################################
$(TARGET): $(LIBFT) $(OBJS)
	@echo "$(BLUE)Linking $(TARGET)...$(NC)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS)
	@echo "$(GREEN)Build complete!$(NC)"

###############################################################################
# Object File Compilation
###############################################################################
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

###############################################################################
# Libft
###############################################################################
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

###############################################################################
# Cleanup
###############################################################################
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(DEPS) $(BUILD_MODE_FILE)

fclean: clean
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

###############################################################################
# Others Rules
###############################################################################
debug: CFLAGS := $(DEBUG_FLAGS)
debug:
	@if [ ! -f $(BUILD_MODE_FILE) ] || [ "$$(cat $(BUILD_MODE_FILE))" != "debug" ]; then \
		$(MAKE) fclean; \
		mkdir -p $(OBJ_DIR); \
		echo "debug" > $(BUILD_MODE_FILE); \
	fi
	@$(MAKE) all

help:
	@echo "Available targets:"
	@echo "  all      : Build $(TARGET)"
	@echo "  debug    : Build with debug flags"
	@echo "  clean    : Remove object files"
	@echo "  fclean   : Remove all generated files"
	@echo "  re       : Rebuild everything"

###############################################################################
# Tests Rules
###############################################################################
source_ip = "$(shell hostname -I | awk '{print $$1}')"
source_mask = "255.255.255.0"
destination_ip = "10.0.2.19"
destination_mask = "255.255.254.255"

test: all
	./$(TARGET) $(source_ip) $(source_mask) $(destination_ip) $(destination_mask)

###############################################################################
# Dependency Handling
###############################################################################
-include $(DEPS)

###############################################################################
# Phony Targets
###############################################################################
.PHONY: all clean fclean re debug help