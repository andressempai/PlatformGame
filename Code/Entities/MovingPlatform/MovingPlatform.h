#ifndef MOVING_PLATFORM
#define MOVING_PLATFORM

#include <CryEntitySystem/IEntityComponent.h>

namespace Cry::DefaultComponents
{
	class CStaticMeshComponent;
	class CRigidBodyComponent;
}

class CMovingPlatform final : public IEntityComponent
{
	using StaticMesh = Cry::DefaultComponents::CStaticMeshComponent;
	using RigidBody = Cry::DefaultComponents::CRigidBodyComponent;

	struct SDefaults
	{
		inline static constexpr float movement_scale{ 1.0f };
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CMovingPlatform>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

	void point_to_first_entity_link();

	float movement_scale_{ SDefaults::movement_scale };

	StaticMesh* static_mesh_{};
	RigidBody* rigid_body_{};

	uint8 current_way_point_index_{ 0 };
	uint8 next_way_point_index_{ 1 };

	IEntityLink* entity_link_{};
};

inline void CMovingPlatform::ReflectType(Schematyc::CTypeDesc<CMovingPlatform>& desc)
{
	desc.SetGUID("{EBA66845-EA09-4F4B-83FF-018BC2671D25}"_cry_guid);
	desc.SetEditorCategory("Game Entities");
	desc.SetName(Schematyc::CTypeName{ "movingplatform" });
	desc.SetLabel("Moving Platform");
	desc.SetDescription("Moving Platform");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CMovingPlatform::movement_scale_
		, 'mpms'
		, "movementspeed"
		, "Movement Speed"
		, "How strong the platform can move in every frame"
		, SDefaults::movement_scale);
}

#endif // !MOVING_PLATFORM