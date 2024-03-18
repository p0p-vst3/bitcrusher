/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RaceCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RaceCrusherAudioProcessorEditor (RaceCrusherAudioProcessor&);
    ~RaceCrusherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RaceCrusherAudioProcessor& audioProcessor;
    juce::Slider bitDepthSlider;
    juce::Slider rateDivideSlider;
    juce::Slider dryWetSlider;
    
    juce::AudioProcessorValueTreeState::SliderAttachment bitDepthAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment rateDivideAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment dryWetAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaceCrusherAudioProcessorEditor)
};
