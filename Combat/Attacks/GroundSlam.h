#pragma once
#include "Attack.h"

class GroundSlam : public Attack
{

	virtual void OnAttack(AActor* target, ASlimePawn* player) override;
	virtual void ExecuteAttack(AEnemyCharacter* target, ASlimePawn* player) override;
	void Impact(ASlimePawn* player);
	float fallSpeed = 2000.f;
	float sweepLengthDownwards = 45.f;
	float fallTimeLimit = 10.f;
	float timer = 0.f;
	float impactRadius = 450.f;
	float impactForce = 45000.f;
	float damage = 65.f;
	FVector PreviousPosition;
};
