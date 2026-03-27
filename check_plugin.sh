#!/bin/bash

echo "=== VCV Rack Plugin Diagnostic ==="
echo ""

PLUGIN_DIR="$HOME/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay"

echo "1. Checking plugin directory..."
if [ -d "$PLUGIN_DIR" ]; then
    echo "   ✅ Directory exists: $PLUGIN_DIR"
else
    echo "   ❌ Directory missing: $PLUGIN_DIR"
    exit 1
fi

echo ""
echo "2. Checking plugin files..."
FILES=("plugin.dylib" "plugin.json" "res/VCONoiseDelay.svg")
for file in "${FILES[@]}"; do
    if [ -f "$PLUGIN_DIR/$file" ]; then
        echo "   ✅ $file exists"
    else
        echo "   ❌ $file missing"
    fi
done

echo ""
echo "3. Checking plugin.dylib architecture..."
ARCH=$(file "$PLUGIN_DIR/plugin.dylib" | grep -o "arm64\|x86_64")
if [ "$ARCH" = "arm64" ]; then
    echo "   ✅ Architecture: arm64 (correct)"
else
    echo "   ⚠️  Architecture: $ARCH (should be arm64)"
fi

echo ""
echo "4. Checking init() function..."
if nm "$PLUGIN_DIR/plugin.dylib" | grep -q " T _init"; then
    echo "   ✅ init() function exported"
else
    echo "   ❌ init() function NOT found"
fi

echo ""
echo "5. Checking modelPlasmata..."
if nm "$PLUGIN_DIR/plugin.dylib" | grep -q "modelPlasmata"; then
    echo "   ✅ modelPlasmata symbol found"
else
    echo "   ❌ modelPlasmata symbol NOT found"
fi

echo ""
echo "6. Checking plugin.json validity..."
if python3 -m json.tool "$PLUGIN_DIR/plugin.json" > /dev/null 2>&1; then
    echo "   ✅ plugin.json is valid JSON"
    echo ""
    echo "   Plugin info:"
    cat "$PLUGIN_DIR/plugin.json" | python3 -c "import sys, json; d=json.load(sys.stdin); print(f\"      Slug: {d['slug']}\"); print(f\"      Name: {d['name']}\"); print(f\"      Version: {d['version']}\"); print(f\"      Modules: {len(d['modules'])}\")"
else
    echo "   ❌ plugin.json is invalid JSON"
fi

echo ""
echo "7. Checking for other plugins..."
OTHER_PLUGINS=$(ls -d "$HOME/Documents/Rack2/plugins-mac-arm64"/*/ 2>/dev/null | wc -l | tr -d ' ')
echo "   Found $OTHER_PLUGINS plugin directories"

echo ""
echo "=== Next Steps ==="
echo "1. Make sure VCV Rack is completely quit (not just closed)"
echo "2. Restart VCV Rack"
echo "3. Press '/' to search for 'VCO' or 'Noise' or 'Delay'"
echo "4. Check VCV Rack's console/terminal for any error messages"
echo "5. Look in Help menu for 'Open user folder' or 'Show log'"
