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
  memberValueTreeState.state.addListener(this);

  // adding the number of voices declared in PluginProcessor.h
  //for (int i = 0; i < memberNumberOfVoices; i++) {
  //  memberSampler.addVoice(new juce::SamplerVoice());
  //}
  for (int i = 0; i < memberVoiceInitNumber; i++) {
    memberSampler.addVoice(new juce::SamplerVoice());
  }
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
  int startSample = 0;
  int numSamples = buffer.getNumSamples();
  auto outputAudio = buffer;
  auto inputMidi = midiMessages;

  // calling to get adsr values (and printing them to console if needed)
  // getADSRGainValue();
  if (memberShouldUpdateParameters) {
    getADSRGainValue();
  }
  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // renders the next audio block of audio output
  // the midi notes are used to trigger voices
  // outputAudio.applyGainRamp(startSample, numSamples, 5.0, 0.0); -- need to fix pops
  memberSampler.renderNextBlock(outputAudio, inputMidi, startSample,
                                numSamples);

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* channelData = buffer.getWritePointer(channel);

    // ..do something to the data...
  }
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
}

void VibeSamplerAudioProcessor::setStateInformation(const void* data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
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
    int attackTimeSecs = 0.4;
    int releaseTimeSecs = 0.4;
    // max sample length
    int maxSampleLengthSecs = 10.0;
    getADSRGainValue();
    // creating new sampler sound containing the audio file selected by user
    memberSampler.addSound(new juce::SamplerSound(
        "Sample", *memberFormatReader, midiRange, midiNoteForNormalPitch,
        attackTimeSecs, releaseTimeSecs, maxSampleLengthSecs));

    return userFile.getFileNameWithoutExtension();
  }
  return "";
}

// method for loading file and creating sampler sound with dropped file
void VibeSamplerAudioProcessor::loadDroppedFile(const juce::String& path) {
  memberSampler.clearSounds();
  auto userFile = juce::File(path);
  memberFormatReader = memberFormatManager.createReaderFor(userFile);

  // reading waveform
  auto sampleLength = memberFormatReader->lengthInSamples;
  memberWaveform.setSize(1, sampleLength);
  memberFormatReader->read(&memberWaveform, 0, sampleLength, 0, true, false);

  // testing that we are properly reading file into audio buffer
  // read through audio file
  // uto buffer = memberWaveform.getReadPointer(0);
  // for (int sample = 0; sample < memberWaveform.getNumSamples(); sample++) {
  //   std::cout << buffer[sample];
  // }

  // range of playable midi notes - 128 midi notes
  juce::BigInteger midiRange;
  midiRange.setRange(0, 128, true);
  // C3 = midi note 60
  int midiNoteForNormalPitch = 60;
  // attack and release time
  int attackTimeSecs = 0.4;
  int releaseTimeSecs = 0.4;
  // max sample length
  int maxSampleLengthSecs = 10.0;

  // creating new sampler sound containing the audio file selected by user
  memberSampler.addSound(new juce::SamplerSound(
      "Sample", *memberFormatReader, midiRange, midiNoteForNormalPitch,
      attackTimeSecs, releaseTimeSecs, maxSampleLengthSecs));

  getADSRGainValue();
}

// listening for adsr and gain
void VibeSamplerAudioProcessor::getADSRGainValue() {
  /* std::cout << "Attack: " << attack << std::endl;
  std::cout << "Decay: " << decay << std::endl;
  std::cout << "Sustain: " << sustain << std::endl;
  std::cout << "Release: " << release << std::endl;
  std::cout << "Gain: " << gain << std::endl; */

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

  // getting and updating sounds
  for (int i = 0; i < memberSampler.getNumSounds(); i++) {
    if (auto sound = dynamic_cast<juce::SamplerSound*>(memberSampler.getSound(i).get())) {
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
      "attack", "Attack", minValue, 2.0, defaultValue));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "decay", "Decay", minValue, maxValue, defaultValue));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "sustain", "Sustain", minValue, maxValue, defaultValue));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "release", "Release", minValue, maxValue, defaultValue));
  parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
      "gain", "Gain", minValue, maxValue, defaultValue));
  parameters.push_back(std::make_unique<juce::AudioParameterInt>(
      "polyphony", "Polyphony", minValue, 32, defaultValue));

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
