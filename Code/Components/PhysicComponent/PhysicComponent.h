#ifndef PHYSIC_COMPONENT
#define PHYSIC_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

#include "../../Utils/PlayerDimensions.h"
#include "../../Utils/PlayerDynamics.h"

class CPhysicComponent final : public IEntityComponent
#ifndef RELEASE
	, IEntityComponentPreviewer
#endif // !RELEASE
{
public:
	static void ReflectType(Schematyc::CTypeDesc<CPhysicComponent>& desc);

private:
	void Initialize() override;
	void OnShutDown() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE // ENTITY COMPONENT PREVIEWER INTERFACE
	IEntityComponentPreviewer* GetPreviewer() override { return this; }
	void SerializeProperties(Serialization::IArchive& archive) override {}
	void Render(
		  const IEntity& entity
		, const IEntityComponent& component
		, SEntityPreviewContext& context) const override;
#endif // !RELEASE

public:
	void add_linear_velocity(const Vec3& direction) const noexcept;

	[[nodiscard]]
	Vec3 get_velocity() const noexcept;

private:
	void physicalize();

private:
	SPlayerDimensions player_dimensions_{};
	SPlayerDynamics player_dynamics_{};

	int entity_slot_{ EmptySlotId };

	IPhysicalEntity* physical_entity_{};
};

inline void CPhysicComponent::ReflectType(Schematyc::CTypeDesc<CPhysicComponent>& desc)
{
	desc.SetGUID("{61C3AAD3-A68E-4119-8963-7109BEB1202A}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("physiccomponent"));
	desc.SetLabel("Physic Component");
	desc.SetDescription("Physic Component for any Living Entity");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
		  &CPhysicComponent::player_dimensions_
		, 'ppdm'
		, "playerdimensions"
		, "Player Dimensions"
		, "Control how is the Physics form of the Player"
		, SPlayerDimensions{});

	desc.AddMember(
		  &CPhysicComponent::player_dynamics_
		, 'ppdy'
		, "playerdynamics"
		, "Player Dynamics"
		, "Control dynamics behavior of the player"
		, SPlayerDynamics{});
}

#endif // !PHYSIC_COMPONENT