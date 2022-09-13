#ifndef ANIMATION_COMPONENT
#define ANIMATION_COMPONENT

#include <CryEntitySystem/IEntitySystem.h>

namespace Cry::DefaultComponents
{
	class CAdvancedAnimationComponent;
}

class CAnimationComponent final : public IEntityComponent
{
	using AdvancedAnimationComponent = Cry::DefaultComponents::CAdvancedAnimationComponent;

	struct SDefaults
	{
		inline static constexpr auto rotation_speed = 7.35f;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CAnimationComponent>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

private:
	float rotation_speed_{ SDefaults::rotation_speed };

	AdvancedAnimationComponent* advanced_animation_component_{};

	uint32 active_fragment_id_{};

	uint32 idle_fragment_id_{};
	uint32 walk_fragment_id_{};
	uint32 jump_fragment_id_{};
	uint32 fall_fragment_id_{};
	uint32 land_fragment_id_{};
};

inline void CAnimationComponent::ReflectType(Schematyc::CTypeDesc<CAnimationComponent>& desc)
{
	desc.SetGUID("{495004BE-A288-4057-A01E-42030339A477}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("animationcomponent"));
	desc.SetLabel("Animation Component");
	desc.SetDescription("Control animations blends and how an animation is showing");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CAnimationComponent::rotation_speed_
		, 'anrs'
		, "rotationspeed"
		, "Rotation Speed"
		, "Control how fast the mesh can rotation"
		, SDefaults::rotation_speed);
}

#endif // !ANIMATION_COMPONENT