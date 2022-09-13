#ifndef BOUNCING_PLATFORM
#define BOUNCING_PLATFORM

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

class CBouncingPlatform final : public IEntityComponent
#ifndef RELEASE
	, public IEntityComponentPreviewer
#endif
{
	struct SDefaults
	{
		inline static constexpr auto trigger_size = 0.5f;
		inline static constexpr auto launch_constants = 100.0f;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CBouncingPlatform>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE
	IEntityComponentPreviewer* GetPreviewer() override { return this; }
	void SerializeProperties(Serialization::IArchive& archive) override {}
	void Render(
		  const IEntity& entity
		, const IEntityComponent& component
		, SEntityPreviewContext& context) const override;
#endif

	void generate_trigger_aabb();

	Schematyc::PositiveFloat trigger_size_{ SDefaults::trigger_size };
	Schematyc::PositiveFloat launch_speed_{ SDefaults::launch_constants };

	IEntityTriggerComponent* trigger_component_{};
};

inline void CBouncingPlatform::ReflectType(Schematyc::CTypeDesc<CBouncingPlatform>& desc)
{
	desc.SetGUID("{F57EB3BF-5416-420B-998E-E8CB5F95262E}"_cry_guid);
	desc.SetEditorCategory("Game Entities");
	desc.SetName(Schematyc::CTypeName("bouncingplatform"));
	desc.SetLabel("Bouncing Platform");
	desc.SetDescription("Apply Linear impulse to Entity has touch this entity");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
		  &CBouncingPlatform::trigger_size_
		, 'bpts'
		, "triggersize"
		, "Trigger Size"
		, "Trigger Size in all axis"
		, SDefaults::trigger_size);

	desc.AddMember(
		  &CBouncingPlatform::launch_speed_
		, 'bpls'
		, "launchspeed"
		, "Launch Speed"
		, "Launch Speed"
		, SDefaults::launch_constants);
}

#endif // !BOUNCING_PLATFORM