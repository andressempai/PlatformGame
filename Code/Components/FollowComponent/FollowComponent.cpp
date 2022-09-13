#include "StdAfx.h"
#include "FollowComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryEntitySystem/IEntitySystem.h>

namespace
{
	void register_follow_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CFollowComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_follow_component);
}

Cry::Entity::EventFlags CFollowComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::Update;
}

void CFollowComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted:
	{
		if (auto* const entity_links = m_pEntity->GetEntityLinks())
			target_entity_ = gEnv->pEntitySystem->GetEntity(entity_links->entityId);
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		if (gEnv->IsEditing())
			return;

		if (target_entity_)
		{
			const auto target_position = target_entity_->GetWorldPos();
			const auto cameraman_position = m_pEntity->GetWorldPos();
			const auto frame_time = event.fParam[0];

			const auto new_position =
				Vec3::CreateLerp(
					  cameraman_position
					, target_position + cameraman_configuration_.cameraman_offset_position
					, cameraman_configuration_.follow_speed * frame_time);

			m_pEntity->SetWorldTM(Matrix34::CreateTranslationMat(new_position));
		}
	}
	break;

	}
}