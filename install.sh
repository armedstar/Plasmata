#!/bin/bash

# VCV Rack Plugin Installation Script

echo "Installing VCO Noise Delay plugin..."

# Try to find VCV Rack plugins directory
PLUGIN_DIRS=(
    "$HOME/Documents/Rack2/plugins-mac-arm64"
    "$HOME/Documents/Rack2/plugins-mac-x64"
    "$HOME/.Rack2/plugins"
    "$HOME/Library/Application Support/Rack2/plugins"
)

PLUGIN_DIR=""
for dir in "${PLUGIN_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        PLUGIN_DIR="$dir"
        echo "Found plugins directory: $PLUGIN_DIR"
        break
    fi
done

# If not found, use the standard location
if [ -z "$PLUGIN_DIR" ]; then
    PLUGIN_DIR="$HOME/.Rack2/plugins"
    echo "Using default plugins directory: $PLUGIN_DIR"
fi

# Create plugin directory
PLUGIN_INSTALL_DIR="$PLUGIN_DIR/VCONoiseDelay"
mkdir -p "$PLUGIN_INSTALL_DIR"

# Check if source files exist
if [ ! -f "plugin.dylib" ]; then
    echo "Error: plugin.dylib not found. Please build the plugin first."
    exit 1
fi

if [ ! -f "plugin.json" ]; then
    echo "Error: plugin.json not found."
    exit 1
fi

# Copy files
echo "Copying plugin files to $PLUGIN_INSTALL_DIR..."
cp plugin.dylib "$PLUGIN_INSTALL_DIR/"
cp plugin.json "$PLUGIN_INSTALL_DIR/"
cp -r res "$PLUGIN_INSTALL_DIR/"

echo ""
echo "Installation complete!"
echo "Plugin installed to: $PLUGIN_INSTALL_DIR"
echo ""
echo "Files installed:"
ls -lh "$PLUGIN_INSTALL_DIR"
echo ""
echo "Please restart VCV Rack to see the module in the browser."
