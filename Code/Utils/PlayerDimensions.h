#ifndef PLAYER_DIMENSIONS
#define PLAYER_DIMENSIONS

#include <CrySchematyc/Reflection/TypeDesc.h>

struct SPlayerDimensions
{
	struct SDefaults
	{
		inline static constexpr auto height = 0.91f;
		inline static constexpr auto radius = 0.600f;
	};

	bool operator==(const SPlayerDimensions& other) const;
	bool operator!=(const SPlayerDimensions& other) const;

	static void ReflectType(Schematyc::CTypeDesc<SPlayerDimensions>& desc);

	float height{ SDefaults::height };
	float radius{ SDefaults::radius	};
};

inline void SPlayerDimensions::ReflectType(Schematyc::CTypeDesc<SPlayerDimensions>& desc)
{
	desc.SetGUID("{6A225BC6-9C4D-4FCF-8EBA-118A5872A47B}"_cry_guid);
	desc.AddMember(
		  &SPlayerDimensions::height
		, 'pdh'
		, "height"
		, "Height"
		, "Height of the Player in meters"
		, SDefaults::height);

	desc.AddMember(
		  &SPlayerDimensions::radius
		, 'pdr'
		, "radius"
		, "Radius"
		, "Radius of the Player"
		, SDefaults::radius);
}

inline bool SPlayerDimensions::operator==(const SPlayerDimensions& other) const
{
	return memcmp(this, &other, sizeof(other)) != 0;
}

inline bool SPlayerDimensions::operator!=(const SPlayerDimensions& other) const
{
	return memcmp(this, &other, sizeof(other)) != 0;
}

#endif // !PLAYER_DIMENSIONS