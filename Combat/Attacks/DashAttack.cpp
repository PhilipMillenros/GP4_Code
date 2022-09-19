#include "DashAttack.h"

#include "GP4Team3/Character/MovementComp.h"
#include "Kismet/GameplayStatics.h"


void DashAttack::ExecuteAttack(AEnemyCharacter* target, ASlimePawn* player)
{
	
	if(target == nullptr || player == nullptr)
	{
		ExitAttack();
		return;
	}
	if(FVector::Distance(target->GetActorLocation(), player->GetActorLocation()) > Range)
	{
		ExitAttack();
		return;
	}
	TimeSinceAttackStarted += player->GetWorld()->DeltaTimeSeconds;
	if(TimeSinceAttackStarted > ExitTime)
	{
		ExitAttack();
	}
	
	FVector Start = player->GetActorLocation();
	FVector End = target->GetActorLocation();
	float TotalSpeed = FMath::InterpEaseIn(0.f, Speed,  FMath::Clamp(TimeSinceAttackStarted / MaxSpeedTime, 0.f, 1.f), 2);
	player->SetActorLocation(Start + (End - Start).GetSafeNormal() * TotalSpeed * player->GetWorld()->DeltaTimeSeconds);
	if((End - Start).Length() < Offset)
	{
		target->TakeDamage(Damage);
		bBouncing = true;
		//target->FindComponentByClass<UPrimitiveComponent>()->AddImpulse((originLocation - player->GetActorLocation()).GetSafeNormal() * 50.f); Might use later
		ExitAttack();
	}
}

void DashAttack::OnAttack(AActor* target, ASlimePawn* player)
{
	player->movementComp->active = false;
	UKismetSystemLibrary::DrawDebugSphere(player->GetWorld(), player->GetActorLocation(), Range, 24, FLinearColor::Green, 1.f);
	originLocation = player->GetActorLocation();
}

void DashAttack::ExitAttack()
{
	Attack::ExitAttack();
	
}
