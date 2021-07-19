/*
  ==============================================================================

    ADSRGainPolyButtons.cpp
    Created: 15 Jul 2021 10:55:44pm
    Author:  Brian

  ==============================================================================
*/

#include "ADSRGainPolyButtons.h"

#include <JuceHeader.h>

//==============================================================================
ADSRGainPolyButtons::ADSRGainPolyButtons(VibeSamplerAudioProcessor& p)
    : audioProcessor(p) {
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  // setting up ADSR and gain knobs and labels
  // Attack Knob
  memberAttackKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberAttackKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                             juce::Colours::white);
  memberAttackKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                             juce::Colours::whitesmoke);
  memberAttackKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                             juce::Colours::rebeccapurple);
  memberAttackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberAttackKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  memberAttackKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "attack", memberAttackKnob);
  // no longer using listener or range with Value Tree State solution
  // memberAttackKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberAttackKnob.addListener(this);
  addAndMakeVisible(memberAttackKnob);
  memberAttackLabel.setFont(15.0f);
  memberAttackLabel.setText("Attack",
                            juce::NotificationType::dontSendNotification);
  memberAttackLabel.setJustificationType(juce::Justification::centredTop);
  memberAttackLabel.setColour(juce::Label::ColourIds::textColourId,
                              juce::Colours::white);
  memberAttackLabel.attachToComponent(&memberAttackKnob, false);

  // Decay Knob
  memberDecayKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberDecayKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                            juce::Colours::white);
  memberDecayKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                            juce::Colours::whitesmoke);
  memberDecayKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                            juce::Colours::rebeccapurple);
  memberDecayKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberDecayKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  memberDecayKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "decay", memberDecayKnob);
  // no longer using listener or range with Value Tree State solution
  // memberDecayKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberDecayKnob.addListener(this);
  addAndMakeVisible(memberDecayKnob);
  memberDecayLabel.setFont(15.0f);
  memberDecayLabel.setText("Decay",
                           juce::NotificationType::dontSendNotification);
  memberDecayLabel.setJustificationType(juce::Justification::centredTop);
  memberDecayLabel.setColour(juce::Label::ColourIds::textColourId,
                             juce::Colours::white);
  memberDecayLabel.attachToComponent(&memberDecayKnob, false);
  // addAndMakeVisible(memberDecayLabel);

  // Sustain Knob
  memberSustainKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberSustainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::white);
  memberSustainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::whitesmoke);
  memberSustainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  memberSustainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberSustainKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  memberSustainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "sustain", memberSustainKnob);
  // no longer using listener or range with Value Tree State solution
  // memberSustainKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberSustainKnob.addListener(this);
  addAndMakeVisible(memberSustainKnob);
  memberSustainLabel.setFont(15.0f);
  memberSustainLabel.setText("Sustain",
                             juce::NotificationType::dontSendNotification);
  memberSustainLabel.setJustificationType(juce::Justification::centredTop);
  memberSustainLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::white);
  memberSustainLabel.attachToComponent(&memberSustainKnob, false);
  // addAndMakeVisible(memberSustainLabel);

  // Release Knob
  memberReleaseKnob.setSliderStyle(
      juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                              juce::Colours::white);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                              juce::Colours::whitesmoke);
  memberReleaseKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                              juce::Colours::rebeccapurple);
  memberReleaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  memberReleaseKnob.setTextValueSuffix(" s");
  // value tree state solution for listener knob
  memberReleaseKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "release", memberReleaseKnob);
  // no longer using listener or range with Value Tree State solution
  // memberReleaseKnob.setRange(0.0f, 5.0f, 0.01f);
  // memberReleaseKnob.addListener(this);
  addAndMakeVisible(memberReleaseKnob);
  memberReleaseLabel.setFont(15.0f);
  memberReleaseLabel.setText("Release",
                             juce::NotificationType::dontSendNotification);
  memberReleaseLabel.setJustificationType(juce::Justification::centredTop);
  memberReleaseLabel.setColour(juce::Label::ColourIds::textColourId,
                               juce::Colours::white);
  memberReleaseLabel.attachToComponent(&memberReleaseKnob, false);
  // addAndMakeVisible(memberReleaseLabel);

  // Gain Knob
  memberGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  memberGainKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                           juce::Colours::white);
  memberGainKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                           juce::Colours::whitesmoke);
  memberGainKnob.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,
                           juce::Colours::rebeccapurple);
  memberGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
  // memberGainKnob.setTextValueSuffix(" ");
  // value tree state solution for listener knob
  memberGainKnobAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.getValueTreeState(), "gain", memberGainKnob);
  // no longer using listener or range with Value Tree State solution
  // memberGainKnob.setRange(-75.0f, 12.0f, 0.01f);
  // memberGainKnob.setValue(0.0f);
  // memberGainKnob.addListener(this);
  addAndMakeVisible(memberGainKnob);
  memberGainLabel.setFont(15.0f);
  memberGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
  memberGainLabel.setJustificationType(juce::Justification::centredTop);
  memberGainLabel.setColour(juce::Label::ColourIds::textColourId,
                            juce::Colours::white);
  memberGainLabel.attachToComponent(&memberGainKnob, false);
  // addAndMakeVisible(memberGainLabel);

  // Polyphony Knob
  memberPolyphonyKnob.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::textBoxTextColourId,
                                juce::Colours::white);
  memberPolyphonyKnob.setColour(juce::Slider::ColourIds::thumbColourId,
                                juce::Colours::rebeccapurple);
  //memberPolyphonyKnob.setColour(juce::Slider::ColourIds::textBoxBackgroundColourId,
  //                           juce::Colours::black);
  //memberPolyphonyKnob.setIncDecButtonsMode(
  //    juce::Slider::IncDecButtonMode::incDecButtonsDraggable_Horizontal);
  getLookAndFeel().setColour(juce::TextButton::ColourIds::buttonColourId,
                             juce::Colours::black);
  memberPolyphonyKnob.setColour(
      juce::ComboBox::ColourIds::textColourId,
      juce::Colours::yellow);
  memberPolyphonyKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55,
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
}

ADSRGainPolyButtons::~ADSRGainPolyButtons() {}

void ADSRGainPolyButtons::paint(juce::Graphics& g) {
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
  if (audioProcessor.polyphony > 1) {
    memberPolyphonyKnob.setTextValueSuffix(" poly");
  } else {
    memberPolyphonyKnob.setTextValueSuffix(" mono");
  }

  // g.fillAll(juce::Colours::black);
}

void ADSRGainPolyButtons::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
  // coordinates
  /*const auto proportionalX = 0.07f;*/
  const auto proportionalX = 0.08f;
  const auto spacing = 0.15f;
  const auto proportionalY = 0.7f;
  const auto proportionalWidth = 0.25f;
  const auto proportionalHeight = 0.25f;

  // resize and move adsr knobs
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
      proportionalX, 0.1, proportionalWidth / 2, proportionalHeight / 2);
}
