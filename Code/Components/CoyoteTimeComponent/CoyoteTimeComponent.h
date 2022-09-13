#ifndef COYOTE_TIME_COMPONENT
#define COYOTE_TIME_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>

class CSensorComponent;

class CCoyoteTimeComponent final : public IEntityComponent
{
	struct SDefaults
	{
		inline static constexpr float time_before_falling = 0.1f;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CCoyoteTimeComponent>& desc);

	bool is_on_time_before_falling() const;

private:
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	Schematyc::PositiveFloat time_before_falling_{ SDefaults::time_before_falling };

	CSensorComponent* sensor_component_{};

	float time_elapsed_{};
};

inline void CCoyoteTimeComponent::ReflectType(Schematyc::CTypeDesc<CCoyoteTimeComponent>& desc)
{
	desc.SetGUID("{81EDFCD9-362C-4A76-BCFC-61F29E364AE0}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("coyotetimecomponent"));
	desc.SetLabel("Coyote Time Component");
	desc.SetDescription("Handle coyote time logic");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CCoyoteTimeComponent::time_before_falling_
		, 'cttb'
		, "timebeforefalling"
		, "Time Before Falling"
		, "Time Before Falling"
		, SDefaults::time_before_falling);
}

inline bool CCoyoteTimeComponent::is_on_time_before_falling() const
{
	return time_elapsed_ < time_before_falling_;
}

#endif // !COYOTE_TIME_COMPONENT