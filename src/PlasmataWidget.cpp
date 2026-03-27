#include "PlasmataWidget.hpp"
#include "Plasmata.hpp"
#include "../plugin.hpp"

VCONoiseDelayWidget::VCONoiseDelayWidget(VCONoiseDelay* module) {
	setModule(module);
	setPanel(createPanel(asset::plugin(pluginInstance, "res/VCONoiseDelay.svg")));

	addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

	addParam(createParamCentered<RoundHugeBlackKnob>(Vec(box.size.x / 2, mm2px(25) - 15), module, VCONoiseDelay::FREQ_PARAM));

	K2DLabel* freqLabel = createWidget<K2DLabel>(Vec(0, mm2px(31)));
	freqLabel->box.size = Vec(box.size.x, 28);
	freqLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	freqLabel->text = "FREQUENCY";
	freqLabel->fontSize = 15;
	freqLabel->alignment = NVG_ALIGN_CENTER | NVG_ALIGN_TOP;
	freqLabel->color = nvgRGB(0, 0, 0);
	addChild(freqLabel);

	K2DLabel* noiseLabel = createWidget<K2DLabel>(Vec(11, mm2px(42) - 8));
	noiseLabel->box.size = Vec(box.size.x, 28);
	noiseLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	noiseLabel->text = "NOISE";
	noiseLabel->fontSize = 15;
	noiseLabel->alignment = NVG_ALIGN_CENTER | NVG_ALIGN_TOP;
	noiseLabel->color = nvgRGB(0, 0, 0);
	addChild(noiseLabel);

	addParam(createParamCentered<VCVLatch>(mm2px(Vec(9.45, 50)) + Vec(0, 15), module, VCONoiseDelay::NOISE_ENABLE_PARAM));
	
	NoiseTypeDisplay* noiseDisplay = new NoiseTypeDisplay;
	noiseDisplay->module = module;
	noiseDisplay->box.pos = mm2px(Vec(18.9 - 12.5, 50 - 4)) + Vec(5, 10);
	addChild(noiseDisplay);
	
	NoiseTypeUpButton* upButton = createParamCentered<NoiseTypeUpButton>(mm2px(Vec(28.35, 47)) + Vec(25, 10), module, VCONoiseDelay::NOISE_UP_PARAM);
	upButton->module = module;
	addParam(upButton);
	
	NoiseTypeDownButton* downButton = createParamCentered<NoiseTypeDownButton>(mm2px(Vec(28.35, 54)) + Vec(25, 10), module, VCONoiseDelay::NOISE_DOWN_PARAM);
	downButton->module = module;
	addParam(downButton);

	K2DLabel* delayLabel = createWidget<K2DLabel>(Vec(0, mm2px(68) - 5));
	delayLabel->box.size = Vec(box.size.x, 28);
	delayLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	delayLabel->text = "DELAY";
	delayLabel->fontSize = 15;
	delayLabel->alignment = NVG_ALIGN_CENTER | NVG_ALIGN_TOP;
	delayLabel->color = nvgRGB(0, 0, 0);
	addChild(delayLabel);

	Vec delaySwitchPos = mm2px(Vec(9.45, 75)) + Vec(-10, 40);  // left 10px, down 30px (was 10, now +30 more)
	addChild(createLightCentered<SmallSimpleLight<DelayLight>>(delaySwitchPos + Vec(0, -20), module, VCONoiseDelay::DELAY_LED));
	addParam(createParamCentered<CKSS>(delaySwitchPos, module, VCONoiseDelay::DELAY_ENABLE_PARAM));
	
	float timeKnobX = mm2px(20);
	float timeKnobY = mm2px(80);
	addParam(createParamCentered<RoundBigBlackKnob>(Vec(timeKnobX, timeKnobY), module, VCONoiseDelay::DELAY_TIME_PARAM));
	K2DLabel* timeLabel = createWidget<K2DLabel>(Vec(timeKnobX - 13, timeKnobY + 21));
	timeLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	timeLabel->text = "time";
	timeLabel->fontSize = 15;
	timeLabel->color = nvgRGB(0, 0, 0);
	addChild(timeLabel);
	
	float fbKnobX = box.size.x - 35;
	float fbKnobY = mm2px(80);
	addParam(createParamCentered<RoundBigBlackKnob>(Vec(fbKnobX, fbKnobY), module, VCONoiseDelay::DELAY_FEEDBACK_PARAM));
	K2DLabel* fbLabel = createWidget<K2DLabel>(Vec(fbKnobX - 25, fbKnobY + 21));
	fbLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	fbLabel->text = "feedback";
	fbLabel->fontSize = 15;
	fbLabel->color = nvgRGB(0, 0, 0);
	addChild(fbLabel);
	
	// Delay input: trig only (reset removed)
	float inputY = timeKnobY + 48;
	addInput(createInputCentered<PJ301MPort>(Vec(fbKnobX - 34, inputY), module, VCONoiseDelay::TRIG_INPUT));
	K2DLabel* trigLabel = createWidget<K2DLabel>(Vec(fbKnobX - 45, inputY + 13));
	trigLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	trigLabel->text = "trig";
	trigLabel->fontSize = 15;
	trigLabel->color = nvgRGB(0, 0, 0);
	addChild(trigLabel);

	// Wave outputs: 21px from bottom, first at 35px from left, 18px between each
	float outY = box.size.y - 21 - mm2px(3.5);  // moved up another 3px
	float outX = 20;
	K2DLabel* sinLabel = createWidget<K2DLabel>(Vec(outX - 7, outY - 29));
	sinLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	sinLabel->text = "sin";
	sinLabel->fontSize = 15;
	sinLabel->color = nvgRGB(0, 0, 0);
	addChild(sinLabel);
	addOutput(createOutputCentered<PJ301MPort>(Vec(outX, outY), module, VCONoiseDelay::SIN_OUTPUT));
	outX += 34;
	K2DLabel* triLabel = createWidget<K2DLabel>(Vec(outX - 7, outY - 29));
	triLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	triLabel->text = "tri";
	triLabel->fontSize = 15;
	triLabel->color = nvgRGB(0, 0, 0);
	addChild(triLabel);
	addOutput(createOutputCentered<PJ301MPort>(Vec(outX, outY), module, VCONoiseDelay::TRI_OUTPUT));
	outX += 34;
	K2DLabel* sawLabel = createWidget<K2DLabel>(Vec(outX - 9, outY - 29));
	sawLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	sawLabel->text = "saw";
	sawLabel->fontSize = 15;
	sawLabel->color = nvgRGB(0, 0, 0);
	addChild(sawLabel);
	addOutput(createOutputCentered<PJ301MPort>(Vec(outX, outY), module, VCONoiseDelay::SAW_OUTPUT));
	outX += 34;
	K2DLabel* sqrLabel = createWidget<K2DLabel>(Vec(outX - 9, outY - 29));
	sqrLabel->font = APP->window->loadFont(asset::plugin(pluginInstance, "res/K2D-Regular.ttf"));
	sqrLabel->text = "sqr";
	sqrLabel->fontSize = 15;
	sqrLabel->color = nvgRGB(0, 0, 0);
	addChild(sqrLabel);
	addOutput(createOutputCentered<PJ301MPort>(Vec(outX, outY), module, VCONoiseDelay::SQR_OUTPUT));
}

Model* modelPlasmata = createModel<VCONoiseDelay, VCONoiseDelayWidget>("VCONoiseDelay");

