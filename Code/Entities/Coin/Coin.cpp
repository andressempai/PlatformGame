#include "StdAfx.h"
#include "Coin.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include "../../Components/RotationComponent/RotationComponent.h"

#include <CryRenderer/IRenderAuxGeom.h>

#include "../Fox/Fox.h"

namespace
{
	static void register_coin_entity(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		auto componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCoin));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_coin_entity);
}

void CCoin::Initialize()
{
	assign_geometry();
	assign_material();
	
	auto entity_flags = m_pEntity->GetFlags();
	entity_flags |= ENTITY_FLAG_CASTSHADOW;
	m_pEntity->SetFlags(entity_flags);

	orient_geometry();

	trigger_component_ = m_pEntity->GetOrCreateComponent<IEntityTriggerComponent>();
	rotation_component_ = m_pEntity->GetOrCreateComponent<CRotationComponent>();

	generate_trigger_aabb();
}

Cry::Entity::EventFlags CCoin::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::EntityEnteredThisArea;
}

void CCoin::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::EditorPropertyChanged:
	{
		assign_geometry();
		assign_material();

		orient_geometry();

		generate_trigger_aabb();
	}
	break;

	case Cry::Entity::EEvent::EntityEnteredThisArea:
	{
		if (gEnv->IsEditing())
			break;

		if (auto* const entity = gEnv->pEntitySystem->GetEntity(static_cast<EntityId>(event.nParam[0])))
		{
			if (entity->GetComponent<CFox>())
			{
				gEnv->pAudioSystem->ExecuteTrigger(CryAudio::StringToId("coin"));
				gEnv->pEntitySystem->RemoveEntity(m_pEntity->GetId());
			}
		}
	}
	break;

	}
}

void CCoin::Render(
	  const IEntity& entity
	, const IEntityComponent& component
	, SEntityPreviewContext& context) const
{
	if (trigger_component_)
	{
		AABB trigger_bbox{};
		trigger_component_->GetTriggerBounds(trigger_bbox);

		static constexpr auto can_render_as_solid_object = false;
		static constexpr auto draw_style = eBBD_Faceted;

		gEnv->pRenderer->GetIRenderAuxGeom()->DrawOBB(
			  OBB::CreateOBBfromAABB(Matrix33::CreateIdentity(), trigger_bbox)
			, GetWorldTransformMatrix()
			, can_render_as_solid_object
			, Col_Red
			, draw_style);
	}
}

void CCoin::assign_geometry()
{
	geometry_slot_ = m_pEntity->LoadGeometry(GetOrMakeEntitySlotId(), geometry_file_path_.value);
}

void CCoin::assign_material()
{
	auto* const material_manager = gEnv->p3DEngine->GetMaterialManager();

	if (auto* const material = material_manager->LoadMaterial(material_file_path_.value, false))
		m_pEntity->SetSlotMaterial(geometry_slot_, material);
}

void CCoin::orient_geometry()
{
	m_pEntity->SetSlotLocalTM(geometry_slot_, Matrix34::CreateRotationX(gf_PI / 2.0f, ZERO));
}

void CCoin::generate_trigger_aabb()
{
	const AABB aabb{ trigger_size_ };

	if(trigger_component_)
		trigger_component_->SetTriggerBounds(aabb);
}