/*
  ==============================================================================

    WaveformVisual.h
    Created: 12 Jul 2021 12:01:02pm
    Author:  Brian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

//==============================================================================
/*
 */
class WaveformVisual : public juce::Component,
                       public juce::FileDragAndDropTarget {
 public:
  WaveformVisual(VibeSamplerAudioProcessor&);
  ~WaveformVisual() override;

  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

  void activateWaveForm(bool activate) { memberActivateWaveformVisual = true; };

 private:
  // place to hold samples
  std::vector<float> memberAudioSnapshotLocations;
  // bool for drawing waveform
  bool memberActivateWaveformVisual{false};

  juce::String memberFilename{""};

  // reference to the audio processor
  VibeSamplerAudioProcessor& audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformVisual)
};
