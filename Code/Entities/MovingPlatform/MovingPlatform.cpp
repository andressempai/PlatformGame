#include "StdAfx.h"
#include "MovingPlatform.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/RigidBodyComponent.h>

namespace
{
	static void register_moving_platform(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CMovingPlatform));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_moving_platform);
}

void CMovingPlatform::Initialize()
{
	static_mesh_ = m_pEntity->GetOrCreateComponent<StaticMesh>();
	rigid_body_ = m_pEntity->GetOrCreateComponent<RigidBody>();
	
	static_mesh_->SetFilePath("%engine%/engineassets/objects/primitive_cube.cgf");
	static_mesh_->LoadFromDisk();
	static_mesh_->GetPhysicsParameters().m_weightType =
		Cry::DefaultComponents::SPhysicsParameters::EWeightType::Immovable;
}

Cry::Entity::EventFlags CMovingPlatform::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::Reset | Cry::Entity::EEvent::Update;
}

void CMovingPlatform::ProcessEvent(const SEntityEvent& event)
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
			const auto frame_time = event.fParam[0];

			static constexpr auto min_distance = 0.1f;

			Vec3 velocity{ ZERO };

			if (entity_link_)
			{
				auto* const way_point_entity = gEnv->pEntitySystem->GetEntity(entity_link_->entityId);
				const auto way_point_position = way_point_entity->GetWorldPos();

				pe_status_pos platform_position_status{};
				physical_entity->GetStatus(&platform_position_status);

				const auto movement_direction = way_point_position - platform_position_status.pos;

				if (movement_direction.len2() <= (min_distance * min_distance))
				{
					if (auto* const next_way_point_entity = entity_link_->next)
						entity_link_ = next_way_point_entity;
					else
						point_to_first_entity_link();
				}
				else
				{
					velocity = movement_direction.GetNormalized() * movement_scale_;
				}
			}

			pe_action_set_velocity action_velocity{};
			action_velocity.v = velocity;

			physical_entity->Action(&action_velocity);
		}
	}
	break;

	}
}

void CMovingPlatform::point_to_first_entity_link()
{
	entity_link_ = m_pEntity->GetEntityLinks();
}