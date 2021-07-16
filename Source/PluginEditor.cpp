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
// constructor
VibeSamplerAudioProcessorEditor::VibeSamplerAudioProcessorEditor(
    VibeSamplerAudioProcessor &p)
    : AudioProcessorEditor(&p),
      memberWaveformVisual(p),
      memberADSRGainPoly(p),
      audioProcessor(p) {
  // lambda function to run on button click
  memberLoadButton.onClick = [&]() {
    juce::String filenameFromClickAndLoad = audioProcessor.loadFile();
    memberWaveformVisual.setFilename(filenameFromClickAndLoad);
    // memberWaveformVisual.activateWaveForm(true);
    repaint();
  };

  // make waveform visual
  addAndMakeVisible(memberWaveformVisual);

  // make adsr, gain, and polyphonic knobs visible
  addAndMakeVisible(memberADSRGainPoly);

  // make load button a child component of this current component
  addAndMakeVisible(memberLoadButton);
  memberLoadLabel.setFont(15.0f);
  memberLoadLabel.setText(memberWaveformVisual.getFilename(),
                          juce::NotificationType::dontSendNotification);
  memberLoadLabel.setJustificationType(juce::Justification::centredTop);
  memberLoadLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::white);
  memberLoadLabel.attachToComponent(&memberLoadButton, false);

  setSize(600, 400);
}

// destructor
VibeSamplerAudioProcessorEditor::~VibeSamplerAudioProcessorEditor() {}

//==============================================================================
void VibeSamplerAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);
  g.setColour(juce::Colours::white);
  g.setFont(15.0f);

  g.drawImageAt(myimage.rescaled(125, 80), getWidth() / 2 - 62.5, 8);

  g.drawFittedText("Vibe Audio Sampler", getLocalBounds().reduced(90, 90),
                   juce::Justification::centredTop, 1);
  // paint filename
  g.setColour(juce::Colours::grey);
  g.setFont(15.0f);
  g.drawText(memberWaveformVisual.getFilename(), 337.5, 75, 200, 40,
             juce::Justification::topRight, true);
}

void VibeSamplerAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  // waveform visual
  memberWaveformVisual.setBoundsRelative(0.18f, 0.30f, 0.63f, 0.5f);
  memberADSRGainPoly.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

  // get dimensions for button
  int x = getWidth() / 2 + 160;
  int y = getHeight() / 2 - 160;
  int width = 80;
  int height = 30;
  memberLoadButton.setBounds(x, y, width, height);
}

// method for seeing if the file type dropped on the sampler is an appropriate
// for music
bool VibeSamplerAudioProcessorEditor::isInterestedInFileDrag(
    const juce::StringArray &files) {
  for (auto file : files) {
    if (file.contains(".wav") || file.contains(".mp3") ||
        file.contains(".aiff")) {
      return true;
    }
  }
  return false;
}

// method for loading the dropped file
void VibeSamplerAudioProcessorEditor::filesDropped(
    const juce::StringArray &files, int x, int y) {
  for (auto file : files) {
    if (isInterestedInFileDrag(file)) {
      auto myFile = std::make_unique<juce::File>(file);
      memberWaveformVisual.setFilename(myFile->getFileNameWithoutExtension());
      // load the file
      audioProcessor.loadDroppedFile(file);
      // draw waveform?
      // memberActivateWaveformVisual = true;
      repaint();
    }
  }
  // repaint to display successful file load message
  repaint();
}

// listener for knobs
// void VibeSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  // if (slider == &memberAttackKnob) {
  //  audioProcessor.getADSRParameters().attack = memberAttackKnob.getValue();
  //} else if (slider == &memberDecayKnob) {
  //  audioProcessor.getADSRParameters().decay = memberDecayKnob.getValue();
  //} else if (slider == &memberSustainKnob) {
  //  audioProcessor.getADSRParameters().sustain = memberSustainKnob.getValue();
  //} else if (slider == &memberReleaseKnob) {
  //  audioProcessor.getADSRParameters().release = memberReleaseKnob.getValue();
  //} else if (slider == &memberGainKnob) {
  //  audioProcessor.gain = memberGainKnob.getValue();
  //} else if (slider == &memberPolyphonyKnob) {
  //  audioProcessor.changePolyphony(memberPolyphonyKnob.getValue());
  //}
  // update adsr
//  audioProcessor.getADSRGainValue();
//}