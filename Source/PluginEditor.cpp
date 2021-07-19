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
      audioProcessor(p),
      keyboardComponent(keyboardState,
                        juce::MidiKeyboardComponent::horizontalKeyboard) {
  // loading logo from memory
  auto vibeLogoBarsFromMemory = juce::ImageCache::getFromMemory(
      BinaryData::V22020VibeLogoTransparent45012_png,
      BinaryData::V22020VibeLogoTransparent45012_pngSize);
  auto vibeLogoTextFromMemory = juce::ImageCache::getFromMemory(
      BinaryData::fixed_sampler_vibe_logo_png,
      BinaryData::fixed_sampler_vibe_logo_pngSize);
  // if (!vibeLogoFromMemory.isNull()) {
  //  vibeLogo.setImage(vibeLogoFromMemory,
  //                    juce::RectanglePlacement::stretchToFit);
  //} else {
  //  jassert(!vibeLogoFromMemory.isNull());
  //}
  vibeLogoBars.setImage(vibeLogoBarsFromMemory,
                        juce::RectanglePlacement::stretchToFit);
  vibeLogoText.setImage(vibeLogoTextFromMemory,
                        juce::RectanglePlacement::stretchToFit);

  // make logo visible
  // addAndMakeVisible(vibeLogoBars);
  addAndMakeVisible(vibeLogoText);

  // make waveform visible
  addAndMakeVisible(memberWaveformVisual);

  // make adsr, gain, and polyphonic knobs visible
  addAndMakeVisible(memberADSRGainPoly);

  // make keyboard visible
  addAndMakeVisible(keyboardComponent);

  memberLoadButtonAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
          audioProcessor.getValueTreeState(), "sample", memberLoadButton);
  memberLoadButton.setColour(juce::TextButton::buttonColourId,
                             juce::Colours::black);
  // make load button a child component of this current component
  addAndMakeVisible(memberLoadButton);
  // lambda function to run on button click
  memberLoadButton.onClick = [&]() {
    juce::String filenameFromClickAndLoad = audioProcessor.loadFile();
    memberWaveformVisual.setFilename(filenameFromClickAndLoad);
    // memberWaveformVisual.activateWaveForm(true);
    repaint();
  };
  memberLoadLabel.setFont(15.0f);
  memberLoadLabel.setText(memberWaveformVisual.getFilename(),
                          juce::NotificationType::dontSendNotification);
  memberLoadLabel.setJustificationType(juce::Justification::centredTop);
  memberLoadLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::white);
  memberLoadLabel.attachToComponent(&memberLoadButton, false);

    // Polyphony Knob
  memberPolyphonyKnob.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                                juce::Colours::white);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                                juce::Colours::rebeccapurple);
  // memberPolyphonyKnob.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId,
  //                           juce::Colours::black);
  // memberPolyphonyKnob.setIncDecButtonsMode(
  //    juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Horizontal);
  getLookAndFeel().setColour(juce::TextButton::ColourIds::buttonColourId,
                             juce::Colours::black);
  memberPolyphonyKnob.setColour(juce::ComboBox::ColourIds::textColourId,
                                juce::Colours::yellow);
  memberPolyphonyKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 55,
                                      20);
  // if (audioProcessor.polyphony > 1) {
  //  memberPolyphonyKnob.setTextValueSuffix(" voices");
  //} else {
  //  memberPolyphonyKnob.setTextValueSuffix(" voice");
  //}

  // value tree state solution for listener knob
  memberPolyphonyKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "polyphony", memberPolyphonyKnob);
  // no longer using listener or range with Value Tree State solution
  memberPolyphonyKnob.setRange(1.0, 32.0, 1.0);
  // memberPolyphonyKnob.addListener(this);
  addAndMakeVisible(memberPolyphonyKnob);
  memberPolyphonyLabel.setFont(20.0f);
  // memberPolyphonyLabel.setText("Polyphony",
  //                              juce::NotificationType::dontSendNotification);
  memberPolyphonyLabel.setJustificationType(juce::Justification::centredTop);
  memberPolyphonyLabel.setColour(juce::Label::ColourIds::textColourId,
                                 juce::Colours::white);
  memberPolyphonyLabel.attachToComponent(&memberPolyphonyKnob, false);

  // start timer for playhead
  startTimerHz(30);

  setSize(600, 500);
}

// destructor
VibeSamplerAudioProcessorEditor::~VibeSamplerAudioProcessorEditor() {
  stopTimer();
}

//==============================================================================
void VibeSamplerAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);
  g.setColour(juce::Colours::white);
  g.setFont(15.0f);

    if (audioProcessor.polyphony > 1) {
    memberPolyphonyKnob.setTextValueSuffix(" poly");
  } else {
    memberPolyphonyKnob.setTextValueSuffix(" mono");
  }

  // original logo and sample title
  // g.drawImageAt(vibeLogo.getImage().rescaled(125, 80), getWidth() / 2 - 62.5,
  // 8); g.drawFittedText("Vibe Audio Sampler", getLocalBounds().reduced(90,
  // 90),
  //                  juce::Justification::centredTop, 1);

  // paint filename
  g.setColour(juce::Colours::grey);
  g.setFont(15.0f);
  g.drawText(audioProcessor.getAudioFilename(), 337.5, 75, 200, 40,
             juce::Justification::topRight, true);

  if (audioProcessor.polyphony > 1 && audioProcessor.polyphony < 10) {
    juce::String string = "Polyphonic:";
    string.append(std::to_string(audioProcessor.polyphony), 1);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  } else if (audioProcessor.polyphony > 9) {
    juce::String string = "Polyphonic:";
    string.append(std::to_string(audioProcessor.polyphony), 2);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  }
  else {
    juce::String string = "Monophonic:";
    string.append(std::to_string(audioProcessor.polyphony), 1);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  }


  // g.drawRect(getLocalBounds().reduced(150,125));
  // juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20,
  //                                     getHeight() - 120);
  g.drawRoundedRectangle(getWidth() / 2 - (getWidth() / 3), getHeight() / 4.5,
                         getWidth() / 1.5, getHeight() / 3.3, 10.0f, 1.0f);
  // g.drawRect(getWidth() / 2 - (getWidth() / 3), getHeight() / 3.5, getWidth()
  // / 1.5, getHeight() / 3);
  g.setColour(juce::Colours::rebeccapurple.fromHSV(
      juce::Colours::rebeccapurple.getHue(), 0.5f, 0.075f, 1.0f));
  g.fillRoundedRectangle(getWidth() / 2 - (getWidth() / 3), getHeight() / 4.5,
                         getWidth() / 1.5, getHeight() / 3.3, 10.0f);
}

void VibeSamplerAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  // waveform visual
  memberWaveformVisual.setBoundsRelative(0.18f, 0.24f, 0.63f, 0.40f);

  // adsr
  memberADSRGainPoly.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.85f);

  // keyboard
  keyboardComponent.setBounds(0, 425, getWidth(), 75);

  // placing and scaling logo (image)
  double scaleFactor = 2.5;  // image size / scaleFactor = new image size
  int center = (getWidth() / 2) -
               (vibeLogoBars.getImage().getWidth() / (2 * scaleFactor));
  // vibe logo audio meter
  vibeLogoBars.setBounds(center, 10,
                         vibeLogoBars.getImage().getWidth() / scaleFactor,
                         (vibeLogoBars.getImage().getHeight() / scaleFactor));
  scaleFactor = 1.25;
  // vibe logo text
  center = (getWidth() / 2) -
           (vibeLogoText.getImage().getWidth() / (2 * scaleFactor));
  vibeLogoText.setBounds(center, 15,
                         vibeLogoText.getImage().getWidth() / scaleFactor,
                         (vibeLogoText.getImage().getHeight() / scaleFactor));

  // get dimensions for button
  int y = getHeight() / 2 - 210;
  int width = 80;
  int height = 30;
  memberLoadButton.setBounds(getWidth() - (getWidth() / 6) - width / 2, y, width, height);
  memberPolyphonyKnob.setBounds( (getWidth() / 6) - width / 2, y, width, height);
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
// void VibeSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider
// *slider) { if (slider == &memberAttackKnob) {
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

void VibeSamplerAudioProcessorEditor::timerCallback() { repaint(); }