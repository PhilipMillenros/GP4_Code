#pragma once
#include "Attack.h"

class GroundSlam : public Attack
{
	float FallSpeed = 4000.f;
	float MaxSpeed = 6000.f;
	float Acceleration = 2000.f;
	float SweepLengthDownwards = 45.f;
	float FallTimeLimit = 10.f;
	float Timer = 0.f;
	float ImpactRadius = 450.f;
	float ImpactForce = 25000.f;
	float ImpactForceEnemies = 750;
	float Damage = 35.f;
	float SlimeDeformation = 450.f;
	FVector PreviousPosition;
	
	FTimerHandle FixedTickHandle;
	virtual void AttackStart(AEnemyCharacter* target, ASlimePawn* player) override;
	
	virtual void AttackTick(AEnemyCharacter* target, ASlimePawn* player) override;
	
	virtual void ExitAttack(AEnemyCharacter* target, ASlimePawn* player) override;
	
	void GroundSlamImpactPhysicsObjects(ASlimePawn* player);

	void GroundSlamApplyDamageAndPhysics(ASlimePawn* player);

	void ApplyDeformation(ASlimePawn* player);
	
	void Impact(ASlimePawn* player);

	void GroundSlamFall(ASlimePawn* player);
};

