#pragma once
#include "rack.hpp"

using namespace rack;

// Forward declaration
struct VCONoiseDelayWidget;

// NOTE: Struct name kept as VCONoiseDelay so the module slug and patch
// compatibility remain unchanged. This file is renamed for project hygiene.
struct VCONoiseDelay : Module {
	enum ParamId {
		FREQ_PARAM,
		NOISE_ENABLE_PARAM,
		NOISE_TYPE_PARAM,
		NOISE_UP_PARAM,
		NOISE_DOWN_PARAM,
		DELAY_ENABLE_PARAM,
		DELAY_TIME_PARAM,
		DELAY_FEEDBACK_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		RESET_INPUT,
		TRIG_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SIN_OUTPUT,
		TRI_OUTPUT,
		SAW_OUTPUT,
		SQR_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		DELAY_LED,
		LIGHTS_LEN
	};

	VCONoiseDelay();
	void process(const ProcessArgs& args) override;

private:
	// VCO state
	float phase = 0.f;
	
	// Noise state
	enum NoiseType {
		NOISE_WHITE,
		NOISE_PINK,
		NOISE_BROWN,
		NOISE_BLACK
	};
	
	// Pink noise filter state
	float pinkB0 = 0.f, pinkB1 = 0.f, pinkB2 = 0.f, pinkB3 = 0.f, pinkB4 = 0.f, pinkB5 = 0.f, pinkB6 = 0.f;
	
	// Delay buffers (one per output to avoid interference)
	std::vector<float> delayBufferSin;
	std::vector<float> delayBufferTri;
	std::vector<float> delayBufferSaw;
	std::vector<float> delayBufferSqr;
	int delayBufferIndexSin = 0;
	int delayBufferIndexTri = 0;
	int delayBufferIndexSaw = 0;
	int delayBufferIndexSqr = 0;
	
	float prevResetVoltage = 0.f;
	float prevTrigVoltage = 0.f;
	
	// Helper functions
	float generateSine(float freq);
	float generateTriangle(float freq);
	float generateSawtooth(float freq);
	float generateSquare(float freq);
	float generateNoise(NoiseType type);
	float processDelay(float input, float time, float feedback, bool enabled, std::vector<float>& buffer, int& bufferIndex);
};

