/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   ADSRGainPolyButtons.cpp
*/

#include "ADSRGainPolyButtons.h"
#include <JuceHeader.h>

// Attack, Decay, Sustain, Release, and Polyphony Knobs
ADSRGainPolyButtons::ADSRGainPolyButtons(VibeSamplerAudioProcessor& p)
    : audioProcessor(p)
{
  // Attack Knob - set style, color, text
	makeKnob(attackKnob, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::white, juce::Colours::whitesmoke, juce::Colours::rebeccapurple, juce::Slider::TextBoxBelow);
  attackKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  attackKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "attack", attackKnob);
  addAndMakeVisible(attackKnob);
	makeLabel(attackLabel, attackKnob, 15.0f, "Attack", juce::NotificationType::dontSendNotification, juce::Justification::centredTop, juce::Colours::white); 

  // Decay Knob - set style, color, text
	makeKnob(decayKnob, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::white, juce::Colours::whitesmoke, juce::Colours::rebeccapurple, juce::Slider::TextBoxBelow);
  decayKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  decayKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "decay", decayKnob);
  addAndMakeVisible(decayKnob);
	makeLabel(decayLabel, decayKnob, 15.0f, "Decay", juce::NotificationType::dontSendNotification, juce::Justification::centredTop, juce::Colours::white);


  // Sustain Knob - set style, color, text
	makeKnob(sustainKnob, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::white, juce::Colours::whitesmoke, juce::Colours::rebeccapurple, juce::Slider::TextBoxBelow);
  sustainKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  sustainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "sustain", sustainKnob);
  addAndMakeVisible(sustainKnob);
	makeLabel(sustainLabel, sustainKnob, 15.0f, "Sustain", juce::NotificationType::dontSendNotification, juce::Justification::centredTop, juce::Colours::white);


  // Release Knob - set style, color, text
	makeKnob(releaseKnob, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::white, juce::Colours::whitesmoke, juce::Colours::rebeccapurple, juce::Slider::TextBoxBelow);
  releaseKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  releaseKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "release", releaseKnob);
  addAndMakeVisible(releaseKnob);
	makeLabel(releaseLabel, releaseKnob, 15.0f, "Release", juce::NotificationType::dontSendNotification, juce::Justification::centredTop, juce::Colours::white);

  // Gain Knob - set style, color, text
	makeKnob(gainKnob, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::white, juce::Colours::whitesmoke, juce::Colours::rebeccapurple, juce::Slider::TextBoxBelow);
  // value tree state solution for listener knob
  gainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "gain", gainKnob);
  addAndMakeVisible(gainKnob);
	makeLabel(gainLabel, gainKnob, 15.0f, "Gain", juce::NotificationType::dontSendNotification, juce::Justification::centredTop, juce::Colours::white);
}

ADSRGainPolyButtons::~ADSRGainPolyButtons() {}

// function to make slider knob with text box for value
void ADSRGainPolyButtons::makeKnob(juce::Slider &knob, juce::Slider::SliderStyle knobStyle, juce::Colour textValueColor, juce::Colour knobThumbColor, juce::Colour sliderFillColor, juce::Slider::TextEntryBoxPosition textBoxPosition)
{
	knob.setSliderStyle(knobStyle);
	knob.setColour(juce::Slider::ColourIds::textBoxTextColourId, textValueColor);
	knob.setColour(juce::Slider::ColourIds::thumbColourId, knobThumbColor);
	knob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, sliderFillColor);
	knob.setTextBoxStyle(textBoxPosition, false, 55, 20);
}

// function to make label and attach it to knob
void ADSRGainPolyButtons::makeLabel(juce::Label &label, juce::Slider &knob, float fontSize, juce::StringRef labelText, juce::NotificationType toSendNotification, juce::Justification textJustification, juce::Colour textColor)
{
	label.setFont(fontSize);
	label.setText(labelText, toSendNotification);
	label.setJustificationType(textJustification);
	label.setColour(juce::Label::ColourIds::textColourId, textColor);
	label.attachToComponent(&knob, false);
}

void ADSRGainPolyButtons::paint(juce::Graphics& g) {}

// sizing
void ADSRGainPolyButtons::resized()
{
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
