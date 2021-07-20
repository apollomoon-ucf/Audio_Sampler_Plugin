/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginProcessor.cpp
  Description: This file contains the basic framework code for a JUCE plugin
               processor.
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================

// constructor
VibeSamplerAudioProcessor::VibeSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      memberValueTreeState(*this, nullptr, "PARAMS", getParameterLayout())
#endif
{
  // setting up format manager for different audio formats
  memberFormatManager.registerBasicFormats();

  memberStoredAudioFile.referTo(
      memberValueTreeState.state.getPropertyAsValue("sample", nullptr));
  memberStoredAudioFilename.referTo(
      memberValueTreeState.state.getPropertyAsValue("sample_name", nullptr));

  memberValueTreeState.state.setProperty(
      "sample", juce::var(memberAudioFilePath), nullptr);
  memberValueTreeState.state.setProperty(
      "sample_name", juce::var(memberAudioFilename), nullptr);

  memberValueTreeState.state.addListener(this);

  // adding the number of voices declared in PluginProcessor.h
  for (int i = 0; i < memberVoiceInitNumber; i++) {
    memberSampler.addVoice(new juce::SamplerVoice());
  }
  // memberSampler.setNoteStealingEnabled(true);
}

// destructor
VibeSamplerAudioProcessor::~VibeSamplerAudioProcessor() {
  memberFormatReader = nullptr;
}

//==============================================================================
const juce::String VibeSamplerAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool VibeSamplerAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool VibeSamplerAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool VibeSamplerAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double VibeSamplerAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int VibeSamplerAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int VibeSamplerAudioProcessor::getCurrentProgram() { return 0; }

void VibeSamplerAudioProcessor::setCurrentProgram(int index) {}

const juce::String VibeSamplerAudioProcessor::getProgramName(int index) {
  return {};
}

void VibeSamplerAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
}

//==============================================================================
void VibeSamplerAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  memberSampler.setCurrentPlaybackSampleRate(sampleRate);
  // for (int i = 0; i < memberSampler.getNumVoices(); i++) {
  //  if (myVoice =
  //          dynamic_cast<juce::SamplerVoice*>(memberSampler.getVoice(i))) {
  //    myVoice->setCurrentPlaybackSampleRate(sampleRate);
  //  }
  //}

  // previous gain - (for gain smoothing)
  memberPreviousGain = *memberValueTreeState.getRawParameterValue("gain");

  if (memberValueTreeState.state.getPropertyAsValue("sample", nullptr)
          .toString()
          .isNotEmpty()) {
    loadDroppedFile(
        memberValueTreeState.state.getPropertyAsValue("sample", nullptr)
            .toString());
  }

  getADSRGainValue();
}

void VibeSamplerAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VibeSamplerAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void VibeSamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  // int startSample = 0;
  // int numSamples = buffer.getNumSamples();
  // auto outputAudio = buffer;
  // auto inputMidi = midiMessages;
  // calling to get adsr values (and printing them to console if needed)
  // if (memberShouldUpdateParameters) {

  //  getADSRGainValue();
  //}
  getADSRGainValue();

  juce::MidiMessage midiMessage;
  juce::MidiBuffer::Iterator midiBufferIterator{midiMessages};
  int sample;
  double noteFrequency;
  double middleC = 261.625565;
  double samples;

  // if the note is being played
  while (midiBufferIterator.getNextEvent(midiMessage, sample)) {
    if (midiMessage.isNoteOn()) {
      // start playhead
      noteFrequency =
          midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber());
      memberIsNoteBeingPlayed = true;
    } else if (midiMessage.isNoteOff()) {
      // stop playhead
      memberSampleCount = 0;
      memberIsNoteBeingPlayed = false;
    }
  }

  // else if (midiMessage.isNoteOff() && !buffer.hasBeenCleared()) {
  //  // start playhead
  //  memberSampleCount = 0;
  //  noteFrequency =
  //  midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber());
  //  memberIsNoteBeingPlayed = true;
  //}

  samples = buffer.getNumSamples();
  // how long the note has been played for
  memberSampleCount =
      memberIsNoteBeingPlayed ? memberSampleCount += samples : 0;

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // getActiveEditor().keyboard .processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),
  //                                    true);
  // renders the next audio block of audio output
  // the midi notes are used to trigger voices
  // outputAudio.applyGainRamp(startSample, numSamples, 5.0, 0.0); // -- need to
  // fix pops
  memberSampler.renderNextBlock(buffer, midiMessages, 0,
                                buffer.getNumSamples());

  // apply gain smoothing
  if (gain == memberPreviousGain) {
    buffer.applyGain(gain);
  } else {
    buffer.applyGainRamp(0, buffer.getNumSamples(), memberPreviousGain, gain);
    memberPreviousGain = gain;
  }

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  // for (int channel = 0; channel < totalNumInputChannels; ++channel) {
  //  auto* channelData = buffer.getWritePointer(channel);
  //  buffer.applyGain(channel, 0, buffer.getNumSamples(),
  //                   juce::Decibels::gainToDecibels(gain));
  //  // ..do something to the data...
  //  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
  //    // buffer.setSample(0, channel, juce::Decibels::decibelsToGain(gain));
  //    // buffer.setSample(1, sample, juce::Decibels::decibelsToGain(gain));
  //
  //    channelData[sample] =
  //        channelData[sample] * juce::Decibels::decibelsToGain(gain);
  //  }
  //}
}

//==============================================================================
bool VibeSamplerAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VibeSamplerAudioProcessor::createEditor() {
  return new VibeSamplerAudioProcessorEditor(*this);
}

//==============================================================================
void VibeSamplerAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  auto state = memberValueTreeState.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void VibeSamplerAudioProcessor::setStateInformation(const void* data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));

  if (xmlState.get() != nullptr) {
    if (xmlState->hasTagName(memberValueTreeState.state.getType())) {
      memberValueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
  }
}
// method for loading file and creating sampler sound with file
juce::String VibeSamplerAudioProcessor::loadFile() {
  memberSampler.clearSounds();
  // setting up the JUCE file chooser to select
  // and load an audio file from the user's computer
  juce::FileChooser chooseFile(
      "Select Audio File (.wav, .mp3, or .aiff)",
      juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
      "*.wav; *.mp3; *.aiff");

  // getting result from user's selection
  if (chooseFile.browseForFileToOpen()) {
    // setting up file reader
    auto userFile = chooseFile.getResult();
    memberAudioFilePath = userFile.getFullPathName();
    memberAudioFilename = userFile.getFileNameWithoutExtension();
    getADSRGainValue();
    // valueTreePropertyChanged(memberValueTreeState.state, "gain");

    memberFormatReader = memberFormatManager.createReaderFor(userFile);

    // reading waveform
    auto sampleLength = memberFormatReader->lengthInSamples;
    memberWaveform.setSize(1, sampleLength);
    memberFormatReader->read(&memberWaveform, 0, sampleLength, 0, true, false);

    // range of playable midi notes - 128 midi notes
    juce::BigInteger midiRange;
    midiRange.setRange(0, 128, true);
    // C3 = midi note 60
    int midiNoteForNormalPitch = 60;
    // attack and release time
    int attackTimeSecs = 0.0;
    int releaseTimeSecs = 0.0;
    // max sample length
    int maxSampleLengthSecs = 10.0;

    // creating new sampler sound containing the audio file selected by user
    memberSampler.addSound(new juce::SamplerSound(
        "Sample", *memberFormatReader, midiRange, midiNoteForNormalPitch, 0.1f,
        0.1f, maxSampleLengthSecs));
    // memberSampler.setNoteStealingEnabled(true);
    return userFile.getFileNameWithoutExtension();
  }
  return "";
}

// method for loading file and creating sampler sound with dropped file
void VibeSamplerAudioProcessor::loadDroppedFile(const juce::String& path) {
  memberSampler.clearSounds();

  // getValueTreeState().state.sendPropertyChangeMessage("sample");
  auto userFile = juce::File(path);
  memberFormatReader = memberFormatManager.createReaderFor(userFile);
  memberAudioFilePath = path;
  memberAudioFilename = userFile.getFileNameWithoutExtension();
  // valueTreePropertyChanged(getValueTreeState().state, "gain");
  getADSRGainValue();

  // reading waveform
  auto sampleLength = memberFormatReader->lengthInSamples;
  memberWaveform.setSize(1, sampleLength);
  memberFormatReader->read(&memberWaveform, 0, sampleLength, 0, true, false);

  // range of playable midi notes - 128 midi notes
  juce::BigInteger midiRange;
  midiRange.setRange(0, 128, true);
  // C3 = midi note 60
  int midiNoteForNormalPitch = 60;
  // attack and release time
  int attackTimeSecs = 0.0;
  int releaseTimeSecs = 0.0;
  // max sample length
  int maxSampleLengthSecs = 10.0;

  // creating new sampler sound containing the audio file selected by user
  memberSampler.addSound(new juce::SamplerSound(
      "Sample", *memberFormatReader, midiRange, midiNoteForNormalPitch, 0.1f,
      0.1f, maxSampleLengthSecs));
  // memberSampler.setNoteStealingEnabled(true);
}

// listening for adsr and gain
void VibeSamplerAudioProcessor::getADSRGainValue() {
  // adding params for Value State Tree solution
  memberADSRGainParameters.attack =
      *memberValueTreeState.getRawParameterValue("attack");
  memberADSRGainParameters.decay =
      *memberValueTreeState.getRawParameterValue("decay");
  memberADSRGainParameters.sustain =
      *memberValueTreeState.getRawParameterValue("sustain");
  memberADSRGainParameters.release =
      *memberValueTreeState.getRawParameterValue("release");
  gain = *memberValueTreeState.getRawParameterValue("gain");
  polyphony = *memberValueTreeState.getRawParameterValue("polyphony");
  // valueTreePropertyChanged(getValueTreeState().state, "samplo");
  memberValueTreeState.state.setProperty(
      "sample", juce::var(memberAudioFilePath), nullptr);
  memberValueTreeState.state.setProperty(
      "sample_name", juce::var(memberAudioFilename), nullptr);

  // getting and updating sounds
  for (int i = 0; i < memberSampler.getNumSounds(); i++) {
    if (auto sound = dynamic_cast<juce::SamplerSound*>(
            memberSampler.getSound(i).get())) {
      sound->setEnvelopeParameters(memberADSRGainParameters);
    }
  }

  changePolyphony(polyphony);
}

// parameter layout method
juce::AudioProcessorValueTreeState::ParameterLayout
VibeSamplerAudioProcessor::getParameterLayout() {
  // declare vectors and vars
  std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
  // std::string paramID = "attack";
  // std::string paramName = "Attack";
  float minValue = 0.0;
  float maxValue = 5.0;
  float defaultValue = 0.0;

  // Parameters for knobs -- default values may need to be tweaked

  // Method 1 (using vector to return ParameterLayour):
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "attack", "Attack", 0.0f, 6.0f, 0.0f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "decay", "Decay", 0.0f, 6.0f, 2.18f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "sustain", "Sustain", 0.0f, 6.0f, 3.95f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "release", "Release", 0.0f, 6.0f, 0.36f));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "gain", "Gain", 0.0f, 0.5f, 0.22f));
  parameters.push_back(std::make_unique<juce::AudioParameterInt>(
      "polyphony", "Polyphony", minValue, 32, 32.0));

  return {parameters.begin(), parameters.end()};

  // method crashing program..(review logs when possible)
  //
  // Method 2 (New - adding to and returning ParameterLayout itself):
  // juce::AudioProcessorValueTreeState::ParameterLayout layout;
  // layout.add(std::make_unique<juce::AudioParameterFloat>(
  //    "attack", "Attack", minValue, maxValue, defaultValue));
  // layout.add(std::make_unique<juce::AudioParameterFloat>(
  //    "decay", "Decay", minValue, maxValue, defaultValue));
  // layout.add(std::make_unique<juce::AudioParameterFloat>(
  //    "sustain", "Sustain", minValue, maxValue, defaultValue));
  // layout.add(std::make_unique<juce::AudioParameterFloat>(
  //    "release", "Release", minValue, maxValue, defaultValue));

  // return layout;
}

void VibeSamplerAudioProcessor::valueTreePropertyChanged(
    juce::ValueTree& treeWhosePropertyHasChanged,
    const juce::Identifier& property) {
  memberShouldUpdateParameters = true;
}

// method to change polyphony
void VibeSamplerAudioProcessor::changePolyphony(int numberOfVoices) {
  if (numberOfVoices > memberSampler.getNumVoices()) {
    while (memberSampler.getNumVoices() != numberOfVoices) {
      memberSampler.addVoice(new juce::SamplerVoice());
    }
  } else {
    while (memberSampler.getNumVoices() != numberOfVoices &&
           memberSampler.getNumVoices() != 1) {
      memberSampler.removeVoice(0);
    }
  }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new VibeSamplerAudioProcessor();
}
