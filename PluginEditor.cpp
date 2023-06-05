/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(juce::Drawable* knobDrawable) : knobDrawable(knobDrawable) {}

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        knobDrawable->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
    }

private:
    juce::Drawable* knobDrawable;
};

BaxterAudioProcessorEditor::BaxterAudioProcessorEditor (BaxterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    // Load the images
    auto dogImageData = BinaryData::BaxFace_png;
    auto dogImageSize = BinaryData::BaxFace_pngSize;
    auto dogImage = juce::ImageCache::getFromMemory(dogImageData, dogImageSize);
    dogDrawable = std::unique_ptr<juce::Drawable>(new juce::DrawableImage());
    reinterpret_cast<juce::DrawableImage*>(dogDrawable.get())->setImage(dogImage);

    auto knobImageData = BinaryData::BaxKnob_png;
    auto knobImageSize = BinaryData::BaxKnob_pngSize;
    auto knobImage = juce::ImageCache::getFromMemory(knobImageData, knobImageSize);
    knobDrawable = std::unique_ptr<juce::Drawable>(new juce::DrawableImage());
    reinterpret_cast<juce::DrawableImage*>(knobDrawable.get())->setImage(knobImage);

    // Configure the fuzz amount slider (knob)
    fuzzAmountSlider.setSliderStyle(juce::Slider::Rotary);
    fuzzAmountSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    fuzzAmountSlider.setRange(0.0, 1.0, 0.01);
    fuzzAmountSlider.setValue(audioProcessor.parameters.getRawParameterValue("fuzzAmount")->load());

    // Set the look and feel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>(knobDrawable.get());
    fuzzAmountSlider.setLookAndFeel(customLookAndFeel.get());

    addAndMakeVisible(fuzzAmountSlider);

    // Configure the fuzz amount label
    fuzzAmountLabel.setText("Fuzz", juce::dontSendNotification);
    fuzzAmountLabel.attachToComponent(&fuzzAmountSlider, false);
    addAndMakeVisible(fuzzAmountLabel);

    // Attach the slider to the corresponding parameter in the processor
    auto& params = audioProcessor.getParameters();
    fuzzAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (params, "fuzz", fuzzAmountSlider);
}

BaxterAudioProcessorEditor::~BaxterAudioProcessorEditor()
{
    fuzzAmountAttachment = nullptr;
    fuzzAmountSlider.setLookAndFeel(nullptr);
    customLookAndFeel = nullptr;
}

//==============================================================================
void BaxterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Draw the dog image
    dogDrawable->drawWithin(g, juce::Rectangle<float>(0, 0, getWidth(), getHeight()),
                            juce::RectanglePlacement::centred, 1.0f);
}
void BaxterAudioProcessorEditor::resized()
{
    // Set the bounds of the fuzzAmountSlider and fuzzAmountLabel
    fuzzAmountSlider.setBounds (getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    fuzzAmountLabel.setBounds (0, fuzzAmountSlider.getY() - 20, getWidth(), 20);
}


