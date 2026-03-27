#include "Plasmata.hpp"
#include <random>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

VCONoiseDelay::VCONoiseDelay() {
	config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
	configParam(FREQ_PARAM, -2.f, 4.f, 0.f, "Frequency", " oct");
	// Noise enable: use configSwitch so it's discrete (no smoothing), default 1 = on
	configSwitch(NOISE_ENABLE_PARAM, 0.f, 1.f, 1.f, "Noise Enable", {"Off", "On"});
	configSwitch(NOISE_TYPE_PARAM, 0.f, 4.f, 0.f, "Noise Type", {"Off", "White", "Pink", "Red", "Black"});
	configButton(NOISE_UP_PARAM, "Noise selection");
	configButton(NOISE_DOWN_PARAM, "Noise selection");
	configParam(DELAY_ENABLE_PARAM, 0.f, 1.f, 0.f, "Delay Enable");
	configParam(DELAY_TIME_PARAM, 0.f, 2.f, 0.5f, "Delay Time", " s");
	configParam(DELAY_FEEDBACK_PARAM, 0.f, 0.98f, 0.6f, "Delay Feedback");
	
	configLight(DELAY_LED, "Delay on");
	configInput(RESET_INPUT, "Reset");
	configInput(TRIG_INPUT, "Trig");
	configOutput(SIN_OUTPUT, "Sine");
	configOutput(TRI_OUTPUT, "Triangle");
	configOutput(SAW_OUTPUT, "Sawtooth");
	configOutput(SQR_OUTPUT, "Square");
	
	// Initialize delay buffers (max 3 seconds at 48kHz = 144000 samples)
	int bufferSize = 144000;
	delayBufferSin.resize(bufferSize, 0.f);
	delayBufferTri.resize(bufferSize, 0.f);
	delayBufferSaw.resize(bufferSize, 0.f);
	delayBufferSqr.resize(bufferSize, 0.f);
}

float VCONoiseDelay::generateSine(float freq) {
	return std::sin(2.f * M_PI * phase);
}

float VCONoiseDelay::generateTriangle(float freq) {
	if (phase < 0.5f) {
		return 4.f * phase - 1.f;
	} else {
		return 3.f - 4.f * phase;
	}
}

float VCONoiseDelay::generateSawtooth(float freq) {
	return 2.f * phase - 1.f;
}

float VCONoiseDelay::generateSquare(float freq) {
	return (phase < 0.5f) ? 1.f : -1.f;
}

float VCONoiseDelay::generateNoise(NoiseType type) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.f, 1.f);
	
	float white = dis(gen);
	
	switch (type) {
		case NOISE_WHITE:
			return white;
			
		case NOISE_PINK: {
			pinkB0 = 0.99886f * pinkB0 + white * 0.0555179f;
			pinkB1 = 0.99332f * pinkB1 + white * 0.0750759f;
			pinkB2 = 0.96900f * pinkB2 + white * 0.1538520f;
			pinkB3 = 0.86650f * pinkB3 + white * 0.3104856f;
			pinkB4 = 0.55000f * pinkB4 + white * 0.5329522f;
			pinkB5 = -0.7616f * pinkB5 - white * 0.0168980f;
			float pink = pinkB0 + pinkB1 + pinkB2 + pinkB3 + pinkB4 + pinkB5 + pinkB6 + white * 0.5362f;
			pinkB6 = white * 0.115926f;
			return pink * 0.11f;
		}
		
		case NOISE_BROWN: {
			// Red noise - very strong low-frequency emphasis
			static float redState1 = 0.f;
			static float redState2 = 0.f;
			redState1 = redState1 * 0.998f + white * 0.002f;
			redState2 = redState2 * 0.999f + redState1 * 0.001f;
			return (redState1 + redState2 * 0.5f) * 5.0f;
		}
		
		case NOISE_BLACK: {
			static float blackState = 0.f;
			blackState = blackState * 0.95f + white * 0.05f;
			return (white - blackState) * 2.f;
		}
		
		default:
			return white;
	}
}

float VCONoiseDelay::processDelay(float input, float time, float feedback, bool enabled, std::vector<float>& buffer, int& bufferIndex) {
	if (!enabled || time < 0.001f) {
		return input;
	}
	
	float delaySamples = time * APP->engine->getSampleRate();
	int delayInt = (int)delaySamples;
	
	if (delayInt >= (int)buffer.size()) delayInt = buffer.size() - 1;
	if (delayInt < 1) delayInt = 1;
	
	int readIndex = (bufferIndex - delayInt + buffer.size()) % buffer.size();
	float delayed = buffer[readIndex];
	
	buffer[bufferIndex] = input + delayed * feedback;
	bufferIndex = (bufferIndex + 1) % buffer.size();
	
	return input * 0.5f + delayed * 0.5f;
}

void VCONoiseDelay::process(const ProcessArgs& args) {
	float freq = params[FREQ_PARAM].getValue();
	int noiseType = (int)std::round(params[NOISE_TYPE_PARAM].getValue());
	noiseType = std::max(0, std::min(4, noiseType));
	float noiseEnableVal = params[NOISE_ENABLE_PARAM].getValue();
	bool delayEnabled = params[DELAY_ENABLE_PARAM].getValue() > 0.5f;
	float delayTime = params[DELAY_TIME_PARAM].getValue();
	float delayFeedback = params[DELAY_FEEDBACK_PARAM].getValue();
	
	// Noise modulates V/Oct. Applied whenever type is not Off. Enable button scales it (0=off, 1=on).
	float noiseMod = 0.f;
	if (noiseType > 0) {
		noiseMod = generateNoise((NoiseType)(noiseType - 1)) * 3.f * (noiseEnableVal > 0.5f ? 1.f : 0.f);
	}
	
	float modulatedFreq = freq + noiseMod;
	float frequency = 261.626f * std::pow(2.f, modulatedFreq);
	float sampleRate = (args.sampleRate > 0.f) ? args.sampleRate : 44100.f;
	
	// Reset: clear delay buffers on rising edge
	float resetVoltage = inputs[RESET_INPUT].getVoltage();
	if (resetVoltage >= 1.f && prevResetVoltage < 1.f) {
		std::fill(delayBufferSin.begin(), delayBufferSin.end(), 0.f);
		std::fill(delayBufferTri.begin(), delayBufferTri.end(), 0.f);
		std::fill(delayBufferSaw.begin(), delayBufferSaw.end(), 0.f);
		std::fill(delayBufferSqr.begin(), delayBufferSqr.end(), 0.f);
		delayBufferIndexSin = delayBufferIndexTri = delayBufferIndexSaw = delayBufferIndexSqr = 0;
	}
	prevResetVoltage = resetVoltage;
	
	// Trig: retrigger delay on rising edge (reset write position for fresh playback)
	float trigVoltage = inputs[TRIG_INPUT].getVoltage();
	if (trigVoltage >= 1.f && prevTrigVoltage < 1.f) {
		delayBufferIndexSin = delayBufferIndexTri = delayBufferIndexSaw = delayBufferIndexSqr = 0;
	}
	prevTrigVoltage = trigVoltage;
	
	// Update phase (use args.sampleRate)
	phase += frequency / sampleRate;
	if (phase >= 1.f) phase -= 1.f;
	
	float sine = generateSine(modulatedFreq);
	float triangle = generateTriangle(modulatedFreq);
	float sawtooth = generateSawtooth(modulatedFreq);
	float square = generateSquare(modulatedFreq);
	
	float sinOut = processDelay(sine, delayTime, delayFeedback, delayEnabled, delayBufferSin, delayBufferIndexSin);
	float triOut = processDelay(triangle, delayTime, delayFeedback, delayEnabled, delayBufferTri, delayBufferIndexTri);
	float sawOut = processDelay(sawtooth, delayTime, delayFeedback, delayEnabled, delayBufferSaw, delayBufferIndexSaw);
	float sqrOut = processDelay(square, delayTime, delayFeedback, delayEnabled, delayBufferSqr, delayBufferIndexSqr);
	
	lights[DELAY_LED].setBrightness(delayEnabled ? 1.f : 0.f);
	outputs[SIN_OUTPUT].setVoltage(sinOut * 5.f);
	outputs[TRI_OUTPUT].setVoltage(triOut * 5.f);
	outputs[SAW_OUTPUT].setVoltage(sawOut * 5.f);
	outputs[SQR_OUTPUT].setVoltage(sqrOut * 5.f);
}

