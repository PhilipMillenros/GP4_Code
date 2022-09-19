#include "GroundSlam.h"

#include "GP4Team3/Character/MovementComp.h"
#include "GP4Team3/Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void GroundSlam::OnAttack(AActor* target, ASlimePawn* player)
{
	if(player->movementComp->IsGrounded())
	{
		ExitAttack();
		return;
	}
	Attack::OnAttack(target, player);
	player->movementComp->active = false;
	PreviousPosition = player->GetActorLocation();
	player->movementComp->SetMainForce(FVector::Zero());
	timer = 0;
	
}

void GroundSlam::ExecuteAttack(AEnemyCharacter* target, ASlimePawn* player)
{
	Attack::ExecuteAttack(target, player);
	timer += player->GetWorld()->DeltaTimeSeconds;
	player->SetActorLocation(player->GetActorLocation() + FVector::DownVector * fallSpeed * player->GetWorld()->DeltaTimeSeconds);
	FVector Start = PreviousPosition;
	FVector End = player->GetActorLocation() + FVector::DownVector * sweepLengthDownwards;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(player);
	FHitResult HitResult;
	player->GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(10), TraceParams);
	if(HitResult.bBlockingHit)
	{
		Impact(player);
		ExitAttack();
	}
	if(timer > fallTimeLimit)
	{
		ExitAttack();
	}
	PreviousPosition = player->GetActorLocation();
}

void GroundSlam::Impact(ASlimePawn* player)
{
	FVector PlayerPosition = player->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	UClass* filterClass = AEnemyCharacter::StaticClass();
	TArray<AActor*> actorsIgnore;
	TArray<UPrimitiveComponent*> outPhysicsObjects;
	TArray<AEnemyCharacter*> outEnemies;
	actorsIgnore.Add(player);
	UKismetSystemLibrary::SphereOverlapComponents(player->GetWorld(), player->GetActorLocation(), impactRadius, traceObjectTypes, filterClass, actorsIgnore, outPhysicsObjects);
	//UKismetSystemLibrary::SphereOverlapActors(player->GetWorld(), player->GetActorLocation(), impactRadius, traceObjectTypes, filterClass, actorsIgnore, outEnemies);
	//for (int i = 0; i < .Num(); i++)
	//{
	//	//outPhysicsObjects[i]->AddImpulse((outPhysicsObjects[i]->GetOwner()->GetActorLocation() - PlayerPosition).GetSafeNormal() * impactForce + FVector::UpVector * impactForce / 2);
	//}
	UKismetSystemLibrary::DrawDebugSphere(player->GetWorld(), PlayerPosition, impactRadius, 12, FLinearColor::White, 2);
}

