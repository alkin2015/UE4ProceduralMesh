// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//


#include "ProceduralMesh.h"
#include "ProceduralCubeActor.h"





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - GENERATION FUNCTIONS ------------------------------------------------- \\

AProceduralCubeActor::AProceduralCubeActor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralCube"));
	
	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/BaseColor.BaseColor'"));
	
	// TODO Apply a real material with textures, using UVs
	mesh->SetMaterial(0, Material.Object);

	RootComponent = mesh;

}

//void AProceduralCubeActor::GenerateCube(const float& InSize, TArray<FProceduralMeshTriangle>& OutTriangles)
void AProceduralCubeActor::GenerateCube(FVector StarterP0Location, float XSize, float YSize, float ZSize)
{

	TArray<FProceduralMeshTriangle> OutTriangles;

	GenerateCubePs(StarterP0Location, XSize, YSize, ZSize);
	GenerateCubeVs();
	SetCubeVColors(FColor::Green);
	FProceduralMeshTriangle t1, t2;

	// front face
	GenerateCubeFace(v0, v1, v2, v3, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// back face
	GenerateCubeFace(v4, v5, v6, v7, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// left face
	GenerateCubeFace(v7, v6, v1, v0, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// right face
	GenerateCubeFace(v3, v2, v5, v4, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// top face
	GenerateCubeFace(v1, v6, v5, v2, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// bottom face
	GenerateCubeFace(v3, v4, v7, v0, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	mesh->SetProceduralMeshTriangles(OutTriangles);

}

void AProceduralCubeActor::GenerateCubePs(FVector P0Coords, float XSize, float YSize, float ZSize)
{
	p0 = P0Coords + FVector(0.f, 0.f, 0.f);
	p1 = P0Coords + FVector(0.f, 0.f, ZSize);
	p2 = P0Coords + FVector(XSize, 0.f, ZSize);
	p3 = P0Coords + FVector(XSize, 0.f, 0.f);
	p4 = P0Coords + FVector(XSize, YSize, 0.f);
	p5 = P0Coords + FVector(XSize, YSize, ZSize);
	p6 = P0Coords + FVector(0.f, YSize, ZSize);
	p7 = P0Coords + FVector(0.f, YSize, 0.f);
}

void AProceduralCubeActor::GenerateCubeVs()
{
	v0.Position = p0; v0.Id = 0;
	v1.Position = p1; v1.Id = 1;
	v2.Position = p2; v2.Id = 2;
	v3.Position = p3; v3.Id = 3;
	v4.Position = p4; v4.Id = 4;
	v5.Position = p5; v5.Id = 5;
	v6.Position = p6; v6.Id = 6;
	v7.Position = p7; v7.Id = 7;
}

void AProceduralCubeActor::SetCubeVColors(FColor VertexColor)
{
	v0.Color = VertexColor;
	v1.Color = VertexColor;
	v2.Color = VertexColor;
	v3.Color = VertexColor;
	v4.Color = VertexColor;
	v5.Color = VertexColor;
	v6.Color = VertexColor;
	v7.Color = VertexColor;
}

void AProceduralCubeActor::GenerateCubeFace(FProceduralMeshVertex GivenV0, FProceduralMeshVertex GivenV1, FProceduralMeshVertex GivenV2, FProceduralMeshVertex GivenV3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2)
{
	// Problemas con .U .V si van cambiando pero se almacenan con el ultimo valor... (?)
	GivenV0.U = 0.f; GivenV0.V = 0.f;
	GivenV1.U = 0.f; GivenV1.V = 1.f;
	GivenV2.U = 1.f; GivenV2.V = 1.f;
	GivenV3.U = 1.f; GivenV3.V = 0.f;
	
	t1.Vertex0 = GivenV0;
	t1.Vertex1 = GivenV1;
	t1.Vertex2 = GivenV2;
	
	t2.Vertex0 = GivenV0;
	t2.Vertex1 = GivenV2;
	t2.Vertex2 = GivenV3;
	
	//return CalculateArrowLocationFromFaceVertex(GivenV0, GivenV1, GivenV2, GivenV3);
}

int32 AProceduralCubeActor::ExtrusionFromGivenFaceVertexes(AProceduralCubeActor* NewCube, TArray<FProceduralMeshVertex> FaceVertexes)
{
	// This function returns an integer representing wich arrow will be moved after extruding (from 0 to 5)
	int32 ArrowIdToMoveAfterExtruding;

	//AProceduralCubeActor* NewCube = GetWorld()->SpawnActor<AProceduralCubeActor>(AProceduralCubeActor::StaticClass());
	FProceduralMeshVertex FVertex0 = FaceVertexes[0];
	FProceduralMeshVertex FVertex1 = FaceVertexes[1];
	FProceduralMeshVertex FVertex2 = FaceVertexes[2];
	FProceduralMeshVertex FVertex3 = FaceVertexes[3];

	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Begin extrusion!"));
	FRotator ParentCubeRotation = this->GetActorRotation();
	FVector ParentCubeLocation = this->GetActorLocation();
	float ParentXSize = abs(p3.X - p0.X);
	float ParentYSize = abs(p7.Y - p0.Y);
	float ParentZSize = abs(p1.Z - p0.Z);
	
	// Resize extruded cube dimensions
	FVector CrossProdVector = CrossProd(FVertex1.Position - FVertex0.Position, FVertex2.Position - FVertex0.Position);
	if (CrossProdVector.X != 0) { ParentXSize = 10; }
	else if (CrossProdVector.Y != 0) { ParentYSize = 10; }
	else if (CrossProdVector.Z != 0) { ParentZSize = 10; }

	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("Given Id's: %f %f %f %f"), FVertex0.Id, FVertex1.Id, FVertex2.Id, FVertex3.Id));
	TArray<int32> frontFace;	frontFace.Add(v0.Id);	frontFace.Add(v1.Id);	frontFace.Add(v2.Id);	frontFace.Add(v3.Id);
	TArray<int32> backFace;		backFace.Add(v4.Id);	backFace.Add(v5.Id);	backFace.Add(v6.Id);	backFace.Add(v7.Id);
	TArray<int32> leftFace;		leftFace.Add(v7.Id);	leftFace.Add(v6.Id);	leftFace.Add(v1.Id);	leftFace.Add(v0.Id);
	TArray<int32> rightFace;	rightFace.Add(v3.Id);	rightFace.Add(v2.Id);	rightFace.Add(v5.Id);	rightFace.Add(v4.Id);
	TArray<int32> topFace;		topFace.Add(v1.Id);		topFace.Add(v6.Id);		topFace.Add(v5.Id);		topFace.Add(v2.Id);
	TArray<int32> bottomFace;	bottomFace.Add(v3.Id);	bottomFace.Add(v4.Id);	bottomFace.Add(v7.Id);	bottomFace.Add(v0.Id);

	// Given Vertexes are Front Face Vertexes
	if (VectorContainsElement(frontFace, FVertex0.Id) && VectorContainsElement(frontFace, FVertex1.Id) && VectorContainsElement(frontFace, FVertex2.Id) && VectorContainsElement(frontFace, FVertex3.Id))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Front Face"));
		NewCube->GenerateCubePs(FVector(p0.X, p0.Y - ParentYSize, p0.Z), ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 0;
	}
	// Given Vertexes are Back Face Vertexes
	else if (VectorContainsElement(backFace, FVertex0.Id) && VectorContainsElement(backFace, FVertex1.Id) && VectorContainsElement(backFace, FVertex2.Id) && VectorContainsElement(backFace, FVertex3.Id))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Back Face"));
		NewCube->GenerateCubePs(p7, ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 1;
	}
	// Given Vertexes are Left Face Vertexes
	else if (VectorContainsElement(leftFace, FVertex0.Id) && VectorContainsElement(leftFace, FVertex1.Id) && VectorContainsElement(leftFace, FVertex2.Id) && VectorContainsElement(leftFace, FVertex3.Id))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Left Face"));
		NewCube->GenerateCubePs(FVector(p0.X - ParentXSize, p0.Y, p0.Z), ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 2;
	}
	// Given Vertexes are Right Face Vertexes
	else if (VectorContainsElement(rightFace, FVertex0.Id) && VectorContainsElement(rightFace, FVertex1.Id) && VectorContainsElement(rightFace, FVertex2.Id) && VectorContainsElement(rightFace, FVertex3.Id))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Right Face"));
		NewCube->GenerateCubePs(p3, ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 3;
	}
	// Given Vertexes are Top Face Vertexes
	else if (VectorContainsElement(topFace, FVertex0.Id) && VectorContainsElement(topFace, FVertex1.Id) && VectorContainsElement(topFace, FVertex2.Id) && VectorContainsElement(topFace, FVertex3.Id))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Top Face"));
		NewCube->GenerateCubePs(p1, ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 4;
	}
	// Given Vertexes are Bottom Face Vertexes
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, TEXT("Extruding Bottom Face"));
		NewCube->GenerateCubePs(FVector(p0.X, p0.Y, p0.Z - ParentZSize), ParentXSize, ParentYSize, ParentZSize);
		ArrowIdToMoveAfterExtruding = 5;
	}

	NewCube->GenerateCubeVs();

	NewCube->SetCubeVColors(FColor::Red);

	FProceduralMeshTriangle t1, t2;
	TArray<FProceduralMeshTriangle> OutTriangles;

	// front face
	NewCube->GenerateCubeFace(NewCube->v0, NewCube->v1, NewCube->v2, NewCube->v3, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// back face
	NewCube->GenerateCubeFace(NewCube->v4, NewCube->v5, NewCube->v6, NewCube->v7, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// left face
	NewCube->GenerateCubeFace(NewCube->v7, NewCube->v6, NewCube->v1, NewCube->v0, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// right face
	NewCube->GenerateCubeFace(NewCube->v3, NewCube->v2, NewCube->v5, NewCube->v4, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// top face
	NewCube->GenerateCubeFace(NewCube->v1, NewCube->v6, NewCube->v5, NewCube->v2, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	// bottom face
	NewCube->GenerateCubeFace(NewCube->v3, NewCube->v4, NewCube->v7, NewCube->v0, t1, t2);
	OutTriangles.Add(t1);
	OutTriangles.Add(t2);

	NewCube->SetActorRotation(ParentCubeRotation);
	NewCube->SetActorLocation(ParentCubeLocation);

	NewCube->mesh->SetProceduralMeshTriangles(OutTriangles);

	ExtrudedCubes.Add(NewCube);

	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, FString::Printf(TEXT("Sizes: %f %f %f"), ParentXSize, ParentYSize, ParentZSize));
	return ArrowIdToMoveAfterExtruding;

}

// ------------------------------------------------- End - GENERATION FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - VERTEX FUNCTIONS ------------------------------------------------- \\

FProceduralMeshVertex AProceduralCubeActor::MoveVertexP0(FVector Direction, FProceduralMeshVertex Vertex, FRotator CubeRotation)
{
	TArray<FProceduralMeshTriangle> currentTriangles = mesh->GetProceduralMeshTriangles();
	FVector newP0, currentP0, movementDirection;
	bool found = false;

	movementDirection = ObtainMovementDirection(Direction, CubeRotation, Vertex.Position);
	GEngine->AddOnScreenDebugMessage(1, 20, FColor::Red, TEXT("1: " + UnitVector(GetActorForwardVector()).ToString()));
	GEngine->AddOnScreenDebugMessage(2, 20, FColor::Red, TEXT("2: " + UnitVector(RotateOnX(RotateOnY(RotateOnZ(FVector(1, 0, 0), CubeRotation.Euler()[2]), CubeRotation.Euler()[1]), CubeRotation.Euler()[0])).ToString()));
	GEngine->AddOnScreenDebugMessage(3, 20, FColor::Red, TEXT("3: " + UnitVector(RotateOnZ(RotateOnY(RotateOnX(FVector(1, 0, 0), CubeRotation.Euler()[0]), CubeRotation.Euler()[1]), CubeRotation.Euler()[2])).ToString()));
	GEngine->AddOnScreenDebugMessage(4, 20, FColor::Red, TEXT("4: " + UnitVector(RotateOnXYZ(FVector(1, 0, 0), CubeRotation.Euler()[2], CubeRotation.Euler()[1], CubeRotation.Euler()[0])).ToString()));

	for (int i = 0; i < currentTriangles.Num(); i++){
		if (currentTriangles[i].Vertex0.Id == Vertex.Id)
		{
			if (!found)
			{
				currentP0 = currentTriangles[i].Vertex0.Position;
				newP0 = currentP0 + movementDirection;
				found = true;
			}
			currentTriangles[i].Vertex0.Position = newP0;
			Vertex.Position = newP0;
		}
		else if (currentTriangles[i].Vertex1.Id == Vertex.Id)
		{
			if (!found)
			{
				currentP0 = currentTriangles[i].Vertex1.Position;
				newP0 = currentP0 + movementDirection;
				found = true;
			}
			currentTriangles[i].Vertex1.Position = newP0;
			Vertex.Position = newP0;
		}
		else if (currentTriangles[i].Vertex2.Id == Vertex.Id)
		{
			if (!found)
			{
				currentP0 = currentTriangles[i].Vertex2.Position;
				newP0 = currentP0 + movementDirection;
				found = true;
			}
			currentTriangles[i].Vertex2.Position = newP0;
			Vertex.Position = newP0;
		}
	}

	mesh->SetProceduralMeshTriangles(currentTriangles);
	return Vertex;

}

FVector AProceduralCubeActor::FindAndMoveVertex(FVector MovementDirection, FProceduralMeshVertex VertexToMove, TArray<FProceduralMeshTriangle>& CurrentTriangles)
{
	FVector NewVertexPosition = VertexToMove.Position;

	if (VertexToMove.Id == 0) { p0 = p0 + MovementDirection; }
	else if (VertexToMove.Id == 1) { p1 = p1 + MovementDirection; }
	else if (VertexToMove.Id == 2) { p2 = p2 + MovementDirection; }
	else if (VertexToMove.Id == 3) { p3 = p3 + MovementDirection; }
	else if (VertexToMove.Id == 4) { p4 = p4 + MovementDirection; }
	else if (VertexToMove.Id == 5) { p5 = p5 + MovementDirection; }
	else if (VertexToMove.Id == 6) { p6 = p6 + MovementDirection; }
	else if (VertexToMove.Id == 7) { p7 = p7 + MovementDirection; }


	bool found = false;
	for (int i = 0; i < CurrentTriangles.Num(); i++)
	{
		if (CurrentTriangles[i].Vertex0.Id == VertexToMove.Id)
		{
			if (!found) { found = true; NewVertexPosition = CurrentTriangles[i].Vertex0.Position + MovementDirection; }
			CurrentTriangles[i].Vertex0.Position = NewVertexPosition;
		}
		else if (CurrentTriangles[i].Vertex1.Id == VertexToMove.Id)
		{
			if (!found) { found = true; NewVertexPosition = CurrentTriangles[i].Vertex1.Position + MovementDirection; }
			CurrentTriangles[i].Vertex1.Position = NewVertexPosition;
		}
		else if (CurrentTriangles[i].Vertex2.Id == VertexToMove.Id)
		{
			if (!found) { found = true; NewVertexPosition = CurrentTriangles[i].Vertex2.Position + MovementDirection; }
			CurrentTriangles[i].Vertex2.Position = NewVertexPosition;
		}
	}
	VertexToMove.Position = NewVertexPosition;
	UpdateCubeVertexLocation(VertexToMove);
	return NewVertexPosition;
}

void AProceduralCubeActor::UpdateCubeVertexLocation(FProceduralMeshVertex VertexToUpdate)
{
	if (v0.Id == VertexToUpdate.Id) { v0.Position = VertexToUpdate.Position; }
	else if (v1.Id == VertexToUpdate.Id) { v1.Position = VertexToUpdate.Position; }
	else if (v2.Id == VertexToUpdate.Id) { v2.Position = VertexToUpdate.Position; }
	else if (v3.Id == VertexToUpdate.Id) { v3.Position = VertexToUpdate.Position; }
	else if (v4.Id == VertexToUpdate.Id) { v4.Position = VertexToUpdate.Position; }
	else if (v5.Id == VertexToUpdate.Id) { v5.Position = VertexToUpdate.Position; }
	else if (v6.Id == VertexToUpdate.Id) { v6.Position = VertexToUpdate.Position; }
	else if (v7.Id == VertexToUpdate.Id) { v7.Position = VertexToUpdate.Position; }
}

// ------------------------------------------------- End - VERTEX FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - FACES and ARROWS FUNCTIONS ------------------------------------------------- \\

TArray<FProceduralMeshVertex> AProceduralCubeActor::MoveFace(float MovementSign, TArray<FProceduralMeshVertex> VertexesArray)
{
	FProceduralMeshVertex MoveVertex0 = VertexesArray[0];
	FProceduralMeshVertex MoveVertex1 = VertexesArray[1];
	FProceduralMeshVertex MoveVertex2 = VertexesArray[2];
	FProceduralMeshVertex MoveVertex3 = VertexesArray[3];

	TArray<FProceduralMeshTriangle> currentTriangles = mesh->GetProceduralMeshTriangles();
	TArray<FProceduralMeshVertex> VertexArray;

	FVector ab = MoveVertex1.Position - MoveVertex0.Position;
	FVector bc = MoveVertex2.Position - MoveVertex1.Position;

	// Calculate movement direction (ortogonal vector)
	FVector Direction = UnitVector(ab.CrossProduct(ab,bc));
	if (MovementSign < 0) { Direction = - Direction; }
	
	if (MovementSign != 0) { // It is possible that MovementSign = 0 so in this case no movement should take place

		MoveVertex0.Position = FindAndMoveVertex(Direction, MoveVertex0, currentTriangles);
		MoveVertex1.Position = FindAndMoveVertex(Direction, MoveVertex1, currentTriangles);
		MoveVertex2.Position = FindAndMoveVertex(Direction, MoveVertex2, currentTriangles);
		MoveVertex3.Position = FindAndMoveVertex(Direction, MoveVertex3, currentTriangles);

		VertexArray.Add(MoveVertex0);
		VertexArray.Add(MoveVertex1);
		VertexArray.Add(MoveVertex2);
		VertexArray.Add(MoveVertex3);

		mesh->SetProceduralMeshTriangles(currentTriangles);

	}

	return VertexArray;
}

FVector AProceduralCubeActor::CalculateArrowLocationFromFaceVertex(TArray<FProceduralMeshVertex> FaceVertexes)
{

	FProceduralMeshVertex FaceVertex0 = FaceVertexes[0];
	FProceduralMeshVertex FaceVertex1 = FaceVertexes[1];
	FProceduralMeshVertex FaceVertex2 = FaceVertexes[2];
	FProceduralMeshVertex FaceVertex3 = FaceVertexes[3];

	FVector V01 = FaceVertex1.Position - FaceVertex0.Position;
	FVector V02 = FaceVertex2.Position - FaceVertex0.Position;
	FVector V03 = FaceVertex3.Position - FaceVertex0.Position;

	float D01 = sqrt(V01[0] * V01[0] + V01[1] * V01[1] + V01[2] * V01[2]);
	float D02 = sqrt(V02[0] * V02[0] + V02[1] * V02[1] + V02[2] * V02[2]);
	float D03 = sqrt(V03[0] * V03[0] + V03[1] * V03[1] + V03[2] * V03[2]);

	if (D01 > D02 && D01 > D03) 
	{
		FVector CalculatedLocation = (FaceVertex0.Position + FaceVertex1.Position)/2;
		return CalculatedLocation;
	}
	if (D02 > D01 && D02 > D03)
	{
		FVector CalculatedLocation = (FaceVertex0.Position + FaceVertex2.Position)/2;
		return CalculatedLocation;
	}
	FVector CalculatedLocation = (FaceVertex0.Position + FaceVertex3.Position)/2;
	return CalculatedLocation;
}

TArray<FProceduralMeshVertex> AProceduralCubeActor::FindFaceVertexesFromArrowLocation(FVector ArrowLocation)
{

	FProceduralMeshVertex CubeVertexes[] = {v0, v1, v2, v3, v4, v5, v6, v7};
	
	// Store each euclidean distance from each cube vertex to the given arrow
	float VertexesDistances[8];
	for (int i = 0; i < 8; i++)
	{
		VertexesDistances[i] = EuclideanDistance(CubeVertexes[i].Position, ArrowLocation);
	}

	float MinDist; int iMinDist; FProceduralMeshVertex VertexMinDist;

	// Calculate the 4 nearer vertex
	TArray<FProceduralMeshVertex> FaceVertexes;
	while (FaceVertexes.Num() < 4)
	{
		MinDist = VertexesDistances[0];
		iMinDist = 0;
		for (int i = 1; i < 8; i++)
		{
			if (MinDist > VertexesDistances[i])
			{
				MinDist = VertexesDistances[i];
				iMinDist = i;
			}
		}
		VertexesDistances[iMinDist] = FLT_MAX;
		FaceVertexes.Add(CubeVertexes[iMinDist]);
	}

	return FaceVertexes;
}

FRotator AProceduralCubeActor::GetOrtogonalFaceDirectionFromFaceVertex(FVector GivenLocation, TArray<FProceduralMeshVertex> VertexesArray)
{
	// Face Vectors (using 3 face vertex)
	FVector V01 = FVector(VertexesArray[1].Position - VertexesArray[0].Position);
	FVector V12 = FVector(VertexesArray[2].Position - VertexesArray[1].Position);
	
	// Face ortogonal vector
	FVector OrtogonalVector = UnitVector(CrossProd(V12, V01));

	// Rotator to set this calculated ortogonal direction
	FRotator ReturningRot;
	ReturningRot.Roll = 90 * OrtogonalVector[2];
	ReturningRot.Pitch = 90 * OrtogonalVector[1];
	ReturningRot.Yaw = 90 * OrtogonalVector[0];

	if (ReturningRot.Pitch != 0) // Necessary to correct from (x,y,z) to (r,p,y)
	{
		ReturningRot.Pitch = - ReturningRot.Pitch;
		ReturningRot.Roll = ReturningRot.Roll + ReturningRot.Pitch;
		ReturningRot.Yaw = ReturningRot.Yaw + ReturningRot.Pitch;
	}

	return ReturningRot;

}

// ------------------------------------------------- End - FACE ARROWS FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------ Begin - AUX FUNCTIONS ------------------------------------------------ \\

FVector AProceduralCubeActor::ObtainMovementDirection(FVector Direction, FRotator CubeRotation, FVector VPosition)
{
	FVector movementDirection;
	if (Direction.X == 0)
	{
		if (Direction.Y == 0) // (0, 0, movement)
		{
			if (Direction.Z < 0) { movementDirection = -FVector(0, 0, 1); }
			else { movementDirection = FVector(0, 0, 1); }
		}
		else // (0, movement, 0)
		{
			if (Direction.Y < 0) { movementDirection = -FVector(0, 1, 0); }
			else { movementDirection = FVector(0, 1, 0); }
		}
	}
	else // (movement, 0, 0)
	{
		if (Direction.X < 0) { movementDirection = -FVector(1, 0, 0); }
		else{ movementDirection = FVector(1, 0, 0); }
	}
	return UnitVector(movementDirection);
}

FVector AProceduralCubeActor::UnitVector(FVector GivenVector)
{
	float VModule = VectorModule(GivenVector);
	return FVector(GivenVector.X / VModule, GivenVector.Y / VModule, GivenVector.Z / VModule);
}

float AProceduralCubeActor::EuclideanDistance(FVector P, FVector Q)
{
	return sqrt((P.X - Q.X)*(P.X - Q.X) + (P.Y - Q.Y)*(P.Y - Q.Y) + (P.Z - Q.Z)*(P.Z - Q.Z));
}

float AProceduralCubeActor::VectorModule(FVector GivenVector)
{
	return sqrt(GivenVector.X*GivenVector.X + GivenVector.Y*GivenVector.Y + GivenVector.Z*GivenVector.Z);
}

FVector AProceduralCubeActor::CrossProd(FVector U, FVector V)
{
	return FVector(U[1] * V[2] - U[2] * V[1], U[2] * V[0] - U[0] * V[2], U[0] * V[1] - U[1] * V[0]);
}

bool AProceduralCubeActor::VectorContainsElement(TArray<int32> GivenVector, int32 GivenElement)
{
	bool ElementFound = false;
	for (int32 i = 0; i < GivenVector.Num() && !(ElementFound); i++)
	{
		if (GivenVector[i] == GivenElement)
		{
			ElementFound = true;
		}
	}
	return ElementFound;
}

// ------------------------------------------------ End - AUX FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------ Begin - ROTATION FUNCTIONS ------------------------------------------------ \\

FVector AProceduralCubeActor::RotateOnX(FVector Point, float Angle)
{
	return FVector( 180 * (Point.X ) / PI, 180 * (Point.Y*cos(Angle) - Point.Z*sin(Angle)) / PI, 180 * (Point.Z*cos(Angle) + Point.Y*sin(Angle)) / PI);
}

FVector AProceduralCubeActor::UnRotateOnX(FVector Point, float Angle)
{
	return FVector(180 * (Point.X) / PI, 180 * (Point.Y*cos(Angle) + Point.Z*sin(Angle)) / PI, 180* (Point.Z*cos(Angle) - Point.Y*sin(Angle))/ PI);
}

FVector AProceduralCubeActor::RotateOnY(FVector Point, float Angle)
{
	return FVector(180 * (Point.X*cos(Angle) + Point.Z*sin(Angle)) / PI, 180 * (Point.Y) / PI, 180 * (Point.Z*cos(Angle) - Point.X*sin(Angle)) / PI);
}

FVector AProceduralCubeActor::UnRotateOnY(FVector Point, float Angle)
{
	return FVector(180 * (Point.X*cos(Angle) - Point.Z*sin(Angle)) / PI, 180 * (Point.Y) / PI, 180 * (Point.Z*cos(Angle) + Point.X*sin(Angle))/ PI);
}

FVector AProceduralCubeActor::RotateOnZ(FVector Point, float Angle)
{
	return FVector(180 * (Point.X*cos(Angle) - Point.Y*sin(Angle)) / PI, 180 * (Point.Y*cos(Angle) + Point.X*sin(Angle)) / PI, 180 * (Point.Z) / PI);
}

FVector AProceduralCubeActor::UnRotateOnZ(FVector Point, float Angle)
{
	return FVector(180 * (Point.X*cos(Angle) + Point.Y*sin(Angle)) / PI, 180 * (Point.Y*cos(Angle) - Point.X*sin(Angle)) / PI, 180 * (Point.Z)/ PI);
}

FVector AProceduralCubeActor::RotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ)
{
	return FVector(180 * (Point.X*cos(AngleY)*cos(AngleZ) + Point.Y*(cos(AngleZ)*sin(AngleX)*sin(AngleY) - cos(AngleX)*sin(AngleZ)) + Point.Z *(cos(AngleX)*cos(AngleZ)*sin(AngleY) + sin(AngleX)*sin(AngleZ))) / PI,
		180 * (Point.X*cos(AngleY)*sin(AngleZ) + Point.Z*(-cos(AngleZ)*sin(AngleX) + cos(AngleX)*sin(AngleY)*sin(AngleZ)) + Point.Y*(cos(AngleX)*cos(AngleZ) + sin(AngleX)*sin(AngleY)*sin(AngleZ))) / PI,
		180 * (Point.Z*cos(AngleX)*cos(AngleY) + Point.Y*cos(AngleY)*sin(AngleX) + Point.X*sin(AngleY)) / PI);

}

FVector AProceduralCubeActor::UnRotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ)
{
	return FVector( 180 * (Point.X*cos(AngleY)*cos(AngleZ) - Point.Z*sin(AngleY) + Point.Y *cos(AngleY)*sin(AngleZ))/PI,
		180 * (Point.Z*cos(AngleY)*sin(AngleX) + Point.X*(cos(AngleZ)*sin(AngleX)*sin(AngleY) - cos(AngleX)*sin(AngleZ)) + Point.Y*(cos(AngleX)*cos(AngleZ) + sin(AngleX)*sin(AngleY)*sin(AngleZ)))/PI,
		180 * (Point.Z*cos(AngleX)*cos(AngleY) + Point.X*(cos(AngleX)*cos(AngleZ)*sin(AngleY) + sin(AngleX)*sin(AngleZ)) + Point.Y*(-cos(AngleZ)*sin(AngleX) + cos(AngleX)*sin(AngleY)*sin(AngleZ))) / PI);
}

// ------------------------------------------------ End - ROTATION FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\