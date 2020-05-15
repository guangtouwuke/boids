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
	FVector Center(TArray<class AAgent*>AgentsArray, AAgent* agent);
	float AlignWeight = 10;
	float SeparationWeight = 50;
	float CohesionWeight = 10;
	//float CenterWeight = 50;
	float localRadius = 5000;
	float MaxForce = 0.005;
	float MaxSpeed = 30;
	TArray<class AAgent*> Agents;
	TArray<class AAgent*> getArray();
	int score = 0;
	void Clean();
private:
	UWorld* World;
	bool initialized;
	
};