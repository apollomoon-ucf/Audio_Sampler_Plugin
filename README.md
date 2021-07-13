![V2-2020-Vibe-Logo-Transparent-450-1-2](https://user-images.githubusercontent.com/25870426/124214978-276e1080-dac1-11eb-8ba0-3751a11327ae.png)
# Vibe Music Productions - Audio Sampler Plugin

## Introduction
For one of my summer projects, I will be building an audio sampler plugin prototype for Vibe Music Productions. 
Vibe is a professional recording studio and music production facility located in Ponte Vedra Beach, Florida. 

Recording Studio Website: https://www.vibemusicproductions.com/

The development process of the plugin will be documented in this readme.


## Technology and Setup
The recording studio will need the plugin to be compatible with Native Instrument's Maschine 2 running on Windows 10. 
C++ and the JUCE audio framework will be used to develop the plugin.

(Loading VST tester in Native Instrument's Mashine 2 Software)
![Screenshot_Loading_Vibe_Sampler](https://user-images.githubusercontent.com/25870426/124214309-ffca7880-dabf-11eb-9eed-066e948a09a0.png)

(VST success message)
![Screenshot_SuccessMessage_Vibe_Sampler](https://user-images.githubusercontent.com/25870426/124215843-d9f2a300-dac2-11eb-90a8-cd4d554ba7ac.png)


## Layout
To visualize a rough layout of the plugin, I created a sketch using https://sketch.io/sketchpad/, simply putting some images together to get a feel for the layout. 
Knobs are a modified version of a template from KnobMan.

Note: This visualization is not meant to represent the final design of the user interface. Once plugin functionality is established, I will work on making it aesthetically pleasing.

(First sketch of plugin prototype)
![Screenshot_vibe_sampler_sketch](https://user-images.githubusercontent.com/25870426/124827433-0899cf00-df44-11eb-906f-ef3fe2c57b89.png)

Update (7/7/21): Users would like to have a sound library located on the left side of the plugin to streamline the process of dragging and dropping samples onto the sample editor located in the center of the plugin. 
Maschine 2 has a sidebar for samples if we can't make this work properly, but it would be a nice addition. Also, the Gain knob may look better in the top right corner as a dedicated master volume knob.

(Second sketch of plugin prototype with proposed sample/sound library sidebar)
![Screenshot_vibe_sampler_sketch_with_library](https://user-images.githubusercontent.com/25870426/124839763-64ba1e80-df57-11eb-98a8-1e24caae70ae.png)

## Development
Now it's time to dive into the core functionality of the plugin. Drag and drop capabilities were added so users can simply drop their desired sound onto the plugin.
I also added a file upload link for .wav, .mp3, and .aiff/.aif files to give the user an additional upload method. 

Upon successful upload of an acceptable audio file, a success message is displayed (as seen in the image below).

At this point, the plugin loads a file, and the sound can be triggered via midi in Maschine 2 (a test beat was successfully created using the VibeSampler in this raw form as the only instrument).
Note: plugin states are not reloadable at this stage; plugin is cleared when daw is restarted.

(Plugin displaying success message "Sound file loaded!" after loading sound/sample)
![Screenshot_plugin_loading_success_drag_drop](https://user-images.githubusercontent.com/25870426/124999586-76192e80-e01c-11eb-9e0c-0585727ece39.png)

Continuing development, I added knobs and labels and displayed them on the plugin. I attempted to match the placement of the knobs to the plugin layout seen in the previous section.

(Plugin displaying knobs, labels, and sample load options)
![Screenshot_knobs_labels_coded](https://user-images.githubusercontent.com/25870426/125114151-90ecb100-e0b7-11eb-8f0d-fbd6cbc28ead.png)

Added button to change polyphony of sampler, and ADSR knobs are fully functional. Gain knob still needs to be connected to output.

(Plugin displaying polyphonic control)
![Screenshot_polyphony](https://user-images.githubusercontent.com/25870426/125218924-e977a600-e291-11eb-910a-62d93dca71fb.png)

Displayed waveform in center of window with width equal to window length. 
I want to create a sample window to house the waveform, therefore, the next step will be to draw the waveform inside of a centered box.

(Plugin displaying waveform)
![waveform_image_for_sketch](https://user-images.githubusercontent.com/25870426/125361438-4c714780-e33b-11eb-8cde-75046963175f.png)

Centered waveform on plugin, and preparing to outline/frame it to match layout.
Displayed Sample Vibe logo, but need to replace it with a high resolution version, along with the text "Vibe Audio Sampler" during the design phase.

(Plugin displaying waveform)
![Screenshot_waveform_drawn_v2](https://user-images.githubusercontent.com/25870426/125387967-afc79d80-e36c-11eb-8714-768a8e039a12.png)