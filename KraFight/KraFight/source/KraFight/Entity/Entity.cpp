#include "Entity.h"
#include "KraFight/Context.h"
#include "KraFight/Physics/PhysicsManager.h"

using namespace kra;

void kra::Entity::OnCreated(const Context & Con)
{
	PhysicsObject Push;
	Push.SetSize({ 100k, 100k });
	Push.SetPosition({ 0k, 50k });
	PhysicsBody = Con.PhysicsObjects->Add(Push);
}

void kra::Entity::OnDestroyed(const Context & Con)
{
	Con.PhysicsObjects->Destroy(PhysicsBody);
}

Handle<PhysicsObject> kra::Entity::GetPhysicsBody() const
{
	return PhysicsBody;
}
