# Audio Sampler Plugin

## Introduction

For one of my summer projects, I will be building an audio sampler plugin prototype for Vibe Music Productions, a professional recording studio and music production facility located in Ponte Vedra Beach, Florida. https://www.vibemusicproductions.com/.

## Objective

The goal is to build a plugin that can load audio samples, trigger/play the samples via an external and on-screen midi keyboard, control output level, and manipulate the loaded sound using an adsr envelope.

The entire development process of the plugin is documented in this readme.

More changes and features coming soon, please see end of README for latest updates.

-Brian Moon

(Plugin prototype demo as of 7|20|21 - updates coming soon)
![Plugin_Demo_Vibe_Audio_Sampler](Documentation/Plugin_Demo_Vibe_Audio_Sampler.gif)

(Plugin UI as of 7|20|21 - updates coming soon)
![Screenshot_with_KeysLit2](https://user-images.githubusercontent.com/25870426/126427916-dade0854-8c2c-4eb9-bc91-57acc5ebff2c.png)

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

(Plugin displaying knobs, labels (showing ms but needs to be changed to seconds, and sample load options)
![Screenshot_knobs_labels_coded](https://user-images.githubusercontent.com/25870426/125114151-90ecb100-e0b7-11eb-8f0d-fbd6cbc28ead.png)

Added button to change polyphony of sampler, and ADSR knobs are fully functional. Gain knob still needs to be connected to output.

(Plugin displaying polyphonic control)
![Screenshot_polyphony](https://user-images.githubusercontent.com/25870426/125218924-e977a600-e291-11eb-910a-62d93dca71fb.png)

Displayed waveform in center of window with width equal to window width.
I want to create a sample window to house the waveform, therefore, the next step will be to draw the waveform inside of a centered box.

(Plugin displaying waveform)
![waveform_image_for_sketch](https://user-images.githubusercontent.com/25870426/125361438-4c714780-e33b-11eb-8cde-75046963175f.png)

Centered waveform on plugin, and preparing to outline/frame it to match layout.
Displayed sample/test Vibe logo, but need to replace it with a high resolution version, along with the text "Vibe Audio Sampler" during the design phase.

(Plugin displaying waveform - need to change labels to seconds, but I might remove labels or only show them on mouse hover for a clean look)
![Screenshot_waveform_drawn_v2](https://user-images.githubusercontent.com/25870426/125387967-afc79d80-e36c-11eb-8714-768a8e039a12.png)

7|13|21 Corrected positioning of loaded waveform visualization, fixed logo dimensions, added plugin title ("Vibe Audio Sampler"), and tested a dark mode layout.

(Plugin displaying dark mode test)
![Screenshot_dark_mode_mono](https://user-images.githubusercontent.com/25870426/125873022-9e2775b6-612b-4783-abba-d9edfea70ed8.png)

7|17|21 -- Connected Gain knob, saved parameters and audio sample file path in Value Tree to allow settings and sounds to be reloaded when daw is restarted, added a visual component
to house the waveform, and made note to fix audio clicking/popping when a note is played fast in monophonic mode. When a voice is stolen, there is an audible click. The solution
may include changing how abrupt the first sample releases, and how fast the new sample attacks when ending the first sample/voice and starting the new/second sample/voice.

(Plugin displaying updated UI)
![Screenshot_waveform_box](https://user-images.githubusercontent.com/25870426/126054452-96d9f4db-8fcc-421a-a180-73c0d7f87475.png)

7|18|21 -- Adjusted position of ADSR knobs, added playhead and waveform tracing when sample is triggered. The playhead and waveform tracing (shown in purple) display for a duration
equal to the amount of time the midi note is active. Another implementation could be displaying the playhead and waveform tracing for the duration of the release time parameter.
Note: I think I want to add a meter, or meters, to monitor input and output levels; displaying the levels using a purple hue as seen in Vibe's logo.

(Plugin displaying playhead and waveform tracing)
![Screenshot_showing_playhead_waveform_tracing](https://user-images.githubusercontent.com/25870426/126084761-c984dc90-adc5-4ed8-a4b3-a2808c363743.png)

7|19|21 -- Updated logo and filled buttons with black.
Added keyboard component and rescaled layout to fit other elements.
Changed polyphony textbox to label.

(Plugin displaying new keyboard component, poly/mono label, and newly scaled UI)
![Screenshot_vibe_sampler_withKeyboard](https://user-images.githubusercontent.com/25870426/126224630-cc9fd8db-7043-4a23-aed8-a8ac47e96a67.png)

ALT Logo Test

(Plugin displaying alt logo without Vibe text)
![Screenshot_vibe_sampler_withKeyboard_withoutvibetext](https://user-images.githubusercontent.com/25870426/126255242-7e5c7a23-e2ba-4d06-b3e8-5f1b72f79fa6.png)

7|20|21 -- Connected on-screen keyboard and changed mouse hover color and note-on color to rebeccapurple to match color scheme.

(Plugin displaying on screen keyboard connected with highlighted keys)
![Screenshot_with_keys_Lit1](https://user-images.githubusercontent.com/25870426/126414559-c44a323b-8e18-4706-8373-c6188761b375.png)
