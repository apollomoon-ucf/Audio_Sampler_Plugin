/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginEditor.h
  Description: This file contains the basic framework code for a JUCE plugin
               editor.
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "WaveformVisual.h"

//==============================================================================
/**
 */
class VibeSamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget,
                                        public juce::Slider::Listener {
 public:
  VibeSamplerAudioProcessorEditor(VibeSamplerAudioProcessor&);
  ~VibeSamplerAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

  // no longer using this with Value Tree State solution
  void sliderValueChanged(juce::Slider* slider) override;

 private:
  // Text button for loading sound/sample from computer
  juce::TextButton memberLoadButton{"Load sample"};

  juce::Image myimage = juce::ImageFileFormat::loadFrom(
      juce::File("C:/Users/Brian/OneDrive - Knights - University of Central "
                 "Florida/c++_dev_folder/VibeSampler/Documentation/"
                 "V2-2020-Vibe-Logo-Transparent-450-1-2.png"));
  //// place to hold samples
  // std::vector<float> memberAudioSnapshotLocations;
  //// bool for drawing waveform
  // bool memberActivateWaveformVisual{false};
  // WaveformVisual solution for above code
  WaveformVisual memberWaveformVisual;
  // attack slider
  juce::Slider memberAttackKnob, memberDecayKnob, memberSustainKnob,
      memberReleaseKnob, memberGainKnob, memberPolyphonyKnob;
  juce::Label memberAttackLabel, memberDecayLabel, memberSustainLabel,
      memberReleaseLabel, memberGainLabel, memberPolyphonyLabel,
      memberLoadLabel;
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  VibeSamplerAudioProcessor& audioProcessor;

  // unqiue pointers for attaching knobs/sliders
  // hover SliderAttachment for more info
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberAttackKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberDecayKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberSustainKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberReleaseKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberGainKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      memberPolyphonyKnobAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessorEditor)
};
