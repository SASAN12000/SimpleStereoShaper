# Simple Stereo Shaper (VST3 source project)

A basic stereo utility plugin project for Studio One and other VST3 hosts.

## Features
- Stereo width via Mid/Side
- 2x2 stereo matrix (L→L, L→R, R→L, R→R)
- Independent left/right level
- Left/right polarity inversion
- Stereo / Mid / Side monitor
- Dry/Wet and output level
- Parameter state save/restore

## Important
This ZIP contains the **C++ source project**, not a precompiled VST3 binary.
To build the VST3, you need JUCE 8, CMake, and a C++ compiler (Visual Studio 2022 Build Tools on Windows).
No Rust is needed.

## Build outline (Windows)
1. Install JUCE 8 and CMake.
2. Open CMake GUI or configure with `cmake -S . -B build -DJUCE_DIR="C:/path/to/JUCE"`.
3. Build with `cmake --build build --config Release`.
4. The VST3 will be produced under the build folder. Copy the `.vst3` bundle to:
   `C:\Program Files\Common Files\VST3`
5. In Studio One, rescan plug-ins if it does not appear.

The project is a starting version; test at low monitoring volume and verify routing before using it on a master bus.
