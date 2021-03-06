/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   WaveformVisual.cpp
*/

#include "WaveformVisual.h"

#include "PluginEditor.h"

WaveformVisual::WaveformVisual(VibeSamplerAudioProcessor& p)
    : audioProcessor(p)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

WaveformVisual::~WaveformVisual() {}

void WaveformVisual::paint(juce::Graphics& g)
{
  // get waveform from the processor
  auto waveform = audioProcessor.getWaveform();

  // if waveform should be drawn
  if (waveform.getNumSamples() > 0)
	{
    audioSnapshotLocations.clear();
    // get waveform from the processor
    // auto waveform = audioProcessor.getWaveform();
    // scaling waveform to size of window
    auto scaling = waveform.getNumSamples() / getWidth();
    // buffer read pointer
    auto buffer = waveform.getReadPointer(0);

    // setting up path for drawing waveform
    juce::Path p;
    g.setColour(juce::Colours::whitesmoke);
    p.startNewSubPath(0, getHeight() - 130);

    // scaling on x-axis
    // selecting snapshot locations of waveform using the scaling factor
    for (int sample = 0; sample < waveform.getNumSamples(); sample += scaling)
		{
      audioSnapshotLocations.push_back(buffer[sample]);
    }
    // scaling on y-axis
    // selecting snapshot locations of waveform using the scaling factor
    for (int sample = 0; sample < audioSnapshotLocations.size(); sample++)
		{
      auto snapshot = juce::jmap<float>(audioSnapshotLocations[sample], -1.0f, 1.0f, getHeight() / 1.5, 0);
      p.lineTo(sample, snapshot);
    }

    // actually drawing waveform here; thickness 2
    g.strokePath(p, juce::PathStrokeType(2));

    // drawing playhead
    auto playheadPosition = juce::jmap<int>(audioProcessor.getSampleCount(),
                                      0, audioProcessor.getWaveform().getNumSamples(), 0.5, getWidth());
    g.setColour(juce::Colours::white);
    g.drawLine(playheadPosition, 0, playheadPosition, getHeight() / 1.5, 2.0f);
    g.setColour(juce::Colours::rebeccapurple.withAlpha(0.35f));
    g.fillRoundedRectangle(0, 0, playheadPosition, getHeight() / 1.5, 2.0f);


  }
}

void WaveformVisual::resized() {}

// method for seeing if the file type dropped on the sampler is an appropriate
// for audio
bool WaveformVisual::isInterestedInFileDrag(const juce::StringArray& files)
{
  for (auto file : files)
	{
    if (file.contains(".wav") || file.contains(".mp3") ||
        file.contains(".aiff") || file.contains(".aif"))
		{
      return true;
    }
  }
  return false;
}

// method for loading the dropped file
void WaveformVisual::filesDropped(const juce::StringArray& files, int x,
                                  int y)
{
  for (auto file : files)
	{
    if (isInterestedInFileDrag(file))
		{
      // create file and get filename to display upon loading of a sample
      auto myFile = std::make_unique<juce::File>(file);
      filename = myFile->getFileNameWithoutExtension();

      // load the file
      audioProcessor.loadDroppedFile(file);
    }
  }
  // repaint to display successful file load message
  repaint();
  audioProcessor.getActiveEditor()->repaint();
}
