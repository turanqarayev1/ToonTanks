// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
 
ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm -> SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &ATank::Move);   //Adindan ne oldugu bilinir, BindAxis-Basili tutuldugu ucun axis olur;
    PlayerInputComponent -> BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent -> BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TankPlayerController)
    {   
        //Asagidakilar oyuncunun imleciyle bir hedefi işaretlediğinde, bir taretin bu hedefe doğru otomatik olarak dönmesini sağlamak için kullanılır;
        FHitResult HitResult;
        TankPlayerController -> GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult);

        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);   //Hidden-Gizle;
    SetActorTickEnabled(false);   //Enable-Aktiv et, false-Tick funksiyasini deaktiv edir;
    bAlive = false;
}


void ATank::BeginPlay()
{
    Super::BeginPlay();
   
    TankPlayerController = Cast<APlayerController>(GetController());   //Bu kod genellikle, oyuncunun kontrol ettiği karaktere erişmek ve onun üzerinde işlemler yapmak için kullanılır;

}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;   //Vectoru sifir edir;
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);   //dünya zaman dilimindeki bir kare (frame) arasındaki süreyi saniye cinsinden döndürür. Bu, oyunun akış hızına bağlı olarak değişebilir.
    AddActorLocalOffset(DeltaLocation, true);   //Kuvvet ekleyir;
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}
