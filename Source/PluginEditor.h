/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"

//==============================================================================
/**
*/
class DialStyle : public juce::LookAndFeel_V4
{
public:
    DialStyle();
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
private:
    float sliderWidth;
};



class RaceCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RaceCrusherAudioProcessorEditor (RaceCrusherAudioProcessor&);
    ~RaceCrusherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    

private:
    
    void setCommonSliderProps(juce::Slider& slider);
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RaceCrusherAudioProcessor& audioProcessor;
    juce::Slider bitDepthSlider;
    juce::Slider rateDivideSlider;
    juce::Slider dryWetSlider;
    
    Gui::PresetPanel presetPanel;
    
    juce::AudioProcessorValueTreeState::SliderAttachment bitDepthAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment rateDivideAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment dryWetAttachment;
    
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
   

    
private:
    DialStyle customDialLookAndFeel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RaceCrusherAudioProcessorEditor)
};



