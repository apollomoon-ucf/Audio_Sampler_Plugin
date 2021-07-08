/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginEditor.cpp
  Description: This file contains the basic framework code for a JUCE plugin
               editor.
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
VibeSamplerAudioProcessorEditor::VibeSamplerAudioProcessorEditor(
    VibeSamplerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  // lambda function to run on button click
  memberLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
  // make load button a child component of this current component
  addAndMakeVisible(memberLoadButton);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 300);
}

VibeSamplerAudioProcessorEditor::~VibeSamplerAudioProcessorEditor() {}

//==============================================================================
void VibeSamplerAudioProcessorEditor::paint(juce::Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
  g.drawFittedText(
      " \n\n\nVibe Music Productions\n Testing Maschine 2 - VST \n Brian Moon",
      getLocalBounds(), juce::Justification::centred, 1);
}

void VibeSamplerAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  int x = getWidth() / 2 - 100;
  int y = getHeight() / 2 - 100;
  int width = 200;
  int height = 50;
  memberLoadButton.setBounds(x, y, width, height);
}
