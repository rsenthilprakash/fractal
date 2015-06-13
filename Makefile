CC = gcc
CFLAGS = -Wall
LIBS = -lm
CPPLIBS = -lstdc++
PNG_CFLAGS = `pkg-config --cflags libpng`
PNG_LIBS = `pkg-config --libs libpng`

BUILD_DIR = _build_

PNG_SOURCES = png_utils.c

C_SOURCES = fern.c \
            poly.c \
            tree.c \
            maple.c \
            ifs.c \
            julia.c \
            mandel.c \

CPP_SOURCES =  julia_iim.cpp \

PNG_OBJECTS = $(PNG_SOURCES:%.c=$(BUILD_DIR)/%.o)

C_TARGETS = $(C_SOURCES:%.c=$(BUILD_DIR)/%)
CPP_TARGETS =  $(CPP_SOURCES:%.cpp=$(BUILD_DIR)/%)
TARGETS = $(C_TARGETS) $(CPP_TARGETS)

DEPENDS = $(C_SOURCES:%.c=$(BUILD_DIR)/%.d) $(CPP_SOURCES:%.cpp=$(BUILD_DIR)/%.d) $(PNG_SOURCES:%.c=$(BUILD_DIR)/%.d)

.PHONY: all
all: $(TARGETS)

$(C_TARGETS): % : %.o $(PNG_OBJECTS)
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

$(CPP_TARGETS): % : %.o $(PNG_OBJECTS)
	$(CC) $^ $(PNG_LIBS) $(CPPLIBS) -o $@

$(PNG_OBJECTS): $(PNG_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(PNG_CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -MMD $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -MMD $< -o $@

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
