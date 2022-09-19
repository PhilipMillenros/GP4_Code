// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "DashAttack.h"
#include "GroundSlam.h"
#include "GP4Team3/Character/MovementComp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttackComponent::QueueDashAttack()
{
	Attack* attack = new DashAttack();
	AttackQueue.push(attack);
	attack->OnAttack(CurrentTarget, SlimePawn);
}

void UAttackComponent::QueueGroundSlam()
{
	Attack* attack = new GroundSlam();
	AttackQueue.push(attack);
	attack->OnAttack(CurrentTarget, SlimePawn);
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(AttackQueue.size() > 0)
	{
		if(!AttackQueue.front()->bFinished)
		{
			AttackQueue.front()->ExecuteAttack(CurrentTarget, SlimePawn);
		}
		else
		{
			AttackQueue.pop();
			SlimePawn->movementComp->active = true;
		}
	}
}


