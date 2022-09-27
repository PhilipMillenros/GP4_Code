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
}

void UAutoTarget::SelectEnemy()
{
	UKismetSystemLibrary::PrintString(GetWorld(),  FString::Printf( TEXT("Enemies: , %i"), Enemies.Num()), true, true, FLinearColor::Green, 0.f);
	if(Enemies.Num() < 1)
	{
		CurrentTarget = nullptr;
		return;
	}
	float ShortestSqrDistance = MaxRange * MaxRange;
	float ClosestToCameraDirection = -1;
	AEnemyCharacter* PreviousTarget = CurrentTarget;
	for (int i = 0; i < Enemies.Num(); i++)
	{
		float SqrDistance = (SlimePawn->GetActorLocation() - Enemies[i]->GetActorLocation()).SquaredLength();
		if(SqrDistance < ShortestSqrDistance) 
		{
			//In range
			FVector CameraForward = Camera->GetForwardVector();
			FVector DirectionToEnemy = (Enemies[i]->GetActorLocation() - Camera->GetComponentLocation()).GetSafeNormal();
			float EnemyCameraDotProduct = FVector::DotProduct(CameraForward, DirectionToEnemy);
			if(EnemyCameraDotProduct > ClosestToCameraDirection)
			{
				
				ClosestToCameraDirection = EnemyCameraDotProduct;
				
				CurrentTarget = Enemies[i];
			}
		}
	}
	if(CurrentTarget != nullptr && CurrentTarget->Dead)
	{
		SlimePawn->OnRemoveTarget(CurrentTarget);
		Enemies.RemoveSingle(CurrentTarget);
		CurrentTarget = nullptr;
		PreviousTarget = nullptr;
	}
	if(CurrentTarget != nullptr && PreviousTarget != CurrentTarget)
	{
		if(PreviousTarget != nullptr)
		{
			SlimePawn->OnRemoveTarget(PreviousTarget);
		}
		SlimePawn->OnSetTarget(CurrentTarget);
	}
}


// Called every frame
void UAutoTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SelectEnemy();
}

