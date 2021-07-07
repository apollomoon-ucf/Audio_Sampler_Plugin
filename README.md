![V2-2020-Vibe-Logo-Transparent-450-1-2](https://user-images.githubusercontent.com/25870426/124214978-276e1080-dac1-11eb-8ba0-3751a11327ae.png)
# Vibe Music Productions - Audio Sampler Plugin

# Introduction
For one of my summer projects, I will be building an audio plugin prototype for Vibe Music Productions. 
Vibe is a professional recording studio and music production facility located in Ponte Vedra Beach, Florida. 

Recording Studio Website: https://www.vibemusicproductions.com/

The development process of the plugin will be documented in this readme.


# Technology and Setup
The recording studio will need the plugin to be compatible with Native Instrument's Maschine 2 running on Windows 10. 
C++ and the JUCE audio framework will be used to develop the plugin.

(Loading VST tester in Native Instrument's Mashine 2 Software)
![Screenshot_Loading_Vibe_Sampler](https://user-images.githubusercontent.com/25870426/124214309-ffca7880-dabf-11eb-9eed-066e948a09a0.png)

(VST success message)
![Screenshot_SuccessMessage_Vibe_Sampler](https://user-images.githubusercontent.com/25870426/124215843-d9f2a300-dac2-11eb-90a8-cd4d554ba7ac.png)


# Layout
To visualize a rough layout of the plugin, I created a sketch using https://sketch.io/sketchpad/.

Note: This visualization is not meant to represent the final design of the user interface. Once plugin functionality is established, I will work on making it aesthetically pleasing.

(First sketch of plugin prototype)
![Screenshot_vibe_sampler_sketch](https://user-images.githubusercontent.com/25870426/124827433-0899cf00-df44-11eb-906f-ef3fe2c57b89.png)

Update (7/7/21): Users would like to have a sound library located on the left side of the plugin to streamline the process of dragging and dropping samples into the sample editor located in the center of the plugin. Also, the Gain knob may look better in the top right corner as a dedicated master volume knob.

(Second sketch of plugin prototype with proposed sample/sound library sidebar)
![Screenshot_vibe_sampler_sketch_with_library](https://user-images.githubusercontent.com/25870426/124839763-64ba1e80-df57-11eb-98a8-1e24caae70ae.png)