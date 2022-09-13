#ifndef ROTATION_COMPONENT
#define ROTATION_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

class CRotationComponent final : public IEntityComponent
{
	struct SDefaults
	{
		inline static constexpr auto rotation_constants{ 45.0f };
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CRotationComponent>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	float rotation_speed_{ SDefaults::rotation_constants };
};

inline void CRotationComponent::ReflectType(Schematyc::CTypeDesc<CRotationComponent>& desc)
{
	desc.SetGUID("{93760F05-6EA0-4B8A-BB76-EF18162C9539}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("rotationcomponent"));
	desc.SetLabel("Rotation Component");
	desc.SetDescription("Apply Rotation animation to Entity");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CRotationComponent::rotation_speed_
		, 'rcrs'
		, "rotationspeed"
		, "Rotation Speed"
		, "Rotation Speed"
		, SDefaults::rotation_constants);
}

#endif // !ROTATION_COMPONENT