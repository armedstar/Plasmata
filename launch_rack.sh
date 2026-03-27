#!/bin/bash

# Launch VCV Rack from terminal to see error messages
# This will show any plugin loading errors

echo "Launching VCV Rack from terminal..."
echo "Look for any error messages about 'VCO' or 'VCONoiseDelay' or 'VCO-Noise-Delay'"
echo ""
echo "Press Ctrl+C to quit VCV Rack when done"
echo ""

"/Applications/VCV Rack 2 Free.app/Contents/MacOS/Rack" 2>&1 | tee rack_output.log
