/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginProcessor.h
  Description: This file contains the basic framework code for a JUCE plugin
               processor.
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 */
class VibeSamplerAudioProcessor : public juce::AudioProcessor,
                                  public juce::ValueTree::Listener {
 public:
  //==============================================================================
  VibeSamplerAudioProcessor();
  ~VibeSamplerAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  // load file method
  juce::String loadFile();
  // load file for dropped file
  void loadDroppedFile(const juce::String& path);

  // getter for getting status of sampler
  int getNumberOfSamplerSounds() { return memberSampler.getNumSounds(); };
  // giving access to memberWaveform
  juce::AudioBuffer<float> getWaveform() { return memberWaveform; };

  // method to set number of voices (change polyphony)
  void VibeSamplerAudioProcessor::changePolyphony(int numberOfVoices);

  void getADSRGainValue();

  juce::ADSR::Parameters& getADSRParameters() {
    return memberADSRGainParameters;
  };
  // adsr (getting from ADSR::Parameters) and gain
  // float attack{0.0};
  // float decay{0.0};
  // float sustain{0.0};
  // float release{0.0};
  float gain{0.0};
  float polyphony{1.0};

  // giving access to AudioProcessorValueTreeState by creating a reference to
  // the object
  juce::AudioProcessorValueTreeState& getValueTreeState() {
    return memberValueTreeState;
  };

 private:
  // creating member variables for the Synthesiser class and polyphony (#
  // voices)
  juce::Synthesiser memberSampler;
  const int memberVoiceInitNumber{1};
  const int memberMaxNumberOfVoices{32};
  // AudioBuffer for storing waveform
  juce::AudioBuffer<float> memberWaveform;
  // ADSR
  juce::ADSR::Parameters memberADSRGainParameters;

  // audio format manager
  juce::AudioFormatManager memberFormatManager;
  juce::AudioFormatReader* memberFormatReader{nullptr};

  // object for value tree state
  juce::AudioProcessorValueTreeState memberValueTreeState;
  // parameter layout
  juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

  void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                const juce::Identifier& property) override;
  std::atomic<bool> memberShouldUpdateParameters{false};

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessor)
};
