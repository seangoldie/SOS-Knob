#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OneKnobVSTAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener, public juce::Button::Listener
{
public:
    OneKnobVSTAudioProcessorEditor (OneKnobVSTAudioProcessor&);
    ~OneKnobVSTAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;
    void buttonClicked(juce::Button *button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OneKnobVSTAudioProcessor& audioProcessor;
    
    juce::TextButton reset_button;
    juce::Slider main_knob;
    juce::Slider gain_in_slider;
    juce::TextButton mute_toggle;
    juce::TextButton highpass_toggle;
    juce::TextButton lowpass_toggle;
    juce::Slider gain_out_slider;
    juce::TextButton auto_gain_toggle;
    juce::TextButton safe_toggle;
    juce::TextButton get_latency;
    juce::Label latency_label;
    juce::Font font;
    
    juce::TextButton mode;
    juce::Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneKnobVSTAudioProcessorEditor)
};
