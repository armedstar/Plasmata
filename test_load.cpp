#include <dlfcn.h>
#include <iostream>

int main() {
    void* handle = dlopen("/Users/armedstar/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/plugin.dylib", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot load library: " << dlerror() << std::endl;
        return 1;
    }
    
    void (*init)(void*) = (void (*)(void*)) dlsym(handle, "init");
    if (!init) {
        std::cerr << "Cannot find symbol init: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }
    
    std::cout << "Plugin loaded successfully!" << std::endl;
    std::cout << "init() function found at: " << (void*)init << std::endl;
    
    dlclose(handle);
    return 0;
}
