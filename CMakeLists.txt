cmake_minimum_required(VERSION 3.22)
project(SimpleStereoShaper VERSION 1.0.0)

# Set JUCE_DIR to your JUCE source folder when configuring, or add JUCE as a subdirectory.
find_package(JUCE CONFIG QUIET)
if(NOT JUCE_FOUND)
    message(FATAL_ERROR "JUCE was not found. Install JUCE 8 and set JUCE_DIR to its cmake folder.")
endif()

juce_add_plugin(SimpleStereoShaper
    COMPANY_NAME "OpenAI"
    BUNDLE_ID "com.openai.simplestereoshaper"
    PLUGIN_MANUFACTURER_CODE OAI1
    PLUGIN_CODE StSh
    FORMATS VST3 Standalone
    PRODUCT_NAME "Simple Stereo Shaper"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

juce_generate_juce_header(SimpleStereoShaper)
target_sources(SimpleStereoShaper PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)
target_compile_features(SimpleStereoShaper PRIVATE cxx_std_17)
target_link_libraries(SimpleStereoShaper PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
