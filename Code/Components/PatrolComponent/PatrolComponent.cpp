#include "StdAfx.h"
#include "PatrolComponent.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>

namespace
{
	static void register_patrol_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPatrolComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_patrol_component);
}

Cry::Entity::EventFlags CPatrolComponent::GetEventMask() const
{
	return
		  Cry::Entity::EEvent::GameplayStarted
		| Cry::Entity::EEvent::Reset
		| Cry::Entity::EEvent::Update;
}

void CPatrolComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted:
	case Cry::Entity::EEvent::Reset:
	{
		point_to_first_entity_link();
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (auto* const physical_entity = m_pEntity->GetPhysicalEntity())
		{
			pe_status_living living_status{};
			physical_entity->GetStatus(&living_status);

			const auto frame_time = event.fParam[0];

			static constexpr auto min_distance = 0.2f;

			Vec3 velocity{ ZERO };

			if (entity_link_)
			{
				auto* const way_point_entity = gEnv->pEntitySystem->GetEntity(entity_link_->entityId);
				const auto way_point_position = way_point_entity->GetWorldPos();

				const auto movement_direction = way_point_position - GetWorldTransformMatrix().GetColumn3();

				if (movement_direction.len2() <= min_distance)
				{
					if (auto* const next_way_point_entity = entity_link_->next)
						entity_link_ = next_way_point_entity;
					else
						point_to_first_entity_link();
				}
				else
				{
					velocity = movement_direction.GetNormalized() * patrol_speed_;
				}
			}

			pe_action_set_velocity action_velocity{};
			action_velocity.v = velocity;
			physical_entity->Action(&action_velocity);

			const auto&& rotate_entity = [this, &living_status, frame_time]
			{
				Vec3 movement_direction{ 0.0f, living_status.vel.Normalize().y, 0.0f };
				movement_direction.Flip();

				static constexpr auto rotation_speed = 7.35f;

				if (!movement_direction.IsZero())
				{
					auto transform_matrix = m_pEntity->GetWorldTM();

					const Quat current_rotation{ transform_matrix };
					const Quat target_rotation{ Quat::CreateRotationVDir(movement_direction) };

					const auto final_rotation =
						Quat::CreateSlerp(current_rotation, target_rotation, rotation_speed * frame_time);

					transform_matrix.SetRotation33(Matrix33(final_rotation));

					m_pEntity->SetWorldTM(transform_matrix);
				}
			};
			rotate_entity();
		}
	}
	break;

	}
}

void CPatrolComponent::point_to_first_entity_link()
{
	entity_link_ = m_pEntity->GetEntityLinks();
}