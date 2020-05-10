// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

		void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

	FVector Align(TArray<class AAgent*> Agents, AAgent* agent);

	FVector Separation(TArray<class AAgent*>Agents, AAgent* agent);
	FVector Cohesion(TArray<class AAgent*>Agents, AAgent* agent);
	float AlignWeight = 10;
	float SeparationWeight = 10;
	float CohesionWeight = 10;
	float localRadius = 3000;
	float MaxForce = 0.01;
private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;
};