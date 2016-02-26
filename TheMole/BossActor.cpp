#include "BossActor.h"
void BossActor::Update()
{
	_bossTree.UpdateVariables(_player->GetPosition(), _curKinematic.position, _health, _heat);
	_bossTree.ExecuteTree();
}