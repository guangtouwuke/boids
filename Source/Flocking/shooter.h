// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "shooter.generated.h"

UCLASS()
class FLOCKING_API Ashooter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Ashooter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* camera;
	UFUNCTION(BlueprintCallable)
	void Shoot();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(BlueprintCallable)

};
