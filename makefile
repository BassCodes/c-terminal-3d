# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := final_program

BUILD_DIR     := ./build
SRC_DIRS 	  := ./src ./includes
RESOURCE_DIRS := ./resources 

CC  := gcc
ld  := gcc


LDFLAGS = -lm
BASEFLAGS = -std=c17 -g -O3 -D_POSIX_C_SOURCE=199309L -Wall -Wextra -Wconversion -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wshadow -Wformat=2 
CFLAGS    = $(BASEFLAGS) -Wjump-misses-init 
# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	cp -r resources/. build 
	$(ld) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@




.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

