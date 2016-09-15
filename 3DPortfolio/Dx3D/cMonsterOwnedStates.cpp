#include "stdafx.h"
#include "cMonsterOwnedStates.h"
#include "cMonster.h"

void cState_Track::Enter(cMonster * pMonster)
{
	pMonster->SetAnim(RUN);
}


void cState_Track::Execute(cMonster * pMonster)
{
	if (pMonster->SenseAtkPlayer())
		pMonster->ChangeToAttack();
	else
		pMonster->Tracking();
}

void cState_Track::Exit(cMonster * pMonster)
{
}

void cState_Attack::Enter(cMonster * pMonster)
{
	pMonster->SetAnim(ATTACK);
}

void cState_Attack::Execute(cMonster * pMonster)
{
	if (pMonster->GetSkinnedMesh()->IsAnimComplete())
	{
		if (pMonster->GetSkinnedMesh()->GetIsCoolTime())
		{
			if (!pMonster->SenseAtkPlayer())
				pMonster->ChangeToTrack();
			else
				pMonster->ChangeToAvoid();
		}
		else
			pMonster->ChangeToAttack();
	}
}

void cState_Attack::Exit(cMonster * pMonster)
{
}

void cState_Avoid::Enter(cMonster * pMonster)
{
	pMonster->SetAnim(RUN);
}

void cState_Avoid::Execute(cMonster * pMonster)
{
	if (!pMonster->GetSkinnedMesh()->GetIsCoolTime())
		pMonster->ChangeToAttack();
	else
		pMonster->KeepDistance();
}

void cState_Avoid::Exit(cMonster * pMonster)
{

}

void cGlobalState_Monster::Execute(cMonster * pMonster)
{
	if (pMonster->GetSkinnedMesh()->GetIsCoolTime())
	{
		if (pMonster->GetSkinnedMesh()->AtkAnimationMatch())
			g_pSkillManager->Fire(pMonster->GetPos(), pMonster->GetDirection());
	}
}
