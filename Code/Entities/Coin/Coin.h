#ifndef COIN
#define COIN

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/ResourceTypes.h>

class CRotationComponent;

class CCoin final : public IEntityComponent
#ifndef RELEASE
	, public IEntityComponentPreviewer
#endif
{
	struct SDefaults
	{
		inline static constexpr const char* coin_geometry_file_path{ "objects/coin/mesh/coin.cgf" };
		inline static constexpr const char* coin_material_file_path{ "objects/coin/materials/coin.mtl" };
		inline static constexpr float trigger_size{ 0.5f };
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CCoin>& desc);

private:
	void Initialize() override;
	Cry::Entity::EventFlags GetEventMask() const override;
	void ProcessEvent(const SEntityEvent& event) override;

#ifndef RELEASE
	IEntityComponentPreviewer* GetPreviewer() override { return this; }
	void SerializeProperties(Serialization::IArchive& archive) override {}
	void Render(
		  const IEntity& entity
		, const IEntityComponent& component
		, SEntityPreviewContext& context) const override;
#endif

	void assign_geometry();
	void assign_material();
	void orient_geometry();
	void generate_trigger_aabb();

	Schematyc::GeomFileName geometry_file_path_{ SDefaults::coin_geometry_file_path };
	Schematyc::MaterialFileName material_file_path_{ SDefaults::coin_material_file_path };
	Schematyc::PositiveFloat trigger_size_{ SDefaults::trigger_size };

	IEntityTriggerComponent* trigger_component_{};
	CRotationComponent* rotation_component_{};

	int geometry_slot_{};
};

inline void CCoin::ReflectType(Schematyc::CTypeDesc<CCoin>& desc)
{
	desc.SetGUID("{7D8B7DB8-88EB-472B-A428-FE274BD0FD5F}"_cry_guid);
	desc.SetEditorCategory("Game Entities");
	desc.SetName(Schematyc::CTypeName{ "Coin" });
	desc.SetDescription("Coin Entity");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
		  &CCoin::geometry_file_path_
		, 'cgfp'
		, "geometryfilepath"
		, "Geometry File Path"
		, "Geometry File Path"
		, SDefaults::coin_geometry_file_path);

	desc.AddMember(
		  &CCoin::material_file_path_
		, 'cmfp'
		, "materialfilepath"
		, "Material File Path"
		, "Material File Path"
		, SDefaults::coin_material_file_path);

	desc.AddMember(
		  &CCoin::trigger_size_
		, 'ccts'
		, "triggersize"
		, "Trigger Size"
		, "Trigger Size in all axis"
		, SDefaults::trigger_size);
}

#endif // !COIN