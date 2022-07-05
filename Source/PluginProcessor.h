/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginProcessor.h
*/

#pragma once

#include <JuceHeader.h>


class VibeSamplerAudioProcessor : public juce::AudioProcessor,
                                  public juce::ValueTree::Listener
{
 public:
  VibeSamplerAudioProcessor();
  ~VibeSamplerAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  // load file method
  juce::String loadFile();
  // load file for dropped file
  void loadDroppedFile(const juce::String& path);

  // getter for getting status of sampler
  int getNumberOfSamplerSounds() { return sampler.getNumSounds(); };
  // giving access to waveform
  juce::AudioBuffer<float> getWaveform() { return waveform; };

  // method to set number of voices (change polyphony)
  void changePolyphony(int numberOfVoices);

  // get adsr and gail value
  void getADSRGainValue();

  // get adsr params
  juce::ADSR::Parameters& getADSRParameters()
	{
    return memberADSRGainParameters;
  };

  // init gain and polyphony
  float gain{0.0};
  float polyphony{1.0};

  // giving access to AudioProcessorValueTreeState by creating a reference to
  // the object
  juce::AudioProcessorValueTreeState& getValueTreeState()
	{
    return valueTreeState;
  };

  juce::String getAudioFilename() { return audioFilename; };
  std::atomic<bool>& isNoteBeingPlayed() { return isNotePlaying; }
  std::atomic<int>& getSampleCount() { return sampleCount; }
  juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }

 private:
  // creating member variables for the Synthesiser class and polyphony (#
  // voices)
  juce::Synthesiser sampler;
  juce::SamplerVoice* myVoice;

  // writing from one thread, reading from another -- using atomic
  std::atomic<bool> shouldUpdatePlayhead{false};
  std::atomic<bool> isNotePlaying{false};
  std::atomic<int> sampleCount{0};

  const int voiceInitNumber{2};
  const int maxNumberOfVoices{32};
  // AudioBuffer for storing waveform
  juce::AudioBuffer<float> waveform;
  // ADSR
  juce::ADSR::Parameters memberADSRGainParameters;

  // previous gain (for gain smoothing)
  float previousGain;

  juce::MidiKeyboardState keyboardState;

  // storing sample and sample name
  juce::Value storedAudioFile;
  juce::String audioFilePath;
  juce::Value storedAudioFilename;
  juce::String audioFilename;

  // audio format manager
  juce::AudioFormatManager formatManager;
  juce::AudioFormatReader* formatReader{nullptr};

  // object for value tree state
  juce::AudioProcessorValueTreeState valueTreeState;
  // parameter layout
  juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

  void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                const juce::Identifier& property) override;
  std::atomic<bool> shouldUpdateParameters{false};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessor)
};
