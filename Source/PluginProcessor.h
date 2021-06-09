#pragma once

#include <JuceHeader.h>

class OneKnobVSTAudioProcessor  : public juce::AudioProcessor
{
public:
    OneKnobVSTAudioProcessor();
    ~OneKnobVSTAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    /* my code additions begin */
    // toggles
    int mute = 0;
    int highpass = 0;
    int lowpass = 0;
    int auto_gain = 0;
    int safe = 0;
    // gain
    double gain_in_db = 0.0;
    double gain_out_db = 0.0;
    double main_knob = 0.0;
    // registers
    double hp_reg_l[50], hp_reg_r[50]; //maximum of 50 filter
    double lp_reg_l[50], lp_reg_r[50]; //coefficients per register
    float latency_reg[100]; //register for 100 recent latency values
    int lri = 0; //latency register index
    // Chebyshev Type II IIR filter coefficients (pre-calculated in Python)
    float hp_b[7] = {0.78860887, -4.73038644, 11.82406645, -15.76457775, 11.82406645,
        -4.73038644, 0.78860887}; //high-pass b coefficients
    float hp_a[7] = {1.0, -5.52394777, 12.73308976, -15.67591688, 10.87035844,
        -4.02548448, 0.62190395}; //high-pass a coefficients
    float lp_b[12] = {0.00873387, 0.07032997, 0.27673247, 0.69702874,
        1.24325642, 1.64468146, 1.64468146, 1.24325642, 0.69702874,
        0.27673247, 0.07032997, 0.00873387}; //low-pass b coefficients
    float lp_a[12] = {1.0, 8.42821720e-01, 2.01759480e+00, 1.34308202e+00,
        1.39690490e+00, 6.96218598e-01, 3.96710793e-01, 1.35765892e-01, 4.27220646e-02,
        8.44714523e-03, 1.18242707e-03, 7.54831047e-05}; //low-pass a coefficients
    int hp_K = 6;  //high-pass filter order (coefficients - 1)
    int lp_K = 11; //low-pass filter order (coefficients - 1)
    /* my code additions end */
    
    /* new additions begin */
    int mode = 0;
    const double knee = 0.25;
    const double ratio = 20.0;
    /* new additions end*/
    
    struct parameterValues
    {
        int mute = 0;
        int highpass = 0;
        int lowpass = 0;
        int auto_gain = 0;
        int safe = 0;
        double gain_in_db = 0.0;
        double gain_out_db = 0.0;
        double main_knob = 0.0;
        int prev_mode = 0; //new addition
    };
    /* original code ends */
    
private:
    
    /* original code begins */
    parameterValues state;
    /* original code ends */
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneKnobVSTAudioProcessor)
};
