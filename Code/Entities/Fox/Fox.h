#ifndef FOX
#define FOX

#include <CryEntitySystem/IEntityComponent.h>

namespace Cry::DefaultComponents
{
	class CInputComponent;
}

class CPhysicComponent;
class CAnimationComponent;
class CFootStepComponent;
class CSensorComponent;
class CCoyoteTimeComponent;

class CFox final : public IEntityComponent
{
	using InputComponent = Cry::DefaultComponents::CInputComponent;

	struct SDefaults
	{
		inline static constexpr auto walk_speed = 10.0f;
		inline static constexpr auto boost_speed = 15.5f;
		inline static constexpr auto fly_speed = 3.75f;
		inline static constexpr auto jump_impulse = 5.5f;

		inline static constexpr auto jump_buffer_time = 0.2f;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CFox>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	float walk_speed_{ SDefaults::walk_speed };
	float boost_speed_{ SDefaults::boost_speed };
	float fly_speed_{ SDefaults::fly_speed };
	float jump_speed_{ SDefaults::jump_impulse };
	float jump_buffer_time_{ SDefaults::jump_buffer_time };

	InputComponent* input_component_{};
	CPhysicComponent* physics_component_{};
	CAnimationComponent* animation_component_{};
	CFootStepComponent* footstep_component_{};
	CSensorComponent* sensor_component_{};
	CCoyoteTimeComponent* coyote_time_component_{};

	float ground_movement_speed_{ walk_speed_ };
	float movement_direction_{};

	bool can_jump_{ true };
	float jump_buffer_{};
};

inline void CFox::ReflectType(Schematyc::CTypeDesc<CFox>& desc)
{
	desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	desc.SetEditorCategory("Game Entities");
	desc.SetName(Schematyc::CTypeName{ "Fox" });
	desc.SetDescription("Represent entity fox main player");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CFox::walk_speed_
		, 'fws'
		, "walkspeed"
		, "Walk Speed"
		, "How strong is the walk impulse"
		, SDefaults::walk_speed);

	desc.AddMember(
		  &CFox::boost_speed_
		, 'fbs'
		, "boostspeed"
		, "Boost Speed"
		, "How strong is the Boost impulse"
		, SDefaults::boost_speed);

	desc.AddMember(
		  &CFox::fly_speed_
		, 'ffs'
		, "flyspeed"
		, "Fly Speed"
		, "How strong is the Fly movement"
		, SDefaults::fly_speed);

	desc.AddMember(
		  &CFox::jump_speed_
		, 'fji'
		, "jumpimpulse"
		, "Jump Impulse"
		, "How strong is the Jump impulse"
		, SDefaults::jump_impulse);

	desc.AddMember(
		  &CFox::jump_buffer_time_
		, 'fjbt'
		, "jumpbuffertime"
		, "Jump Buffer Time"
		, "Grace time to allow jump, when the key is pressed"
		, SDefaults::jump_buffer_time);
}

#endif // !FOX