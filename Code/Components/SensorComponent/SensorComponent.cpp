#include "StdAfx.h"
#include "SensorComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	void register_sensor_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CSensorComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_sensor_component);
}

IEntityComponent::ComponentEventPriority CSensorComponent::GetEventPriority() const
{
	return 15;
}

Cry::Entity::EventFlags CSensorComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CSensorComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	hit_result_ = [this]
	{
		ray_hit out_hit{};

		const auto world_position = GetWorldTransformMatrix().GetColumn3();

		auto  const number_of_hits = 1;
		auto* const ignored_physical_entity = m_pEntity->GetPhysicalEntity();

		if (sensors_.Size() > 0)
		{
			for (uint32 iter = 0; iter < sensors_.Size(); iter++)
			{
				auto sensor = sensors_.At(iter);

				const auto ray_origin = world_position + sensor.position;
				const auto ray_direction = sensor.direction * sensor.distance;

				if (gEnv->pPhysicalWorld->RayWorldIntersection(
					  ray_origin
					, ray_direction
					, ent_all
					, rwi_stop_at_pierceable
					, &out_hit
					, number_of_hits
					, ignored_physical_entity) > 0)
				{
					break;
				}
			}
		}

		return out_hit;
	}();
}

void CSensorComponent::Render(
	  const IEntity& entity
	, const IEntityComponent& component
	, SEntityPreviewContext& context) const
{
	if (sensors_.Size() > 0)
	{
		for (uint32 iter = 0; iter < sensors_.Size(); iter++)
		{
			auto sensor = sensors_.At(iter);

			const auto world_position = GetWorldTransformMatrix().GetColumn3();

			const auto start_vertex = world_position + sensor.position;
			const auto end_vertex = start_vertex + sensor.direction * sensor.distance;
			const auto line_color = Col_Blue;

			gEnv->pAuxGeomRenderer->DrawLine(
				  start_vertex
				, line_color
				, end_vertex
				, line_color
				, 0.1f);
		}
	}
}