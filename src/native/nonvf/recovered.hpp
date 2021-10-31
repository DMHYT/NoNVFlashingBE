#ifndef NONVF_RECOVERED_HPP
#define NONVF_RECOVERED_HPP


class MobEffect {
	public:
	static MobEffect* getById(int);
};

class MobEffectInstance {
	public:
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


#endif //NONVF_RECOVERED_HPP