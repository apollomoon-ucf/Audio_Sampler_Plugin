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

//==============================================================================
/**
 */
class VibeSamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget {
 public:
  VibeSamplerAudioProcessorEditor(VibeSamplerAudioProcessor&);
  ~VibeSamplerAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  bool isInterestedInFileDrag(const juce::StringArray& files) override;
  void filesDropped(const juce::StringArray& files, int x, int y) override;

 private:
  // Text button for loading sound/sample from computer
  juce::TextButton memberLoadButton{"Click or drag and drop sounds here to load into sampler"};
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  VibeSamplerAudioProcessor& audioProcessor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibeSamplerAudioProcessorEditor)
};
