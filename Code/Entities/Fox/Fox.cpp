#include "StdAfx.h"
#include "Fox.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Input/InputComponent.h>

#include "../../Components/PhysicComponent/PhysicComponent.h"
#include "../../Components/AnimationComponent/AnimationComponent.h"
#include "../../Components/FootStepComponent/FootStepComponent.h"
#include "../../Components/SensorComponent/SensorComponent.h"
#include "../../Components/CoyoteTimeComponent/CoyoteTimeComponent.h"

#include "../../Interfaces/IDamageable.h"

namespace
{
	static void register_player_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CFox));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_player_component);
}

void CFox::Initialize()
{
	m_pEntity->SetFlags(EEntityFlags::ENTITY_FLAG_TRIGGER_AREAS | ENTITY_FLAG_CASTSHADOW);

	input_component_ = m_pEntity->GetOrCreateComponent<InputComponent>();
	physics_component_ = m_pEntity->GetOrCreateComponent<CPhysicComponent>();
	animation_component_ = m_pEntity->GetOrCreateComponent<CAnimationComponent>();
	footstep_component_ = m_pEntity->GetOrCreateComponent<CFootStepComponent>();
	sensor_component_ = m_pEntity->GetOrCreateComponent<CSensorComponent>();
	coyote_time_component_ = m_pEntity->GetOrCreateComponent<CCoyoteTimeComponent>();

	if (sensor_component_->sensors_.Size() == 0)
	{
		sensor_component_->sensors_.EmplaceBack(
			SSensor{ Vec3(0.0f, 0.2f, 0.4f), Vec3(0.0f, 0.0f, -1.0f), 0.455f });

		sensor_component_->sensors_.EmplaceBack(
			SSensor{ Vec3(0.0f, -0.2f, 0.4f), Vec3(0.0f, 0.0f, -1.0f), 0.455f });

		sensor_component_->sensors_.EmplaceBack(
			SSensor{ Vec3(0.0f, 0.0f, 0.4f), Vec3(0.0f, 0.0f, -1.0f), 0.455f });
	}

	const auto&& move_forward = [this](int activation_mode, float value)
	{
		movement_direction_ += value;
	};
	
	const auto&& move_back = [this](int activation_mode, float value)
	{
		movement_direction_ -= value;
	};
	
	const auto&& jump = [this](int activation_mode, float value)
	{
		if(activation_mode == eAAM_OnPress)
		{
			jump_buffer_ = jump_buffer_time_;
		}
	};
	
	const auto&& boost = [this](int activation_mode, float value)
	{
		if (activation_mode == eAAM_OnHold)
			ground_movement_speed_ = boost_speed_;
		else if (activation_mode == eAAM_OnRelease)
			ground_movement_speed_ = walk_speed_;
	};

	input_component_->RegisterAction("player", "moveforward", move_forward);
	input_component_->RegisterAction("player", "moveback", move_back);
	input_component_->RegisterAction("player", "jump", jump);
	input_component_->RegisterAction("player", "boost", boost);

	input_component_->BindAction("player", "moveforward", eAID_KeyboardMouse, eKI_D);
	input_component_->BindAction("player", "moveback", eAID_KeyboardMouse, eKI_A);
	input_component_->BindAction("player", "jump", eAID_KeyboardMouse, eKI_Space);
	input_component_->BindAction("player", "boost", eAID_KeyboardMouse, eKI_LShift);
}

Cry::Entity::EventFlags CFox::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::Update;
}

void CFox::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::EditorPropertyChanged:
	{
		ground_movement_speed_ = walk_speed_;
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (gEnv->IsEditing())
			break;

		const auto frame_time = event.fParam[0];
		const auto sensor_hit_result = sensor_component_->get_sensor_result();

		if (jump_buffer_ > ZERO)
			jump_buffer_ -= frame_time;

		auto movement_speed = 0.0f;

		auto* const detected_physical_entity = sensor_hit_result.pCollider;

		if (detected_physical_entity)
		{
			movement_speed = ground_movement_speed_;
			can_jump_ = true;
		}
		else
		{
			movement_speed = fly_speed_;
		}

		physics_component_->add_linear_velocity({ 0.0f, movement_direction_ * movement_speed * frame_time, 0.0f });

		if (jump_buffer_ > ZERO && coyote_time_component_->is_on_time_before_falling() && can_jump_)
		{
			can_jump_ = false;
			jump_buffer_ = ZERO;

			physics_component_->add_linear_velocity({ 0.0f, 0.0f, jump_speed_ });
		}

		movement_direction_ = ZERO;

		if (detected_physical_entity)
			if (auto* const entity = gEnv->pEntitySystem->GetEntityFromPhysics(detected_physical_entity))
				if (auto* const damageable_entity = entity->GetComponent<IDamageable>())
					damageable_entity->take_damage(m_pEntity->GetId());
	}
	break;

	}
}