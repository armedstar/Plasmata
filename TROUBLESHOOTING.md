# Troubleshooting: Plugin Not Appearing

## Current Status
✅ Plugin built successfully for ARM64
✅ init() function exported correctly
✅ modelPlasmata symbol present
✅ plugin.json is valid JSON
✅ Files in correct location: ~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/
✅ SVG file present
✅ Architecture matches (ARM64)

## Things to Check

1. **Restart VCV Rack completely** - Make sure you quit and restart, not just close the window

2. **Check VCV Rack Console/Log:**
   - Open VCV Rack
   - Look for any error messages in the console
   - Check Help → About or look for a log file

3. **Verify Plugin Location:**
   ```bash
   ls -la ~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/
   ```
   Should show: plugin.dylib, plugin.json, res/

4. **Try searching in VCV Rack:**
   - Press `/` to open search
   - Type "VCO" or "Noise" or "Delay"
   - Check all categories (Oscillator, Effect, etc.)

5. **Check if other plugins work:**
   - If no plugins appear, there might be a VCV Rack configuration issue

6. **Manual verification:**
   ```bash
   # Check if plugin can be loaded
   otool -L ~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/plugin.dylib
   
   # Verify symbols
   nm ~/Documents/Rack2/plugins-mac-arm64/VCONoiseDelay/plugin.dylib | grep init
   ```

## Possible Issues

1. **VCV Rack version mismatch** - Make sure you're using Rack 2.x
2. **Plugin folder permissions** - Make sure files are readable
3. **Missing dependencies** - Though we used -undefined dynamic_lookup, there might be runtime issues

## Next Steps

If still not working, try:
1. Check VCV Rack's console output when it starts
2. Try creating a minimal test plugin to verify the setup works
3. Check if VCV Rack has a "Developer" or "Debug" mode that shows more information
