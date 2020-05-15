// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "FlockingGameModeBase.h"
#include "FlockingManager.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

AFlockingGameModeBase::AFlockingGameModeBase() {
	AgentMesh = CreateDefaultSubobject<UStaticMeshComponent>("AgentMesh");
	PrimaryActorTick.bCanEverTick = true;
}

void AFlockingGameModeBase::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GAMEMODE BEGINPLAY()"));
	Manager = NewObject<UFlockingManager>();
	Manager->AddToRoot();
	Manager->Init(GetWorld(), AgentMesh);
	//GetWorld()->GetTimerManager().ClearTimer(CountDownTimer);
	GetWorld()->GetTimerManager().SetTimer(CountDownTimer, this, &AFlockingGameModeBase::CountDown, 1.0f, true, 2.0f);

}

void AFlockingGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Manager->Flock();
};

void AFlockingGameModeBase::CountDown()
{
	time--;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Time Left is: %i"), time));

	if (time<=0){
		//GetWorld()->GetTimerManager().ClearTimer(Count);
		GetWorld()->GetTimerManager().SetTimer(CountDownTimer, this, &AFlockingGameModeBase::CountDown, 1.0f, true, 2.0f);
		time = 30;
		ConditionalBeginDestroy();
		Manager->Clean();
		Manager=nullptr;
		BeginPlay();
	}
}

UFlockingManager* AFlockingGameModeBase::getManager()
{
	return Manager;
}

