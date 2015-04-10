// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMesh.h"
#include "ProceduralCubeActor.h"
#include "CustomPlayerController.h"

ACustomPlayerController::ACustomPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bShowMouseCursor = true;
	
	EnableInput(this);

	KeepMoving = false;
	KeepExtrMovement = false;

	LMBMovementDirection = 0.0;
	RMBMovementDirection = 0.0;

	CubeToEdit = NULL;

}

// --------- --------- LEFT MOUSE BUTTON FUNCTIONS --------- ---------

void ACustomPlayerController::SetKeepMovingToTrue()
{
	KeepMoving = true;
}

void ACustomPlayerController::SetKeepMovingToFalse()
{
	KeepMoving = false;
}

void ACustomPlayerController::UpdateLMBMovementDirection(float value)
{
	LMBMovementDirection = value;
}

// --------- --------- RIGHT MOUSE BUTTON FUNCTIONS --------- ---------

void ACustomPlayerController::SetKeepExtrMovementToFalse()
{
	KeepExtrMovement = false;
}

void ACustomPlayerController::UpdateRMBMovementDirection(float value)
{
	RMBMovementDirection = value;
}

// --------- --------- TICK (override) --------- ---------

void ACustomPlayerController::Tick(float deltaSeconds){
	Super::Tick(deltaSeconds);

	// Left mouse button events
	InputComponent->BindAxis("XAxis", this, &ACustomPlayerController::UpdateLMBMovementDirection);
	InputComponent->BindAction("LeftMB", IE_Pressed, this, &ACustomPlayerController::SetKeepMovingToTrue);
	InputComponent->BindAction("LeftMB", IE_Released, this, &ACustomPlayerController::SetKeepMovingToFalse);

	// Right mouse button events
	InputComponent->BindAxis("RMBAxis", this, &ACustomPlayerController::UpdateRMBMovementDirection);
	//InputComponent->BindAction("RightMB", IE_Pressed, this, &ACustomPlayerController::SetKeepExtrMovementToTrue);
	InputComponent->BindAction("RightMB", IE_Released, this, &ACustomPlayerController::SetKeepExtrMovementToFalse);

}




// -------------- PROBLEMAS --------------

/*
void ACustomPlayerController::SetKeepExtrMovementToTrue()
{
	FHitResult DetectedHitResult;
	GetHitResultUnderCursorByChannel(visibility, true, &DetectedHitResult);
	KeepExtrMovement = true;
}
*/