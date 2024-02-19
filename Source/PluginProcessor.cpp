/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RaceCrusherAudioProcessor::RaceCrusherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

RaceCrusherAudioProcessor::~RaceCrusherAudioProcessor()
{
}

//==============================================================================
const juce::String RaceCrusherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RaceCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RaceCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RaceCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RaceCrusherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RaceCrusherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RaceCrusherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RaceCrusherAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RaceCrusherAudioProcessor::getProgramName (int index)
{
    return {};
}

void RaceCrusherAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RaceCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    downSamplingOn = false;
    
}

void RaceCrusherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RaceCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RaceCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //clear unused output channels 
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    int numSamples = buffer.getNumSamples();
    
    int bitDepthValue = static_cast<int>(apvts.getRawParameterValue("BIT_DEPTH")->load());
    int rateDivideValue = static_cast<int>(std::round(apvts.getRawParameterValue("RATE_DIVIDE")->load()));
    float dryWetValue = apvts.getRawParameterValue("DRY_WET")->load();
    float stepSize = 2.0f / (std::pow(2.0f, bitDepthValue));
    bool downSamplingOn = rateDivideValue > 1;
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) 
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int i = 0; i < numSamples; i++)
        {
            float ogSample = channelData[i];
            float quantizedSample = quantizeSample(ogSample, stepSize);
            
            if (downSamplingOn && i % rateDivideValue != 0)
            {
                quantizedSample = channelData[i - i % rateDivideValue];
            }

            channelData[i] = applyDryWetMix(ogSample, quantizedSample, dryWetValue);
        }
    }
}

//==============================================================================
bool RaceCrusherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RaceCrusherAudioProcessor::createEditor()
{
    //return new RaceCrusherAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void RaceCrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RaceCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout RaceCrusherAudioProcessor::createAudioParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("NOISE_LEVEL",1),
                                                           "Noise Level",
                                                           juce::NormalisableRange<float>(0.f ,1.f ,0.01f),
                                                           0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("BIT_DEPTH",1),
                                                           "Bit Depth",
                                                            juce::NormalisableRange<float>(1.0f, 16.0f, 0.01f, 0.4f),
                                                            16.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RATE_DIVIDE",1),
                                                           "Rate Divide",
                                                           juce::NormalisableRange<float>(1.0f, 512.0f, 0.01f, 0.3f),
                                                           1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DRY_WET",1),
                                                           "Dry/Wet",
                                                           0.0f,
                                                           1.0f,
                                                           0.5f));
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RaceCrusherAudioProcessor();
}
