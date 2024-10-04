/*
  ==============================================================================

    PresetManager.cpp
    Created: 4 Oct 2024 3:11:08pm
    Author:  Quincy Winkler

  ==============================================================================
*/

#include "PresetManager.h"

namespace Service
{
    const juce::File PresetManager::defaultDirectory 
    {
        juce::File::getSpecialLocation(
        juce::File::SpecialLocationType::commonApplicationDataDirectory)
        .getChildFile(ProjectInfo::companyName)
        .getChildFile(ProjectInfo::projectName)
    };

    const juce::String PresetManager::extension{"xml"};
  //  const juce::String PresetManager::presetNameProperty{ "presetName" };
    PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) :
        apvtsRef(apvts)
    {
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("could not create preset directory" + result.getErrorMessage());
                jassertfalse;
            }
        }
    }

    void PresetManager::savePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty()) return;
        
        currentPreset.setValue(presetName);
        
        const auto xml = apvtsRef.copyState().createXml();
        const auto presetFile = defaultDirectory.getChildFile( presetName + "." + extension);
        if ( !xml->writeTo(presetFile) )
        {
            DBG("could not creaete preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::deletePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty()) return;
        
        const auto presetFile = defaultDirectory.getChildFile( presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }
        
        if (!presetFile.deleteFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " could not be deleted");
            jassertfalse;
            return;
        }
        currentPreset.setValue("");
    }
}
