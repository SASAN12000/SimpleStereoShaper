#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout makeLayout()
{
    using P = juce::AudioParameterFloat;
    using C = juce::AudioParameterChoice;
    using B = juce::AudioParameterBool;
    juce::AudioProcessorValueTreeState::ParameterLayout p;

    p.add(std::make_unique<P>("width", "Stereo Width", juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));
    p.add(std::make_unique<P>("ll", "L to L", juce::NormalisableRange<float>(-2.0f, 2.0f, 0.01f), 1.0f));
    p.add(std::make_unique<P>("lr", "L to R", juce::NormalisableRange<float>(-2.0f, 2.0f, 0.01f), 0.0f));
    p.add(std::make_unique<P>("rl", "R to L", juce::NormalisableRange<float>(-2.0f, 2.0f, 0.01f), 0.0f));
    p.add(std::make_unique<P>("rr", "R to R", juce::NormalisableRange<float>(-2.0f, 2.0f, 0.01f), 1.0f));
    p.add(std::make_unique<P>("leftGain", "Left Level", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    p.add(std::make_unique<P>("rightGain", "Right Level", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    p.add(std::make_unique<P>("output", "Output", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    p.add(std::make_unique<P>("mix", "Dry / Wet", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    p.add(std::make_unique<B>("invertL", "Invert Left Phase", false));
    p.add(std::make_unique<B>("invertR", "Invert Right Phase", false));
    p.add(std::make_unique<C>("monitor", "Monitor", juce::StringArray{ "Stereo", "Mid", "Side" }, 0));
    return p;
}

SimpleStereoShaperAudioProcessor::SimpleStereoShaperAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", makeLayout())
{
}

void SimpleStereoShaperAudioProcessor::prepareToPlay(double, int) {}

bool SimpleStereoShaperAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SimpleStereoShaperAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();
    if (buffer.getNumChannels() < 2) return;

    auto get = [this](const char* id) { return parameters.getRawParameterValue(id)->load(); };
    const float width = get("width");
    const float ll = get("ll"), lr = get("lr"), rl = get("rl"), rr = get("rr");
    const float lg = juce::Decibels::decibelsToGain(get("leftGain"));
    const float rg = juce::Decibels::decibelsToGain(get("rightGain"));
    const float og = juce::Decibels::decibelsToGain(get("output"));
    const float mix = get("mix");
    const bool invL = parameters.getRawParameterValue("invertL")->load() > 0.5f;
    const bool invR = parameters.getRawParameterValue("invertR")->load() > 0.5f;
    const int monitor = (int)get("monitor");

    auto* L = buffer.getWritePointer(0);
    auto* R = buffer.getWritePointer(1);

    for (int i = 0; i < n; ++i)
    {
        const float dryL = L[i], dryR = R[i];
        const float mid = 0.5f * (dryL + dryR);
        const float side = 0.5f * (dryL - dryR);

        // Width is applied in M/S before the user matrix.
        const float wideL = mid + side * width;
        const float wideR = mid - side * width;

        float wetL = (wideL * ll + wideR * rl) * lg;
        float wetR = (wideL * lr + wideR * rr) * rg;

        if (invL) wetL = -wetL;
        if (invR) wetR = -wetR;

        if (monitor == 1) { const float m = 0.5f * (wetL + wetR); wetL = m; wetR = m; }
        else if (monitor == 2) { const float s = 0.5f * (wetL - wetR); wetL = s; wetR = -s; }

        L[i] = (dryL * (1.0f - mix) + wetL * mix) * og;
        R[i] = (dryR * (1.0f - mix) + wetR * mix) * og;
    }
}

juce::AudioProcessorEditor* SimpleStereoShaperAudioProcessor::createEditor()
{
    return new SimpleStereoShaperAudioProcessorEditor(*this);
}
void SimpleStereoShaperAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}
void SimpleStereoShaperAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleStereoShaperAudioProcessor();
}
