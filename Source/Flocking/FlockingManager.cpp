// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"
#include "Components/StaticMeshComponent.h"
#include "Agent.h"
#include "Chaos/Array.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#define AGENT_COUNT 30

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
			agent->Init(mesh, i, this);
			Agents.Add(agent);
		}
	}

	initialized = true;
}

void UFlockingManager::Flock() {
	//UE_LOG(LogTemp, Warning, TEXT("flocking"));

	for (int i = 0; i < AGENT_COUNT; i++) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Velocity %d is: %s"), i, *(Agents[i]->Velocity.ToString())));
		//Agents[i]->Velocity += Align(Agents, Agents[i]) * AlignWeight;
		//Agents[i]->Velocity += FVector(0.1, 0, 0);
		//Agents[i]->Velocity += Separation(Agents, Agents[i]) * SeparationWeight;
		//Agents[i]->Velocity = Cohesion(Agents,Agents[i])* CohesionWeight;
		//Agents[i]->Velocity 
		Agents[i]->Velocity += Align(Agents, Agents[i]) * AlignWeight+ Separation(Agents, Agents[i]) * SeparationWeight+ Cohesion(Agents, Agents[i]) * CohesionWeight;// + Center(Agents,Agents[i])*CenterWeight;
		if (Agents[i]->Velocity.Size()>MaxSpeed)
		{
			Agents[i]->Velocity.Normalize(1);
			Agents[i]->Velocity*=MaxSpeed;
		}
		FVector shooterLocation = FVector(125, 10, -84);
		float distance = FVector::Dist(Agents[i]->GetActorLocation(), shooterLocation);

		if (distance >= 10000){
			/*FVector back = FVector::ZeroVector;
			back =  (Agents[i]->GetActorLocation()-shooterLocation);
			back.Normalize(10);
			Agents[i]->Velocity = back;*/
			Agents[i]->Velocity*= -1;
		
		}


	}

}


FVector UFlockingManager::Align(TArray<class AAgent*> AgentsArray, AAgent* agent) {
	FVector target = FVector::ZeroVector;
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
	if (target.Size() > MaxForce) {
		target.Normalize(1);
		target *= MaxForce;

	}
	//agent->Velocity = target;
	//return FVector(0.01, 0.01, 0.1);
	return target;
}

FVector UFlockingManager::Separation(TArray<class AAgent*>AgentsArray, AAgent* agent) {
	FVector target = FVector::ZeroVector;// = FVector();
	/*float xAxis = 0;// FVector();
	float yAxis = 0;// FVector();*/
	int localCount = 0;
	for (int i = 0; i < AGENT_COUNT; i++) {
		float distance = FVector::Dist(agent->GetActorLocation(), AgentsArray[i]->GetActorLocation());
		/*xAxis += agent->GetActorLocation().X - AgentsArray[i]->GetActorLocation().X;
		yAxis += agent->GetActorLocation().Y - AgentsArray[i]->GetActorLocation().Y;*/
		if ((distance <= localRadius) && (AgentsArray[i] != agent) && distance != 0)
		{
			localCount++;
			FVector force = (agent->GetActorLocation() - AgentsArray[i]->GetActorLocation());
			force.Normalize(1);
			force /= distance;
			target += force; /// distance;
		}
	}
	if (localCount > 0) {
		target /= (localCount);
	}
	if (target.Size() > MaxForce) {
		target.Normalize(1);
		target *= MaxForce;

	}
	//target.Normalize(1);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My target is: %s"), *target.ToString()));
	return target;
	//return FVector(0.1,0.1,0.1);
}

FVector UFlockingManager::Cohesion(TArray<class AAgent*>AgentsArray, AAgent* agent) {

	FVector target = FVector::ZeroVector;
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
		target /= (localCount);
	}
	//FVector force = target-agent->GetActorLocation();
	//force /= (distance / localRadius);
	target = target - agent->GetActorLocation();

	if (target.Size() > MaxForce) {
		target.Normalize(1);
		target *= MaxForce;

	}
	return target;
}

FVector UFlockingManager::Center(TArray<class AAgent*>AgentsArray, AAgent* agent) {

	FVector shooterLocation = FVector(125,10,-84);
	FVector target = FVector::ZeroVector;
	int localCount = 0;
	/*for (int i = 0; i < AGENT_COUNT; i++) {
		float distance = FVector::Dist(agent->GetActorLocation(), AgentsArray[i]->GetActorLocation());
		//target = AgentsArray[i]->GetActorLocation();
		if ((distance <= localRadius) && (AgentsArray[i] != agent)) {
			target += (AgentsArray[i]->GetActorLocation());// / (distance / localRadius);
			localCount++;
		}
	}*/
	//target =(target-(agent->GetActorLocation())) /(AGENT_COUNT-1);
	if (localCount > 0) {
		target /= (localCount);
	}

	float distance = FVector::Dist(agent->GetActorLocation(), shooterLocation);

	//FVector force = target-agent->GetActorLocation();
	//force /= (distance / localRadius);
	if (distance >= 5000.f){
		
		target = (shooterLocation - agent->GetActorLocation()) * distance/1000;
		
	}else{
	
		target = (shooterLocation - agent->GetActorLocation()) / 5000 * distance;
		

	}
	
	//target = target - agent->GetActorLocation();


	if (target.Size() > MaxForce) {
		target.Normalize(2);
		target *= MaxForce;

	}
	
	return target;
}

TArray<class AAgent*> UFlockingManager::getArray()
{
	return Agents;
}

void UFlockingManager::Clean()
{
	for (int i= 0; i < Agents.Num(); i++) {
		Agents[i]->Destroy();
	}
}
