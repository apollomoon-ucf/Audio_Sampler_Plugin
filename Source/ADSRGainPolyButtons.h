/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   ADSRGainPolyButtons.h
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ADSRGainPolyButtons : public juce::Component {
 public:
  ADSRGainPolyButtons(VibeSamplerAudioProcessor &p);
  ~ADSRGainPolyButtons() override;

  void paint(juce::Graphics &) override;
  void resized() override;

 private:
  VibeSamplerAudioProcessor &audioProcessor;

  juce::Slider attackKnob, decayKnob, sustainKnob,
      releaseKnob, gainKnob;

  juce::Label attackLabel, decayLabel, sustainLabel,
      releaseLabel, gainLabel,
      loadLabel;

  // unqiue pointers for attaching knobs/sliders
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      attackKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      decayKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      sustainKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      releaseKnobAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainKnobAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRGainPolyButtons)
};
