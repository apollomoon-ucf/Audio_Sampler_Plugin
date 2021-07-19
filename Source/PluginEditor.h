/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginEditor.h
  Description: This file contains the basic framework code for a JUCE plugin
               editor.
*/

#pragma once

#include <JuceHeader.h>

#include "ADSRGainPolyButtons.h"
#include "PluginProcessor.h"
#include "WaveformVisual.h"

//==============================================================================
/**
 */
class VibeSamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget,
                                        public juce::Timer {
 public:
  VibeSamplerAudioProcessorEditor(VibeSamplerAudioProcessor&);
  ~VibeSamplerAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

  void timerCallback() override;

  // no longer using this with Value Tree State solution
  // void sliderValueChanged(juce::Slider* slider) override;

 private:
  // Text button for loading sound/sample from computer
  juce::TextButton memberLoadButton{"Load sample"};

  //juce::Image myimage = juce::ImageFileFormat::loadFrom(
  //    juce::File("C:/Users/Brian/OneDrive - Knights - University of Central "
  //               "Florida/c++_dev_folder/VibeSampler/Documentation/"
  //               "V2-2020-Vibe-Logo-Transparent-450-1-2.png"));

  juce::ImageComponent vibeLogoBars;
  juce::ImageComponent vibeLogoText;

  WaveformVisual memberWaveformVisual;
  ADSRGainPolyButtons memberADSRGainPoly;

  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      memberLoadButtonAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberPolyphonyKnobAttachment;

  juce::Slider memberPolyphonyKnob;

  juce::Label memberLoadLabel, memberPolyphonyLabel;
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  VibeSamplerAudioProcessor& audioProcessor;

  juce::MidiKeyboardState keyboardState;
  //SynthA
  juce::MidiKeyboardComponent keyboardComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessorEditor)
};
