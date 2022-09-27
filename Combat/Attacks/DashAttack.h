#pragma once
#include "Attack.h"

class DashAttack : public Attack
{
public:
	FVector OriginLocation;
	float Range = 3000.f;
	float BounceSpeed = 50.f;
	float BounceRange = 150.f;
	float HomingSpeed = 30.f;
	float Damage = 35.f;
	float Offset = 200.f;
	float TimeSinceAttackStarted = 0;
	float MaxSpeedTime = 0.15f;
	float ExitTime = 2.f;
	float DashRadius = 100.f;
	float DashForce = 350.f;
	bool bBouncing;
	
	virtual void AttackTick(AEnemyCharacter* target, ASlimePawn* player) override;
	
	virtual void AttackStart(AEnemyCharacter* target, ASlimePawn* player) override;
	
	virtual void ExitAttack(AEnemyCharacter* target, ASlimePawn* player) override;
	
	void SetDashSpeed(AEnemyCharacter* target, ASlimePawn* player);
	
	bool IsValidAttack(AEnemyCharacter* target, ASlimePawn* player);
	
	
};


