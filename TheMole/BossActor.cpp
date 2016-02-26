#include "BossActor.h"
void BossActor::UpdateWorldState()
{
	_bossTree.UpdateVariables(_player->GetPosition(), _curKinematic.position, _health, _heat);
}