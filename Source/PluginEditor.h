/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginEditor.h
*/

#pragma once

#include <JuceHeader.h>

#include "ADSRGainPolyButtons.h"
#include "PluginProcessor.h"
#include "WaveformVisual.h"

class VibeSamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget,
                                        public juce::Timer
{
 public:
  VibeSamplerAudioProcessorEditor(VibeSamplerAudioProcessor& p);
  ~VibeSamplerAudioProcessorEditor() override;

  
  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

  void timerCallback() override;


 private:
  // Text button for loading sound/sample from computer
  juce::TextButton loadButton;
  juce::ImageComponent vibeLogoBars;
  juce::ImageComponent vibeLogoText;
  juce::Slider polyphonyKnob;
  juce::Label loadLabel, polyphonyLabel;
  juce::MidiKeyboardComponent keyboardComponent;

  WaveformVisual waveformVisual;

  ADSRGainPolyButtons memberADSRGainPoly;

  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      loadButtonAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      polyphonyKnobAttachment;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  VibeSamplerAudioProcessor& audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessorEditor)
};
