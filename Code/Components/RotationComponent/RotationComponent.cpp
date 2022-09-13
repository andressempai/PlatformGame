#include "StdAfx.h"
#include "RotationComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	void register_rotation_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CRotationComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_rotation_component);
}

Cry::Entity::EventFlags CRotationComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CRotationComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;
	
	const auto frame_time = event.fParam[0];

	const auto& transform_matrix = m_pEntity->GetWorldTM();

	m_pEntity->SetWorldTM(transform_matrix * Matrix33::CreateRotationZ(DEG2RAD(rotation_speed_ * frame_time)));
}