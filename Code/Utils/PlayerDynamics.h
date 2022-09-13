#ifndef PLAYER_DYNAMICS
#define PLAYER_DYNAMICS

#include <CrySchematyc/MathTypes.h>

struct SPlayerDynamics
{
	struct SDefaults
	{
		inline static constexpr auto mass				= 15.0f;
		inline static const		auto gravity			= Vec3(0.0f, 0.0f, -9.8f);
		inline static constexpr auto air_control		= 0.200f;
		inline static constexpr auto air_resistance		= 0.360f;
		inline static constexpr auto inertia			= 3.50f;
		inline static constexpr auto accelerate_inertia	= 5.5f;
	};

	bool operator==(const SPlayerDynamics& other) const;
	bool operator!=(const SPlayerDynamics& other) const;

	static void ReflectType(Schematyc::CTypeDesc<SPlayerDynamics>& desc);

	Schematyc::PositiveFloat mass{ SDefaults::mass };
	Vec3 gravity_{ SDefaults::gravity };
	Schematyc::Range<0, 1> air_control{ SDefaults::air_control };
	Schematyc::Range<0, 1> air_resistance{ SDefaults::air_resistance };
	Schematyc::Range<0, 100> inertia{ SDefaults::inertia };
	Schematyc::Range<0, 100> accelerate_inertia{ SDefaults::accelerate_inertia };
};

inline void SPlayerDynamics::ReflectType(Schematyc::CTypeDesc<SPlayerDynamics>& desc)
{
	desc.SetGUID("{6A225BC6-9C4D-4FCF-8EBA-118A5872A47B}"_cry_guid);
	desc.AddMember(
		  &SPlayerDynamics::gravity_
		, 'pdgv'
		, "gravity"
		, "Gravity"
		, "Player Gravity"
		, SDefaults::gravity);

	desc.AddMember(
		  &SPlayerDynamics::mass
		, 'pdym'
		, "mass"
		, "Mass"
		, "Player Mass in kilograms"
		, SDefaults::mass);

	desc.AddMember(
		  &SPlayerDynamics::air_control
		, 'pdyc'
		, "aircontrol"
		, "Air Control"
		, "How strong the Player Character can move in Air"
		, SDefaults::air_control);

	desc.AddMember(
		  &SPlayerDynamics::air_resistance
		, 'pdyr'
		, "airresistance"
		, "Air Resistance"
		, "How strong is the Air, this param prevent movement of the Player Character in the Air"
		, SDefaults::air_resistance);

	desc.AddMember(
		  &SPlayerDynamics::inertia
		, 'pdyi'
		, "deaccelerateinertia"
		, "De Accelerate Inertia"
		, "De Accelerate Inertia coefficient when Player Character stop his Movement"
		, SDefaults::inertia);

	desc.AddMember(
		  &SPlayerDynamics::accelerate_inertia
		, 'pdya'
		, "accelerateinertia"
		, "Accelerate Inertia"
		, "Accelerate Inertia coefficient when Player Character begin Walking"
		, SDefaults::accelerate_inertia);
}

inline bool SPlayerDynamics::operator==(const SPlayerDynamics& other) const
{
	return memcmp(this, &other, sizeof(other)) != 0;
}

inline bool SPlayerDynamics::operator!=(const SPlayerDynamics& other) const
{
	return memcmp(this, &other, sizeof(other)) != 0;
}

#endif // !PLAYER_DYNAMICS