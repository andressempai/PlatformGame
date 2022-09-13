#ifndef PATROL_COMPONENT
#define PATROL_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

class CPatrolComponent final : public IEntityComponent
{
	struct SDefaults
	{
		inline static constexpr float patrol_speed{ 1.0f };
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CPatrolComponent>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void point_to_first_entity_link();

	float patrol_speed_{ SDefaults::patrol_speed };

	uint8 current_way_point_index_{ 0 };
	uint8 next_way_point_index_{ 1 };

	struct IEntityLink* entity_link_{};
};

inline void CPatrolComponent::ReflectType(Schematyc::CTypeDesc<CPatrolComponent>& desc)
{
	desc.SetGUID("{969530B2-3875-4B02-A054-EFFD28BF0C9D}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName{ "patrolcomponent" });
	desc.SetLabel("Patrol Component");
	desc.SetDescription("Patrol Behavior");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CPatrolComponent::patrol_speed_
		, 'pcps'
		, "patrolspeed"
		, "Patrol Speed"
		, "How strong the entity can move when he is patrolling"
		, SDefaults::patrol_speed);
}

#endif // !PATROL_COMPONENT