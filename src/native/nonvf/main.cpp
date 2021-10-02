#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <nativejs.h>
#include "config.h"
#include <typeinfo>

class MobEffect {
	public:
	static const MobEffect NIGHT_VISION;
};
class MobEffectInstance {
	public:
	bool isNoCounter() const;
	int getDuration() const;
};
class Actor {
	public:
	MobEffectInstance* getEffect(MobEffect const&) const;
};
class Mob : public Actor {public:};
class Player : public Mob {public:};
class LocalPlayer : public Player {public:};

namespace GlobalContext {
	LocalPlayer* getLocalPlayer();
}

template<typename smth>
void logType(const char* what, smth something) {
	std::string typeinfo = typeid(something).name();
	const char* typeinfo_c_str = typeinfo.c_str();
	Logger::debug("TYPELOGGING", "%s is %s", what, typeinfo_c_str);
	Logger::flush();
}

class MainModule : public Module {
public:
	MainModule(const char* id): Module(id) {};

	virtual void initialize() {
		logType<MobEffect>("MobEffect::NIGHT_VISION", MobEffect::NIGHT_VISION);
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		HookManager::addCallback(
		SYMBOL("mcpe", "_ZN19LevelRendererPlayer19getNightVisionScaleERK3Mobf"), 
		LAMBDA((HookManager::CallbackController* controller), {
			controller->replace();
			if(!NoNVFConfig::fadeOut) return NoNVFConfig::maxBrightness;
			MobEffectInstance* effect = GlobalContext::getLocalPlayer()->getEffect(MobEffect::NIGHT_VISION);
			if(effect == nullptr) {
				// Logger::debug("NONVF", "Night vision MobEffectInstance is nullptr!");
				// Logger::flush();
				return 0.0f;
			}
			if(effect->isNoCounter()) {
				return NoNVFConfig::maxBrightness;
			} else {
				int duration = effect->getDuration();
				return duration > NoNVFConfig::fadeTicks ? NoNVFConfig::maxBrightness : duration * NoNVFConfig::fadeRate;
			}
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