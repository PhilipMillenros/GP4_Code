#include "DashAttack.h"

#include "GP4Team3/Character/MovementComp.h"
#include "Kismet/GameplayStatics.h"


void DashAttack::AttackTick(AEnemyCharacter* target, ASlimePawn* player)
{
	if(!player->movementComp->IsDashing())
	{
		ExitAttack(target, player);
		return;
	}
	if(target == nullptr)
	{
		return;
	}
	
	TimeSinceAttackStarted += player->GetWorld()->DeltaTimeSeconds;
	FVector PlayerPosition = player->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	UClass* FilterClass = AActor::StaticClass();
	TArray<AActor*> ActorsIgnore;
	TArray<AActor*> OutEnemies;
	ActorsIgnore.Add(player);
	
	UKismetSystemLibrary::SphereOverlapActors(player->GetWorld(), PlayerPosition, DashRadius, traceObjectTypes, FilterClass, ActorsIgnore, OutEnemies);
	UKismetSystemLibrary::DrawDebugSphere(player->GetWorld(), PlayerPosition, DashRadius, 12, FLinearColor::White, 2);
	for(int i = 0; i < OutEnemies.Num(); i++)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OutEnemies[i]);
		
		if(Enemy != nullptr)
		{
			Enemy->KnockUp(DashForce, (Enemy->GetActorLocation() - PlayerPosition).GetSafeNormal() + FVector::UpVector);
			Enemy->TakeDamage(Damage);
			player->OnDashAttackHit();
		}
	}
	if(TimeSinceAttackStarted > ExitTime)
	{
		ExitAttack(target, player);
	}
}

void DashAttack::AttackStart(AEnemyCharacter* target, ASlimePawn* player)
{
	Attack::AttackStart(target, player);
	player->movementComp->InputPressDash();
	player->movementComp->InputReleaseDash();
	player->OnDashAttack();
}

void DashAttack::ExitAttack(AEnemyCharacter* target, ASlimePawn* player)
{
	Attack::ExitAttack(target, player);
}


bool DashAttack::IsValidAttack(AEnemyCharacter* target, ASlimePawn* player)
{
	if(target == nullptr || player == nullptr)
	{
		return false;
	}
	if(FVector::Distance(target->GetActorLocation(), player->GetActorLocation()) > Range)
	{
		return false;
	}
	TimeSinceAttackStarted += player->GetWorld()->DeltaTimeSeconds;
	if(TimeSinceAttackStarted > ExitTime)
	{
		return false;
	}
	return true;
}

