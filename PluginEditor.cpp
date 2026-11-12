#include "PluginEditor.h"

SimpleStereoShaperAudioProcessorEditor::SimpleStereoShaperAudioProcessorEditor(SimpleStereoShaperAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(560, 390);
    heading.setText("SIMPLE  STEREO  SHAPER", juce::dontSendNotification);
    heading.setFont(juce::Font(22.0f, juce::Font::bold));
    heading.setColour(juce::Label::textColourId, juce::Colour(0xfff2f2f2));
    addAndMakeVisible(heading);

    setupSlider(width, widthLabel, "WIDTH");
    setupSlider(ll, llLabel, "L → L");
    setupSlider(lr, lrLabel, "L → R");
    setupSlider(rl, rlLabel, "R → L");
    setupSlider(rr, rrLabel, "R → R");
    setupSlider(leftGain, leftGainLabel, "LEFT");
    setupSlider(rightGain, rightGainLabel, "RIGHT");
    setupSlider(output, outputLabel, "OUTPUT");
    setupSlider(mix, mixLabel, "DRY / WET");

    auto addToggle = [this](juce::ToggleButton& b, const juce::String& text) {
        b.setButtonText(text); b.setColour(juce::ToggleButton::textColourId, juce::Colours::white); addAndMakeVisible(b);
    };
    addToggle(invertL, "Invert L"); addToggle(invertR, "Invert R");

    monitor.addItem("Stereo", 1); monitor.addItem("Mid", 2); monitor.addItem("Side", 3);
    addAndMakeVisible(monitor);
    monitorLabel.setText("MONITOR", juce::dontSendNotification);
    monitorLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(monitorLabel);

    auto& apvts = processor.parameters;
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "width", width));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "ll", ll));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "lr", lr));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "rl", rl));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "rr", rr));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "leftGain", leftGain));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "rightGain", rightGain));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "output", output));
    sliderAttachments.push_back(std::make_unique<SliderAttachment>(apvts, "mix", mix));
    invLAttachment = std::make_unique<ButtonAttachment>(apvts, "invertL", invertL);
    invRAttachment = std::make_unique<ButtonAttachment>(apvts, "invertR", invertR);
    monitorAttachment = std::make_unique<ComboAttachment>(apvts, "monitor", monitor);
}

void SimpleStereoShaperAudioProcessorEditor::setupSlider(juce::Slider& s, juce::Label& l, const juce::String& name)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 18);
    s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff35c6b0));
    s.setColour(juce::Slider::thumbColourId, juce::Colour(0xfff5f5f5));
    s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff242a31));
    addAndMakeVisible(s);
    l.setText(name, juce::dontSendNotification);
    l.setJustificationType(juce::Justification::centred);
    l.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(l);
}

void SimpleStereoShaperAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff171c22));
    g.setColour(juce::Colour(0xff2a333d));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8.0f), 10.0f, 1.0f);
    g.setColour(juce::Colour(0xff35c6b0));
    g.fillRoundedRectangle(16.0f, 54.0f, 528.0f, 2.0f, 1.0f);
    g.setColour(juce::Colour(0xff252d36));
    g.drawRoundedRectangle(16.0f, 68.0f, 528.0f, 172.0f, 8.0f, 1.0f);
    g.drawRoundedRectangle(16.0f, 250.0f, 528.0f, 124.0f, 8.0f, 1.0f);
}
void SimpleStereoShaperAudioProcessorEditor::resized()
{
    heading.setBounds(18, 12, 520, 36);
    widthLabel.setBounds(24, 74, 100, 20); width.setBounds(24, 92, 100, 112);
    matrixLabel.setBounds(0, 0, 0, 0);
    llLabel.setBounds(142, 74, 80, 20); ll.setBounds(132, 92, 90, 112);
    lrLabel.setBounds(238, 74, 80, 20); lr.setBounds(228, 92, 90, 112);
    rlLabel.setBounds(334, 74, 80, 20); rl.setBounds(324, 92, 90, 112);
    rrLabel.setBounds(430, 74, 80, 20); rr.setBounds(420, 92, 90, 112);
    leftGainLabel.setBounds(24, 258, 90, 20); leftGain.setBounds(18, 276, 90, 88);
    rightGainLabel.setBounds(116, 258, 90, 20); rightGain.setBounds(110, 276, 90, 88);
    outputLabel.setBounds(208, 258, 90, 20); output.setBounds(202, 276, 90, 88);
    mixLabel.setBounds(300, 258, 90, 20); mix.setBounds(294, 276, 90, 88);
    invertL.setBounds(392, 278, 130, 24); invertR.setBounds(392, 306, 130, 24);
    monitorLabel.setBounds(392, 332, 70, 20); monitor.setBounds(456, 330, 72, 24);
}
