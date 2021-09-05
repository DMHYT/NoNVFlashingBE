#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <nativejs.h>
#include "config.h"

class MainModule : public Module {
public:
	MainModule(const char* id): Module(id) {};

	virtual void initialize() {
		
    }
};

MAIN {
	Module* main_module = new MainModule("sample_library");
}

JS_MODULE_VERSION(NoNVFConfigJS, 1);
JS_EXPORT(NoNVFConfigJS, setMaxBrightness, "V(F)", (JNIEnv* env, float brightness) {
	NoNVFConfig::maxBrightness = brightness;
	NoNVFConfig::fadeRate = NoNVFConfig::maxBrightness / NoNVFConfig::fadeTicks;
	return 0;
});
JS_EXPORT(NoNVFConfigJS, setFadeOutEnabled, "V(B)", (JNIEnv* env, bool enabled) {
	NoNVFConfig::fadeOut = enabled;
	return 0;
});
JS_EXPORT(NoNVFConfigJS, setFadeTicks, "V(I)", (JNIEnv* env, int ticks) {
	NoNVFConfig::fadeTicks = ticks;
	NoNVFConfig::fadeRate = NoNVFConfig::maxBrightness / NoNVFConfig::fadeTicks;
	return 0;
});