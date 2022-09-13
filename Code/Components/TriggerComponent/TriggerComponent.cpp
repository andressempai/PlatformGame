#include "StdAfx.h"
#include "TriggerComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryPhysics/primitives.h>
#include <CryPhysics/physinterface.h>

#include <CryRenderer/IRenderAuxGeom.h>

namespace
{
	void register_trigger_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CTriggerComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_trigger_component);
}

void CTriggerComponent::Initialize()
{
	physicalize();
}

Cry::Entity::EventFlags CTriggerComponent::GetEventMask() const
{
	return 
		  Cry::Entity::EEvent::Initialize
		| Cry::Entity::EEvent::EditorPropertyChanged
		| Cry::Entity::EEvent::TransformChanged;
}

void CTriggerComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		physicalize();
}

#ifndef RELEASE
void CTriggerComponent::Render(
	  const IEntity& entity
	, const IEntityComponent& component
	, SEntityPreviewContext& context) const
{
	const auto world_transform = GetWorldTransformMatrix();

	auto* const trigger_geometry = create_geometry();

	geom_world_data geom_data{};
	geom_data.R = Matrix33(world_transform);
	geom_data.scale = world_transform.GetUniformScale();
	geom_data.offset = world_transform.GetColumn3();

	gEnv->pSystem->GetIPhysRenderer()->DrawGeometry(
		  trigger_geometry
		, &geom_data
		, -1
		, 0
		, ZERO
		, Col_Aquamarine);

	trigger_geometry->Release();
}
#endif

IGeometry* CTriggerComponent::create_geometry() const
{
	primitives::box primitive;
	primitive.Basis = IDENTITY;
	primitive.bOriented = 1;
	primitive.center = Vec3(0.0f);
	primitive.size = trigger_size_;

	return gEnv->pPhysicalWorld->GetGeomManager()->CreatePrimitive(primitive.type, &primitive);
}

void CTriggerComponent::physicalize()
{
	if (auto* const physical_entity = m_pEntity->GetPhysicalEntity())
		gEnv->pPhysicalWorld->DestroyPhysicalEntity(physical_entity, IPhysicalWorld::DM_NORMAL);

	const auto world_transform = GetWorldTransformMatrix();

	auto* const area =
		gEnv->pPhysicalWorld->AddArea(
			  create_geometry()
			, world_transform.GetColumn3()
			, quaternionf(world_transform)
			, trigger_size_.x);

	m_pEntity->AssignPhysicalEntity(area);
}