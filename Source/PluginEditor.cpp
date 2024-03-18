/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RaceCrusherAudioProcessorEditor::RaceCrusherAudioProcessorEditor (RaceCrusherAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
bitDepthAttachment(audioProcessor.apvts, "BIT_DEPTH", bitDepthSlider),
rateDivideAttachment(audioProcessor.apvts, "RATE_DIVIDE", rateDivideSlider),
dryWetAttachment(audioProcessor.apvts, "DRY_WET", dryWetSlider)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    bitDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    bitDepthSlider.setRange(0.0, 1.0, 0.01);
    
    rateDivideSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    rateDivideSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    rateDivideSlider.setRange(0.0, 1.0, 0.01);
    
    dryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    dryWetSlider.setRange(0.0, 1.0, 0.01);
    
    addAndMakeVisible(bitDepthSlider);
    addAndMakeVisible(rateDivideSlider);
    addAndMakeVisible(dryWetSlider);
}

RaceCrusherAudioProcessorEditor::~RaceCrusherAudioProcessorEditor()
{
}

//==============================================================================
void RaceCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void RaceCrusherAudioProcessorEditor::resized()
{
    bitDepthSlider.setBounds(getX(), getY() + 100 , getWidth(), 100);
    rateDivideSlider.setBounds(getX(), getY() + 200 , getWidth(), 100);
    dryWetSlider.setBounds(getX(), getY() + 300 , getWidth(), 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
