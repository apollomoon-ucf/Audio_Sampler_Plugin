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
class VibeSamplerAudioProcessor : public juce::AudioProcessor {
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
  void loadFile();
  // load file for dropped file
  void loadDroppedFile(const juce::String& path);

  // getter for getting status of sampler
  int getNumberOfSamplerSounds() { return memberSampler.getNumSounds(); };
  // giving access to memberWaveform
  juce::AudioBuffer<float> getWaveform() { return memberWaveform; };

 private:
  // creating member variables for the Synthesiser class and polyphony (#
  // voices)
  juce::Synthesiser memberSampler;
  const int memberNumberOfVoices{12};
  // AudioBuffer for storing waveform
  juce::AudioBuffer<float> memberWaveform;

  // audio format manager
  juce::AudioFormatManager memberFormatManager;
  juce::AudioFormatReader* memberFormatReader{nullptr};

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessor)
};
