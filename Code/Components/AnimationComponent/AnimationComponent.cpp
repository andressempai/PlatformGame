#include "StdAfx.h"
#include "AnimationComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

namespace
{
	void register_animation_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAnimationComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_animation_component);
}

void CAnimationComponent::Initialize()
{
	advanced_animation_component_ = m_pEntity->GetOrCreateComponent<AdvancedAnimationComponent>();

	const Matrix34 initial_transform{ Vec3(1.0f), Quat::CreateRotationZ(gf_PI), Vec3(0.0f, 0.0f, -0.29f) };
	advanced_animation_component_->SetTransformMatrix(initial_transform);

	advanced_animation_component_->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/Fox.adb");
	advanced_animation_component_->SetCharacterFile("objects/fox/fox.cdf");
	advanced_animation_component_->SetControllerDefinitionFile("Animations/Mannequin/ADB/ControllerDefinitions.xml");
	advanced_animation_component_->SetDefaultScopeContextName("ThirdPersonCharacter");
	advanced_animation_component_->SetDefaultFragmentName("Idle");
	advanced_animation_component_->SetAnimationDrivenMotion(false);
	advanced_animation_component_->LoadFromDisk();

	idle_fragment_id_ = advanced_animation_component_->GetFragmentId("Idle");
	walk_fragment_id_ = advanced_animation_component_->GetFragmentId("Walk");
	jump_fragment_id_ = advanced_animation_component_->GetFragmentId("Jump");
	fall_fragment_id_ = advanced_animation_component_->GetFragmentId("Fall");
	land_fragment_id_ = advanced_animation_component_->GetFragmentId("Land");
}

Cry::Entity::EventFlags CAnimationComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Reset | Cry::Entity::EEvent::Update;
}

void CAnimationComponent::ProcessEvent(const SEntityEvent& event)
{
	switch(event.event)
	{
	case Cry::Entity::EEvent::Reset:
	{
		active_fragment_id_ = idle_fragment_id_;
		advanced_animation_component_->QueueFragmentWithId(active_fragment_id_);
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (gEnv->IsEditing())
			break;

		if (const auto physical_entity = m_pEntity->GetPhysicalEntity())
		{
			pe_status_living living_status{};
			physical_entity->GetStatus(&living_status);

			const auto&& rotate_through_velocity = [this, &living_status, delta_time = event.fParam[0]]
			{
				Vec3 living_horizontal_velocity{ 0.0f, living_status.velUnconstrained.y, 0.0f };
				living_horizontal_velocity.Flip();

				if (!living_horizontal_velocity.IsZero())
				{
					auto transform_matrix = advanced_animation_component_->GetTransformMatrix();

					const Quat current_rotation{ transform_matrix };
					const Quat target_rotation{ Quat::CreateRotationVDir(living_horizontal_velocity) };

					const auto final_rotation =
						Quat::CreateSlerp(current_rotation, target_rotation, rotation_speed_ * delta_time);

					transform_matrix.SetRotation33(Matrix33{ final_rotation });

					advanced_animation_component_->SetTransformMatrix(transform_matrix);
				}
			};
			rotate_through_velocity();

			const auto&& update_fragments = [this, &living_status]
			{
				FragmentID desired_fragment_id{ FRAGMENT_ID_INVALID };

				static constexpr auto min_horizontal_speed = 0.2f;

				if (living_status.velUnconstrained.GetLengthSquared2D() > min_horizontal_speed)
					desired_fragment_id = walk_fragment_id_;
				else
					desired_fragment_id = idle_fragment_id_;
				
				static constexpr auto min_vertical_speed = 0.1f;

				if (living_status.bFlying)
					desired_fragment_id
					= living_status.velUnconstrained.z > min_vertical_speed
					? jump_fragment_id_
					: fall_fragment_id_;

				if (active_fragment_id_ != desired_fragment_id)
				{
					active_fragment_id_ = desired_fragment_id;
					advanced_animation_component_->QueueFragmentWithId(active_fragment_id_);
				}
			};
			update_fragments();
		}
	}
	break;

	}
}