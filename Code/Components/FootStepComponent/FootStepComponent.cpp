#include "StdAfx.h"
#include "FootStepComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryAnimation/ICryAnimation.h>
#include <MaterialEffects/IMFXEffect.h>
#include <CryPhysics/physinterface.h>

namespace
{
	void register_footstep_component(Schematyc::IEnvRegistrar& registrar)
	{
		auto scope = registrar.Scope(IEntity::GetEntityScopeGUID());

		scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CFootStepComponent));
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_footstep_component);
}

Cry::Entity::EventFlags CFootStepComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::AnimationEvent;
}

void CFootStepComponent::ProcessEvent(const SEntityEvent& event)
{
	if (gEnv->IsEditing())
		return;

	auto* const animation_event = reinterpret_cast<const AnimEventInstance*>(event.nParam[0]);
	auto* const character_instance = reinterpret_cast<const ICharacterInstance*>(event.nParam[1]);

	constexpr auto event_name = "footstep";
	constexpr auto footstep_library_name = "footsteps";

	if (animation_event)
	{
		if (stricmp(animation_event->m_EventName, event_name) == 0)
		{
			QuatT joint_transform{ IDENTITY };

			if (animation_event->m_BonePathName && character_instance)
			{
				joint_transform =
					character_instance->GetISkeletonPose()->GetAbsJointByID(
						character_instance->GetIDefaultSkeleton().GetJointIDByName(animation_event->m_BonePathName));
			}

			SMFXRunTimeEffectParams runtime_effects_params{};
			runtime_effects_params.audioProxyEntityId = m_pEntity->GetId();
			runtime_effects_params.angle = m_pEntity->GetWorldAngles().z + (gf_PI * 0.5f);
			runtime_effects_params.pos = m_pEntity->GetSlotWorldTM(0) * joint_transform.t;
			runtime_effects_params.audioProxyOffset =
				m_pEntity->GetWorldTM().GetInverted().TransformVector(
					runtime_effects_params.pos - m_pEntity->GetWorldPos());

			if (auto* const material_effects = gEnv->pMaterialEffects)
			{
				pe_status_living living_status{};
				m_pEntity->GetPhysicalEntity()->GetStatus(&living_status);

				if (living_status.groundSurfaceIdx > 0)
				{
					if (const auto effect_id =
						material_effects->GetEffectId(footstep_library_name, living_status.groundSurfaceIdx))
					{
						material_effects->ExecuteEffect(effect_id, runtime_effects_params);
					}
				}
			}
		}
	}
}