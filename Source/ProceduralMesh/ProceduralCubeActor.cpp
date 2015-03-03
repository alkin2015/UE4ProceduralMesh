// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//

#include "ProceduralMesh.h"
#include "ProceduralCubeActor.h"

AProceduralCubeActor::AProceduralCubeActor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{


	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralCube"));



	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/BaseColor.BaseColor'"));
	// TODO Apply a real material with textures, using UVs
	//	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/M_Concrete_Poured.M_Concrete_Poured'"));
	mesh->SetMaterial(0, Material.Object);


	// Generate a cube
	TArray<FProceduralMeshTriangle> triangles;
	GenerateCube(100.f, triangles);
	mesh->SetProceduralMeshTriangles(triangles);

	RootComponent = mesh;
}

// Generate a full cube
void AProceduralCubeActor::GenerateCube(const float& InSize, TArray<FProceduralMeshTriangle>& OutTriangles)
{
	p0 = FVector(0.f, 0.f, 0.f);
	p1 = FVector(0.f, 0.f, InSize);
	p2 = FVector(InSize, 0.f, InSize);
	p3 = FVector(InSize, 0.f, 0.f);
	p4 = FVector(InSize, InSize, 0.f);
	p5 = FVector(InSize, InSize, InSize);
	p6 = FVector(0.f, InSize, InSize);
	p7 = FVector(0.f, InSize, 0.f);

	// The 8 vertices of the cube


	v0.U = 0.f; v0.V = 0.f;
	v1.U = 0.f; v1.V = 1.f;
	v2.U = 1.f; v2.V = 1.f;
	v3.U = 1.f; v3.V = 0.f;

	FProceduralMeshTriangle t1;
	FProceduralMeshTriangle t2;

	// front face
	v0.Position = p0;
	v1.Position = p1;
	v2.Position = p2;
	v3.Position = p3;
	v4.Position = p4;
	v5.Position = p5;
	v6.Position = p6;
	v7.Position = p7;


	v0.Id = 0;
	v1.Id = 1;
	v2.Id = 2;
	v3.Id = 3;
	v4.Id = 4;
	v5.Id = 5;
	v6.Id = 6;
	v7.Id = 7;


	t1.Vertex0 = v0;
	t1.Vertex1 = v1;
	t1.Vertex2 = v2;
	t2.Vertex0 = v0;
	t2.Vertex1 = v2;
	t2.Vertex2 = v3;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// back face
	t1.Vertex0 = v4;
	t1.Vertex1 = v5;
	t1.Vertex2 = v6;
	t2.Vertex0 = v4;
	t2.Vertex1 = v6;
	t2.Vertex2 = v7;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// left face
	t1.Vertex0 = v7;
	t1.Vertex1 = v6;
	t1.Vertex2 = v1;
	t2.Vertex0 = v7;
	t2.Vertex1 = v1;
	t2.Vertex2 = v0;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// right face
	t1.Vertex0 = v3;
	t1.Vertex1 = v2;
	t1.Vertex2 = v5;
	t2.Vertex0 = v3;
	t2.Vertex1 = v5;
	t2.Vertex2 = v4;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// top face
	t1.Vertex0 = v1;
	t1.Vertex1 = v6;
	t1.Vertex2 = v5;
	t2.Vertex0 = v1;
	t2.Vertex1 = v5;
	t2.Vertex2 = v2;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// bottom face
	t1.Vertex0 = v3;
	t1.Vertex1 = v4;
	t1.Vertex2 = v7;
	t2.Vertex0 = v3;
	t2.Vertex1 = v7;
	t2.Vertex2 = v0;
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);
}

FProceduralMeshVertex AProceduralCubeActor::MoveVertexP0(FVector Direction, FProceduralMeshVertex Vertex)
{ 
	TArray<FProceduralMeshTriangle> currentTriangles = mesh->GetProceduralMeshTriangles();

	


	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, FString::Printf(TEXT("Direction %s"), *Direction.ToString()));
	for (int i = 0; i < currentTriangles.Num(); i++){

		if (currentTriangles[i].Vertex0.Id == Vertex.Id){
			FVector currentP0 = currentTriangles[i].Vertex0.Position;
			FVector newP0 = FVector(currentP0[0] + Direction.X, currentP0[1] + Direction.Y, currentP0[2] + Direction.Z);
			currentTriangles[i].Vertex0.Position = newP0;
			Vertex.Position = newP0;
			
		}
		else if (currentTriangles[i].Vertex1.Id == Vertex.Id){
			FVector currentP0 = currentTriangles[i].Vertex1.Position;
			FVector newP0 = FVector(currentP0[0] + Direction.X, currentP0[1] + Direction.Y, currentP0[2] + Direction.Z);
			currentTriangles[i].Vertex1.Position = newP0;
			Vertex.Position = newP0;
			
		}

		else if (currentTriangles[i].Vertex2.Id == Vertex.Id){
			FVector currentP0 = currentTriangles[i].Vertex2.Position;
			FVector newP0 = FVector(currentP0[0] + Direction.X, currentP0[1] + Direction.Y, currentP0[2] + Direction.Z);
			currentTriangles[i].Vertex2.Position = newP0;
			Vertex.Position = newP0;
			//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, FString::Printf(TEXT("Triangle %d - Vertex2"), i));
			
		}
	}

	

	mesh->SetProceduralMeshTriangles(currentTriangles);


	return Vertex;
}