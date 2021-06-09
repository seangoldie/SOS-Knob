#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <chrono> //for latency reporting

OneKnobVSTAudioProcessor::OneKnobVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // initialize the register arrays
    for (int k = 0; k < 50; ++k)
    {
        hp_reg_l[k] = 0.0;
        hp_reg_r[k] = 0.0;
        lp_reg_l[k] = 0.0;
        lp_reg_r[k] = 0.0;
    }
}

OneKnobVSTAudioProcessor::~OneKnobVSTAudioProcessor()
{
}

//==============================================================================
const juce::String OneKnobVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OneKnobVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OneKnobVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OneKnobVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OneKnobVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OneKnobVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OneKnobVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OneKnobVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OneKnobVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void OneKnobVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    
}

//==============================================================================
void OneKnobVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OneKnobVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneKnobVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OneKnobVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    /* my code additions begin - (4) */
    using namespace std::chrono;
    auto start = high_resolution_clock::now(); //capture time at beginning of block
    /* my code additions end - (4) */
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        /* ORIGINAL AUDIO PROCESSING COMPONENTS BEGIN */
        
        // declare and define `N` as the length of the buffer
        int N = buffer.getNumSamples();
        // declare register and filter variables
        double h;
        int i, n, k;
        // declare and initialize temporary buffers
        double temp_buff_1[N],
            temp_buff_2[N],
            temp_buff_3[N];
        for (i = 0; i < N; ++i)
        {
            temp_buff_1[i] = 0.0;
            temp_buff_2[i] = 0.0;
            temp_buff_3[i] = 0.0;
        }
        
        // core processing loop 1 of 2
        for (n = 0; n < N; ++n)
        {
            // break loop if mute toggle is on
            if (mute) break;

            // apply input gain and pass values into the first temporary buffer
            temp_buff_1[n] = channelData[n] * pow(10, gain_in_db/20); //dB to scalar

            // high-pass filter audio if toggle is on
            if (highpass)
            {
                if (!channel) //process the left channel
                {
                    // shift register values
                    for (k = hp_K; k > 0; --k)
                    {
                        hp_reg_l[k] = hp_reg_l[k - 1];
                    }
                    // denominator
                    hp_reg_l[0] = temp_buff_1[n];
                    for (k = 1; k <= hp_K; ++k)
                    {
                        hp_reg_l[0] -= hp_a[k] * hp_reg_l[k];
                    }
                    // numerator
                    h = 0;
                    for (k = 0; k <= hp_K; ++k)
                    {
                        h += hp_b[k] * hp_reg_l[k];
                    }
                }
                else //process the right channel
                {
                    // shift register values
                    for (k = hp_K; k > 0; --k)
                    {
                        hp_reg_r[k] = hp_reg_r[k - 1];
                    }
                    // denominator
                    hp_reg_r[0] = temp_buff_1[n];
                    for (k = 1; k <= hp_K; ++k)
                    {
                        hp_reg_r[0] -= hp_a[k] * hp_reg_r[k];
                    }
                    // numerator
                    h = 0;
                    for (k = 0; k <= hp_K; ++k)
                    {
                        h += hp_b[k] * hp_reg_r[k];
                    }
                }
                // output filtered values to next temporary buffer
                temp_buff_2[n] = h;
            }
            // otherwise, pass directly to next temporary buffer
            else temp_buff_2[n] = temp_buff_1[n];
            
            /* NEW SWITCHCASE ADDITIONS BEGIN*/
//            main_knob = main_knob / 100;
            short int polarity = temp_buff_2[n]/abs(temp_buff_2[n]);
            double threshold = main_knob/100.0f;
            switch (mode)
            {
                case 0: // Clippy
                    threshold = pow(1 - threshold, 2);
                    if (abs(temp_buff_2[n]) >= threshold)
                    {
                        temp_buff_2[n] = temp_buff_2[n] / abs(temp_buff_2[n]) * threshold;
                    }
                    break;
                case 1: // Sunlight-Yellow Overdrivuu
                    temp_buff_2[n] += temp_buff_2[n] * (main_knob / 10);
                    break;
                case 2: // Bad (smol) squaash
                    threshold = pow(1 - threshold, 2);
                    if (abs(temp_buff_2[n]) >= threshold - knee && abs(temp_buff_2[n]) <= threshold + knee)
                    {
                        temp_buff_2[n] = polarity * ((abs(temp_buff_2[n] - threshold)/ratio) + threshold);
                    }
                    break;
                case 3: // Fuzzy bubbler
                    temp_buff_2[n] *= pow(2, (main_knob / 10));
                    break;
                case 4: // BIG (sassy) squatch
                    if (abs(temp_buff_2[n]) <= threshold)
                    {
                        temp_buff_2[n] += polarity * (threshold - (abs(temp_buff_2[n]) / ratio));
                    }
                        break;
                default:
                    break;
            }
            
            /* NEW SWITCHCASE ADDITIONS END */
            
            // low-pass filter audio if toggle is on
            if (lowpass)
            {
                if (!channel) //process left channel
                {
                    // shift register values
                    for (k = lp_K; k > 0; --k)
                    {
                        lp_reg_l[k] = lp_reg_l[k - 1];
                    }
                    // denominator
                    lp_reg_l[0] = temp_buff_2[n];
                    for (k = 1; k <= lp_K; ++k)
                    {
                        lp_reg_l[0] -= lp_a[k] * lp_reg_l[k];
                    }
                    // numerator
                    h = 0;
                    for (k = 0; k <= lp_K; ++k)
                    {
                        h += lp_b[k] * lp_reg_l[k];
                    }
                }
                else //process right channel
                {
                    // shift register values
                    for (k = lp_K; k > 0; --k)
                    {
                        lp_reg_r[k] = lp_reg_r[k - 1];
                    }
                    // denominator
                    lp_reg_r[0] = temp_buff_2[n];
                    for (k = 1; k <= lp_K; ++k)
                    {
                        lp_reg_r[0] -= lp_a[k] * lp_reg_r[k];
                    }
                    // numerator
                    h = 0.0;
                    for (k = 0; k <= lp_K; ++k)
                    {
                        h += lp_b[k] * lp_reg_r[k];
                    }
                }
                // output filtered values to next temporary buffer
                temp_buff_3[n] = h;
            }
            // otherwise, pass directly to next temporary buffer
            else temp_buff_3[n] = temp_buff_2[n];
            
            // auto-set output gain if toggle is on
            if (auto_gain)
            {
                gain_out_db = 20 * log10(threshold);
            }
            
            // apply output gain
            temp_buff_3[n] *= pow(10, gain_out_db/20); //dB to scalar
            
            // set max output to +-0.99 if safe mode is on
            if (safe)
            {
                if (abs(temp_buff_3[n]) > 0.99)
                {
                    temp_buff_3[n] = 0.99 * temp_buff_3[n] / abs(temp_buff_3[n]);
                }
            }
        }
        // core processing loop 2 of 2
        for (n = 0; n < N; ++n)
        {
            // set outputs to `0` if toggle is on
            if (mute)
            {
                channelData[n] = 0.0;
                continue;
            }
            // feed final processing buffer back into channelData output buffer
            channelData[n] = temp_buff_3[n];
        }
    }
    // latency reporting
    auto stop = high_resolution_clock::now(); //capture time at end of block
    auto latency = duration_cast<microseconds>(stop - start); //cast difference to latency in microseconds
    // display latency in debugger output window (comment out if not in use--adds to latency when engaged)
//    std::cout << "Processing Latency: " << latency.count() << " microseconds" << std::endl;
    // set latency register index
    ++lri;
    lri = lri % 100;
    latency_reg[lri] = latency.count(); //insert current block's latency value into register
    /* my code additions end - (5) */
}

//==============================================================================
bool OneKnobVSTAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OneKnobVSTAudioProcessor::createEditor()
{
    return new OneKnobVSTAudioProcessorEditor (*this);
}

//==============================================================================
void OneKnobVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // write parameter state information to memory
    
    /* original code begins */
    destData.reset();
    
    state.main_knob = main_knob;
    state.gain_in_db = gain_in_db;
    state.gain_out_db = gain_out_db;
    state.highpass = highpass;
    state.lowpass = lowpass;
    state.mute = mute;
    state.safe = safe;
    state.auto_gain = auto_gain;
    state.prev_mode = mode;
    
    destData.append(&state, sizeof(state));
    /* original code ends */
}

void OneKnobVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // restore parameter state information from memory
    
    /* original code begins */
    const parameterValues* newData = reinterpret_cast<const parameterValues*>(data);
    
    main_knob = newData[0].main_knob;
    gain_in_db = newData[0].gain_in_db;
    gain_out_db = newData[0].gain_out_db;
    highpass = newData[0].highpass;
    lowpass = newData[0].lowpass;
    mute = newData[0].mute;
    safe = newData[0].safe;
    auto_gain = newData[0].auto_gain;
    mode = newData[0].prev_mode;
    /* original code ends */
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneKnobVSTAudioProcessor();
}
