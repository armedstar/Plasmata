#pragma once
#include "Plasmata.hpp"
#include "rack.hpp"
#include "../plugin.hpp"

using namespace rack;

/** Panel drawn in code (no SVG file) so it always shows the gradient design. */
struct GradientPanel : widget::Widget {
	GradientPanel() {
		box.size = Vec(150, 380); // 10HP
	}
	void draw(const DrawArgs& args) override {
		float w = box.size.x;
		float h = box.size.y;
		// Dark base
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 0, 0, w, h);
		nvgFillColor(args.vg, nvgRGB(0x1a, 0x1a, 0x1a));
		nvgFill(args.vg);
		// Top band: pink gradient (light top -> dark bottom)
		NVGpaint pink = nvgLinearGradient(args.vg, w/2, 0, w/2, 114,
			nvgRGB(0xff, 0xd3, 0xee), nvgRGB(0xae, 0x2d, 0x7e));
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 1.7f, 0.5f, w - 3.4f, 113.7f);
		nvgFillPaint(args.vg, pink);
		nvgFill(args.vg);
		// Second: gray
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 1.7f, 114.2f, w - 3.4f, 75.2f);
		nvgFillColor(args.vg, nvgRGB(0xc9, 0xc9, 0xe9));
		nvgFill(args.vg);
		// Third: yellow/gold gradient
		NVGpaint yellow = nvgLinearGradient(args.vg, w/2, 189, w/2, 314,
			nvgRGB(0xf3, 0xed, 0x87), nvgRGB(0x81, 0x7e, 0x40));
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 1.7f, 189.4f, w - 3.4f, 124.4f);
		nvgFillPaint(args.vg, yellow);
		nvgFill(args.vg);
		// Bottom: cyan
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 1.7f, 313.8f, w - 3.4f, 66.7f);
		nvgFillColor(args.vg, nvgRGB(0x8f, 0xf5, 0xf1));
		nvgFill(args.vg);
	}
};

struct PinkLight : GrayModuleLightWidget {
	PinkLight() {
		this->addBaseColor(nvgRGB(255, 105, 180));
	}
};

struct DelayLight : GrayModuleLightWidget {
	DelayLight() {
		this->addBaseColor(nvgRGB(0x8f, 0xf5, 0xf1));
	}
};

/** Label that uses the K2D font from res/K2D-Regular.ttf */
struct K2DLabel : widget::Widget {
	std::shared_ptr<rack::window::Font> font;
	std::string text;
	float fontSize = 15;
	NVGcolor color = nvgRGB(0, 0, 0);
	int alignment = NVG_ALIGN_LEFT | NVG_ALIGN_TOP;

	K2DLabel() {
		box.size = Vec(80, 12);
	}

	void draw(const DrawArgs& args) override {
		if (!font || font->handle < 0) return;
		nvgFontSize(args.vg, fontSize);
		nvgFontFaceId(args.vg, font->handle);
		nvgTextAlign(args.vg, alignment);
		nvgFillColor(args.vg, color);
		float x = (alignment & NVG_ALIGN_CENTER) ? box.size.x / 2 : 0;
		nvgText(args.vg, x, 0, text.c_str(), NULL);
	}
};

struct NoiseTypeDisplay : widget::OpaqueWidget {
	VCONoiseDelay* module;
	std::string text = "WHITE";
	
	NoiseTypeDisplay() {
		box.size = mm2px(Vec(25, 8));
	}
	
	void step() override {
		if (module) {
			int noiseType = (int)std::round(module->params[VCONoiseDelay::NOISE_TYPE_PARAM].getValue());
			if (noiseType < 0) noiseType = 0;
			if (noiseType > 4) noiseType = 4;
			switch (noiseType) {
				case 0: text = "OFF"; break;
				case 1: text = "WHITE"; break;
				case 2: text = "PINK"; break;
				case 3: text = "RED"; break;
				case 4: text = "BLACK"; break;
				default: text = "OFF"; break;
			}
		}
		OpaqueWidget::step();
	}
	
	void draw(const DrawArgs& args) override {
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 2);
		nvgFillColor(args.vg, nvgRGB(0, 0, 0));
		nvgFill(args.vg);
		nvgStrokeWidth(args.vg, 0.5);
		nvgStrokeColor(args.vg, nvgRGB(20, 20, 20));
		nvgStroke(args.vg);
		nvgFontSize(args.vg, 15);
		nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFillColor(args.vg, nvgRGB(255, 255, 255));
		nvgText(args.vg, box.size.x / 2, box.size.y / 2, text.c_str(), NULL);
	}
};

struct NoiseTypeUpButton : app::SvgSwitch {
	VCONoiseDelay* module;

	NoiseTypeUpButton() {
		momentary = true;
		addFrame(Svg::load(asset::plugin(pluginInstance, "res/NoiseUp_0.svg")));  // frame 0 = default (gray/white)
		addFrame(Svg::load(asset::plugin(pluginInstance, "res/NoiseUp_1.svg")));  // frame 1 = pressed (pink)
	}

	void onButton(const ButtonEvent& e) override {
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && module) {
			float v = module->params[VCONoiseDelay::NOISE_TYPE_PARAM].getValue();
			module->params[VCONoiseDelay::NOISE_TYPE_PARAM].setValue(std::min(4.f, v + 1.f));
		}
		app::SvgSwitch::onButton(e);
	}
};

struct NoiseTypeDownButton : app::SvgSwitch {
	VCONoiseDelay* module;

	NoiseTypeDownButton() {
		momentary = true;
		addFrame(Svg::load(asset::plugin(pluginInstance, "res/NoiseDown_0.svg")));  // frame 0 = default (gray/white)
		addFrame(Svg::load(asset::plugin(pluginInstance, "res/NoiseDown_1.svg")));  // frame 1 = pressed (pink)
	}

	void onButton(const ButtonEvent& e) override {
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && module) {
			float v = module->params[VCONoiseDelay::NOISE_TYPE_PARAM].getValue();
			module->params[VCONoiseDelay::NOISE_TYPE_PARAM].setValue(std::max(0.f, v - 1.f));
		}
		app::SvgSwitch::onButton(e);
	}
};

struct VCONoiseDelayWidget : ModuleWidget {
	VCONoiseDelayWidget(VCONoiseDelay* module);
};

