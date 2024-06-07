// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasedPawn.generated.h"


UCLASS()
class TOONTANKS_API ABasedPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasedPawn();

	void HandleDestruction();   //Sadece Destroy ucun ist edecem, GameMode da cagira bilmem ucun public olur;

protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))   //Meta-burada bize BP(EventGraph)-de deyerleri gostermeye imkan verir, private to public kimi;
	class UCapsuleComponent* CapsuleComp;   //BP-deki Capsule componenti C++ da yaradir, class = ileri goturur;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class TSubclassOf<class AProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat");
	class UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, Category = "Combat");
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat");
	TSubclassOf<class UCameraShakeBase> DeadCameraShakeClass;
};
