/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   ADSRGainPolyButtons.cpp
*/

#include "ADSRGainPolyButtons.h"
#include <JuceHeader.h>

// Attack, Decay, Sustain, Release, and Polyphony Knobs
ADSRGainPolyButtons::ADSRGainPolyButtons(VibeSamplerAudioProcessor& p)
    : audioProcessor(p) {


  // Attack Knob - set style, color, text
  attackKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  attackKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                             juce::Colours::white);
  attackKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                             juce::Colours::whitesmoke);
  attackKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                             juce::Colours::rebeccapurple);
  attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  attackKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  attackKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "attack", attackKnob);
  addAndMakeVisible(attackKnob);
  attackLabel.setFont(15.0f);
  attackLabel.setText("Attack",
                            juce::NotificationType::dontSendNotification);
  attackLabel.setJustificationType(juce::Justification::centredTop);
  attackLabel.setColour(juce::Label::ColourIds::textColourId,
                              juce::Colours::white);
  attackLabel.attachToComponent(&attackKnob, false);


  // Decay Knob - set style, color, text
  decayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  decayKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                            juce::Colours::white);
  decayKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                            juce::Colours::whitesmoke);
  decayKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                            juce::Colours::rebeccapurple);
  decayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  decayKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  decayKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "decay", decayKnob);
  addAndMakeVisible(decayKnob);
  decayLabel.setFont(15.0f);
  decayLabel.setText("Decay",
                           juce::NotificationType::dontSendNotification);
  decayLabel.setJustificationType(juce::Justification::centredTop);
  decayLabel.setColour(juce::Label::ColourIds::textColourId,
                             juce::Colours::white);
  decayLabel.attachToComponent(&decayKnob, false);


  // Sustain Knob - set style, color, text
  sustainKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  sustainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::white);
  sustainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::whitesmoke);
  sustainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  sustainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  sustainKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  sustainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "sustain", sustainKnob);
  addAndMakeVisible(sustainKnob);
  sustainLabel.setFont(15.0f);
  sustainLabel.setText("Sustain",
                             juce::NotificationType::dontSendNotification);
  sustainLabel.setJustificationType(juce::Justification::centredTop);
  sustainLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::white);
  sustainLabel.attachToComponent(&sustainKnob, false);


  // Release Knob - set style, color, text
  releaseKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  releaseKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::white);
  releaseKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::whitesmoke);
  releaseKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  releaseKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  releaseKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "release", releaseKnob);
  addAndMakeVisible(releaseKnob);
  releaseLabel.setFont(15.0f);
  releaseLabel.setText("Release",
                             juce::NotificationType::dontSendNotification);
  releaseLabel.setJustificationType(juce::Justification::centredTop);
  releaseLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::white);
  releaseLabel.attachToComponent(&releaseKnob, false);

  // Gain Knob - set style, color, text
  gainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  gainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                           juce::Colours::white);
  gainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                           juce::Colours::whitesmoke);
  gainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                           juce::Colours::rebeccapurple);
  gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  // value tree state solution for listener knob
  gainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "gain", gainKnob);
  addAndMakeVisible(gainKnob);
  gainLabel.setFont(15.0f);
  gainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
  gainLabel.setJustificationType(juce::Justification::centredTop);
  gainLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::white);
  gainLabel.attachToComponent(&gainKnob, false);
}

ADSRGainPolyButtons::~ADSRGainPolyButtons() {}

void ADSRGainPolyButtons::paint(juce::Graphics& g) {
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
}

// sizing
void ADSRGainPolyButtons::resized() {
  const auto proportionalX = 0.08f;
  const auto spacing = 0.15f;
  const auto proportionalY = 0.7f;
  const auto proportionalWidth = 0.25f;
  const auto proportionalHeight = 0.25f;

  // resize and move adsr knobs
  attackKnob.setBoundsRelative(proportionalX, proportionalY,
                                     proportionalWidth, proportionalHeight);
  decayKnob.setBoundsRelative(proportionalX + spacing, proportionalY,
                                    proportionalWidth, proportionalHeight);
  sustainKnob.setBoundsRelative(proportionalX + (spacing * 2),
                                      proportionalY, proportionalWidth,
                                      proportionalHeight);
  releaseKnob.setBoundsRelative(proportionalX + (spacing * 3),
                                      proportionalY, proportionalWidth,
                                      proportionalHeight);
  gainKnob.setBoundsRelative(proportionalX + (spacing * 4), proportionalY,
                                   proportionalWidth, proportionalHeight);
}
