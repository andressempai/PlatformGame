#ifndef FOLLOW_COMPONENT
#define FOLLOW_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

#include "../../Utils/CameraManConfiguration.h"

class CFollowComponent final : public IEntityComponent
{
public:
	static void ReflectType(Schematyc::CTypeDesc<CFollowComponent>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	SCameraManConfiguration cameraman_configuration_{};

	IEntity* target_entity_{};
};

inline void CFollowComponent::ReflectType(Schematyc::CTypeDesc<CFollowComponent>& desc)
{
	desc.SetGUID("{413EF649-1E40-4CCE-9A59-BB8F29CA98B1}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("followcomponent"));
	desc.SetLabel("Follow Component");
	desc.SetDescription("This component has the responsibility to follow the linked entity");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CFollowComponent::cameraman_configuration_
		, 'fccc'
		, "cameramanconfiguration"
		, "CameraMan Configuration"
		, "Configuration for the Camera Entity to follow his linked entity"
		, SCameraManConfiguration{});
}

#endif // !FOLLOW_COMPONENT