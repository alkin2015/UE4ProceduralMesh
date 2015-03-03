// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

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
	// Allow viewing/changing the Material ot the procedural Mesh in editor (if placed in a level at construction)
	UPROPERTY(BlueprintReadWrite, Category = Materials)
	TSubobjectPtr<UProceduralMeshComponent> mesh;

	void GenerateCube(const float& InSize, TArray<FProceduralMeshTriangle>& OutTriangles);

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

	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
		FProceduralMeshVertex MoveVertexP0(FVector Direction, FProceduralMeshVertex Vertex);
};
