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

	KeepMovingFace = false;
	KeepExtrMovement = false;
	KeepLeftMovingVertex = false;
	KeepRightMovingVertex = false;

	LMBMovementDirection = 0.0;
	RMBMovementDirection = 0.0;

	LMB_CubeToEdit = NULL;
	LMB_SelectedArrow = NULL;

	RMB_CubeToEdit = NULL;
	RMB_SelectedArrow = NULL;

}

// --------- --------- LEFT MOUSE BUTTON FUNCTIONS --------- ---------

void ACustomPlayerController::EnableKeepMovings()
{

	FHitResult HitRes;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);
	
	LMB_CubeToEdit = Cast<AProceduralCubeActor>(HitRes.GetActor());
	LMB_SelectedArrow = Cast<UStaticMeshComponent>(HitRes.GetComponent());

	if (LMB_CubeToEdit != NULL && LMB_SelectedArrow != NULL) // Successfully cast
	{
		if (LMB_SelectedArrow->ComponentHasTag(TEXT("FaceArrow")))
		{
			KeepMovingFace = true;
			Refresh();
		}
		else if (LMB_SelectedArrow->ComponentHasTag(TEXT("VertexArrow")))
		{
			KeepLeftMovingVertex = true;
			Refresh();
		}
	}
}

void ACustomPlayerController::DisableKeepMovings()
{
	KeepMovingFace = false;
	KeepLeftMovingVertex = false;
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

	if (RMB_CubeToEdit != NULL && RMB_SelectedArrow != NULL ) // Successfully cast
	{
		if (RMB_SelectedArrow->ComponentHasTag(TEXT("FaceArrow")))
		{
			KeepExtrMovement = true;
			AProceduralCubeActor* ExtrudedCube = RMB_CubeToEdit->ExtrudeFaceOfCube(RMB_SelectedArrow);

			// Update RMB_SelectedArrow (it should be moved the one from the new extruded cube)
			TArray<FProceduralMeshVertex> FaceVertexes = RMB_CubeToEdit->FindFaceVertexesFromArrowLocation(RMB_SelectedArrow->RelativeLocation);
			int32 IndexOf_RMB_SelectedArrow = RMB_CubeToEdit->IdentifyFaceFromVertexes(FaceVertexes[0], FaceVertexes[1], FaceVertexes[2], FaceVertexes[3]);
			switch (IndexOf_RMB_SelectedArrow)
			{
				case 0: RMB_SelectedArrow = ExtrudedCube->FrontFaceArrow;	break;
				case 1: RMB_SelectedArrow = ExtrudedCube->BackFaceArrow;	break;
				case 2: RMB_SelectedArrow = ExtrudedCube->LeftFaceArrow;	break;
				case 3: RMB_SelectedArrow = ExtrudedCube->RightFaceArrow;	break;
				case 4: RMB_SelectedArrow = ExtrudedCube->TopFaceArrow;		break;
				case 5: RMB_SelectedArrow = ExtrudedCube->BottomFaceArrow;	break;
			}
			// Update RMB_CubeToEdit (it should be moved the new extruded cube)
			RMB_CubeToEdit = ExtrudedCube;
			Refresh();
		}

		else if (RMB_SelectedArrow->ComponentHasTag(TEXT("VertexArrow")))
		{
			KeepRightMovingVertex = true;
			Refresh();
		}
	}
}

void ACustomPlayerController::SetKeepExtrMovementToFalse()
{
	KeepExtrMovement = false;
	RMB_SelectedArrow = NULL;
	KeepRightMovingVertex = false;
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
	InputComponent->BindAction("LeftMB", IE_Pressed, this, &ACustomPlayerController::EnableKeepMovings);
	InputComponent->BindAction("LeftMB", IE_Released, this, &ACustomPlayerController::DisableKeepMovings);
	InputComponent->BindAction("RightMB", IE_Pressed, this, &ACustomPlayerController::SetKeepExtrMovementToTrue);
	InputComponent->BindAction("RightMB", IE_Released, this, &ACustomPlayerController::SetKeepExtrMovementToFalse);
}

void ACustomPlayerController::Refresh()
{
	if (!(KeepMovingFace) && !(KeepExtrMovement) && !(KeepLeftMovingVertex) && !(KeepRightMovingVertex)) { return; }

	if (KeepMovingFace)				{	LMB_CubeToEdit->MoveFace(LMBMovementDirection, LMB_CubeToEdit->FindFaceVertexesFromArrowLocation(LMB_SelectedArrow->RelativeLocation), LMB_SelectedArrow);	}
	else if (KeepExtrMovement)		{	RMB_CubeToEdit->MoveFace(RMBMovementDirection, RMB_CubeToEdit->FindFaceVertexesFromArrowLocation(RMB_SelectedArrow->RelativeLocation), RMB_SelectedArrow);	}
	else if (KeepLeftMovingVertex)	{	LMB_CubeToEdit->MoveVertexAlongWorldAxis(RecognizeArrowDirectionInWorld(LMB_SelectedArrow, LMB_CubeToEdit), RecognizeSphereFromArrow(LMB_SelectedArrow, LMB_CubeToEdit), RecognizeVertexFromArrow(LMB_SelectedArrow, LMB_CubeToEdit), LMBMovementDirection); }
	else if (KeepRightMovingVertex)	{	RMB_CubeToEdit->MoveVertexAlongRotatedAxis(RecognizeArrowDirectionInWorld(RMB_SelectedArrow, RMB_CubeToEdit), RecognizeSphereFromArrow(RMB_SelectedArrow, RMB_CubeToEdit), RecognizeVertexFromArrow(RMB_SelectedArrow, RMB_CubeToEdit), RMBMovementDirection); }

	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACustomPlayerController::Refresh);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 0.05f, false);
}

FVector ACustomPlayerController::RecognizeArrowDirectionInWorld(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube)
{
	if (
		(SelectedArrow->GetName() == SelectedCube->V0Sphere_Arrow0->GetName()) || (SelectedArrow->GetName() == SelectedCube->V1Sphere_Arrow0->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V2Sphere_Arrow0->GetName()) || (SelectedArrow->GetName() == SelectedCube->V3Sphere_Arrow0->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V4Sphere_Arrow0->GetName()) || (SelectedArrow->GetName() == SelectedCube->V5Sphere_Arrow0->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V6Sphere_Arrow0->GetName()) || (SelectedArrow->GetName() == SelectedCube->V7Sphere_Arrow0->GetName())
		)
	{
		return FVector(0, 0, 1);
	}
	else if (
		(SelectedArrow->GetName() == SelectedCube->V0Sphere_Arrow1->GetName()) || (SelectedArrow->GetName() == SelectedCube->V1Sphere_Arrow1->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V2Sphere_Arrow1->GetName()) || (SelectedArrow->GetName() == SelectedCube->V3Sphere_Arrow1->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V4Sphere_Arrow1->GetName()) || (SelectedArrow->GetName() == SelectedCube->V5Sphere_Arrow1->GetName()) ||
		(SelectedArrow->GetName() == SelectedCube->V6Sphere_Arrow1->GetName()) || (SelectedArrow->GetName() == SelectedCube->V7Sphere_Arrow1->GetName())
		)
	{
		return FVector(0, 1, 0);
	}
	else
	{
		return FVector(1, 0, 0);
	}
}

UStaticMeshComponent* ACustomPlayerController::RecognizeSphereFromArrow(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube)
{

	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p0)) { return SelectedCube->V0Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p1)) { return SelectedCube->V1Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p2)) { return SelectedCube->V2Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p3)) { return SelectedCube->V3Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p4)) { return SelectedCube->V4Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p5)) { return SelectedCube->V5Sphere; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p6)) { return SelectedCube->V6Sphere; }
	return SelectedCube->V7Sphere;
}

FProceduralMeshVertex ACustomPlayerController::RecognizeVertexFromArrow(UStaticMeshComponent* SelectedArrow, AProceduralCubeActor* SelectedCube)
{
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p0)) { return SelectedCube->v0; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p1)) { return SelectedCube->v1; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p2)) { return SelectedCube->v2; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p3)) { return SelectedCube->v3; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p4)) { return SelectedCube->v4; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p5)) { return SelectedCube->v5; }
	if (SelectedArrow->RelativeLocation.Equals(SelectedCube->p6)) { return SelectedCube->v6; }
	return SelectedCube->v7;
}

void ACustomPlayerController::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	
	FHitResult HitRes;
	this->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);
	
	CubeUnderMouse = Cast<AProceduralCubeActor>(HitRes.GetActor());
	ComponentUnderMouse = Cast<UStaticMeshComponent>(HitRes.GetComponent());
}
