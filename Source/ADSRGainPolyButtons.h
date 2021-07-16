/*
  ==============================================================================

    ADSRGainPolyButtons.h
    Created: 15 Jul 2021 10:55:44pm
    Author:  Brian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

//==============================================================================
/*
 */
class ADSRGainPolyButtons : public juce::Component {
 public:
  ADSRGainPolyButtons(VibeSamplerAudioProcessor &);
  ~ADSRGainPolyButtons() override;

  void paint(juce::Graphics &) override;
  void resized() override;

 private:
  juce::Slider memberAttackKnob, memberDecayKnob, memberSustainKnob,
      memberReleaseKnob, memberGainKnob, memberPolyphonyKnob;
  juce::Label memberAttackLabel, memberDecayLabel, memberSustainLabel,
      memberReleaseLabel, memberGainLabel, memberPolyphonyLabel,
      memberLoadLabel;

  VibeSamplerAudioProcessor &audioProcessor;

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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRGainPolyButtons)
};
