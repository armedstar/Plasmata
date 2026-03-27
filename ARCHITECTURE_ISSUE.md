# Architecture Mismatch Issue

## Problem
Your VCV Rack is running as **x86_64** (Intel/Rosetta), but your Rack SDK was built for **arm64** (Apple Silicon). This creates an incompatibility when building plugins.

## Solutions

### Option 1: Use Native ARM64 VCV Rack (Recommended)
If VCV Rack has a native ARM64 version available:
1. Download/install the ARM64 version of VCV Rack
2. Your plugin folder will be `~/Documents/Rack2/plugins-mac-arm64/`
3. Build the plugin normally (it will be ARM64, matching your SDK):
   ```bash
   cd /Users/armedstar/Projects/VCVRack
   make RACK_DIR=~/Rack
   ```
4. Install to: `~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/`

### Option 2: Rebuild Rack SDK for x86_64
This requires rebuilding the entire SDK and all dependencies for x86_64:

1. **Rebuild dependencies for x86_64:**
   ```bash
   cd ~/Rack
   make clean
   arch -x86_64 zsh
   make dep
   make
   exit  # exit Rosetta shell
   ```

2. **Then build your plugin:**
   ```bash
   cd /Users/armedstar/Projects/VCVRack
   arch -x86_64 zsh
   make RACK_DIR=~/Rack
   exit
   ```

### Option 3: Use Rosetta for Everything
Run VCV Rack under Rosetta and build everything in a Rosetta shell. This is what you're currently doing, but the SDK needs to match.

## Current Status
- ✅ Plugin code is complete and correct
- ✅ Version fixed to 2.0.0
- ❌ Architecture mismatch: SDK is ARM64, Rack is x86_64
- ❌ Plugin cannot link properly due to architecture mismatch

## Quick Check
To see what architecture your VCV Rack is:
```bash
file /Applications/VCV\ Rack.app/Contents/MacOS/Rack
```

If it says `x86_64`, you need Option 2 or 3.
If it says `arm64`, you can use Option 1.
