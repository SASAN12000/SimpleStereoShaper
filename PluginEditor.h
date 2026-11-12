#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimpleStereoShaperAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SimpleStereoShaperAudioProcessorEditor(SimpleStereoShaperAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    SimpleStereoShaperAudioProcessor& processor;
    juce::Label heading;
    juce::Slider width, ll, lr, rl, rr, leftGain, rightGain, output, mix;
    juce::ToggleButton invertL, invertR;
    juce::ComboBox monitor;
    juce::Label widthLabel, matrixLabel, llLabel, lrLabel, rlLabel, rrLabel,
                leftGainLabel, rightGainLabel, outputLabel, mixLabel, monitorLabel;
    std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
    std::unique_ptr<ButtonAttachment> invLAttachment, invRAttachment;
    std::unique_ptr<ComboAttachment> monitorAttachment;

    void setupSlider(juce::Slider&, juce::Label&, const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleStereoShaperAudioProcessorEditor)
};
