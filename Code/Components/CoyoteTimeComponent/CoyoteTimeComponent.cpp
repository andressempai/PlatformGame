#include "StdAfx.h"
#include "CoyoteTimeComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include "../SensorComponent/SensorComponent.h"

namespace
{
	void register_coyote_time_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCoyoteTimeComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_coyote_time_component);
}

Cry::Entity::EventFlags CCoyoteTimeComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::Update;
}

void CCoyoteTimeComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted:
	{
		sensor_component_ = m_pEntity->GetComponent<CSensorComponent>();
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (sensor_component_)
		{
			if (!sensor_component_->get_sensor_result().pCollider)
			{
				if (time_elapsed_ < time_before_falling_.value)
				{
					const auto frame_time = event.fParam[0];
					time_elapsed_ += frame_time;
				}
			}
			else
				time_elapsed_ = 0.0f;
		}
	}
	break;

	}
}