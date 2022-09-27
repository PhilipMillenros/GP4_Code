#include "GroundSlam.h"

#include <GP4Team3/Character/DeformNodeHandeler.h>

#include "Engine/StaticMeshActor.h"
#include "GP4Team3/Character/DeformNodeComp.h"
#include "GP4Team3/Character/MovementComp.h"
#include "GP4Team3/Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void GroundSlam::AttackStart(AEnemyCharacter* target, ASlimePawn* player)
{
	if(player->movementComp->IsGrounded())
	{
		ExitAttack(target, player);
		return;
	}
	Attack::AttackStart(target, player);
	PreviousPosition = player->GetActorLocation();
	player->movementComp->SetMainForce(FVector::Zero());
	Timer = 0;
}

void GroundSlam::AttackTick(AEnemyCharacter* target, ASlimePawn* player)
{
	Attack::AttackTick(target, player);
	GroundSlamFall(player);
}
void GroundSlam::ExitAttack(AEnemyCharacter* target, ASlimePawn* player)
{
	Attack::ExitAttack(target, player);
}
void GroundSlam::Impact(ASlimePawn* player)
{
	player->OnGroundSlam();
	GroundSlamApplyDamageAndPhysics(player);
	ApplyDeformation(player);
}

void GroundSlam::GroundSlamFall(ASlimePawn* player)
{
	float DeltaTime = player->GetWorld()->DeltaTimeSeconds;
	Timer += DeltaTime;
	FallSpeed = FMath::Clamp(FallSpeed + Acceleration * DeltaTime, 0.f, MaxSpeed);
	player->SetActorLocation(player->GetActorLocation() + FVector::DownVector * FallSpeed * DeltaTime);
	FVector Start = PreviousPosition;
	FVector End = player->GetActorLocation() + FVector::DownVector * SweepLengthDownwards;

	//Sweep checking
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(player);
	FHitResult HitResult;
	player->GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(50), TraceParams);
	if(HitResult.bBlockingHit) 
	{
		//Ground hit
		Impact(player);
		player->SetActorLocation(PreviousPosition);
		ExitAttack(nullptr, player);
	}
	if(Timer > FallTimeLimit)
	{
		//Ground slam max fall time reached
		ExitAttack(nullptr, player);
	}
	PreviousPosition = player->GetActorLocation();
}

void GroundSlam::GroundSlamImpactPhysicsObjects(ASlimePawn* player)
{
	FVector PlayerPosition = player->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	
	UClass* FilterClass = AActor::StaticClass();
	TArray<AActor*> ActorsIgnore;
	TArray<AActor*> OutEnemies;
	ActorsIgnore.Add(player);
	
	UKismetSystemLibrary::SphereOverlapActors(player->GetWorld(), PreviousPosition, ImpactRadius, traceObjectTypes, FilterClass, ActorsIgnore, OutEnemies);
	for(int i = 0; i < OutEnemies.Num(); i++)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OutEnemies[i]);
		
		if(Enemy != nullptr)
		{
			Enemy->KnockUp(ImpactForce, FVector::UpVector + (Enemy->GetActorLocation() - PlayerPosition).GetSafeNormal());
			Enemy->TakeDamage(Damage);
			player->OnGroundSlamHit();
		}
	}
}

void GroundSlam::GroundSlamApplyDamageAndPhysics(ASlimePawn* player)
{
	FVector PlayerPosition = player->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	
	UClass* FilterClass = AActor::StaticClass();
	TArray<AActor*> ActorsIgnore;
	TArray<AStaticMeshActor*> StaticMeshActors; 
	TArray<AActor*> Actors;
	ActorsIgnore.Add(player);
	
	UKismetSystemLibrary::SphereOverlapActors(player->GetWorld(), PreviousPosition, ImpactRadius, traceObjectTypes, FilterClass, ActorsIgnore, Actors);
	UKismetSystemLibrary::DrawDebugSphere(player->GetWorld(), PreviousPosition, ImpactRadius, 12, FLinearColor::White, 2);
	for(int i = 0; i < Actors.Num(); i++)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actors[i]);
		
		if(Enemy != nullptr)
		{
			Enemy->KnockUp(ImpactForceEnemies, FVector::UpVector + (Enemy->GetActorLocation() - PlayerPosition).GetSafeNormal());
			Enemy->TakeDamage(Damage);
			player->OnGroundSlamHit();
			continue;
		}
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>( Actors[i]->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		if(PrimitiveComponent != nullptr)
		{
			FVector GroundSlamDirection = FVector::UpVector + (Actors[i]->GetActorLocation() - PlayerPosition).GetSafeNormal();
			PrimitiveComponent->AddImpulse(GroundSlamDirection * ImpactForce);
			PrimitiveComponent->AddAngularImpulseInRadians(GroundSlamDirection * ImpactForce * 30);
			player->OnGroundSlamHit();
		}
	}
}

void GroundSlam::ApplyDeformation(ASlimePawn* player)
{
	FVector PlayerPosition = player->GetActorLocation();
	UDeformNodeHandeler* DeformNodeHandler= Cast<UDeformNodeHandeler>( player->GetComponentByClass(UDeformNodeHandeler::StaticClass()));
	DeformNodeHandler->bSpringsActive = false;
	
	player->GetWorldTimerManager().SetTimer(FixedTickHandle, player, &ASlimePawn::ActivateDeformSpring, 0.1f, false, 0.15f);
	for (int i = 0; i < DeformNodeHandler->deformNodes.Num(); i++)
	{
		FVector NodeOrigin = player->GetActorRotation().RotateVector(DeformNodeHandler->deformNodes[i]->GetLocalStartPos()) + PlayerPosition;
		FVector NewPosition;
		if(!FMath::IsNearlyZero( DeformNodeHandler->deformNodes[i]->GetLocalStartPos().Z))
		{
			NewPosition = (NodeOrigin - PlayerPosition).GetSafeNormal() * ImpactRadius * 0.0002f;
		}
		else
		{
			NewPosition = (NodeOrigin - PlayerPosition).GetSafeNormal() * ImpactRadius;
		}
		DeformNodeHandler->deformNodes[i]->SetWorldLocation(NewPosition + PlayerPosition);
	}
}






