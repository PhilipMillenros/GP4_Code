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
	SlimePawn = Cast<ASlimePawn>( UGameplayStatics::GetPlayerPawn(this, 0));
	ensure(SlimePawn != nullptr);
	AutoTarget = Cast<UAutoTarget>(SlimePawn->GetComponentByClass(UAutoTarget::StaticClass()));
	ensure(AutoTarget != nullptr);
}

bool UAttackComponent::AttackIsReady()
{
	if(CurrentCoolDown > CoolDownTime)
	{
		return true;
	}
	return false;
}

void UAttackComponent::QueueDashAttack()
{
	if(!AttackIsReady())
	{
		return;
	}
	std::shared_ptr<Attack> attack = std::make_shared<DashAttack>();
	if(AttackQueue.size() < 1)
	{
		attack->AttackStart(AutoTarget->CurrentTarget, SlimePawn);
	}
	AttackQueue.push(attack);
	CurrentCoolDown = 0.f;
}

void UAttackComponent::QueueGroundSlam()
{
	if(AttackQueue.size() > 0)
	{
		std::shared_ptr< GroundSlam> test = std::static_pointer_cast<GroundSlam>(AttackQueue.front());
		if(test != nullptr)
		{
			AttackQueue.front()->ExitAttack(AutoTarget->CurrentTarget, SlimePawn);
			return;
		}
	}
	if(!AttackIsReady())
	{
		return;
	}
	std::shared_ptr<Attack> attack = std::make_shared<GroundSlam>();
	if(AttackQueue.size() < 1)
	{
		attack->AttackStart(AutoTarget->CurrentTarget, SlimePawn);
	}
	CurrentCoolDown = 0.f;
	AttackQueue.push(attack);
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentCoolDown += DeltaTime;
	if(AttackQueue.size() < 1)
	{
		return;
	}
	if(!AttackQueue.front()->bFinished)
	{
		AttackQueue.front()->AttackTick(AutoTarget->CurrentTarget, SlimePawn);
		return;
	}
	AttackQueue.pop();
	
	if(AttackQueue.size() > 0)
	{
		AttackQueue.front()->AttackStart(AutoTarget->CurrentTarget, SlimePawn);
	}
}


