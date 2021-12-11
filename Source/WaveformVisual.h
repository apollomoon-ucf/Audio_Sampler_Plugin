/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   WaveformVisual.h
*/


#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

class WaveformVisual : public juce::Component,
                       public juce::FileDragAndDropTarget {
 public:
  WaveformVisual(VibeSamplerAudioProcessor& p);
  ~WaveformVisual() override;

  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

  void activateWaveForm(bool activate) { activateWaveformVisual = true; };

  juce::String getFilename() { return filename; };
  void setFilename(juce::String filename) { filename = filename; };

 private:
  // place to hold samples
  std::vector<float> audioSnapshotLocations;
  // bool for drawing waveform
  bool activateWaveformVisual{false};

  juce::String filename{""};

  // reference to the audio processor
  VibeSamplerAudioProcessor& audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformVisual)
};
