#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OneKnobVSTAudioProcessorEditor  : public juce::AudioProcessorEditor,
    /* original additions begin */
    public juce::Slider::Listener, public juce::Button::Listener
    /* original additions end */
{
public:
    OneKnobVSTAudioProcessorEditor (OneKnobVSTAudioProcessor&);
    ~OneKnobVSTAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* original code begins */
    void sliderValueChanged(juce::Slider *slider) override;
    void buttonClicked(juce::Button *button) override;
    /* original code ends */

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OneKnobVSTAudioProcessor& audioProcessor;
    
    /* my code additions begin */
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
    /* my code additions end */
    
    /* new additions begin */
    juce::TextButton mode;
    juce::Image background;
    /* new additions end*/

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneKnobVSTAudioProcessorEditor)
};
