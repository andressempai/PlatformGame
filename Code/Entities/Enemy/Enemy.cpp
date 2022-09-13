#include "StdAfx.h"
#include "Enemy.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>

namespace
{
	static void register_enemy(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CEnemy));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_enemy);
}

void CEnemy::Initialize()
{
	
}

Cry::Entity::EventFlags CEnemy::GetEventMask() const
{
	return
		  Cry::Entity::EEvent::EditorPropertyChanged
		| Cry::Entity::EEvent::Remove
		| Cry::Entity::EEvent::Update;
}

void CEnemy::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::EditorPropertyChanged:
	{
		if (auto* const physical_entity = m_pEntity->GetPhysicalEntity())
		{
			pe_params_flags flags_params{};
			physical_entity->GetParams(&flags_params);

			if (can_be_affected_by_gravity_)
				flags_params.flags &= ~pef_ignore_areas;
			else
				flags_params.flags |= pef_ignore_areas;

			physical_entity->SetParams(&flags_params);
		}
	}
	break;

	case Cry::Entity::EEvent::Remove:
	{
		gEnv->pAudioSystem->ExecuteTrigger(CryAudio::StringToId("stomp"));
	}
	break;

	}
}

void CEnemy::take_damage(const EntityId entity_id)
{
	if (auto* const damage_causer = gEnv->pEntitySystem->GetEntity(entity_id))
	{
		if (auto* const physical_entity = damage_causer->GetPhysicalEntity())
		{
			pe_action_set_velocity set_velocity_action{};
			set_velocity_action.v = ZERO;
			physical_entity->Action(&set_velocity_action);

			pe_action_impulse explode_impulse_action{};
			explode_impulse_action.point = m_pEntity->GetWorldPos();
			explode_impulse_action.impulse = { 0.0f, 0.0f, explode_impulse_ };
			explode_impulse_action.iApplyTime = 0;
			physical_entity->Action(&explode_impulse_action);
		}
	}

	gEnv->pEntitySystem->RemoveEntity(GetEntityId());
}