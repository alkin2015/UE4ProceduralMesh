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
	// static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/M_Concrete_Poured.M_Concrete_Poured'"));
	mesh->SetMaterial(0, Material.Object);

	// Generate a cube
	TArray<FProceduralMeshTriangle> triangles;
	GenerateCube(100.f, triangles);
	mesh->SetProceduralMeshTriangles(triangles);

	RootComponent = mesh;

	// Enable OnHover, OnExitHover
//	this->OnBeginCursorOver.AddDynamic(this, &AProceduralCubeActor::PlayerOnHover);
//	this->OnEndCursorOver.AddDynamic(this, &AProceduralCubeActor::PlayerOnExitHover);

}

void AProceduralCubeActor::GenerateCube(const float& InSize, TArray<FProceduralMeshTriangle>& OutTriangles)
{

	GenerateCubePs(InSize);
	GenerateCubeVs();
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

void AProceduralCubeActor::GenerateCubePs(const float& InSize)
{
	p0 = FVector(0.f, 0.f, 0.f);
	p1 = FVector(0.f, 0.f, InSize);
	p2 = FVector(InSize, 0.f, InSize);
	p3 = FVector(InSize, 0.f, 0.f);
	p4 = FVector(InSize, InSize, 0.f);
	p5 = FVector(InSize, InSize, InSize);
	p6 = FVector(0.f, InSize, InSize);
	p7 = FVector(0.f, InSize, 0.f);
}

void AProceduralCubeActor::GenerateCubeVs()
{
	FColor VertexColor = FColor::Red;
	v0.Position = p0; v0.Id = 0; v0.Color = VertexColor;
	v1.Position = p1; v1.Id = 1; v1.Color = VertexColor;
	v2.Position = p2; v2.Id = 2; v2.Color = VertexColor;
	v3.Position = p3; v3.Id = 3; v3.Color = VertexColor;
	v4.Position = p4; v4.Id = 4; v4.Color = VertexColor;
	v5.Position = p5; v5.Id = 5; v5.Color = VertexColor;
	v6.Position = p6; v6.Id = 6; v6.Color = VertexColor;
	v7.Position = p7; v7.Id = 7; v7.Color = VertexColor;
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

// ------------------------------------------------ End - AUX FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------ Begin - ROTATION FUNCTIONS ------------------------------------------------ \\

FVector AProceduralCubeActor::RotateOnX(FVector Point, float Angle)
{
	return FVector(Point.X, Point.Y*cos(Angle) - Point.Z*sin(Angle), Point.Z*cos(Angle) + Point.Y*sin(Angle));
}

FVector AProceduralCubeActor::UnRotateOnX(FVector Point, float Angle)
{
	return FVector(Point.X, Point.Y*cos(Angle) + Point.Z*sin(Angle), Point.Z*cos(Angle) - Point.Y*sin(Angle));
}

FVector AProceduralCubeActor::RotateOnY(FVector Point, float Angle)
{
	return FVector(Point.X*cos(Angle) + Point.Z*sin(Angle), Point.Y, Point.Z*cos(Angle) - Point.X*sin(Angle));
}

FVector AProceduralCubeActor::UnRotateOnY(FVector Point, float Angle)
{
	return FVector(Point.X*cos(Angle) - Point.Z*sin(Angle), Point.Y, Point.Z*cos(Angle) + Point.X*sin(Angle));
}

FVector AProceduralCubeActor::RotateOnZ(FVector Point, float Angle)
{
	return FVector(Point.X*cos(Angle) - Point.Y*sin(Angle), Point.Y*cos(Angle) + Point.X*sin(Angle), Point.Z);
}

FVector AProceduralCubeActor::UnRotateOnZ(FVector Point, float Angle)
{
	return FVector(Point.X*cos(Angle) + Point.Y*sin(Angle), Point.Y*cos(Angle) - Point.X*sin(Angle), Point.Z);
}

FVector AProceduralCubeActor::RotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ)
{
	return FVector(Point.X*cos(AngleY)*cos(AngleZ) + Point.Y*(cos(AngleZ)*sin(AngleX)*sin(AngleY) - cos(AngleX)*sin(AngleZ)) + Point.Z *(cos(AngleX)*cos(AngleZ)*sin(AngleY) + sin(AngleX)*sin(AngleZ)),
		Point.X*cos(AngleY)*sin(AngleZ) + Point.Z*(-cos(AngleZ)*sin(AngleX) + cos(AngleX)*sin(AngleY)*sin(AngleZ)) + Point.Y*(cos(AngleX)*cos(AngleZ) + sin(AngleX)*sin(AngleY)*sin(AngleZ)),
		Point.Z*cos(AngleX)*cos(AngleY) + Point.Y*cos(AngleY)*sin(AngleX) + Point.X*sin(AngleY));

}

FVector AProceduralCubeActor::UnRotateOnXYZ(FVector Point, float AngleX, float AngleY, float AngleZ)
{
	return FVector(Point.X*cos(AngleY)*cos(AngleZ) - Point.Z*sin(AngleY) + Point.Y *cos(AngleY)*sin(AngleZ),
		Point.Z*cos(AngleY)*sin(AngleX) + Point.X*(cos(AngleZ)*sin(AngleX)*sin(AngleY) - cos(AngleX)*sin(AngleZ)) + Point.Y*(cos(AngleX)*cos(AngleZ) + sin(AngleX)*sin(AngleY)*sin(AngleZ)),
		Point.Z*cos(AngleX)*cos(AngleY) + Point.X*(cos(AngleX)*cos(AngleZ)*sin(AngleY) + sin(AngleX)*sin(AngleZ)) + Point.Y*(-cos(AngleZ)*sin(AngleX) + cos(AngleX)*sin(AngleY)*sin(AngleZ)));
}

// ------------------------------------------------ End - ROTATION FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\