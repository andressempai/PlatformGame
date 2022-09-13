#include "StdAfx.h"
#include "BouncingPlatform.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryPhysics/physinterface.h>
#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	void register_bouncing_platform(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBouncingPlatform));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_bouncing_platform);
}

void CBouncingPlatform::Initialize()
{
	trigger_component_ = m_pEntity->GetOrCreateComponent<IEntityTriggerComponent>();

	generate_trigger_aabb();
}

Cry::Entity::EventFlags CBouncingPlatform::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::EntityEnteredThisArea;
}

void CBouncingPlatform::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::EditorPropertyChanged:
	{
		generate_trigger_aabb();
	}
	break;

	case Cry::Entity::EEvent::EntityEnteredThisArea:
	{
		if (gEnv->IsEditing())
			break;

		if (auto* const entity = gEnv->pEntitySystem->GetEntity(static_cast<EntityId>(event.nParam[0])))
		{
			if (auto* const physical_entity = entity->GetPhysicalEntity())
			{
				pe_action_set_velocity set_velocity_action{};
				set_velocity_action.v = ZERO;

				physical_entity->Action(&set_velocity_action);

				pe_action_impulse impulse_action{};
				impulse_action.impulse = { 0.0f, 0.0f, launch_speed_ };
				impulse_action.point = GetWorldTransformMatrix().GetColumn3();

				physical_entity->Action(&impulse_action);
			}

			gEnv->pAudioSystem->ExecuteTrigger(CryAudio::StringToId("spring_jump"));
		}
	}
	break;

	}
}

void CBouncingPlatform::Render(
	  const IEntity& entity
	, const IEntityComponent& component
	, SEntityPreviewContext& context) const
{
	if (trigger_component_)
	{
		AABB trigger_bbox{};
		trigger_component_->GetTriggerBounds(trigger_bbox);

		const auto identity_rotation = Matrix33::CreateIdentity();
		const auto render_obb = OBB::CreateOBBfromAABB(identity_rotation, trigger_bbox);

		gEnv->pRenderer->GetIRenderAuxGeom()->DrawOBB(render_obb, GetWorldTransformMatrix(), false, Col_Red, eBBD_Faceted);
	}
}

void CBouncingPlatform::generate_trigger_aabb()
{
	const AABB aabb{ trigger_size_ };

	if (trigger_component_)
		trigger_component_->SetTriggerBounds(AABB::CreateTransformedAABB(GetTransformMatrix(), aabb));
}