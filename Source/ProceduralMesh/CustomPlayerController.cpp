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

	LMB_CubeToEdit = NULL;
	LMB_SelectedArrow = NULL;

	RMB_CubeToEdit = NULL;
	RMB_SelectedArrow = NULL;

}

// --------- --------- LEFT MOUSE BUTTON FUNCTIONS --------- ---------

void ACustomPlayerController::SetKeepMovingToTrue()
{
	FHitResult HitRes;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);
	
	LMB_CubeToEdit = Cast<AProceduralCubeActor>(HitRes.GetActor());
	LMB_SelectedArrow = Cast<UStaticMeshComponent>(HitRes.GetComponent());

	if (LMB_CubeToEdit != NULL && LMB_SelectedArrow != NULL) // Successfully cast
	{
		KeepMoving = true;
		Refresh();
	}
}

void ACustomPlayerController::SetKeepMovingToFalse()
{
	KeepMoving = false;
	LMB_SelectedArrow = NULL;
	LMB_CubeToEdit = NULL;
}

void ACustomPlayerController::UpdateLMBMovementDirection(float value)
{
	LMBMovementDirection = value;
}

// --------- --------- RIGHT MOUSE BUTTON FUNCTIONS --------- ---------

void ACustomPlayerController::SetKeepExtrMovementToTrue()
{

	FHitResult HitRes;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);

	RMB_CubeToEdit = Cast<AProceduralCubeActor>(HitRes.GetActor());
	RMB_SelectedArrow = Cast<UStaticMeshComponent>(HitRes.GetComponent());

	if (RMB_CubeToEdit != NULL && RMB_SelectedArrow != NULL) // Successfully cast
	{
		KeepExtrMovement = true;
		
		AProceduralCubeActor* ExtrudedCube = RMB_CubeToEdit->ExtrudeFaceOfCube(RMB_SelectedArrow);
		
		// Update RMB_SelectedArrow (it should be moved the one from the new extruded cube)
		TArray<FProceduralMeshVertex> FaceVertexes = RMB_CubeToEdit->FindFaceVertexesFromArrowLocation(RMB_SelectedArrow->RelativeLocation);
		int32 IndexOf_RMB_SelectedArrow = RMB_CubeToEdit->IdentifyFaceFromVertexes(FaceVertexes[0], FaceVertexes[1], FaceVertexes[2], FaceVertexes[3]);
		switch (IndexOf_RMB_SelectedArrow)
		{
			case 0: RMB_SelectedArrow = ExtrudedCube->FrontFaceArrow; break;
			case 1: RMB_SelectedArrow = ExtrudedCube->BackFaceArrow; break;
			case 2: RMB_SelectedArrow = ExtrudedCube->LeftFaceArrow; break;
			case 3: RMB_SelectedArrow = ExtrudedCube->RightFaceArrow; break;
			case 4: RMB_SelectedArrow = ExtrudedCube->TopFaceArrow; break;
			case 5: RMB_SelectedArrow = ExtrudedCube->BottomFaceArrow; break;
		}

		// Update RMB_CubeToEdit (it should be moved the new extruded cube)
		RMB_CubeToEdit = ExtrudedCube;
		
		Refresh();
	}
}

void ACustomPlayerController::SetKeepExtrMovementToFalse()
{
	KeepExtrMovement = false;
	RMB_SelectedArrow = NULL;
	RMB_CubeToEdit = NULL;
}

void ACustomPlayerController::UpdateRMBMovementDirection(float value)
{
	RMBMovementDirection = value;
}

// --------- --------- (override) --------- ---------

void ACustomPlayerController::SetupInputComponent(/*class UInputComponent * InputComponent*/)
{
	Super::SetupInputComponent();
	check(InputComponent);
	
	InputComponent->BindAxis("XAxis", this, &ACustomPlayerController::UpdateLMBMovementDirection);
	InputComponent->BindAxis("RMBAxis", this, &ACustomPlayerController::UpdateRMBMovementDirection);

	InputComponent->BindAction("LeftMB", IE_Pressed, this, &ACustomPlayerController::SetKeepMovingToTrue);
	InputComponent->BindAction("LeftMB", IE_Released, this, &ACustomPlayerController::SetKeepMovingToFalse);

	InputComponent->BindAction("RightMB", IE_Pressed, this, &ACustomPlayerController::SetKeepExtrMovementToTrue);
	InputComponent->BindAction("RightMB", IE_Released, this, &ACustomPlayerController::SetKeepExtrMovementToFalse);
}

void ACustomPlayerController::Refresh()
{
	if (!(KeepMoving) && !(KeepExtrMovement)) { return; }

	if (KeepMoving) { LMB_CubeToEdit->MoveFace(LMBMovementDirection, LMB_CubeToEdit->FindFaceVertexesFromArrowLocation(LMB_SelectedArrow->RelativeLocation), LMB_SelectedArrow); }
	if (KeepExtrMovement) { RMB_CubeToEdit->MoveFace(RMBMovementDirection, RMB_CubeToEdit->FindFaceVertexesFromArrowLocation(RMB_SelectedArrow->RelativeLocation), RMB_SelectedArrow); }

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACustomPlayerController::Refresh);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 0.05f, false);
}