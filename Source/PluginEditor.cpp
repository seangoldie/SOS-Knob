#include "PluginProcessor.h"
#include "PluginEditor.h"

/* my code additions begin - (1) */
#define GUI_WD 520 //pixels
#define GUI_HT 380 //pixels
#define DB_LOW -60.0f //decibels
#define DB_HIGH 12.0f //decibels
#define INIT 0.0f //initialize
#define INC 0.1f //increment
/* my code additions end - (1) */

OneKnobVSTAudioProcessorEditor::OneKnobVSTAudioProcessorEditor (OneKnobVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    /* GUI COMPONENTS BEGIN */
    
    setResizable(false, false);
    background = juce::ImageCache::getFromMemory(BinaryData::SOSKnobBG1_0_png, BinaryData::SOSKnobBG1_0_pngSize);
    
    addAndMakeVisible(reset_button);
    reset_button.setButtonText("reset");
    reset_button.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    reset_button.addListener(this);
    // input gain slider
    addAndMakeVisible(gain_in_slider);
    gain_in_slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gain_in_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gain_in_slider.setRange(DB_LOW, DB_HIGH, INC);
    gain_in_slider.setValue(INIT);
    gain_in_slider.setTextValueSuffix(" dB");
    gain_in_slider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    gain_in_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    gain_in_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    gain_in_slider.addListener(this);
    // main knob
    addAndMakeVisible(main_knob);
    main_knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    main_knob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    main_knob.setRange(0.0, 100.0, INC);
    main_knob.setValue(INIT);
    main_knob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    main_knob.addListener(this);
    // mute toggle
    addAndMakeVisible(mute_toggle);
    mute_toggle.setButtonText("MUTE");
    mute_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    mute_toggle.addListener(this);
    // high-pass filter toggle
    addAndMakeVisible(highpass_toggle);
    highpass_toggle.setButtonText("HIGH-PASS");
    highpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    highpass_toggle.addListener(this);
    // low-pass filter toggle
    addAndMakeVisible(lowpass_toggle);
    lowpass_toggle.setButtonText("LOW-PASS");
    lowpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    lowpass_toggle.addListener(this);
    // output gain slider
    addAndMakeVisible(gain_out_slider);
    gain_out_slider.setSliderStyle(juce::Slider::Slider::LinearVertical);
    gain_out_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gain_out_slider.setRange(DB_LOW, DB_HIGH, INC);
    gain_out_slider.setValue(INIT);
    gain_out_slider.setTextValueSuffix(" dB");
    gain_out_slider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    gain_out_slider.addListener(this);
    // automatic output gain toggle
    addAndMakeVisible(auto_gain_toggle);
    auto_gain_toggle.setButtonText("AUTO");
    auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    auto_gain_toggle.addListener(this);
    // safe-mode toggle
    addAndMakeVisible(safe_toggle);
    safe_toggle.setButtonText("safe");
    safe_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    safe_toggle.addListener(this);
    // latency reporting button
    addAndMakeVisible(get_latency);
    get_latency.setButtonText("latency");
    get_latency.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    get_latency.addListener(this);
    // latency report GUI display label
    addAndMakeVisible(latency_label);
    latency_label.setText(" ", juce::dontSendNotification);
    latency_label.attachToComponent(&get_latency, true);
    
    /* new additions begin */
    addAndMakeVisible(mode);
//    mode.Component::setColour(juce::TextButton::textColourOnId, juce::Colours::grey);
//    mode.Component::setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    mode.setButtonText("FLAVOR");
    mode.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
    mode.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    mode.addListener(this);
    /* new additions end */
    
    // default application window dimensions
    setSize(GUI_WD, GUI_HT);
    // default typeface
    font.setTypefaceName("Futura");
    
    /* GUI COMPONENTS END */
}

OneKnobVSTAudioProcessorEditor::~OneKnobVSTAudioProcessorEditor()
{
    
}

void OneKnobVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    /* my code additions begin - (6) */
    /* set GUI look and feel */
//    g.fillAll(juce::Colours::black);
    g.drawImageAt(background, 0, 0);

    g.setColour(juce::Colours::white);
    g.setFont(font);
    
    g.setFont(15.0f);
    g.drawText("IN", getWidth() * 0.125, getHeight() * 0.20, getWidth() * 0.125, getHeight() * 0.10, juce::Justification::centredTop);
    g.setFont(19.0f);
    g.drawText("Sean & Jack's", getWidth() * 0.375, getHeight() * 0.10, getWidth() * 0.25, getHeight() * 0.10, juce::Justification::centredTop);
    g.setFont(22.0f);
    g.drawText("Spicy SOS Knob", getWidth() * 0.375, getHeight() * 0.14, getWidth() * 0.25, getHeight() * 0.10, juce::Justification::centredTop);
    g.setFont(15.0f);
    g.drawText("Cow Elvis DSP", getWidth() * 0.375, getHeight() * 0.86, getWidth() * 0.25, getHeight() * 0.10, juce::Justification::centredTop);
    g.setFont(15.0f);
    g.drawText("OUT", getWidth() * 0.75, getHeight() * 0.20, getWidth() * 0.125, getHeight() * 0.10, juce::Justification::centredTop);
    /* my code additions end - (6) */
}

void OneKnobVSTAudioProcessorEditor::resized()
{
    /* my code additions begin - (7) */
    /* set relative positions of GUI elements */
    reset_button.setBounds(5, 5, 50, 20);
    
    gain_in_slider.setBounds(getWidth() * 0.125, getHeight() * 0.3, getWidth() * 0.125, getHeight() * 0.45);

    main_knob.setBounds(getWidth() * 0.25, getHeight() * 0.25, getWidth() * 0.5, getHeight() * 0.60);

    mute_toggle.setBounds(getWidth() * 0.125, getHeight() * 0.775, getWidth() * 0.125, getHeight() * 0.1);
    
    highpass_toggle.setBounds(getWidth() * 0.125, getHeight() * 0.9, getWidth() * 0.125, 20);
    
    lowpass_toggle.setBounds(getWidth() * 0.75, getHeight() * 0.9, getWidth() * 0.125, 20);
    
    gain_out_slider.setBounds(getWidth() * 0.75, getHeight() * 0.3, getWidth() * 0.125, getHeight() * 0.45);
    
    auto_gain_toggle.setBounds(getWidth() * 0.75, getHeight() * 0.775, getWidth() * 0.125, getHeight() * 0.1);
    
    safe_toggle.setBounds(60, 5, 50, 20);
    
    get_latency.setBounds(getWidth() - 55, 5, 50, 20);
    
    mode.setBounds(getWidth() * 0.5 - getWidth() * 0.125 / 2, getHeight() * 0.9, getWidth() * 0.125, 20);
    
    gain_in_slider.setValue(audioProcessor.gain_in_db);
    main_knob.setValue(audioProcessor.main_knob);
    if (audioProcessor.mute)
    {
        mute_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        gain_in_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
        gain_in_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        main_knob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
        main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
        gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
        gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    }
    if (audioProcessor.highpass) highpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    if (audioProcessor.lowpass) lowpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    if (!audioProcessor.auto_gain) gain_out_slider.setValue(audioProcessor.gain_out_db);
    if (audioProcessor.safe) safe_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    if (audioProcessor.auto_gain)
    {
        auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
        gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    }
    
    /* new additions begin */
    if (audioProcessor.mode == 0)
        {
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::red);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        }
        else if (audioProcessor.mode == 1)
        {
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgoldenrod);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgoldenrod);
        }
        else if (audioProcessor.mode == 2)
        {
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgreen);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
        }
        else if (audioProcessor.mode == 3)
        {
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkviolet);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::violet);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkviolet);
        }
        else if (audioProcessor.mode == 4)
        {
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::turquoise);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
        }
    
    /* new additions end */
    
}

void OneKnobVSTAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    /* slider actions */
    // set input gain with GUI slider
    if (slider == &gain_in_slider) audioProcessor.gain_in_db = gain_in_slider.getValue();
    // set clipper threshold with GUI slider
    if (slider == &main_knob) audioProcessor.main_knob = main_knob.getValue();
    // disengage `auto_gain` if output gain GUI slider is interacted with
    // set output gain with GUI slider
    if (slider == &gain_out_slider)
    {
        if (audioProcessor.auto_gain) audioProcessor.auto_gain = 0;
        auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        audioProcessor.gain_out_db = gain_out_slider.getValue();
        if (!audioProcessor.mute)
        {
            gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
            gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
        }
    }
}

void OneKnobVSTAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    /* button actions */
    if (button == &reset_button)
    {
        // `reset_button` re-initializes all parameters (except for safe-mode)
        gain_in_slider.setValue(INIT);
        gain_out_slider.setValue(INIT);
        main_knob.setValue(INIT);
        audioProcessor.mute = 0;
        mute_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        
        audioProcessor.highpass = 0;
        highpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        audioProcessor.lowpass = 0;
        lowpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        
        audioProcessor.gain_out_db = gain_out_slider.getValue();
        gain_in_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        gain_in_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
        
        audioProcessor.auto_gain = 0;
        auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        
        audioProcessor.mode = 0;
        main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred);
        main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        
        gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
        
        latency_label.setText(" ", juce::dontSendNotification);
        setSize(GUI_WD, GUI_HT);
    }
    if (button == &mute_toggle)
    {
        // `mute_toggle` toggles the `mute` statement in the DSP block and changes the GUI accordingly
        // when engaged, DSP calculations are bypassed and audio output signal values are set to zero
        if (audioProcessor.mute == 0)
        {
            audioProcessor.mute = 1;
            mute_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
            gain_in_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
            gain_in_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgrey);
            main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
            gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
            gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        }
        else
        {
            audioProcessor.mute = 0;
            mute_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            gain_in_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
            gain_in_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
            
            if (audioProcessor.mode == 1)
            {
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgoldenrod);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgoldenrod);
            }
            else if (audioProcessor.mode == 2)
            {
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgreen);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
            }
            else if (audioProcessor.mode == 3)
            {
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkviolet);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::violet);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkviolet);
            }
            else if (audioProcessor.mode == 4)
            {
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::turquoise);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
            }
            else if (audioProcessor.mode == 0)
            {
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::red);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
            }
            
            if (audioProcessor.auto_gain == 0)
            {
                gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
                gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
            }
        }
    }
    if (button == &highpass_toggle)
    {
        // `highpass_toggle` toggles the `highpass` filter statement in the DSP block
        // when engaged, feeds audio data through an IIR high-pass filter
        if (!audioProcessor.highpass)
        {
            audioProcessor.highpass = 1;
            highpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        }
        else
        {
            audioProcessor.highpass = 0;
            highpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        }
    }
    if (button == &lowpass_toggle)
    {
        // `lowpass_toggle` toggles the `lowpass` filter statement in the DSP block
        // when engaged, feeds audio data through an IIR low-pass filter
        if (!audioProcessor.lowpass)
        {
            audioProcessor.lowpass = 1;
            lowpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        }
        else
        {
            audioProcessor.lowpass = 0;
            lowpass_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        }
    }
    if (button == &auto_gain_toggle)
    {
        // `auto_gain_toggle` toggles the `auto_gain` statement in the DSP block
        // when engaged, automatically sets output gain values to mirror input gain/clipper threshold
        if (audioProcessor.auto_gain == 0)
        {
            audioProcessor.auto_gain = 1;
            auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
            gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::darkgrey);
            gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        }
        else
        {
            audioProcessor.auto_gain = 0;
            auto_gain_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            audioProcessor.gain_out_db = gain_out_slider.getValue();
            if (!audioProcessor.mute)
            {
                gain_out_slider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
                gain_out_slider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
            }
        }
    }
    if (button == &safe_toggle)
    {
        // `safe_toggle` toggles the `safe` statement in the DSP block
        // safe-mode stops outputs from exceeding +-0.99
        if (audioProcessor.safe == 0)
        {
            audioProcessor.safe = 1;
            safe_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        }
        else
        {
            audioProcessor.safe = 0;
            safe_toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        }
    }
    if (button == &get_latency)
    {
        // on click of `get_latency`, compute mean latency over a sample of recent blocks and display in GUI
        using namespace std;
        float sum = 0.0; //calculate mean latency over 100 recent blocks
        for (int i = 0; i < 100; ++i) sum += audioProcessor.latency_reg[i];
        float mean_latency = sum / 100;
        float latency_ms = mean_latency / 1000; //convert mean latency to milliseconds
        if (latency_ms >= 1.0) //display overload warning if latency calculated is >= 1 ms
        {
            char overload[] = "OVERLOAD\0";
            // display warning in `orangered`
            latency_label.setColour(juce::Label::textColourId, juce::Colours::orangered);
            latency_label.setText(overload, juce::dontSendNotification);
        }
        else //otherwise, display latency values in corresponding colors
        {
            string str = to_string(latency_ms); //convert latency value to a string
            char dest[9] = "      "; //declare and define destination string
            for (int i = 0; i < 5; ++i) dest[i] = str[i]; //copy first 5 values into destination string
            strcat(dest, "ms\0"); //append unit of measurement
            // display in `green` if latency is <= 0.2 ms
            if (latency_ms <= 0.2) latency_label.setColour(juce::Label::textColourId, juce::Colours::limegreen);
            // display in `yellow` if latency is > 0.2 ms and <= 0.3 ms
            else if (latency_ms <= 0.3) latency_label.setColour(juce::Label::textColourId, juce::Colours::yellow);
            // otherwise, display in `coral`
            else latency_label.setColour(juce::Label::textColourId, juce::Colours::coral);
            latency_label.setText(dest, juce::dontSendNotification);
        }
    }
    
    /* new additions begin */
    if (button == &mode)
    {
        if(audioProcessor.mute)
        {
            return;
        }
        else
        {
            if (audioProcessor.mode == 0)
            {
                audioProcessor.mode = 1;
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgoldenrod);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgoldenrod);
            }
            else if (audioProcessor.mode == 1)
            {
                audioProcessor.mode = 2;
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkgreen);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
            }
            else if (audioProcessor.mode == 2)
            {
                audioProcessor.mode = 3;
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkviolet);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::violet);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkviolet);
            }
            else if (audioProcessor.mode == 3)
            {
                audioProcessor.mode = 4;
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::turquoise);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
            }
            else if (audioProcessor.mode == 4)
            {
                audioProcessor.mode = 0;
                main_knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred);
                main_knob.setColour(juce::Slider::thumbColourId, juce::Colours::red);
                mode.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
            }
        }
    }
    /* new additions end */
}
