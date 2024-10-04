/*
  ==============================================================================

    PresetPanel.h
    Created: 4 Oct 2024 12:44:01pm
    Author:  Quincy Winkler

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Gui 
{
class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
    {
    public:
        PresetPanel()
        {
            configureButton(saveButton, "Save");
            configureButton(deleteButton, "Delete");
            configureButton(previousButton, "<-");
            configureButton(nextButton, "->");
            
            presetList.setTextWhenNothingSelected("None");
            presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(presetList);
            presetList.addListener(this);
        }
        
        ~PresetPanel()
        {
            saveButton.removeListener(this);
            deleteButton.removeListener(this);
            previousButton.removeListener(this);
            nextButton.removeListener(this);
            presetList.removeListener(this);
        }
        
        void resized() override
        {
            const auto container = getLocalBounds().reduced(4);
            auto bounds = container;
            
            saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
            previousButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
            presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
            nextButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
            deleteButton.setBounds(bounds.reduced(4));
        }
        
        
    private:
        void configureButton(juce::Button& button, const juce::String& buttonText)
        {
            button.setButtonText(buttonText);
            button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(button);
            button.addListener(this);
        }
        
        void buttonClicked(juce::Button* button) override {}
        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override {}
        juce::TextButton saveButton, deleteButton, previousButton, nextButton;
        juce::ComboBox presetList;
        std::unique_ptr<juce::FileChooser> fileChooser;
    };
}
