#include "Entity.h"
#include "KraFight/Physics/PhysicsManager.h"

using namespace kra;

void kra::Entity::OnCreated(const EntContext & Con)
{
	PhysicsObject Push;
	Push.SetSize({ 100_k, 100_k });
	Push.SetPosition({ 0_k, 50_k });
	PhysicsBody = Con.PhysicsObjects->Add(Push);
}

void kra::Entity::OnDestroyed(const EntContext & Con)
{
	Con.PhysicsObjects->Destroy(PhysicsBody);
}

Handle<PhysicsObject> kra::Entity::GetPhysicsBody() const
{
	return PhysicsBody;
}
