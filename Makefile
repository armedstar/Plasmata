RACK_DIR ?= ../Rack

SOURCES += plugin.cpp
SOURCES += $(wildcard src/*.cpp)

DISTRIBUTABLES += res
LDFLAGS += -Wl,-exported_symbol,_init

include $(RACK_DIR)/plugin.mk
