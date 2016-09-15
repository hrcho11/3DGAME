#include "stdafx.h"
#include "cMonsterOwnedStates.h"
#include "cMonster.h"

void cState_Run::Enter(cMonster * pMonster)
{
	pMonster->SetAnim(RUN);
}


void cState_Run::Execute(cMonster * pMonster)
{
	if (pMonster->SenseAtkPlayer())
	{
		pMonster->Attack();
	}
	else
		pMonster->Tracking();
}

void cState_Run::Exit(cMonster * pMonster)
{
}

void cState_Attack::Enter(cMonster * pMonster)
{
	pMonster->SetAnim(ATTACK);
}

void cState_Attack::Execute(cMonster * pMonster)
{
	if (!pMonster->SenseAtkPlayer() && !pMonster->GetSkinnedMesh()->GetIsAtk())
		pMonster->Run();
	else if (pMonster->SenseAtkPlayer() && !pMonster->GetSkinnedMesh()->GetIsAtk())
	{
		pMonster->Attack();
	}
	if (pMonster->GetSkinnedMesh()->AtkAnimationMatch())
		g_pSkillManager->Fire(pMonster->GetPos(), pMonster->GetDirection());
}

void cState_Attack::Exit(cMonster * pMonster)
{
}
