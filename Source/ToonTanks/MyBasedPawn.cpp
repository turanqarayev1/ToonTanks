// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasedPawn.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void AMyBasedPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }
}

void AMyBasedPawn::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy(); 
}

void AMyBasedPawn::BeginPlay()
{
    Super::BeginPlay();
    
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));   //oyuncunun kontrol ettiği tank karakterine erişmek ve onunla ilgili işlemler yapmak için kullanılır;
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AMyBasedPawn::CheckFireCondition, FireRate, true);   //Her iki saniyeden bir ates etmek ucun funksiya;
}

void AMyBasedPawn::CheckFireCondition()
{
    if(Tank == nullptr)
    {
        return;
    }
    if(InFireRange() && Tank->bAlive)
    {
       Fire();
    }
}

bool AMyBasedPawn::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        if(Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}
