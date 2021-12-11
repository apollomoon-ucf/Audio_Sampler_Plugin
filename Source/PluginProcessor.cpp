/*
  Author:      Brian Moon
  Project:     Vibe Audio Plugin (Sampler/Sample Player)
  File Name:   PluginProcessor.cpp
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"

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
      valueTreeState(*this, nullptr, "PARAMS", getParameterLayout())
#endif
{
  // setting up format manager for different audio formats
  formatManager.registerBasicFormats();

  storedAudioFile.referTo(
      valueTreeState.state.getPropertyAsValue("sample", nullptr));
  storedAudioFilename.referTo(
      valueTreeState.state.getPropertyAsValue("sample_name", nullptr));

  valueTreeState.state.setProperty(
      "sample", juce::var(audioFilePath), nullptr);
  valueTreeState.state.setProperty(
      "sample_name", juce::var(audioFilename), nullptr);

  valueTreeState.state.addListener(this);

  // adding the number of voices declared in PluginProcessor.h
  for (int i = 0; i < voiceInitNumber; i++) {
    sampler.addVoice(new juce::SamplerVoice());
  }
}

// destructor
VibeSamplerAudioProcessor::~VibeSamplerAudioProcessor() {
  formatReader = nullptr;
}

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

void VibeSamplerAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  sampler.setCurrentPlaybackSampleRate(sampleRate);

  // previous gain - (for gain smoothing)
  previousGain = *valueTreeState.getRawParameterValue("gain");

  if (valueTreeState.state.getPropertyAsValue("sample", nullptr)
          .toString()
          .isNotEmpty()) {
    loadDroppedFile(
        valueTreeState.state.getPropertyAsValue("sample", nullptr)
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
  getADSRGainValue();

  juce::MidiMessage midiMessage;
  juce::MidiBuffer::Iterator midiBufferIterator{midiMessages};
  int sample;
  double noteFrequency;
  double middleC = 261.625565;
  double samples;


  samples = buffer.getNumSamples();
  // how long the note has been played for
  sampleCount =
      isNoteBeingPlayed ? sampleCount += samples : 0;

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),
                                      true);
  // if the note is being played from external midi
  while (midiBufferIterator.getNextEvent(midiMessage, sample)) {
    if (midiMessage.isNoteOn()) {
      // start playhead
      noteFrequency =
          midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber());
      isNoteBeingPlayed = true;
    } else if (midiMessage.isNoteOff()) {
      // stop playhead
      sampleCount = 0;
      isNoteBeingPlayed = false;
    }
  }
  sampler.renderNextBlock(buffer, midiMessages, 0,
                                buffer.getNumSamples());

  // renders the next audio block of audio output
  // the midi notes are used to trigger voices
  // outputAudio.applyGainRamp(startSample, numSamples, 5.0, 0.0); // -- need to
  // fix pops
  // apply gain smoothing
  if (gain == previousGain) {
    buffer.applyGain(gain);
  } else {
    buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, gain);
    previousGain = gain;
  }

}

bool VibeSamplerAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VibeSamplerAudioProcessor::createEditor() {
  return new VibeSamplerAudioProcessorEditor(*this);
}

void VibeSamplerAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  auto state = valueTreeState.copyState();
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
    if (xmlState->hasTagName(valueTreeState.state.getType())) {
      valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
  }
}
// method for loading file and creating sampler sound with file
juce::String VibeSamplerAudioProcessor::loadFile() {
  sampler.clearSounds();
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
    audioFilePath = userFile.getFullPathName();
    audioFilename = userFile.getFileNameWithoutExtension();
    getADSRGainValue();


    formatReader = formatManager.createReaderFor(userFile);

    // reading waveform
    auto sampleLength = formatReader->lengthInSamples;
    waveform.setSize(1, sampleLength);
    formatReader->read(&waveform, 0, sampleLength, 0, true, false);

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
    sampler.addSound(new juce::SamplerSound(
        "Sample", *formatReader, midiRange, midiNoteForNormalPitch, 0.1f,
        0.1f, maxSampleLengthSecs));
    // sampler.setNoteStealingEnabled(true);
    return userFile.getFileNameWithoutExtension();
  }
  return "";
}

// method for loading file and creating sampler sound with dropped file
void VibeSamplerAudioProcessor::loadDroppedFile(const juce::String& path) {
  sampler.clearSounds();

  auto userFile = juce::File(path);
  formatReader = formatManager.createReaderFor(userFile);
  audioFilePath = path;
  audioFilename = userFile.getFileNameWithoutExtension();
  getADSRGainValue();

  // reading waveform
  auto sampleLength = formatReader->lengthInSamples;
  waveform.setSize(1, sampleLength);
  formatReader->read(&waveform, 0, sampleLength, 0, true, false);

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
  sampler.addSound(new juce::SamplerSound(
      "Sample", *formatReader, midiRange, midiNoteForNormalPitch, 0.1f,
      0.1f, maxSampleLengthSecs));
  // sampler.setNoteStealingEnabled(true);
}

// listening for adsr and gain
void VibeSamplerAudioProcessor::getADSRGainValue() {
  // adding params for Value State Tree solution
  memberADSRGainParameters.attack =
      *valueTreeState.getRawParameterValue("attack");
  memberADSRGainParameters.decay =
      *valueTreeState.getRawParameterValue("decay");
  memberADSRGainParameters.sustain =
      *valueTreeState.getRawParameterValue("sustain");
  memberADSRGainParameters.release =
      *valueTreeState.getRawParameterValue("release");
  gain = *valueTreeState.getRawParameterValue("gain");
  polyphony = *valueTreeState.getRawParameterValue("polyphony");
  // valueTreePropertyChanged(getValueTreeState().state, "samplo");
  valueTreeState.state.setProperty(
      "sample", juce::var(audioFilePath), nullptr);
  valueTreeState.state.setProperty(
      "sample_name", juce::var(audioFilename), nullptr);

  // getting and updating sounds
  for (int i = 0; i < sampler.getNumSounds(); i++) {
    if (auto sound = dynamic_cast<juce::SamplerSound*>(
            sampler.getSound(i).get())) {
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
  float minValue = 0.0;
  float maxValue = 5.0;
  float defaultValue = 0.0;

  // Parameters for knobs -- default values may need to be tweaked

  // Method 1 (using vector to return ParameterLayout):
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
}

void VibeSamplerAudioProcessor::valueTreePropertyChanged(
    juce::ValueTree& treeWhosePropertyHasChanged,
    const juce::Identifier& property) {
  shouldUpdateParameters = true;
}

// method to change polyphony
void VibeSamplerAudioProcessor::changePolyphony(int numberOfVoices) {
  if (numberOfVoices > sampler.getNumVoices()) {
    while (sampler.getNumVoices() != numberOfVoices) {
      sampler.addVoice(new juce::SamplerVoice());
    }
  } else {
    while (sampler.getNumVoices() != numberOfVoices &&
           sampler.getNumVoices() != 1) {
      sampler.removeVoice(0);
    }
  }
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new VibeSamplerAudioProcessor();
}
