#ifndef SENSOR
#define SENSOR

#include <CrySchematyc/CoreAPI.h>

struct SSensor
{
	struct SDefaults
	{
		inline static const auto position	= ZERO;
		inline static const auto direction	= ZERO;
		inline static const auto distance	= 1.0f;
	};

	explicit SSensor()
		: SSensor{ SDefaults::position, SDefaults::direction, SDefaults::distance }
	{}

	explicit SSensor(const Vec3& sensor_position, const Vec3& sensor_direction, float sensor_distance)
		: position{ sensor_position }
		, direction{ sensor_direction }
		, distance{ sensor_distance }
	{}

	bool operator==(const SSensor& rhs) const;
	bool operator!=(const SSensor& rhs) const;

	static void ReflectType(Schematyc::CTypeDesc<SSensor>& desc);

	void Serialize(Serialization::IArchive& archive);

	Vec3 position;
	Vec3 direction;
	Schematyc::PositiveFloat distance;
};

inline void SSensor::ReflectType(Schematyc::CTypeDesc<SSensor>& desc)
{
	desc.SetGUID("{AF064ABA-CE39-4C2F-BE60-F44DDE79F8A3}"_cry_guid);
	desc.AddMember(
		  &SSensor::position
		, 'pts'
		, "position"
		, "position"
		, "Position"
		, SDefaults::position);

	desc.AddMember(
		  &SSensor::direction
		, 'dir'
		, "direction"
		, "direction"
		, "Detection Ray Direction"
		, SDefaults::direction);

	desc.AddMember(
		  &SSensor::distance
		, 'dis'
		, "distance"
		, "distance"
		, "Detection Ray Distance"
		, SDefaults::distance);
}

inline void SSensor::Serialize(Serialization::IArchive& archive)
{
	archive(position, "sensorposition", "Position");
	archive(direction, "sensordirection", "Direction");
	archive(distance, "sensordistance", "Distance");
}

inline bool SSensor::operator==(const SSensor& rhs) const
{
	return memcmp(this, &rhs, sizeof(rhs)) != 0;
}

inline bool SSensor::operator!=(const SSensor& rhs) const
{
	return memcmp(this, &rhs, sizeof(rhs)) != 0;
}

#endif // !SENSOR