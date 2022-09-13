#ifndef ENEMY
#define ENEMY

#include "../../Interfaces/IDamageable.h"

#include <CrySchematyc/MathTypes.h>

class CPhysicComponent;
class CAnimationComponent;

class CEnemy final : public IDamageable
{
	struct SDefaults
	{
		inline static constexpr auto explode_impulse = 100.0f;
		inline static constexpr auto can_be_affected_by_gravity = true;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CEnemy>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void take_damage(const EntityId entity_id) override;

	Schematyc::PositiveFloat explode_impulse_{ SDefaults::explode_impulse };
	bool can_be_affected_by_gravity_{ SDefaults::can_be_affected_by_gravity };
};

inline void CEnemy::ReflectType(Schematyc::CTypeDesc<CEnemy>& desc)
{
	desc.AddBase<IDamageable>();
	desc.SetGUID("{F537970B-41D6-424C-899F-E9E30EF8FA93}"_cry_guid);
	desc.SetEditorCategory("Game Entities");
	desc.SetName(Schematyc::CTypeName{ "Enemy" });
	desc.SetDescription("Represent entity Enemy");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CEnemy::explode_impulse_
		, 'eeim'
		, "explodeimpulse"
		, "Explode Impulse"
		, "Impulse applied to entity that damage this enemy"
		, SDefaults::explode_impulse);

	desc.AddMember(
		  &CEnemy::can_be_affected_by_gravity_
		, 'ecag'
		, "canbeaffectedbygravity"
		, "Can Be Affected by Gravity"
		, "Determinate if Entity is affected by Gravity"
		, SDefaults::can_be_affected_by_gravity);
}

#endif // !ENEMY