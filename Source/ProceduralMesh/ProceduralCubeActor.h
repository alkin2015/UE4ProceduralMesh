// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

#include "NumericLimits.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "CustomPlayerController.h"
#include "ProceduralCubeActor.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALMESH_API AProceduralCubeActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	// --------------------------------------- PROPERTIES --------------------------------------- \\

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	TSubobjectPtr<UProceduralMeshComponent> mesh;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	ACustomPlayerController* CustomPController;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	bool KeepOnHover;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	bool VertexMovementState;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p3;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p4;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p5;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p6;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FVector p7;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v3;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v4;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v5;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v6;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	FProceduralMeshVertex v7;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V0Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V0Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V0Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V0Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V1Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V1Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V1Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V1Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V2Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V2Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V2Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V2Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V3Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V3Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V3Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V3Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V4Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V4Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V4Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V4Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V5Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V5Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V5Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V5Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V6Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V6Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V6Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V6Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V7Sphere;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V7Sphere_Arrow0;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V7Sphere_Arrow1;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* V7Sphere_Arrow2;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* FrontFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* BackFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* LeftFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* RightFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* TopFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	UStaticMeshComponent* BottomFaceArrow;

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	TArray<AProceduralCubeActor*> ExtrudedCubes;

	// --------------------------------------- GENERATION FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCube(FVector StarterP0Location, float XSize, float YSize, float ZSize, FColor VtxsColor, APlayerController* GivenPController);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void ModifyStaticMeshComponent(UStaticMeshComponent * GivenComponent, UStaticMesh * GivenStaticMesh, float GivenScale, TArray<FName> GivenTag);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubePs(FVector P0Coords, float XSize, float YSize, float ZSize);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubeVs();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UpdateVsSpheresLocations();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UpdateFacesArrowsLocationsAndRotations();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UpdateVertexArrowsLocationsAndRotations();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void SetCubeVColors(FColor VertexColor);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubeFaces(TArray<FProceduralMeshTriangle> & OutTriangles);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubeFace(FProceduralMeshVertex GivenV0, FProceduralMeshVertex GivenV1, FProceduralMeshVertex GivenV2, FProceduralMeshVertex GivenV3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	int32 ExtrusionFromGivenFaceVertexes(AProceduralCubeActor* NewCube, TArray<FProceduralMeshVertex> FaceVertexes);

	// --------------------------------------- VERTEX FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector FindAndMoveVertex(FVector MovementDirection, FProceduralMeshVertex VertexToMove, TArray<FProceduralMeshTriangle>& CurrentTriangles);
	
	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UpdateCubeVertexLocation(FProceduralMeshVertex VertexToUpdate);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	int32 IdentifyFaceFromVertexes(FProceduralMeshVertex FVertex0, FProceduralMeshVertex FVertex1, FProceduralMeshVertex FVertex2, FProceduralMeshVertex FVertex3);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void MoveVertexAlongWorldAxis(FVector AxisOfMovement, UStaticMeshComponent* ClickedSphere, FProceduralMeshVertex VToMove, float MovementSign);
	
	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void MoveVertexAlongRotatedAxis(FVector AxisOfMovement, UStaticMeshComponent* ClickedSphere, FProceduralMeshVertex VToMove, float MovementSign);

	// --------------------------------------- FACES and ARROWS FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
		TArray<FProceduralMeshVertex> MoveFace(float MovementSign, TArray<FProceduralMeshVertex> VertexesArray, UStaticMeshComponent* FaceArrow);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector CalculateFaceMiddlePoint(TArray<FProceduralMeshVertex> FaceVertexes);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	TArray<FProceduralMeshVertex> FindFaceVertexesFromArrowLocation(FVector ArrowLocation);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FRotator GetOrtogonalFaceDirectionFromFaceVertex(FVector GivenLocation, TArray<FProceduralMeshVertex> VertexesArray);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	AProceduralCubeActor* ExtrudeFaceOfCube(UStaticMeshComponent* ClickedArrow);


	// --------------------------------------- AUX FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector UnitVector(FVector GivenVector);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	float EuclideanDistance(FVector P, FVector Q);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	float VectorModule(FVector GivenVector);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector CrossProd(FVector U, FVector V);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	bool VectorContainsElement(TArray<int32> GivenVector, int32 GivenElement);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FRotator ConvertToPitchRollYawRotator(FVector VToConvert);

	// --------------------------------------- CUSTOM EVENTS --------------------------------------- \\

	UFUNCTION(BlueprintNativeEvent, Category = SomeCategory)
	void ArrowOnClickEvent(UStaticMeshComponent* ClickedArrow, bool KeepMoving, bool KeepExtrMov, float LMBMovDir, float RMBMovDir);

	UFUNCTION()
	void PlayerOnHover();

	UFUNCTION()
	void PlayerOnExitHover();

	UFUNCTION()
	void KeepingOnHover();

	UFUNCTION()
	void InitFacesMovements();

	// --------------------------------------- HIDE FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void HideVertexSpheres();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void HideFacesArrows();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void HideSpheresArrows();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void HideAllComponents();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void InitVertexMovementState(UStaticMeshComponent* SelectedSphere);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void QuitVertexMovementState();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UnsetVertexMovementState();

	// --------------------------------------- OVERRIDE FUNCTIONS ---------------------------------------

	void Tick(float deltaSeconds) override;

};