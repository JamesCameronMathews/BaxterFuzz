#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class BaxterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // Change the type of customLookAndFeel to match your CustomLookAndFeel class
    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;
    BaxterAudioProcessorEditor (BaxterAudioProcessor&);
    ~BaxterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BaxterAudioProcessor& audioProcessor;

    // Add a slider (knob) and a label
    juce::Slider fuzzAmountSlider;
    juce::Label fuzzAmountLabel;
    std::unique_ptr<juce::Drawable> dogDrawable;
    std::unique_ptr<juce::Drawable> knobDrawable;

    // Declare customLookAndFeel and fuzzAmountAttachment
    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fuzzAmountAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaxterAudioProcessorEditor)
};
