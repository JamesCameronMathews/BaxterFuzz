/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_audio_processors/juce_audio_processors.h>


//==============================================================================
BaxterAudioProcessor::BaxterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    fuzzAmount = parameters.getRawParameterValue("fuzzAmount");
}

BaxterAudioProcessor::~BaxterAudioProcessor()
{
}
//==============================================================================
const juce::String BaxterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BaxterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BaxterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BaxterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BaxterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BaxterAudioProcessor::getNumPrograms()
{
    return 1;
}

int BaxterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BaxterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BaxterAudioProcessor::getProgramName (int index)
{
    return {};
}

void BaxterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BaxterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void BaxterAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BaxterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BaxterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float fuzz = *fuzzAmount;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float inputValue = channelData[i];            float inputValue = channelData[i];

            // Apply the fuzz effect
            float fuzzedValue = (inputValue * (1 + fuzz) - inputValue) / (1 + abs(inputValue) * fuzz);

            // Store the fuzzed value back into the buffer
            channelData[i] = fuzzedValue;
        }
    }
}

//==============================================================================
bool BaxterAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BaxterAudioProcessor::createEditor()
{
    return new BaxterAudioProcessorEditor(*this);
}

//==============================================================================
void BaxterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BaxterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout BaxterAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("fuzzAmount", "Fuzz Amount", 0.0f, 1.0f, 0.5f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BaxterAudioProcessor();
}
