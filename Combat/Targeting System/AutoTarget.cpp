// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoTarget.h"

#include "AttackComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UAutoTarget::UAutoTarget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoTarget::BeginPlay()
{
	Super::BeginPlay();
	SlimePawn = Cast<ASlimePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	ensure(SlimePawn != nullptr);
	Camera = Cast<UCameraComponent>(SlimePawn->GetComponentByClass(UCameraComponent::StaticClass()));
	ensure(Camera != nullptr);
	AttackComponent = Cast<UAttackComponent>(SlimePawn->GetComponentByClass(UAttackComponent::StaticClass()));
	ensure(AttackComponent != nullptr);
}


// Called every frame
void UAutoTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float shortestDistance = MaxRange;
	AEnemyCharacter* enemy = nullptr;
	int length = Enemies.Num();
	for (int i = 0; i < length; i++)
	{
		if(Enemies[i] == nullptr)
		{
			Enemies.RemoveAt(i);
		}
	}
	for (int i = 0; i < Enemies.Num(); i++)
	{
		float distance = FVector::Distance(Enemies[i]->GetActorLocation(), SlimePawn->GetActorLocation());
		if(shortestDistance > distance)
		{
			enemy = Enemies[i];
			shortestDistance = distance;
		}
	}
	if(enemy != nullptr)
	{
		AttackComponent->CurrentTarget = enemy;
		UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), enemy->GetActorLocation(), 200, 90, FRotator::ZeroRotator, FLinearColor::Blue, 0.1f);
	}
}

