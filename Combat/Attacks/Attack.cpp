// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"


void Attack::OnAttack(AActor* target, ASlimePawn* player)
{
}

void Attack::ExecuteAttack(AEnemyCharacter* target, ASlimePawn* player)
{
}

void Attack::ExitAttack()
{
	bFinished = true;
}
