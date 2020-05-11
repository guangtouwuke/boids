// Fill out your copyright notice in the Description page of Project Settings.


#include "shooter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineBaseTypes.h"
#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include "Agent.h"
#include "UObject/NoExportTypes.h"
#include "Projectile.h"

// Sets default values
Ashooter::Ashooter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera ->SetRelativeLocation(FVector(0, 0, 0));
	//ProjectileSpeed = 10.f;
}

// Called when the game starts or when spawned
void Ashooter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Ashooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Ashooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &Ashooter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &Ashooter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &Ashooter::Shoot);
}

void Ashooter::Shoot()
{
	//GetWorld()->GetNumPlayerControllers(0).
	
	/*FVector fireStartPoint = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector CameraLocation;
	CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector fireEndPoint = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector() * 5000 + fireStartPoint;
	//DrawDebugLine(GetWorld(), fireStartPoint, fireEndPoint, FColor::Red, false, 2.f, 100.f, 5.f);

	UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("shooted"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *fireStartPoint.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *fireEndPoint.ToString());*/


	//FRotator rotation = FRotator();
	//AAgent* agent = GetWorld()->SpawnActor<AAgent>(GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector() * 500 + fireStartPoint, rotation);
	
		FVector loc = GetActorLocation();
		loc.Z += 100.f;
		loc.Y += 50.f;

		AProjectile* a = GetWorld()->SpawnActor<AProjectile>(loc, GetActorRotation());

		a->Speed = 100.f;
}

