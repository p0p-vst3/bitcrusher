/*
  ==============================================================================

    PresetManager.h
    Created: 4 Oct 2024 3:11:08pm
    Author:  Quincy Winkler

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Service
{
    class PresetManager : juce::ValueTree::Listener
    {
    public:
        static const juce::File defaultDirectory;
        static const juce::String extension;
        static const juce::String presetNameProperty;
        
        PresetManager(juce::AudioProcessorValueTreeState& );
        
        void savePreset(const juce::String& presetName);
        void deletePreset(const juce::String& presetName);
        void loadPreset(const juce::String& presetName);
        void loadNextPreset();
        void loadPreviousPreset();
        juce::StringArray getAllPresets() const;
        juce::String getCurrentPreset() const;
        
    private:
        juce::AudioProcessorValueTreeState& apvtsRef;
        juce::Value currentPreset;
        
    };
}
