// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GP4Team3/Character/SlimePawn.h"
#include "GP4Team3/Enemy/EnemyCharacter.h"

class GP4TEAM3_API Attack
{
public:
	virtual ~Attack() = default;


	virtual void AttackStart(AEnemyCharacter* target, ASlimePawn* player);

	virtual void AttackTick(AEnemyCharacter* target, ASlimePawn* player);

	virtual void ExitAttack(AEnemyCharacter* target, ASlimePawn* player);
	
	
	bool bFinished = false;
};
