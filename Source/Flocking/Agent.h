// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "FlockingManager.h"
#include "Agent.generated.h"


UCLASS()
class FLOCKING_API AAgent : public AActor
{
	GENERATED_BODY()

public:
	AAgent();
	void Init(UStaticMeshComponent* mesh, int id, UFlockingManager* Manager);
	UStaticMeshComponent* Mesh;
	FVector Velocity;
	float MaxSpeed = 10;
	float MaxForce = 0.01;
	//UPROPERTY()
	 UFlockingManager* Manager;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void onHit();
};