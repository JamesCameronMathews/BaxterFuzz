#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Here you can set specific colours or fonts to customize your plugin's look and feel.
        // For example:
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }
};
