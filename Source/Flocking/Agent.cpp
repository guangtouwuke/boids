// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "FlockingManager.h"
#include "Engine/Engine.h"
#include "Containers/UnrealString.h"

AAgent::AAgent() {
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AgentMesh"));
	RootComponent = Mesh;
	Velocity = FVector(0.f);
}

void AAgent::BeginPlay() {
	Super::BeginPlay();
}

void AAgent::Init(UStaticMeshComponent* mesh, int id, UFlockingManager* InitManager) {
	UE_LOG(LogTemp, Warning, TEXT("Agent initialized."));
	Mesh->SetStaticMesh(mesh->GetStaticMesh());
	Manager = InitManager;
}

void AAgent::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//FVector shooterLocation = FVector(125, 10, -84);
	FVector loc = GetActorLocation();
	SetActorLocation(loc + Velocity);
	
}

void AAgent::onHit()
{
	
	//TArray<class AAgent*> Agents = Manager->Agents;
	for(int i = 0; i< Manager->Agents.Num(); i++){
		float dist = FVector::Dist(GetActorLocation(), Manager->Agents[i]->GetActorLocation());

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Velocity %d is: %s"), GetActorLocation().ToString()));
		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My ball location %d is: %s"), i, *(Manager->Agents[i]->GetActorLocation().ToString())));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My target location %d is: %s"), i, *(GetActorLocation().ToString())));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My distance  is: %f"), dist));
		*/
		if ((dist <= 5000) && (dist > 0)){
			
			FVector force = FVector::ZeroVector;
			force = Manager->Agents[i]->GetActorLocation() - this->GetActorLocation();
			if (force.Size() > MaxForce) {
				force.Normalize(2);
				force *= MaxForce;

			}
			UE_LOG(LogTemp, Warning, TEXT("someOne pushed away"));
			Manager->Agents[i]->Velocity+=force*5000;

		}
			
		
	
	}
	Manager->score++;
	Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Current Score is: %i"), Manager->score));
}

