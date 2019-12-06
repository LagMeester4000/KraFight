#include "KraFight/Entity/Entity.h"
#include "KraFight/Physics/PhysicsManager.h"

using namespace kra;

void kra::Entity::OnCreated(const Context& Con, Handle<Entity> Self)
{
	PhysicsObject Push(Self);
	Push.SetSize({ 100_k, 100_k });
	Push.SetPosition({ 0_k, 50_k });
	Push.SetOwner(Self);
	PhysicsBody = Con.PhysicsObjects->Add(Push);
}

void kra::Entity::OnDestroyed(const Context& Con, Handle<Entity> Self)
{
	Con.PhysicsObjects->Destroy(PhysicsBody);
}

Handle<PhysicsObject> kra::Entity::GetPhysicsBody() const
{
	return PhysicsBody;
}
