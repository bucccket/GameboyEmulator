TARGET_EXEC ?= emulator

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
LIB_DIR ?= ./build/lib

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c99 -g -Iminifb/include 
CPPFLAGS += -Wall -Werror -Wpedantic

LDFLAGS ?= -lX11 -L./$(LIB_DIR) -lminifb -lX11 -lGL -lncurses

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(LIB_DIR)/libminifb.a
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

#dependencies
$(LIB_DIR)/libminifb.a:
	$(MKDIR_P) $(LIB_DIR)
	cmake -B ./$(LIB_DIR) -S ./minifb -DUSE_OPENGL_API=ON -DUSE_WAYLAND_API=OFF
	cmake --build ./$(LIB_DIR)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
