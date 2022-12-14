// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>

#include "CoreMinimal.h"
#include "Attack.h"
#include "AutoTarget.h"
#include "Components/ActorComponent.h"
#include "GP4Team3/Character/SlimePawn.h"

#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GP4TEAM3_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UAttackComponent();
	UPROPERTY()
	ASlimePawn* SlimePawn;

	UPROPERTY()
	UAutoTarget* AutoTarget;
	std::queue<std::shared_ptr<Attack>>AttackQueue;
	float CoolDownTime = 1.f;
	float CurrentCoolDown = 0.f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	bool AttackIsReady();
public:
	void QueueDashAttack();
	void QueueGroundSlam();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//void SetAttack();
};
