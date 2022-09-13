#ifndef FOOTSTEP_COMPONENT
#define FOOTSTEP_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

class CFootStepComponent final : public IEntityComponent
{
public:
	static void ReflectType(Schematyc::CTypeDesc<CFootStepComponent>& desc);

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;
};

inline void CFootStepComponent::ReflectType(Schematyc::CTypeDesc<CFootStepComponent>& desc)
{
	desc.SetGUID("{A1DF9BC6-B238-4A29-9AC1-815ADE6E54DE}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("footstepcomponent"));
	desc.SetLabel("FootStep Component");
	desc.SetDescription("Listen animation event and play footstep sounds");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
}

#endif // !FOOTSTEP_COMPONENT