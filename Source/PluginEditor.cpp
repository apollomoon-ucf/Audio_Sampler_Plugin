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
    : AudioProcessorEditor(&p), memberWaveformVisual(p), audioProcessor(p) {
  // lambda function to run on button click
  memberLoadButton.onClick = [&]() {
    audioProcessor.loadFile();
    memberWaveformVisual.activateWaveForm(true);
    repaint();
  };

  // make waveform visual
  addAndMakeVisible(memberWaveformVisual);
  // make load button a child component of this current component
  addAndMakeVisible(memberLoadButton);

  // setting up ADSR and gain knobs and labels
  // Attack Knob
  memberAttackKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberAttackKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                             juce::Colours::black);
  memberAttackKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                             juce::Colours::rebeccapurple);
  memberAttackKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                             juce::Colours::rebeccapurple);
  memberAttackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberAttackKnob.setTextValueSuffix(" ms");
  // value tree state solution for listener knob
  memberAttackKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "attack", memberAttackKnob);
  // no longer using listener or range with Value Tree State solution
  // memberAttackKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberAttackKnob.addListener(this);
  addAndMakeVisible(memberAttackKnob);
  memberAttackLabel.setFont(10.0f);
  memberAttackLabel.setText("Attack",
                            juce::NotificationType::dontSendNotification);
  memberAttackLabel.setJustificationType(juce::Justification::centredTop);
  memberAttackLabel.setColour(juce::Label::ColourIds::textColourId,
                              juce::Colours::black);
  memberAttackLabel.attachToComponent(&memberAttackKnob, false);

  // Decay Knob
  memberDecayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberDecayKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                            juce::Colours::black);
  memberDecayKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                            juce::Colours::rebeccapurple);
  memberDecayKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                            juce::Colours::rebeccapurple);
  memberDecayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberDecayKnob.setTextValueSuffix(" ms");
  // value tree state solution for listener knob
  memberDecayKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "decay", memberDecayKnob);
  // no longer using listener or range with Value Tree State solution
  // memberDecayKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberDecayKnob.addListener(this);
  addAndMakeVisible(memberDecayKnob);
  memberDecayLabel.setFont(10.0f);
  memberDecayLabel.setText("Decay",
                           juce::NotificationType::dontSendNotification);
  memberDecayLabel.setJustificationType(juce::Justification::centredTop);
  memberDecayLabel.setColour(juce::Label::ColourIds::textColourId,
                             juce::Colours::black);
  memberDecayLabel.attachToComponent(&memberDecayKnob, false);
  // addAndMakeVisible(memberDecayLabel);

  // Sustain Knob
  memberSustainKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberSustainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::black);
  memberSustainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::rebeccapurple);
  memberSustainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  memberSustainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberSustainKnob.setTextValueSuffix(" ms");
  // value tree state solution for listener knob
  memberSustainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "sustain", memberSustainKnob);
  // no longer using listener or range with Value Tree State solution
  // memberSustainKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberSustainKnob.addListener(this);
  addAndMakeVisible(memberSustainKnob);
  memberSustainLabel.setFont(10.0f);
  memberSustainLabel.setText("Sustain",
                             juce::NotificationType::dontSendNotification);
  memberSustainLabel.setJustificationType(juce::Justification::centredTop);
  memberSustainLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::black);
  memberSustainLabel.attachToComponent(&memberSustainKnob, false);
  // addAndMakeVisible(memberSustainLabel);

  // Release Knob
  memberReleaseKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::black);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::rebeccapurple);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  memberReleaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberReleaseKnob.setTextValueSuffix(" ms");
  // value tree state solution for listener knob
  memberReleaseKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "release", memberReleaseKnob);
  // no longer using listener or range with Value Tree State solution
  // memberReleaseKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberReleaseKnob.addListener(this);
  addAndMakeVisible(memberReleaseKnob);
  memberReleaseLabel.setFont(10.0f);
  memberReleaseLabel.setText("Release",
                             juce::NotificationType::dontSendNotification);
  memberReleaseLabel.setJustificationType(juce::Justification::centredTop);
  memberReleaseLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::black);
  memberReleaseLabel.attachToComponent(&memberReleaseKnob, false);
  // addAndMakeVisible(memberReleaseLabel);

  // Gain Knob
  memberGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberGainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                           juce::Colours::black);
  memberGainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                           juce::Colours::rebeccapurple);
  memberGainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                           juce::Colours::rebeccapurple);
  memberGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberGainKnob.setTextValueSuffix(" db");
  // value tree state solution for listener knob
  memberGainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "gain", memberGainKnob);
  // no longer using listener or range with Value Tree State solution
  // memberGainKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberGainKnob.addListener(this);
  addAndMakeVisible(memberGainKnob);
  memberGainLabel.setFont(10.0f);
  memberGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
  memberGainLabel.setJustificationType(juce::Justification::centredTop);
  memberGainLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::black);
  memberGainLabel.attachToComponent(&memberGainKnob, false);
  // addAndMakeVisible(memberGainLabel);

  // Polyphony Knob
  memberPolyphonyKnob.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                                juce::Colours::black);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                                juce::Colours::rebeccapurple);
  memberPolyphonyKnob.setColour(
      juce::Slider::ColourIds::rotarySliderFillColourId,
      juce::Colours::rebeccapurple);
  memberPolyphonyKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55,
                                      20);
  memberPolyphonyKnob.setTextValueSuffix(" Voice(s)");
  // value tree state solution for listener knob
  memberPolyphonyKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "polyphony", memberPolyphonyKnob);
  // no longer using listener or range with Value Tree State solution
  memberPolyphonyKnob.setRange(1.0, 32.0, 1.0);
  // memberPolyphonyKnob.addListener(this);
  addAndMakeVisible(memberPolyphonyKnob);
  memberPolyphonyLabel.setFont(20.0f);
  memberPolyphonyLabel.setText("Polyphony",
                               juce::NotificationType::dontSendNotification);
  memberPolyphonyLabel.setJustificationType(juce::Justification::centredTop);
  memberPolyphonyLabel.setColour(juce::Label::ColourIds::textColourId,
                                 juce::Colours::black);
  memberPolyphonyLabel.attachToComponent(&memberPolyphonyKnob, false);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(600, 400);
}

// destructor
VibeSamplerAudioProcessorEditor::~VibeSamplerAudioProcessorEditor() {}

//==============================================================================
void VibeSamplerAudioProcessorEditor::paint(juce::Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.fillAll(juce::Colours::white);
  g.setColour(juce::Colours::black);
  g.setFont(15.0f);
  //// if waveform should be drawn
  // if (memberActivateWaveformVisual) {
  //  memberAudioSnapshotLocations.clear();
  //  // get waveform from the processor
  //  auto waveform = audioProcessor.getWaveform();
  //  // scaling waveform to size of window
  //  auto scaling = waveform.getNumSamples() / getWidth();
  //  // buffer read pointer
  //  auto buffer = waveform.getReadPointer(0);

  //  // setting up path for drawing waveform
  //  juce::Path p;
  //  p.startNewSubPath(0, getHeight() / static_cast<float>(2));

  //  // scaling on x-axis
  //  // selecting snapshot locations of waveform using the scaling factor
  //  for (int sample = 0; sample < waveform.getNumSamples(); sample += scaling)
  //  {
  //    memberAudioSnapshotLocations.push_back(buffer[sample]);
  //  }
  //  // scaling on y-axis
  //  // selecting snapshot locations of waveform using the scaling factor
  //  for (int sample = 0; sample < memberAudioSnapshotLocations.size();
  //       sample++) {
  //    auto snapshot = juce::jmap<float>(memberAudioSnapshotLocations[sample],
  //                                      -1.0f, 1.0f, 300, 0);
  //    p.lineTo(sample, snapshot);
  //  }

  //  // actually drawing waveform here; thickness 2
  //  g.strokePath(p, juce::PathStrokeType(2));

  //  // deactivate waveform visualization
  //  memberActivateWaveformVisual = false;
  //}
  g.drawFittedText(
      "Vibe Music Productions\n VST Testing - Brian Moon",
      getLocalBounds(), juce::Justification::centredTop, 1);
  if (audioProcessor.getNumberOfSamplerSounds() > 0) {
    g.drawFittedText("     \n \n\n\n\n\n\n\n\n \n\n\n   Sound file loaded!", getLocalBounds(),
                     juce::Justification::centred, 1);
  }
}

void VibeSamplerAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  // waveform visual
  memberWaveformVisual.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);

  // resize and move adsr knobs
  const auto proportionalX = 0.07f;
  const auto spacing = 0.15f;
  const auto proportionalY = 0.7f;
  const auto proportionalWidth = 0.25f;
  const auto proportionalHeight = 0.25f;
  memberAttackKnob.setBoundsRelative(proportionalX, proportionalY,
                                     proportionalWidth, proportionalHeight);
  memberDecayKnob.setBoundsRelative(proportionalX + spacing, proportionalY,
                                    proportionalWidth, proportionalHeight);
  memberSustainKnob.setBoundsRelative(proportionalX + (spacing * 2),
                                      proportionalY, proportionalWidth,
                                      proportionalHeight);
  memberReleaseKnob.setBoundsRelative(proportionalX + (spacing * 3),
                                      proportionalY, proportionalWidth,
                                      proportionalHeight);
  memberGainKnob.setBoundsRelative(proportionalX + (spacing * 4), proportionalY,
                                   proportionalWidth, proportionalHeight);
  memberPolyphonyKnob.setBoundsRelative(
      proportionalX, 0.1, proportionalWidth / 1.5, proportionalHeight / 1.5);

  // get dimensions for button
  int x = getWidth() / 2 - 100;
  int y = getHeight() / 2 - 160;
  int width = 200;
  int height = 50;
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
      // load the file
      audioProcessor.loadDroppedFile(file);
      // draw waveform?
      // memberActivateWaveformVisual = true;
    }
  }
  // repaint to display successful file load message
  repaint();
}

// listener for knobs
void VibeSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  if (slider == &memberAttackKnob) {
    audioProcessor.getADSRParameters().attack = memberAttackKnob.getValue();
  } else if (slider == &memberDecayKnob) {
    audioProcessor.getADSRParameters().decay = memberDecayKnob.getValue();
  } else if (slider == &memberSustainKnob) {
    audioProcessor.getADSRParameters().sustain = memberSustainKnob.getValue();
  } else if (slider == &memberReleaseKnob) {
    audioProcessor.getADSRParameters().release = memberReleaseKnob.getValue();
  } else if (slider == &memberGainKnob) {
    audioProcessor.gain = memberGainKnob.getValue();
  } else if (slider == &memberPolyphonyKnob) {
    audioProcessor.changePolyphony(memberPolyphonyKnob.getValue());
  }

  // update adsr
  audioProcessor.getADSRGainValue();
}