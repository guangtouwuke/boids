// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"
#include "Components/StaticMeshComponent.h"
#include "Agent.h"
#include "Chaos/Array.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "UObject/NoExportTypes.h"
#define AGENT_COUNT 15

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
	UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));

	World = world;
	float incr = (PI * 2.f) / AGENT_COUNT;
	for (int i = 0; i < AGENT_COUNT; i++) {
		if (World != nullptr) {
			FRotator rotation = FRotator();
			FRandomStream rs = FRandomStream();
			FVector location = FVector();
			location.X = FMath::Sin(incr * i) * 1500.f;
			location.Y = FMath::Cos(incr * i) * 1500.f;
			location.Z = FMath::FRandRange(0, 1500.f);

			AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
			agent->Init(mesh, i);
			Agents.Add(agent);
		}
	}

	initialized = true;
}

void UFlockingManager::Flock() {
	//UE_LOG(LogTemp, Warning, TEXT("flocking"));
	
	for (int i = 0; i < AGENT_COUNT; i++) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Velocity is: %s"), *Agents[i]->Velocity.ToString()));
		Agents[i]->Velocity += Align(Agents, Agents[i]) * AlignWeight;
		//Agents[i]->Velocity = Separation(Agents,Agents[i])* SeparationWeight;
		//Agents[i]->Velocity = Cohesion(Agents,Agents[i])* CohesionWeight;
		//Agents[i]->Velocity = /*Align(Agents, Agents[i]) * AlignWeight+*/ Separation(Agents, Agents[i]) * SeparationWeight+ Cohesion(Agents, Agents[i]) * CohesionWeight;
	}

}


FVector UFlockingManager::Align(TArray<class AAgent*> AgentsArray, AAgent * agent) {
	FVector target = FVector();// = FVector();
	int localCount = 0;
	for (int i = 0; i < AGENT_COUNT; i++) {
		float distance = FVector::Dist(agent->GetActorLocation(), AgentsArray[i]->GetActorLocation());
		//target = AgentsArray[i]->Velocity;
		if ((distance <= localRadius) && (AgentsArray[i] != agent)) {
			localCount++;
			target += (AgentsArray[i]->Velocity);// /(distance / 500);
			
		}
	}
	
	/*target.X = (xAxis - (agent->Velocity.X)) / (AGENT_COUNT - 1);
	target.Y = (yAxis - (agent->Velocity.Y)) / (AGENT_COUNT - 1);*/
	if (localCount > 0) {
		target /= localCount;
	}
	//target.Normalize(1);
	if (FVector::Dist(target, FVector())>MaxForce) {
		target.Normalize(1);
		target *= MaxForce;
	
	}
	//agent->Velocity = target;
	return target;
}

FVector UFlockingManager::Separation(TArray<class AAgent*>AgentsArray, AAgent* agent) {
	FVector target = FVector(0,0,0);// = FVector();
	/*float xAxis = 0;// FVector();
	float yAxis = 0;// FVector();*/
	int localCount = 0;
	for (int i = 0; i < AGENT_COUNT; i++) {
		float distance = FVector::Dist(agent->GetActorLocation(), AgentsArray[i]->GetActorLocation());
		/*xAxis += agent->GetActorLocation().X - AgentsArray[i]->GetActorLocation().X;
		yAxis += agent->GetActorLocation().Y - AgentsArray[i]->GetActorLocation().Y;*/
		if ((distance <= localRadius) && (AgentsArray[i]!=agent)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My force is: %f"), distance));
			localCount++;
			FVector force = (agent->GetActorLocation() - AgentsArray[i]->GetActorLocation());
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My force is: %s"), *force.ToString()));
			force.Normalize(1);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My force is: %s"), *force.ToString()));
			target += force; /// distance;
		}
	}
	if (localCount > 0) {
		target /= (localCount);
	}
	//target.Normalize(1);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My target is: %s"), *target.ToString()));
	return target;
	//return FVector(0.1,0.1,0.1);
}

FVector UFlockingManager::Cohesion(TArray<class AAgent*>AgentsArray, AAgent* agent) {

	FVector target = FVector();
	int localCount = 0;
	for (int i = 0; i < AGENT_COUNT; i++) {
		float distance = FVector::Dist(agent->GetActorLocation(), AgentsArray[i]->GetActorLocation());
		//target = AgentsArray[i]->GetActorLocation();
		if ((distance <= localRadius) && (AgentsArray[i] != agent)) {
			target += (AgentsArray[i]->GetActorLocation());// / (distance / localRadius);
			localCount++;
		}
	}
	//target =(target-(agent->GetActorLocation())) /(AGENT_COUNT-1);
	if (localCount > 0) {
		target /= localCount;
	}
	//FVector force = target-agent->GetActorLocation();
	//force /= (distance / localRadius);
	target = target - agent->GetActorLocation();
	//force -= agent->Velocity;
	target.Normalize(1);
	//agent->Velocity = target;
	
	return target;
}