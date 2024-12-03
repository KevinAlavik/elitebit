PACKAGE_NAME := elitebit-emulator
PACKAGE_VERSION := 0.0.1

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wno-unused-parameter -O2 -g -I. $(shell pkg-config --cflags spdlog)
LDFLAGS := $(shell pkg-config --libs spdlog)

prefix := /usr/local
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
libdir := $(exec_prefix)/lib
includedir := $(prefix)/include
datadir := $(prefix)/share

BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/$(PACKAGE_NAME)

SRCS := Main.cpp Bus.cpp Memory.cpp EB16.cpp
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET)
	@mkdir -p $(DESTDIR)$(bindir)
	install -m 0755 $(TARGET) $(DESTDIR)$(bindir)/$(PACKAGE_NAME)

uninstall:
	@rm -f $(DESTDIR)$(bindir)/$(PACKAGE_NAME)

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

dist: $(TARGET)
	@tar -czvf $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(TARGET) LICENSE

-include $(OBJS:.o=.d)

$(BUILD_DIR)/%.d: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CXXFLAGS) $< | sed 's|$(notdir $*)\.o|$(BUILD_DIR)/$(notdir $*)\.o|' > $@
