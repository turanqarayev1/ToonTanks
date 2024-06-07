// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasedPawn.h"
#include "MyBasedPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AMyBasedPawn : public ABasedPawn
{
	GENERATED_BODY()
	

public:
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

protected:
	virtual void BeginPlay() override;

private:
	class ATank* Tank;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")	
	float FireRange = 700.f;

	FTimerHandle FireRateTimerHandle;   //FTimerHandle - zamanlayıcıları yönetmek için kullanılır, digeri oluşturulan zamanlayıcının referansını tutar ;
	float FireRate = 2.f;
	void CheckFireCondition();
	bool InFireRange();
};
