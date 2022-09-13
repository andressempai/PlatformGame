#ifndef TRIGGER_COMPONENT
#define TRIGGER_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>

struct IGeometry;

class CTriggerComponent final : public IEntityComponent
#ifndef RELEASE
	, public IEntityComponentPreviewer
#endif
{
	struct SDefaults
	{
		inline static constexpr auto trigger_size = 0.25f;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CTriggerComponent>& desc);

private:
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE
	virtual IEntityComponentPreviewer* GetPreviewer() override { return this; }
	virtual void SerializeProperties(Serialization::IArchive& archive) override {}
	virtual void Render(
		  const IEntity& entity
		, const IEntityComponent& component
		, SEntityPreviewContext& context) const override;
#endif

	[[nodiscard]]
	IGeometry* create_geometry() const;

	void physicalize();

	Vec3 trigger_size_{ SDefaults::trigger_size };
};

inline void CTriggerComponent::ReflectType(Schematyc::CTypeDesc<CTriggerComponent>& desc)
{
	desc.SetGUID("{1ADA1E3B-529F-4AE2-8330-673C9650D513}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("triggercomponent"));
	desc.SetLabel("Trigger Component");
	desc.SetDescription("Trigger Component");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CTriggerComponent::trigger_size_
		, 'tgts'
		, "triggersize"
		, "Trigger Size"
		, "Define the size of the Trigger"
		, SDefaults::trigger_size);
}

#endif // !TRIGGER_COMPONENT