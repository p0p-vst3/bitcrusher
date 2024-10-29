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
//    juce::File::getSpecialLocation(
//    juce::File::SpecialLocationType::commonApplicationDataDirectory)
//    .getChildFile(ProjectInfo::companyName)
//    .getChildFile(ProjectInfo::projectName)
    "~/Library/Application Support/GTBitcrusher"
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

void PresetManager::loadPreset(const juce::String& presetName) {
    if (presetName.isEmpty()) {
        return;
    }
    
    const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
    if (!presetFile.existsAsFile())
    {
        DBG("preset file " + presetFile.getFullPathName() + "does not exist");
        jassertfalse;
        return;
    }
    
    juce::XmlDocument xmlDocument{ presetFile };
    const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());
    
    apvtsRef.replaceState(valueTreeToLoad);
    currentPreset = presetName;
    
}

int PresetManager::loadNextPreset() { //github has return type as int
    const auto allPresets = getAllPresets();
    if (allPresets.isEmpty()) return -1;
    
    const auto currentIndex = allPresets.indexOf(currentPreset.toString());
    const auto nextIndex = currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;
    loadPreset(allPresets.getReference(nextIndex));
    return nextIndex;
}
int PresetManager::loadPreviousPreset() { //github has return type as int
    const auto allPresets = getAllPresets();
    if (allPresets.isEmpty()) return -1;
    
    const auto currentIndex = allPresets.indexOf(currentPreset.toString());
    const auto previousIndex = currentIndex - 1 < 0 ? allPresets.size() - 1 : currentIndex - 1;
    loadPreset(allPresets.getReference(previousIndex));
    return previousIndex;
}

juce::StringArray PresetManager::getAllPresets() const {
    juce::StringArray presets;
    const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
    for (const auto& file : fileArray)
    {
        presets.add(file.getFileNameWithoutExtension());
    }
    return presets;
}

juce::String PresetManager::getCurrentPreset() const {
    return currentPreset.toString();
}

}
