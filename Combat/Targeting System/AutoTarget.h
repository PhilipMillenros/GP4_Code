// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP4Team3/Character/SlimePawn.h"
#include "GP4Team3/Enemy/EnemyCharacter.h"
#include "AutoTarget.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4TEAM3_API UAutoTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAutoTarget();
	UPROPERTY()
	TArray<AEnemyCharacter*> Enemies = {};

	
	
	UPROPERTY()
	ASlimePawn* SlimePawn;

	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	UAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEnemyCharacter* CurrentTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SelectEnemy();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
