// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALMESH_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	ACustomPlayerController(const FObjectInitializer& ObjectInitializer);

public:

	// --------- --------- VARIABLES

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	bool KeepMoving;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	bool KeepExtrMovement;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	float LMBMovementDirection;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	float RMBMovementDirection;

	UPROPERTY(EditAnywhere, Category = "CustomPlayerController")
	class AProceduralCubeActor* CubeToEdit;

	// --------- --------- LEFT MOUSE BUTTON FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void SetKeepMovingToTrue();

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void SetKeepMovingToFalse();

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void UpdateLMBMovementDirection(float value);

	// --------- --------- RIGHT MOUSE BUTTON FUNCTIONS
	


	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void SetKeepExtrMovementToFalse();

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void UpdateRMBMovementDirection(float value);

	// --------- --------- TICK (override)

	void Tick(float deltaSeconds) override;




	// ---------------------- PROBLEMAS ----------------------
	
	//UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	//void SetKeepExtrMovementToTrue();

};
