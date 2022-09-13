#ifndef IDAMAGEABLE
#define IDAMAGEABLE

#include <CryEntitySystem/IEntityComponent.h>

struct IDamageable : IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IDamageable>& desc);

	virtual void take_damage(const EntityId entity_id) = 0;
};

inline void IDamageable::ReflectType(Schematyc::CTypeDesc<IDamageable>& desc)
{
	desc.SetGUID("{5BF7CC48-9CC4-4FE8-9E7A-4B064A6E68FA}"_cry_guid);
}

#endif // !IDAMAGEABLE