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
	// Allow viewing/changing the Material to the procedural Mesh in editor (if placed in a level at construction)
	UPROPERTY(VisibleAnywhere, Category=Materials)
	TSubobjectPtr<UProceduralMeshComponent> mesh;

	void GenerateCube(const float& InSize, TArray<FProceduralMeshTriangle>& OutTriangles);

	UFUNCTION(BlueprintCallable, Category = Materials)
	void GenerateCubeFace(FProceduralMeshVertex v0, FProceduralMeshVertex v1, FProceduralMeshVertex v2, FProceduralMeshVertex v3, FVector p0, FVector p1, FVector p2, FVector p3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2);

	UFUNCTION(BlueprintCallable, Category = Materials)
	void MoveVertexP0();
};
