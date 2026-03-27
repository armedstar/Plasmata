#include "plugin.hpp"

Plugin* pluginInstance;

extern "C" void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelPlasmata);
}

