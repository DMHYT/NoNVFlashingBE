#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <nativejs.h>
#include "config.h"

class LevelRendererPlayer;
class MobEffect {
	public:
	static const MobEffect NIGHT_VISION;
};
class MobEffectInstance {
	public:
	int getDuration() const;
};
class Actor {
	public:
	MobEffectInstance* getEffect(MobEffect const&) const;
};
class Mob : public Actor {
	public:
};

class MainModule : public Module {
public:
	MainModule(const char* id): Module(id) {};

	virtual void initialize() {
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		HookManager::addCallback(SYMBOL("mcpe", "_ZN19LevelRendererPlayer19getNightVisionScaleERK3Mobf"), LAMBDA((HookManager::CallbackController* controller, LevelRendererPlayer* lrp, Mob const& mob, float partialTicks), {
			controller->replace();
			if(!NoNVFConfig::fadeOut) return NoNVFConfig::maxBrightness;
			int i = mob.getEffect(MobEffect::NIGHT_VISION)->getDuration();
			return i > NoNVFConfig::fadeTicks ? NoNVFConfig::maxBrightness : i * NoNVFConfig::fadeRate;
		}, ), HookManager::CALL | HookManager::LISTENER | HookManager::CONTROLLER | HookManager::RESULT);
    }
};

MAIN {
	Module* main_module = new MainModule("nonvf");
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