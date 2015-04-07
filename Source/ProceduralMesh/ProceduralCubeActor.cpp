// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//


#include "ProceduralMesh.h"
#include "ProceduralCubeActor.h"





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - GENERATION FUNCTIONS ------------------------------------------------- \\

AProceduralCubeActor::AProceduralCubeActor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Define cube mesh and set it as cube root component
	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralCube"));
	RootComponent = mesh;

	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/BaseColor.BaseColor'"));
	
	// Apply a real material with textures, using UVs
	mesh->SetMaterial(0, Material.Object);
}

void AProceduralCubeActor::GenerateCube(FVector StarterP0Location, float XSize, float YSize, float ZSize, FColor VtxsColor)
{
	// P variables
	GenerateCubePs(StarterP0Location, XSize, YSize, ZSize);

	// Vertexes
	GenerateCubeVs();
	SetCubeVColors(VtxsColor);

	// Faces and Mesh triangles
	TArray<FProceduralMeshTriangle> OutTriangles;
	GenerateCubeFaces(OutTriangles);

	// Update cube image at World
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

void AProceduralCubeActor::GenerateCubeFaces(TArray<FProceduralMeshTriangle> & OutTriangles)
{

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
}

void AProceduralCubeActor::GenerateCubeFace(FProceduralMeshVertex GivenV0, FProceduralMeshVertex GivenV1, FProceduralMeshVertex GivenV2, FProceduralMeshVertex GivenV3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2)
{
	// Set vertexes components U, V
	GivenV0.U = 0.f; GivenV0.V = 0.f;
	GivenV1.U = 0.f; GivenV1.V = 1.f;
	GivenV2.U = 1.f; GivenV2.V = 1.f;
	GivenV3.U = 1.f; GivenV3.V = 0.f;
	
	// Face triangle t1
	t1.Vertex0 = GivenV0;
	t1.Vertex1 = GivenV1;
	t1.Vertex2 = GivenV2;

	// Face triangle t2
	t2.Vertex0 = GivenV0;
	t2.Vertex1 = GivenV2;
	t2.Vertex2 = GivenV3;
}

int32 AProceduralCubeActor::ExtrusionFromGivenFaceVertexes(AProceduralCubeActor* NewCube, TArray<FProceduralMeshVertex> FaceVertexes)
{
	// Set new cube location and rotation (the same that its parent cube has)
	NewCube->SetActorRotation(this->GetActorRotation());
	NewCube->SetActorLocation(this->GetActorLocation());

	// The extruded cube will have its parent cube edges sizes except one of them which will be smaller (the one in the orthogonal face direction) in order to extrude a smaller cube
	FVector ParentEdgesSizes = FVector(abs(p3.X - p0.X), abs(p7.Y - p0.Y), abs(p1.Z - p0.Z));
	float NewCubeSize = 10;
	FVector CrossProdVector = UnitVector(CrossProd(FaceVertexes[1].Position - FaceVertexes[0].Position, FaceVertexes[2].Position - FaceVertexes[0].Position));
	FVector NewCubeEdgesSizes = ParentEdgesSizes - ParentEdgesSizes * FVector(abs(CrossProdVector[0]), abs(CrossProdVector[1]), abs(CrossProdVector[2])) + NewCubeSize * FVector(abs(CrossProdVector[0]), abs(CrossProdVector[1]), abs(CrossProdVector[2]));

	// Integer representing which arrow will be moved after extruding (from 0 to 5)
	int32 ArrowIdToMoveAfterExtruding = IdentifyFaceFromVertexes(FaceVertexes[0], FaceVertexes[1], FaceVertexes[2], FaceVertexes[3]);

	// Calculate new cube P0 coordinates
	FVector NewCubeP0Coords;
	switch (ArrowIdToMoveAfterExtruding)
	{
		case 0: NewCubeP0Coords = FVector(p0.X, p0.Y - NewCubeEdgesSizes[1], p0.Z); break;
		case 1: NewCubeP0Coords = p7; break;
		case 2: NewCubeP0Coords = FVector(p0.X - NewCubeEdgesSizes[0], p0.Y, p0.Z); break;
		case 3: NewCubeP0Coords = p3; break;
		case 4: NewCubeP0Coords = p1; break;
		case 5: NewCubeP0Coords = FVector(p0.X, p0.Y, p0.Z - NewCubeEdgesSizes[2]); break;
		default: break;
	}
	
	// Generate new cube
	NewCube->GenerateCube(NewCubeP0Coords, NewCubeEdgesSizes[0], NewCubeEdgesSizes[1], NewCubeEdgesSizes[2], FColor::Red);

	// Add new cube to parent's cube extruded cubes list
	ExtrudedCubes.Add(NewCube);

	// Return integer representing the arrow to move after creating the new cube
	return ArrowIdToMoveAfterExtruding;
}

// ------------------------------------------------- End - GENERATION FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - VERTEX FUNCTIONS ------------------------------------------------- \\

FVector AProceduralCubeActor::FindAndMoveVertex(FVector MovementDirection, FProceduralMeshVertex VertexToMove, TArray<FProceduralMeshTriangle>& CurrentTriangles)
{
	FVector NewVertexPosition = VertexToMove.Position;

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
	if (v0.Id == VertexToUpdate.Id)		 { v0.Position = VertexToUpdate.Position; p0 = VertexToUpdate.Position; }
	else if (v1.Id == VertexToUpdate.Id) { v1.Position = VertexToUpdate.Position; p1 = VertexToUpdate.Position; }
	else if (v2.Id == VertexToUpdate.Id) { v2.Position = VertexToUpdate.Position; p2 = VertexToUpdate.Position; }
	else if (v3.Id == VertexToUpdate.Id) { v3.Position = VertexToUpdate.Position; p3 = VertexToUpdate.Position; }
	else if (v4.Id == VertexToUpdate.Id) { v4.Position = VertexToUpdate.Position; p4 = VertexToUpdate.Position; }
	else if (v5.Id == VertexToUpdate.Id) { v5.Position = VertexToUpdate.Position; p5 = VertexToUpdate.Position; }
	else if (v6.Id == VertexToUpdate.Id) { v6.Position = VertexToUpdate.Position; p6 = VertexToUpdate.Position; }
	else if (v7.Id == VertexToUpdate.Id) { v7.Position = VertexToUpdate.Position; p7 = VertexToUpdate.Position; }
}

int32 AProceduralCubeActor::IdentifyFaceFromVertexes(FProceduralMeshVertex FVertex0, FProceduralMeshVertex FVertex1, FProceduralMeshVertex FVertex2, FProceduralMeshVertex FVertex3)
{
	// Set cube faces vertexes arrays
	TArray<int32> FrontFace;	FrontFace.Add(v0.Id);	FrontFace.Add(v1.Id);	FrontFace.Add(v2.Id);	FrontFace.Add(v3.Id);
	TArray<int32> BackFace;		BackFace.Add(v4.Id);	BackFace.Add(v5.Id);	BackFace.Add(v6.Id);	BackFace.Add(v7.Id);
	TArray<int32> LeftFace;		LeftFace.Add(v7.Id);	LeftFace.Add(v6.Id);	LeftFace.Add(v1.Id);	LeftFace.Add(v0.Id);
	TArray<int32> RightFace;	RightFace.Add(v3.Id);	RightFace.Add(v2.Id);	RightFace.Add(v5.Id);	RightFace.Add(v4.Id);
	TArray<int32> TopFace;		TopFace.Add(v1.Id);		TopFace.Add(v6.Id);		TopFace.Add(v5.Id);		TopFace.Add(v2.Id);
	TArray<int32> BottomFace;	BottomFace.Add(v3.Id);	BottomFace.Add(v4.Id);	BottomFace.Add(v7.Id);	BottomFace.Add(v0.Id);

	// If given vertexes are FRONT FACE vertexes, 0 will be returned
	if (VectorContainsElement(FrontFace, FVertex0.Id) && VectorContainsElement(FrontFace, FVertex1.Id) && VectorContainsElement(FrontFace, FVertex2.Id) && VectorContainsElement(FrontFace, FVertex3.Id)) { return 0; }
	
	// If given vertexes are BACK FACE vertexes, 1 will be returned
	else if (VectorContainsElement(BackFace, FVertex0.Id) && VectorContainsElement(BackFace, FVertex1.Id) && VectorContainsElement(BackFace, FVertex2.Id) && VectorContainsElement(BackFace, FVertex3.Id)) { return 1; }

	// If given vertexes are LEFT FACE vertexes, 2 will be returned
	else if (VectorContainsElement(LeftFace, FVertex0.Id) && VectorContainsElement(LeftFace, FVertex1.Id) && VectorContainsElement(LeftFace, FVertex2.Id) && VectorContainsElement(LeftFace, FVertex3.Id)) { return 2; }
	
	// If given vertexes are RIGHT FACE vertexes, 3 will be returned
	else if (VectorContainsElement(RightFace, FVertex0.Id) && VectorContainsElement(RightFace, FVertex1.Id) && VectorContainsElement(RightFace, FVertex2.Id) && VectorContainsElement(RightFace, FVertex3.Id)) { return 3; }
	
	// If given vertexes are TOP FACE vertexes, 4 will be returned
	else if (VectorContainsElement(TopFace, FVertex0.Id) && VectorContainsElement(TopFace, FVertex1.Id) && VectorContainsElement(TopFace, FVertex2.Id) && VectorContainsElement(TopFace, FVertex3.Id)) { return 4; }
	
	// If given vertexes are BOTTOM FACE vertexes, 5 will be returned
	else if (VectorContainsElement(BottomFace, FVertex0.Id) && VectorContainsElement(BottomFace, FVertex1.Id) && VectorContainsElement(BottomFace, FVertex2.Id) && VectorContainsElement(BottomFace, FVertex3.Id)) { return 5; }

	// If given vertexes doesn't match any face vertexes, -1 will be returned
	else { return -1; }
}

// ------------------------------------------------- End - VERTEX FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - FACES and ARROWS FUNCTIONS ------------------------------------------------- \\

TArray<FProceduralMeshVertex> AProceduralCubeActor::MoveFace(float MovementSign, TArray<FProceduralMeshVertex> VertexesArray)
{
	// Calculate movement direction (orthogonal vector)
	FVector v0v1 = VertexesArray[1].Position - VertexesArray[0].Position;
	FVector v1v2 = VertexesArray[2].Position - VertexesArray[1].Position;
	FVector Direction = UnitVector(v0v1.CrossProduct(v0v1, v1v2));
	if (MovementSign < 0) { Direction = - Direction; }
	
	// Move given vertexes = update vertexes and p's positions (it is possible that MovementSign = 0, so in this case no movement should take place)
	if (MovementSign != 0)
	{
		TArray<FProceduralMeshTriangle> MeshTriangles = mesh->GetProceduralMeshTriangles();
		VertexesArray[0].Position = FindAndMoveVertex(Direction, VertexesArray[0], MeshTriangles);
		VertexesArray[1].Position = FindAndMoveVertex(Direction, VertexesArray[1], MeshTriangles);
		VertexesArray[2].Position = FindAndMoveVertex(Direction, VertexesArray[2], MeshTriangles);
		VertexesArray[3].Position = FindAndMoveVertex(Direction, VertexesArray[3], MeshTriangles);
		mesh->SetProceduralMeshTriangles(MeshTriangles);
	}

	return VertexesArray;
}

FVector AProceduralCubeActor::CalculateFaceMiddlePoint(TArray<FProceduralMeshVertex> FaceVertexes)
{
	// Calculate 'vertex to vertex' vectors
	FVector V01 = FaceVertexes[1].Position - FaceVertexes[0].Position;
	FVector V02 = FaceVertexes[2].Position - FaceVertexes[0].Position;
	FVector V03 = FaceVertexes[3].Position - FaceVertexes[0].Position;

	// Calculate 'vertex to vertex' distances
	float D01 = sqrt(V01[0] * V01[0] + V01[1] * V01[1] + V01[2] * V01[2]);
	float D02 = sqrt(V02[0] * V02[0] + V02[1] * V02[1] + V02[2] * V02[2]);
	float D03 = sqrt(V03[0] * V03[0] + V03[1] * V03[1] + V03[2] * V03[2]);

	// Find 'vertex to vertex' max distance and return the 'middle' point of it
	if (D01 > D02 && D01 > D03) { return FVector((FaceVertexes[0].Position + FaceVertexes[1].Position) / 2); }
	if (D02 > D01 && D02 > D03) { return FVector((FaceVertexes[0].Position + FaceVertexes[2].Position) / 2); }
	return FVector((FaceVertexes[0].Position + FaceVertexes[3].Position) / 2);
}

TArray<FProceduralMeshVertex> AProceduralCubeActor::FindFaceVertexesFromArrowLocation(FVector ArrowLocation)
{	
	// Store each euclidean distance from each cube vertex to the given arrow
	FProceduralMeshVertex CubeVertexes[] = { v0, v1, v2, v3, v4, v5, v6, v7 };
	float VertexesDistances[8];
	for (int i = 0; i < 8; i++) { VertexesDistances[i] = EuclideanDistance(CubeVertexes[i].Position, ArrowLocation); }

	// Calculate the 4 nearer vertex
	float MinDist; int iMinDist; FProceduralMeshVertex VertexMinDist;
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

	// Rotator to set face orthogonal direction
	return ConvertToPitchRollYawRotator(UnitVector(CrossProd(V12, V01)));
}

// ------------------------------------------------- End - FACE ARROWS FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------ Begin - AUX FUNCTIONS ------------------------------------------------ \\

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
		if (GivenVector[i] == GivenElement) { ElementFound = true; }
	}
	return ElementFound;
}

FRotator AProceduralCubeActor::ConvertToPitchRollYawRotator(FVector VToConvert)
{
	FRotator VConverted;
	VConverted.Roll = 90 * VToConvert[2];
	VConverted.Pitch = 90 * VToConvert[1];
	VConverted.Yaw = 90 * VToConvert[0];
	if (VConverted.Pitch != 0) // Necessary to correct from (x,y,z) to (r,p,y)
	{
		VConverted.Pitch = - VConverted.Pitch;
		VConverted.Roll = VConverted.Roll + VConverted.Pitch;
		VConverted.Yaw = VConverted.Yaw + VConverted.Pitch;
	}
	return VConverted;
}

// ------------------------------------------------ End - AUX FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\