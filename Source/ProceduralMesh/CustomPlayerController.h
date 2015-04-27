// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ProceduralMeshComponent.h"
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
	bool KeepMovingFace;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	bool KeepLeftMovingVertex;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	bool KeepRightMovingVertex;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	bool KeepExtrMovement;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	float LMBMovementDirection;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	float RMBMovementDirection;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	UStaticMeshComponent* LMB_SelectedArrow;

	UPROPERTY(BlueprintReadWrite, Category = "CustomPlayerController")
	UStaticMeshComponent* RMB_SelectedArrow;

	UPROPERTY(EditAnywhere, Category = "CustomPlayerController")
	class AProceduralCubeActor* LMB_CubeToEdit;

	UPROPERTY(EditAnywhere, Category = "CustomPlayerController")
	class AProceduralCubeActor* RMB_CubeToEdit;

	// --------- --------- LEFT MOUSE BUTTON FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void EnableKeepMovings(); // Sets to true KeepMovingFace or KeepMovingVertex

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void DisableKeepMovings(); // Sets to false KeepMovingFace or KeepMovingVertex

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void UpdateLMBMovementDirection(float value);

	// --------- --------- RIGHT MOUSE BUTTON FUNCTIONS
	
	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void SetKeepExtrMovementToTrue();

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void SetKeepExtrMovementToFalse();

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void UpdateRMBMovementDirection(float value);

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	void Refresh();

	// --------- --------- (override)

	void SetupInputComponent();
	//void Tick(float deltaSeconds) override;

	// AUX FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	FVector RecognizeArrowDirectionInWorld(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube);

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	UStaticMeshComponent* RecognizeSphereFromArrow(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube);

	UFUNCTION(BlueprintCallable, Category = "CustomPlayerController")
	FProceduralMeshVertex RecognizeVertexFromArrow(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube);


};
