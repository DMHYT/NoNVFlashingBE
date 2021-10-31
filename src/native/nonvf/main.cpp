#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <nativejs.h>
#include "recovered.hpp"


class NoNVFModule : public Module {
public:
	NoNVFModule(const char* id): Module(id) {};

	virtual void initialize() {
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		HookManager::addCallback(SYMBOL("mcpe", "_ZN19LevelRendererPlayer19getNightVisionScaleERK3Mobf"), LAMBDA((HookManager::CallbackController* controller), {
			if(!fadeOut) {
				controller->replace();
				return maxBrightness;
			}
			if(NIGHT_VISION == nullptr) {
				MobEffect* effectType = MobEffect::getById(16);
				if(effectType) {
					NIGHT_VISION = effectType;
				}
			}
			if(NIGHT_VISION != nullptr) {
				MobEffectInstance* effect = GlobalContext::getLocalPlayer()->getEffect(*NIGHT_VISION);
				if(effect != nullptr) {
					controller->replace();
					int duration = effect->getDuration();
					return duration > fadeTicks ? maxBrightness : duration * fadeRate;
				}
			}
		}, ), HookManager::CALL | HookManager::LISTENER | HookManager::CONTROLLER | HookManager::RESULT);
    }
	static float maxBrightness;
    static bool fadeOut;
    static int fadeTicks;
    static float fadeRate;
	static MobEffect* NIGHT_VISION;
};
float NoNVFModule::maxBrightness = 1.0;
bool NoNVFModule::fadeOut = true;
int NoNVFModule::fadeTicks = 20;
float NoNVFModule::fadeRate = NoNVFModule::maxBrightness / NoNVFModule::fadeTicks;
MobEffect* NoNVFModule::NIGHT_VISION = nullptr;


MAIN {
	Module* main_module = new NoNVFModule("nonvf");
}


JS_MODULE_VERSION(NoNVF, 1);
JS_EXPORT(NoNVF, setMaxBrightness, "V(F)", (JNIEnv* env, float brightness) {
	NoNVFModule::maxBrightness = brightness;
	NoNVFModule::fadeRate = NoNVFModule::maxBrightness / NoNVFModule::fadeTicks;
	return 0;
});
JS_EXPORT(NoNVF, setFadeOutEnabled, "V(B)", (JNIEnv* env, bool enabled) {
	NoNVFModule::fadeOut = enabled;
	return 0;
});
JS_EXPORT(NoNVF, setFadeTicks, "V(I)", (JNIEnv* env, int ticks) {
	NoNVFModule::fadeTicks = ticks;
	NoNVFModule::fadeRate = NoNVFModule::maxBrightness / NoNVFModule::fadeTicks;
	return 0;
});