// Fill out your copyright notice in the Description page of Project Settings.


#include "BasedPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Templates/SubclassOf.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasedPawn::ABasedPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;   //Unreal Engine’de bir aktörün her çerçeve güncellenip güncellenmeyeceğini belirler;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));   //CapsuleComponent yaradir, bir AltObject(Subobject) kimi yaradir;
	RootComponent = CapsuleComp;   //CapsuleComp'un RootComponent(main) oldugunu belirtdik;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh -> SetupAttachment(CapsuleComp);   //Oluşturulan BaseMesh nesnesini CapsuleComp bileşenine bağlar. SetupAttachment-bir bileşeni diğerine bağlamak için kullanılır;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh -> SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint -> SetupAttachment(TurretMesh);

}

void ABasedPawn::HandleDestruction()
{
	if(DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());   //Particle Spawn edir;
	}

	if(DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}

	if(DeadCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeadCameraShakeClass);
	}
}


void ABasedPawn::RotateTurret(FVector LookAtTarget)   //Tower'u dondurur;
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(   //bir top kulesinin (turret) hedefe doğru dönmesini sağlamak için kullanılır;
	FMath::RInterpTo(
	   TurretMesh->GetComponentRotation(), 
	   LookAtRotation, 
	   UGameplayStatics::GetWorldDeltaSeconds(this), 
	   25.f)
	); 	
}

void ABasedPawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation); 
	if(Projectile)
	{
		Projectile->SetOwner(this);
	} 
}
