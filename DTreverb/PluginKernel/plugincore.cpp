// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**
	PluginParameter* piParam = nullptr;

	piParam = new PluginParameter(controlID::gain, "Gain", "dB", controlVariableType::kInt, -20.000000, 20.000000, 0.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&gain, boundVariableType::kDouble);
	addPluginParameter(piParam);

	
	piParam = new PluginParameter(controlID::predelaytime, "Predelay", "ms", controlVariableType::kInt, 0.000000, 1000.000000, 10.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&predelaytime, boundVariableType::kDouble);
	addPluginParameter(piParam);

	
	piParam = new PluginParameter(controlID::decayfactor, "DecayFactor", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setBoundVariable(&decayfactor, boundVariableType::kDouble);
	addPluginParameter(piParam);

	
	piParam = new PluginParameter(controlID::cutoff, "CutoffFrequency", "Hz", controlVariableType::kDouble, 20.000000, 20000.000000, 2000.000000, taper::kVoltOctaveTaper);
	piParam->setBoundVariable(&cutoff, boundVariableType::kDouble);
	addPluginParameter(piParam);

	
	piParam = new PluginParameter(controlID::damping, "hfDamping", "", controlVariableType::kDouble, 0.000500, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setBoundVariable(&damping, boundVariableType::kDouble);
	addPluginParameter(piParam);

	
	piParam = new PluginParameter(controlID::diffusion, "Diffusion", "", controlVariableType::kDouble, 0.000000, 0.999999, 0.500000, taper::kLinearTaper);
	piParam->setBoundVariable(&diffusion, boundVariableType::kDouble);
	addPluginParameter(piParam);

	

	piParam = new PluginParameter(controlID::wetdry, "Wetdry", "%", controlVariableType::kDouble, 0.000000, 100.000000, 100.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&wetdry, boundVariableType::kDouble);
	addPluginParameter(piParam);


    
	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

	//reset
	apf1.reset();
	apf2.reset();
	apf3.reset();
	apf4.reset();
	apf5.reset();
	apf6.reset();

	//reset maximum buffer size for each filter
	apf1.Buffersize(resetInfo.sampleRate);
	apf2.Buffersize(resetInfo.sampleRate);
	apf3.Buffersize(resetInfo.sampleRate);
	apf4.Buffersize(resetInfo.sampleRate);
	apf5.Buffersize(resetInfo.sampleRate);
	apf6.Buffersize(resetInfo.sampleRate);

	//reset the delay sample for each filter
	apf1.setdelaytime(resetInfo.sampleRate, 142);  //delay sample value is referenced from dattorr's journal 
	apf2.setdelaytime(resetInfo.sampleRate, 107);
	apf3.setdelaytime(resetInfo.sampleRate, 379);
	apf4.setdelaytime(resetInfo.sampleRate, 277);
	apf5.setdelaytime(resetInfo.sampleRate, 1800);
	apf6.setdelaytime(resetInfo.sampleRate, 2656);

	//reset delayline setting
	delay1.reset();
	delay2.reset();
	delay3.reset();
	delay4.reset();
	delay1.Buffersize(resetInfo.sampleRate);
	delay2.Buffersize(resetInfo.sampleRate);
	delay3.Buffersize(resetInfo.sampleRate);
	delay4.Buffersize(resetInfo.sampleRate);
	delay1.setdelaytime(resetInfo.sampleRate, 4453);
	delay2.setdelaytime(resetInfo.sampleRate, 3720);
	delay3.setdelaytime(resetInfo.sampleRate, 3163);
	delay4.setdelaytime(resetInfo.sampleRate, 4217);

	//reset lowpass filter setting
	lpf1.reset();
	lpf2.reset();
	lpf3.reset();
	lpf1.Buffersize(resetInfo.sampleRate);
	lpf2.Buffersize(resetInfo.sampleRate);
	lpf3.Buffersize(resetInfo.sampleRate);


	//reset modulated allpass filter setting
	mallp1.reset();
	mallp2.reset();
	mallp1.Buffersize(resetInfo.sampleRate);
	mallp2.Buffersize(resetInfo.sampleRate);
	mallp1.setdelaytime(resetInfo.sampleRate, 672);
	mallp2.setdelaytime(resetInfo.sampleRate, 908);
	mallp1.excursion(resetInfo.sampleRate);
	mallp2.excursion(resetInfo.sampleRate);


	//reset predelay setting
	predelay.reset();
	predelay.Buffersize(resetInfo.sampleRate);
	predelay.setdelaytime(resetInfo.sampleRate, 300);


	//reset final delayline setting
	outdelay1.reset();
	outdelay2.reset();
	outdelay3.reset();
	outdelay4.reset();
	outdelay5.reset();
	outdelay6.reset();
	outdelay7.reset();
	outdelay8.reset();
	outdelay9.reset();
	outdelay10.reset();
	outdelay11.reset();
	outdelay12.reset();
	outdelay13.reset();
	outdelay14.reset();

	outdelay1.Buffersize(resetInfo.sampleRate);
	outdelay2.Buffersize(resetInfo.sampleRate);
	outdelay3.Buffersize(resetInfo.sampleRate);
	outdelay4.Buffersize(resetInfo.sampleRate);
	outdelay5.Buffersize(resetInfo.sampleRate);
	outdelay6.Buffersize(resetInfo.sampleRate);
	outdelay7.Buffersize(resetInfo.sampleRate);
	outdelay8.Buffersize(resetInfo.sampleRate);
	outdelay9.Buffersize(resetInfo.sampleRate);
	outdelay10.Buffersize(resetInfo.sampleRate);
	outdelay11.Buffersize(resetInfo.sampleRate);
	outdelay12.Buffersize(resetInfo.sampleRate);
	outdelay13.Buffersize(resetInfo.sampleRate);
	outdelay14.Buffersize(resetInfo.sampleRate);

	outdelay1.setdelaytime(resetInfo.sampleRate, 353);
	outdelay2.setdelaytime(resetInfo.sampleRate, 3627);
	outdelay3.setdelaytime(resetInfo.sampleRate, 1990);
	outdelay4.setdelaytime(resetInfo.sampleRate, 1228);
	outdelay5.setdelaytime(resetInfo.sampleRate, 187);
	outdelay6.setdelaytime(resetInfo.sampleRate, 2673);
	outdelay7.setdelaytime(resetInfo.sampleRate, 1066);
	outdelay8.setdelaytime(resetInfo.sampleRate, 121);
	outdelay9.setdelaytime(resetInfo.sampleRate, 1996);
	outdelay10.setdelaytime(resetInfo.sampleRate, 335);
	outdelay11.setdelaytime(resetInfo.sampleRate, 1913);
	outdelay12.setdelaytime(resetInfo.sampleRate, 2111);
	outdelay13.setdelaytime(resetInfo.sampleRate, 2974);
	outdelay14.setdelaytime(resetInfo.sampleRate, 266);
    // --- other reset inits
    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

    return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

    // --- decode the channelIOConfiguration and process accordingly
    //
	double inL = processFrameInfo.audioInputFrame[0];
	double inR = processFrameInfo.audioInputFrame[1];
	double outL, outR;
	double decor;
	double leftTankout = 0;
	double rightTankout = 0;
	double reverb_L ;
	double reverb_R ;
	double monoin ;
	double gainlinDZ = dz_volume.smooth(gainlin);

    // --- FX Plugin:
    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
		outL = inL * gainlinDZ; //dezip
        processFrameInfo.audioOutputFrame[0] = outL;

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		outL = inL * gainlinDZ;  //dezip
	
		//early reflections and decorrelation 
		double pred = predelay.audioprocessing(outL); //predelay
		double LPF1 = lpf1.audioprocessing(pred);  //lowpassfilter
		double APF1 = apf1.audioprocessing(LPF1); //allpassfilter1
		double APF2 = apf2.audioprocessing(APF1); //allpassfilter2
		double APF3 = apf3.audioprocessing(APF2); //allpassfilter3
		double APF4 = apf4.audioprocessing(APF3); //allpassfilter4
		decor = APF4; //input to the tank

		double leftTankin = rightTankout + decor;    // figure of 8 loop 
		double rightTankin = leftTankout + decor;


		//Left Tank
		double modAPF1 = mallp1.audioprocessing(leftTankin);   
		double delayLine1 = delay1.audioprocessing(modAPF1);
		double leftLowpass = lpf2.audioprocessing(delayLine1);
		double decayed1 = leftLowpass * DF; //decay factor multiplication
		double APF5 = apf5.audioprocessing(decayed1);
		double delayLine2 = delay2.audioprocessing(APF5);
		leftTankout = delayLine2;   //output of the tank, going back to the other side of the input


		//Right Tank
		double modAPF2 = mallp2.audioprocessing(rightTankin);
		double delayLine3 = delay3.audioprocessing(modAPF2);
		double rightLowpass = lpf3.audioprocessing(delayLine3);
		double decayed2 = rightLowpass * DF; //decay factor multiplication
		double APF6 = apf6.audioprocessing(decayed2);
		double delayLine4 = delay4.audioprocessing(APF6);
		rightTankout = delayLine4;

		//Tap out
		double a1 = delay1.dtapout(); //tap out from the delayline1 in the tank
		double a2 = apf5.atapout();    //tap out from all pass filter.5 in the tank
		double a3 = delay2.dtapout();   // tap out from the delayline2 in the tank
		double a4 = delay3.dtapout();   //tap out from the delayline3 in the tank
		double a5 = apf6.atapout();     //tap out from all pass filter.6 in the tank
		double a6 = delay4.dtapout();   //tap out from the dleayline4 in the tank

		//add different delay to tap out signal
		double d1 = outdelay1.audioprocessing(a1);    //delay of delayline1 tap     with different value of sample delay
		double d2 = outdelay2.audioprocessing(a1);    //delay of delayline1 tap 
		double d3 = outdelay3.audioprocessing(a1);    //delay of delayline1 tap 
		double d4 = outdelay4.audioprocessing(a2);    //delay of allpassfilter5 tap
		double d5 = outdelay5.audioprocessing(a2);    //delay of allpassfilter5 tap
		double d6 = outdelay6.audioprocessing(a3);    //delay of delayline2 tap 
		double d7 = outdelay7.audioprocessing(a3);    //delay of delayline2 tap 

		double d8 = outdelay8.audioprocessing(a4);    //delay of delayline3 tap
		double d9 = outdelay9.audioprocessing(a4);    //delay of delayline3 tap
		double d10 = outdelay10.audioprocessing(a4);  //delay of delayline3 tap
		double d11 = outdelay11.audioprocessing(a5);  //delay of allpassfilter6 tap
		double d12 = outdelay12.audioprocessing(a5);  //delay of allpassfilter6 tap
		double d13 = outdelay13.audioprocessing(a6);  //delay of delayline4 tap
		double d14 = outdelay14.audioprocessing(a6);  //delay of delayline4 tap

		//add and subtract them, according to the Dattorro's report
		reverb_L = d1 + d2 - d8 - d4 - d10 + d6 - d12;  //summation of those delayed tap
		reverb_R = d14 + d13 - d7 - d11 - d5 + d9 - d3;
		double wet = (wetdry / 100);
		double dry = (1 - wetdry / 100);
		
		// --- pass through code: change this with your signal processing
		processFrameInfo.audioOutputFrame[0] = reverb_L*wet + outL * dry;
        processFrameInfo.audioOutputFrame[1] = reverb_R*wet + outL * dry;

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		outL = inL * gainlinDZ;   //dezip
		outR = inR * gainlinDZ;

		monoin = (outL + outR) * 0.5;   //chaging stereo into mono
		double pred = predelay.audioprocessing(monoin);
		double LPF1 = lpf1.audioprocessing(pred);  //lowpassfilter
		double APF1 = apf1.audioprocessing(LPF1); //allpassfilter1
		double APF2 = apf2.audioprocessing(APF1); //allpassfilter2
		double APF3 = apf3.audioprocessing(APF2); //allpassfilter3
		double APF4 = apf4.audioprocessing(APF3); //allpassfilter4
		decor = APF4; //input to the tank

		double leftTankin = rightTankout+ decor;
		double rightTankin = leftTankout + decor;

		double modAPF1 = mallp1.audioprocessing(leftTankin); //modulated allpaass filter
		double delayLine1 = delay1.audioprocessing(modAPF1);
		double leftLowpass = lpf2.audioprocessing(delayLine1); //low pass filter for hf damping
		double decayed1 = leftLowpass * DF; //decay factor multiplication
		double APF5 = apf5.audioprocessing(decayed1);
		double delayLine2 = delay2.audioprocessing(APF5);
		 leftTankout = delayLine2; //output of the tank, going back to the other side of the input

		double modAPF2 = mallp2.audioprocessing(rightTankin); 
		double delayLine3 = delay3.audioprocessing(modAPF2);
		double rightLowpass = lpf3.audioprocessing(delayLine3);
		double decayed2 = rightLowpass * DF; //decay factor multiplication
		double APF6 = apf6.audioprocessing(decayed2);
		double delayLine4 = delay4.audioprocessing(APF6);
		 rightTankout = delayLine4;


		double a1 = delay1.dtapout(); //tap out from the delayline1 in the tank
		double a2 = apf5.atapout();    //tap out from all pass filter.5 in the tank
		double a3 = delay2.dtapout();   // tap out from the delayline2 in the tank
		double a4 = delay3.dtapout();   //tap out from the delayline3 in the tank
		double a5 = apf6.atapout();     //tap out from all pass filter.6 in the tank
		double a6 = delay4.dtapout();   //tap out from the dleayline4 in the tank

		double d1 = outdelay1.audioprocessing(a1);    //delay of delayline1 tap     with different value of sample delay
		double d2 = outdelay2.audioprocessing(a1);    //delay of delayline1 tap 
		double d3 = outdelay3.audioprocessing(a1);    //delay of delayline1 tap 
		double d4 = outdelay4.audioprocessing(a2);    //delay of allpassfilter5 tap
		double d5 = outdelay5.audioprocessing(a2);    //delay of allpassfilter5 tap
		double d6 = outdelay6.audioprocessing(a3);    //delay of delayline2 tap 
		double d7 = outdelay7.audioprocessing(a3);    //delay of delayline2 tap 

		double d8 = outdelay8.audioprocessing(a4);    //delay of delayline3 tap
		double d9 = outdelay9.audioprocessing(a4);    //delay of delayline3 tap
		double d10 = outdelay10.audioprocessing(a4);  //delay of delayline3 tap
		double d11 = outdelay11.audioprocessing(a5);  //delay of allpassfilter6 tap
		double d12 = outdelay12.audioprocessing(a5);  //delay of allpassfilter6 tap
		double d13 = outdelay13.audioprocessing(a6);  //delay of delayline4 tap
		double d14 = outdelay14.audioprocessing(a6);  //delay of delayline4 tap

		reverb_L = d1 + d2 - d8 - d4 - d10 + d6 - d12;  //summation of those delayed tap
		reverb_R = d14 + d13 - d7 - d11 - d5 + d9 - d3;
		double wet = (wetdry / 100);
		double dry = (1 - wetdry / 100);

		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = reverb_L*wet + outL*dry;
        processFrameInfo.audioOutputFrame[1] = reverb_R*wet + outR*dry;

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
	switch (controlID)
	{
		

		case controlID::gain:
		{
			gainlin = pow(10, gain / 20);
			
		}
		case controlID::cutoff:
		{
			lpf1.setcutoffparams(cutoff);
			return true;
		}

		case controlID::predelaytime:
		{
			double fs = getSampleRate(); //getting samplerate for the sample conversion
			int delayinsample ;
			delayinsample = round(predelaytime * (fs / 1000)); // conversion from msec to sample
			predelay.setdelayparams(delayinsample);
			return true;
		}

		case controlID::diffusion:
		{	
			apf1.setgainparams(diffusion);      
			apf2.setgainparams(diffusion);    
			apf3.setgainparams(diffusion);
			apf4.setgainparams(diffusion);
			apf5.setgainparams(diffusion);
			apf6.setgainparams(diffusion);
			mallp1.setgainparams(diffusion);
			mallp2.setgainparams(diffusion);
			return true;
		}
		case controlID::decayfactor:
		{
			DF = decayfactor;
			return true;
		}
		case controlID::damping:
		{
			lpf2.setgainparams(damping);
			lpf3.setgainparams(damping);
			return true;
		}
	}
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.auBundleName = kAUBundleName;

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
