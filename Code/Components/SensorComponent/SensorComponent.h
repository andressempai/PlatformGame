#ifndef SENSOR_COMPONENT
#define SENSOR_COMPONENT

#include <CryEntitySystem/IEntityComponent.h>
#include <CryPhysics/physinterface.h>

#include "../../Utils/Sensor.h"

class CSensorComponent final : public IEntityComponent
#ifndef RELEASE
	, IEntityComponentPreviewer
#endif // !RELEASE
{
	friend class CFox;

	struct SDefaults
	{
		inline static const Vec3 sensor_direction{ 0.0f, 0.0f, -1.0f };
		inline static constexpr float sensor_distance{ 1.0f };
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CSensorComponent>& desc);

	[[nodiscard]]
	ray_hit get_sensor_result() const { return hit_result_; }

private:
	virtual ComponentEventPriority GetEventPriority() const override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE // ENTITY COMPONENT PREVIEWER INTERFACE
	virtual IEntityComponentPreviewer* GetPreviewer() override { return this; }
	virtual void SerializeProperties(Serialization::IArchive& archive) override {}
	virtual void Render(
		  const IEntity& entity
		, const IEntityComponent& component
		, SEntityPreviewContext& context) const override;
#endif // !RELEASE

	Schematyc::CArray<SSensor> sensors_{};

	ray_hit hit_result_{};
};

inline void CSensorComponent::ReflectType(Schematyc::CTypeDesc<CSensorComponent>& desc)
{
	desc.SetGUID("{2B38AC85-7D69-4F0C-B385-8D737C9F63C1}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("sensorcomponent"));
	desc.SetLabel("Sensor Component");
	desc.SetDescription("Sensor to handle detection any kind of physical entity");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CSensorComponent::sensors_
		, 'sdpt'
		, "sensors"
		, "Sensors"
		, "Sensors"
		, Schematyc::CArray<SSensor>{});
}

#endif // !SENSOR_COMPONENT