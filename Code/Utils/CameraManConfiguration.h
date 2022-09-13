#ifndef CAMERAMAN_CONFIGURATION
#define CAMERAMAN_CONFIGURATION

#include <CrySchematyc/MathTypes.h>

struct SCameraManConfiguration
{
	struct SDefaults
	{
		inline static const Vec3 offset_position{ 2.8f, 0.0f, 7.35f };
		inline static constexpr auto follow_speed = 7.35f;
	};

	bool operator==(const SCameraManConfiguration& other) const;

	static void ReflectType(Schematyc::CTypeDesc<SCameraManConfiguration>& desc);

	Vec3 cameraman_offset_position{ SDefaults::offset_position };
	Schematyc::Range<1, 10, 1, 10> follow_speed{ SDefaults::follow_speed };
};

inline bool SCameraManConfiguration::operator==(const SCameraManConfiguration& other) const
{
	return memcmp(this, &other, sizeof(other)) != 0;
}

void SCameraManConfiguration::ReflectType(Schematyc::CTypeDesc<SCameraManConfiguration>& desc)
{
	desc.SetGUID("{6A225BC6-9C4D-4FCF-8EBA-118A5872A47B}"_cry_guid);
	desc.AddMember(
		  &SCameraManConfiguration::cameraman_offset_position
		, 'cmcl'
		, "cameramanoffsetposition"
		, "Cameraman Offset Position"
		, "Cameraman Offset Position"
		, SDefaults::offset_position);

	desc.AddMember(
		  &SCameraManConfiguration::follow_speed
		, 'cmcs'
		, "speed"
		, "Speed"
		, "Speed of the CameraMan"
		, SDefaults::follow_speed);
}

#endif // CAMERAMAN_CONFIGURATION