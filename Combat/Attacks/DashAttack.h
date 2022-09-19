#pragma once
#include "Attack.h"

class DashAttack : public Attack
{
public:
	bool bBouncing;
	virtual void ExecuteAttack(AEnemyCharacter* target, ASlimePawn* player) override;
	virtual void OnAttack(AActor* target, ASlimePawn* player) override;
	virtual void ExitAttack() override;
	FVector originLocation;
	float Range = 3000.f;
	float BounceSpeed = 0.8f;
	float Speed = 2500.f;
	float Damage = 35.f;
	float Offset = 300.f;
	float TimeSinceAttackStarted = 0;
	float MaxSpeedTime = 0.6f;
	float ExitTime = 10.f;
};
