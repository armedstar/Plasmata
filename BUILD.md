# Step-by-Step Build Guide

## Step 1: Get the VCV Rack SDK

You need to download and compile the VCV Rack SDK first:

1. **Clone the Rack SDK repository:**
   ```bash
   cd ~
   git clone --recursive https://github.com/VCVRack/Rack.git
   cd Rack
   ```

2. **Build the Rack SDK:**
   ```bash
   make dep
   make
   ```

   This will take some time (10-30 minutes depending on your system).

3. **Note the Rack SDK path:**
   After building, your Rack SDK will be at `~/Rack` (or wherever you cloned it).

## Step 2: Build Your Plugin

Once you have the Rack SDK built, you can build your plugin:

### Option A: Using Makefile (Recommended)

```bash
cd /Users/armedstar/Projects/VCVRack
make RACK_DIR=~/Rack
```

Replace `~/Rack` with the actual path to your Rack SDK if different.

### Option B: Using CMake

```bash
cd /Users/armedstar/Projects/VCVRack
mkdir -p build
cd build
cmake .. -DRACK_DIR=~/Rack
make
```

Replace `~/Rack` with the actual path to your Rack SDK.

## Step 3: Install the Plugin

After building, the compiled plugin will be in the **root directory** of your project as `plugin.dylib` (on macOS), `plugin.so` (on Linux), or `plugin.dll` (on Windows).

1. **Find your VCV Rack plugins folder:**
   - On macOS: `~/Documents/Rack2/plugins-mac-arm64/` (Apple Silicon) or `~/Documents/Rack2/plugins-mac-x64/` (Intel)
   - On Linux: `~/.Rack2/plugins/`
   - On Windows: `%USERPROFILE%\.Rack2\plugins\`
   
   **Note:** You can also find it by opening VCV Rack and going to Help → Open user folder

2. **Create the plugin directory:**
   ```bash
   # On macOS (adjust for your architecture):
   mkdir -p ~/Documents/Rack2/plugins-mac-x64/VCONoiseDelay
   # or for Apple Silicon:
   # mkdir -p ~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay
   ```

3. **Copy the plugin files:**
   ```bash
   # On macOS (adjust path for your architecture):
   cp plugin.dylib ~/Documents/Rack2/plugins-mac-x64/VCONoiseDelay/
   cp plugin.json ~/Documents/Rack2/plugins-mac-x64/VCONoiseDelay/
   cp -r res ~/Documents/Rack2/plugins-mac-x64/VCONoiseDelay/
   
   # On Linux:
   # cp plugin.so ~/.Rack2/plugins/VCONoiseDelay/
   
   # On Windows:
   # cp plugin.dll %USERPROFILE%\.Rack2\plugins\VCONoiseDelay\
   ```

## Step 4: Launch VCV Rack

1. Open VCV Rack
2. Your module should appear in the module browser
3. Search for "VCO Noise Delay" or look in the "Oscillator" or "Effect" categories

## Troubleshooting

### If build fails with "RACK_DIR not found":
- Make sure you've built the Rack SDK first
- Verify the path: `ls ~/Rack/plugin.mk` should exist
- Use the full absolute path: `make RACK_DIR=/Users/yourname/Rack`

### If plugin doesn't appear in VCV Rack:
- Check the plugin folder exists: `ls ~/.Rack2/plugins/VCONoiseDelay/`
- Make sure `plugin.json` is in the plugin folder
- Restart VCV Rack
- Check VCV Rack's log for errors (usually in the console or log file)

### If you get compilation errors:
- Make sure you're using Rack 2.x SDK (not Rack 1.x)
- Ensure you have a C++17 compatible compiler
- Try cleaning and rebuilding: `make clean && make RACK_DIR=~/Rack`

