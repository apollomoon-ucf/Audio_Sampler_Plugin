/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginEditor.cpp
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

// constructor
VibeSamplerAudioProcessorEditor::VibeSamplerAudioProcessorEditor(
    VibeSamplerAudioProcessor &p)
    : AudioProcessorEditor(&p),
      waveformVisual(p),
      memberADSRGainPoly(p),
      audioProcessor(p),
      keyboardComponent(p.getKeyboardState(),
                        juce::MidiKeyboardComponent::horizontalKeyboard) {

  // loading logo from memory
  auto vibeLogoBarsFromMemory = juce::ImageCache::getFromMemory(
      BinaryData::V22020VibeLogoTransparent45012_png,
      BinaryData::V22020VibeLogoTransparent45012_pngSize);
  auto vibeLogoTextFromMemory = juce::ImageCache::getFromMemory(
      BinaryData::fixed_sampler_vibe_logo_png,
      BinaryData::fixed_sampler_vibe_logo_pngSize);
  // setting image
  vibeLogoBars.setImage(vibeLogoBarsFromMemory,
                        juce::RectanglePlacement::stretchToFit);
  vibeLogoText.setImage(vibeLogoTextFromMemory,
                        juce::RectanglePlacement::stretchToFit);
  // make logo visible
  addAndMakeVisible(vibeLogoText);                        

  // make waveform visible
  addAndMakeVisible(waveformVisual);

  // make adsr, gain, and polyphonic knobs visible
  addAndMakeVisible(memberADSRGainPoly);

  // make keyboard visible
  addAndMakeVisible(keyboardComponent);

  // set color for keyboard on hover and when user plays keyboard
  keyboardComponent.setColour(
      juce::MidiKeyboardComponent::ColourIds::mouseOverKeyOverlayColourId,
      juce::Colours::rebeccapurple);
  keyboardComponent.setColour(
      juce::MidiKeyboardComponent::ColourIds::keyDownOverlayColourId,
      juce::Colours::rebeccapurple);  

  // load button
  loadButton.setButtonText("Load Sample");
  loadButtonAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
          audioProcessor.getValueTreeState(), "sample", loadButton);
  loadButton.setColour(juce::TextButton::buttonColourId,
                             juce::Colours::black);
  // make load button a child component of this current component
  addAndMakeVisible(loadButton);
  // lambda function to run on button click for loading audio file
  loadButton.onClick = [&]() {
    juce::String filenameFromClickAndLoad = audioProcessor.loadFile();
    waveformVisual.setFilename(filenameFromClickAndLoad);
    repaint();
  };
  loadLabel.setFont(15.0f);
  loadLabel.setText(waveformVisual.getFilename(),
                          juce::NotificationType::dontSendNotification);
  loadLabel.setJustificationType(juce::Justification::centredTop);
  loadLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::white);
  loadLabel.attachToComponent(&loadButton, false);

  // Polyphony Knob - set style, color, text
  polyphonyKnob.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
  polyphonyKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                                juce::Colours::white);
  polyphonyKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                                juce::Colours::rebeccapurple);
  getLookAndFeel().setColour(juce::TextButton::ColourIds::buttonColourId,
                             juce::Colours::black);
  polyphonyKnob.setColour(juce::ComboBox::ColourIds::textColourId,
                                juce::Colours::yellow);
  polyphonyKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 55, 20);
  // value tree state solution for listener knob
  polyphonyKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "polyphony", polyphonyKnob);
  polyphonyKnob.setRange(1.0, 32.0, 1.0);
  // polyphonyKnob.addListener(this);
  addAndMakeVisible(polyphonyKnob);
  polyphonyLabel.setFont(20.0f);
  polyphonyLabel.setJustificationType(juce::Justification::centredTop);
  polyphonyLabel.setColour(juce::Label::ColourIds::textColourId,
                                 juce::Colours::white);
  polyphonyLabel.attachToComponent(&polyphonyKnob, false);

  // start timer for playhead
  startTimerHz(30);
  setSize(600, 500);
}

// destructor
VibeSamplerAudioProcessorEditor::~VibeSamplerAudioProcessorEditor() {
  stopTimer();
}

void VibeSamplerAudioProcessorEditor::paint(juce::Graphics &g) {
  // fill background
  g.fillAll(juce::Colours::black);
  // set text properties
  g.setColour(juce::Colours::white);
  g.setFont(15.0f);

  // paint filename
  g.setColour(juce::Colours::grey);
  g.setFont(15.0f);
  g.drawText(audioProcessor.getAudioFilename(), 337.5, 75, 200, 40,
             juce::Justification::topRight, true);

  // check if monophonic or polyphonic
  if (audioProcessor.polyphony > 1 && audioProcessor.polyphony < 10) {
    polyphonyKnob.setTextValueSuffix(" poly");
    juce::String string = "Polyphonic:";
    string.append(std::to_string(audioProcessor.polyphony), 1);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  } else if (audioProcessor.polyphony > 9) {
    juce::String string = "Polyphonic:";
    string.append(std::to_string(audioProcessor.polyphony), 2);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  } else {
    polyphonyKnob.setTextValueSuffix(" mono");
    juce::String string = "Monophonic:";
    string.append(std::to_string(audioProcessor.polyphony), 1);
    g.drawText(string, (getWidth() / 6) - 80 / 2, 75, 200, 40,
               juce::Justification::topLeft, true);
  }
 
  g.drawRoundedRectangle(getWidth() / 2 - (getWidth() / 3), getHeight() / 4.5,
                         getWidth() / 1.5, getHeight() / 3.3, 10.0f, 1.0f);
  g.setColour(juce::Colours::rebeccapurple.fromHSV(
      juce::Colours::rebeccapurple.getHue(), 0.5f, 0.075f, 1.0f));
  g.fillRoundedRectangle(getWidth() / 2 - (getWidth() / 3), getHeight() / 4.5,
                         getWidth() / 1.5, getHeight() / 3.3, 10.0f);
    // display load sample if the sampler is empty
    if (audioProcessor.getWaveform().getNumSamples() == 0) {
    g.setFont(13.0f);
    g.setColour(juce::Colours::grey);
    g.drawFittedText("Click the Load Sample button, or drag and drop an audio file here, to get started",
                     getBounds().reduced(175), juce::Justification::centredTop, 2);
  }
}

void VibeSamplerAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  // waveform visual
  waveformVisual.setBoundsRelative(0.18f, 0.24f, 0.63f, 0.40f);

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
  vibeLogoText.setBounds(center, 10,
                         vibeLogoText.getImage().getWidth() / scaleFactor,
                         (vibeLogoText.getImage().getHeight() / scaleFactor));

  // get dimensions for button
  int y = getHeight() / 2 - 210;
  int width = 80;
  int height = 30;
  loadButton.setBounds(getWidth() - (getWidth() / 6) - width / 2, y,
                             width, height);
  polyphonyKnob.setBounds((getWidth() / 6) - width / 2, y - 2, width,
                                height + 2);
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
      waveformVisual.setFilename(myFile->getFileNameWithoutExtension());
      // load the file
      audioProcessor.loadDroppedFile(file);
      // draw waveform
      repaint();
    }
  }
  // repaint to display successful file load message
  repaint();
}


void VibeSamplerAudioProcessorEditor::timerCallback() { repaint(); }