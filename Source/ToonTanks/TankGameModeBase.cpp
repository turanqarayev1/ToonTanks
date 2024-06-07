// Fill out your copyright notice in the Description page of Project Settings.

#include "TankGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "MyBasedPawn.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank)   //eğer ölen aktör tank ise;
    {
        Tank->HandleDestruction();   //tanki oyun sahnesinden kaldirir;
        if(ToonTanksPlayerController)   //null deyilse;
        {
           ToonTanksPlayerController->SetPlayerEnabledState(false);   //Bu fonksiyon, oyuncunun kontrolünü devre dışı bırakır, böylece oyuncu artık karakteri kontrol edemez;
        }
        GameOver(false);
        UE_LOG(LogTemp, Warning, TEXT("You died"));
    }

    else if(AMyBasedPawn* DestroyedTower = Cast<AMyBasedPawn>(DeadActor))
    {
        DestroyedTower->HandleDestruction();   //Kuleni oyun sahnesinden kaldirir;

        TargetTowers--;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }
    }

    FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &ATankGameModeBase::BeginPlay);
}

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void ATankGameModeBase::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    
    StartGame();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));   //oyunun ilk oyuncusunun karakterini (genellikle ana karakter olarak kabul edilir) alır ve ATank tipine dönüştürmeye çalışır;
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));   //oyunun ilk oyuncusunun kontrolcüsünü alır ve AToonTanksPlayerController tipine dönüştürmeye çalışır;

    if(ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);
        
        FTimerHandle PlayerEnableTimerHandle; 
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject( 
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true 
        ); 
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }   
}

int32 ATankGameModeBase::GetTargetTowerCount()
{
    class TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, AMyBasedPawn::StaticClass(), Towers);
    return Towers.Num();
}