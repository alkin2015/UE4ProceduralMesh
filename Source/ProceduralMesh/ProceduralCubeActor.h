// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

#include "NumericLimits.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralCubeActor.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALMESH_API AProceduralCubeActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	// --------------------------------------- BEGIN PROPERTIES --------------------------------------- \\

	UPROPERTY(BlueprintReadWrite, Category = Materials)
	TSubobjectPtr<UProceduralMeshComponent> mesh;

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
	TArray<AProceduralCubeActor*> ExtrudedCubes;

	// --------------------------------------- BEGIN GENERATION FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCube(FVector StarterP0Location, float XSize, float YSize, float ZSize);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubePs(FVector P0Coords, float XSize, float YSize, float ZSize);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubeVs();

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void SetCubeVColors(FColor VertexColor);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void GenerateCubeFace(FProceduralMeshVertex GivenV0, FProceduralMeshVertex GivenV1, FProceduralMeshVertex GivenV2, FProceduralMeshVertex GivenV3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	int32 ExtrusionFromGivenFaceVertexes(AProceduralCubeActor* NewCube, TArray<FProceduralMeshVertex> FaceVertexes);

	// --------------------------------------- BEGIN VERTEX FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FProceduralMeshVertex MoveVertexP0(FVector Direction, FProceduralMeshVertex Vertex, FRotator CubeRotation);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector FindAndMoveVertex(FVector MovementDirection, FProceduralMeshVertex VertexToMove, TArray<FProceduralMeshTriangle>& CurrentTriangles);
	
	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	void UpdateCubeVertexLocation(FProceduralMeshVertex VertexToUpdate);

	// --------------------------------------- BEGIN FACES and ARROWS FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	TArray<FProceduralMeshVertex> MoveFace(float MovementSign, TArray<FProceduralMeshVertex> VertexesArray);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector CalculateArrowLocationFromFaceVertex(TArray<FProceduralMeshVertex> FaceVertexes);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	TArray<FProceduralMeshVertex> FindFaceVertexesFromArrowLocation(FVector ArrowLocation);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FRotator GetOrtogonalFaceDirectionFromFaceVertex(FVector GivenLocation, TArray<FProceduralMeshVertex> VertexesArray);

	// --------------------------------------- AUX FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector ObtainMovementDirection(FVector Direction, FRotator CubeRotation, FVector VPosition);

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

	// --------------------------------------- ROTATION FUNCTIONS --------------------------------------- \\

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector RotateOnX(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector UnRotateOnX(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector RotateOnY(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector UnRotateOnY(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector RotateOnZ(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector UnRotateOnZ(FVector Point, float Angle);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector RotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ);

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	FVector UnRotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ);

};