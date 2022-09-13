#include "StdAfx.h"
#include "PhysicComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryPhysics/physinterface.h>

namespace
{
	void register_physic_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPhysicComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_physic_component);
}

void CPhysicComponent::Initialize()
{
	entity_slot_ = GetOrMakeEntitySlotId();

	physicalize();
}

void CPhysicComponent::OnShutDown()
{
	physical_entity_ = nullptr;
}

Cry::Entity::EventFlags CPhysicComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Initialize | Cry::Entity::EEvent::EditorPropertyChanged;
}

void CPhysicComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		physicalize();
}

#ifndef RELEASE
void CPhysicComponent::Render(
	  const IEntity& entity
	, const IEntityComponent& component
	, SEntityPreviewContext& context) const
{
	if (physical_entity_)
	{
		pe_params_part part_params{};
		part_params.ipart = 0;

		physical_entity_->GetParams(&part_params);

		const auto& transform_matrix = m_pEntity->GetWorldTM();

		geom_world_data geom_data{};
		geom_data.R = Matrix33{ Quat{ transform_matrix } * part_params.q };
		geom_data.scale = transform_matrix.GetUniformScale() * part_params.scale;
		geom_data.offset = transform_matrix.GetTranslation() + transform_matrix.TransformVector(part_params.pos);

		gEnv->pSystem->GetIPhysRenderer()->DrawGeometry(
			  part_params.pPhysGeom->pGeom
			, &geom_data
			, -1
			, 0
			, ZERO
			, context.debugDrawInfo.color);
	}
}
#endif // !RELEASE

void CPhysicComponent::physicalize()
{
	SEntityPhysicalizeParams physicalize_none{};
	physicalize_none.type = PE_NONE;
	physicalize_none.nSlot = entity_slot_;
	m_pEntity->Physicalize(physicalize_none);

	const auto collider_height = player_dimensions_.height * 0.25f;
	const auto collider_radius = player_dimensions_.radius * 0.5f;

	pe_player_dimensions dimensions{};
	dimensions.bUseCapsule = 1;
	dimensions.sizeCollider = { collider_radius, 1.0f, collider_height };
	dimensions.heightPivot = 0.0f;
	dimensions.heightCollider = collider_radius + collider_height + 0.1f;
	dimensions.heightEye = 0.71f;
	dimensions.groundContactEps = 0.004f;

	pe_player_dynamics dynamics{};
	dynamics.gravity = player_dynamics_.gravity_;
	dynamics.mass = player_dynamics_.mass;
	dynamics.bSwimming = 0;
	dynamics.kAirControl = player_dynamics_.air_control;
	dynamics.kAirResistance = player_dynamics_.air_resistance;
	dynamics.kInertia = player_dynamics_.inertia;
	dynamics.kInertiaAccel = player_dynamics_.accelerate_inertia;
	dynamics.minSlideAngle = 70.0f;
	dynamics.maxClimbAngle = 50.0f;
	dynamics.maxJumpAngle = 50.0f;
	dynamics.minFallAngle = 80.0f;
	dynamics.maxVelGround = 16.0f;

	SEntityPhysicalizeParams physicalize_living{};
	physicalize_living.type = PE_LIVING;
	physicalize_living.nSlot = entity_slot_;
	physicalize_living.nFlagsOR = pef_monitor_poststep;
	physicalize_living.pPlayerDimensions = &dimensions;
	physicalize_living.pPlayerDynamics = &dynamics;
	m_pEntity->Physicalize(physicalize_living);

	physical_entity_ = m_pEntity->GetPhysicalEntity();
}

void CPhysicComponent::add_linear_velocity(const Vec3& direction) const noexcept
{
	if (physical_entity_)
	{
		if (!direction.IsZero())
		{
			pe_action_move action_move{};
			action_move.dir = direction;
			action_move.iJump = 2;

			physical_entity_->Action(&action_move);
		}
	}
}

Vec3 CPhysicComponent::get_velocity() const noexcept
{
	if (physical_entity_)
	{
		pe_status_living living_status{};
		physical_entity_->GetStatus(&living_status);

		return living_status.vel;
	}

	return ZERO;
}