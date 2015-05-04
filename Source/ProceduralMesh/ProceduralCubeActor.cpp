// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)
//


#include "ProceduralMesh.h"
#include "ProceduralCubeActor.h"





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - GENERATION FUNCTIONS ------------------------------------------------- \\

AProceduralCubeActor::AProceduralCubeActor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->SetActorTickEnabled(true);

	KeepOnHover = false;
	VertexMovementState = false;

	// Define cube mesh and set it as cube root component
	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralCube"));
	RootComponent = mesh;

	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Materials/BaseColor.BaseColor'"));
	
	// Apply a real material with textures, using UVs
	mesh->SetMaterial(0, Material.Object);

	// Define SphereMeshComponents and ArrowMeshComponents
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereStaticMesh(TEXT("/Game/Sphere_Brush_StaticMesh.Sphere_Brush_StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowStaticMesh(TEXT("/Game/SM_Arrow3.SM_Arrow3"));
	float SpheresScale = 0.5; float FaceArrowsScale = 0.1; float VertexArrowScale = 0.3; float FaceSphereScale = 0.8;
	TArray<FName> VertexSphereTag, FaceArrowTag, VertexArrowTag, FaceSphereTag;
	VertexSphereTag.Add("VertexSphere"); FaceArrowTag.Add("FaceArrow"); VertexArrowTag.Add("VertexArrow"); FaceSphereTag.Add("FaceSphere");
	
	// Set vertexes spheres
	V0Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V0Sphere")); ModifyStaticMeshComponent(V0Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V1Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V1Sphere")); ModifyStaticMeshComponent(V1Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V2Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V2Sphere")); ModifyStaticMeshComponent(V2Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V3Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V3Sphere")); ModifyStaticMeshComponent(V3Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V4Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V4Sphere")); ModifyStaticMeshComponent(V4Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V5Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V5Sphere")); ModifyStaticMeshComponent(V5Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V6Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V6Sphere")); ModifyStaticMeshComponent(V6Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	V7Sphere = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V7Sphere")); ModifyStaticMeshComponent(V7Sphere, SphereStaticMesh.Object, SpheresScale, VertexSphereTag);
	
	// Faces arrows
	FrontFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FrontFaceArrow"));	ModifyStaticMeshComponent(FrontFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);
	BackFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BackFaceArrow"));		ModifyStaticMeshComponent(BackFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);
	LeftFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LeftFaceArrow"));		ModifyStaticMeshComponent(LeftFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);
	RightFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RightFaceArrow"));	ModifyStaticMeshComponent(RightFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);
	TopFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TopFaceArrow"));		ModifyStaticMeshComponent(TopFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);
	BottomFaceArrow = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BottomFaceArrow"));	ModifyStaticMeshComponent(BottomFaceArrow, ArrowStaticMesh.Object, FaceArrowsScale, FaceArrowTag);

	// Vertex arrows
	V0Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V0Sphere_Arrow0")); ModifyStaticMeshComponent(V0Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V0Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V0Sphere_Arrow1")); ModifyStaticMeshComponent(V0Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V0Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V0Sphere_Arrow2")); ModifyStaticMeshComponent(V0Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V1Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V1Sphere_Arrow0")); ModifyStaticMeshComponent(V1Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V1Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V1Sphere_Arrow1")); ModifyStaticMeshComponent(V1Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V1Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V1Sphere_Arrow2")); ModifyStaticMeshComponent(V1Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V2Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V2Sphere_Arrow0")); ModifyStaticMeshComponent(V2Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V2Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V2Sphere_Arrow1")); ModifyStaticMeshComponent(V2Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V2Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V2Sphere_Arrow2")); ModifyStaticMeshComponent(V2Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V3Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V3Sphere_Arrow0")); ModifyStaticMeshComponent(V3Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V3Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V3Sphere_Arrow1")); ModifyStaticMeshComponent(V3Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V3Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V3Sphere_Arrow2")); ModifyStaticMeshComponent(V3Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V4Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V4Sphere_Arrow0")); ModifyStaticMeshComponent(V4Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V4Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V4Sphere_Arrow1")); ModifyStaticMeshComponent(V4Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V4Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V4Sphere_Arrow2")); ModifyStaticMeshComponent(V4Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V5Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V5Sphere_Arrow0")); ModifyStaticMeshComponent(V5Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V5Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V5Sphere_Arrow1")); ModifyStaticMeshComponent(V5Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V5Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V5Sphere_Arrow2")); ModifyStaticMeshComponent(V5Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V6Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V6Sphere_Arrow0")); ModifyStaticMeshComponent(V6Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V6Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V6Sphere_Arrow1")); ModifyStaticMeshComponent(V6Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V6Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V6Sphere_Arrow2")); ModifyStaticMeshComponent(V6Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V7Sphere_Arrow0 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V7Sphere_Arrow0")); ModifyStaticMeshComponent(V7Sphere_Arrow0, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V7Sphere_Arrow1 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V7Sphere_Arrow1")); ModifyStaticMeshComponent(V7Sphere_Arrow1, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);
	V7Sphere_Arrow2 = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("V7Sphere_Arrow2")); ModifyStaticMeshComponent(V7Sphere_Arrow2, ArrowStaticMesh.Object, VertexArrowScale, VertexArrowTag);

	HideAllComponents();
}

void AProceduralCubeActor::ModifyStaticMeshComponent(UStaticMeshComponent* GivenComponent, UStaticMesh* GivenStaticMesh, float GivenScale, TArray<FName> GivenTag)
{
	GivenComponent->StaticMesh = GivenStaticMesh;
	GivenComponent->SetWorldScale3D(FVector(GivenScale, GivenScale, GivenScale));
	GivenComponent->AttachTo(mesh);
	GivenComponent->ComponentTags = GivenTag;
}

void AProceduralCubeActor::GenerateCube(FVector StarterP0Location, float XSize, float YSize, float ZSize, FColor VtxsColor, APlayerController* GivenPController)
{
	// Set Player Controller
	CustomPController = Cast<ACustomPlayerController>(GivenPController);

	// P variables
	GenerateCubePs(StarterP0Location, XSize, YSize, ZSize);

	// Vertexes
	GenerateCubeVs();
	SetCubeVColors(VtxsColor);
	UpdateVsSpheresLocations();
	UpdateVertexArrowsLocationsAndRotations();

	// Faces and Mesh triangles
	TArray<FProceduralMeshTriangle> OutTriangles;
	GenerateCubeFaces(OutTriangles);
	UpdateFacesArrowsLocationsAndRotations();

	// Update cube image at World
	mesh->SetProceduralMeshTriangles(OutTriangles);

	// Enable cursor on hover
	this->OnBeginCursorOver.AddDynamic(this, &AProceduralCubeActor::PlayerOnHover);
	this->OnEndCursorOver.AddDynamic(this, &AProceduralCubeActor::PlayerOnExitHover);
}

void AProceduralCubeActor::GenerateCubePs(FVector P0Coords, float XSize, float YSize, float ZSize)
{
	CubePointsVector.Add(P0Coords + FVector(0.f, 0.f, 0.f));
	CubePointsVector.Add(P0Coords + FVector(0.f, 0.f, ZSize));
	CubePointsVector.Add(P0Coords + FVector(XSize, 0.f, ZSize));
	CubePointsVector.Add(P0Coords + FVector(XSize, 0.f, 0.f));
	CubePointsVector.Add(P0Coords + FVector(XSize, YSize, 0.f));
	CubePointsVector.Add(P0Coords + FVector(XSize, YSize, ZSize));
	CubePointsVector.Add(P0Coords + FVector(0.f, YSize, ZSize));
	CubePointsVector.Add(P0Coords + FVector(0.f, YSize, 0.f));
}

void AProceduralCubeActor::GenerateCubeVs()
{
	for (int32 i = 0; i < 8; i++)
	{
		CubeVertexesVector.Add(FProceduralMeshVertex());
		CubeVertexesVector[i].Position = CubePointsVector[i];
		CubeVertexesVector[i].Id = i;
	}

	FrontFaceVertexes.Add(CubeVertexesVector[0]);	FrontFaceVertexes.Add(CubeVertexesVector[1]);	FrontFaceVertexes.Add(CubeVertexesVector[2]);	FrontFaceVertexes.Add(CubeVertexesVector[3]);
	BackFaceVertexes.Add(CubeVertexesVector[4]);	BackFaceVertexes.Add(CubeVertexesVector[5]);	BackFaceVertexes.Add(CubeVertexesVector[6]);	BackFaceVertexes.Add(CubeVertexesVector[7]);
	LeftFaceVertexes.Add(CubeVertexesVector[7]);	LeftFaceVertexes.Add(CubeVertexesVector[6]);	LeftFaceVertexes.Add(CubeVertexesVector[1]);	LeftFaceVertexes.Add(CubeVertexesVector[0]);
	RightFaceVertexes.Add(CubeVertexesVector[3]);	RightFaceVertexes.Add(CubeVertexesVector[2]);	RightFaceVertexes.Add(CubeVertexesVector[5]);	RightFaceVertexes.Add(CubeVertexesVector[4]);
	TopFaceVertexes.Add(CubeVertexesVector[1]);		TopFaceVertexes.Add(CubeVertexesVector[6]);		TopFaceVertexes.Add(CubeVertexesVector[5]);		TopFaceVertexes.Add(CubeVertexesVector[2]);
	BottomFaceVertexes.Add(CubeVertexesVector[3]);	BottomFaceVertexes.Add(CubeVertexesVector[4]);	BottomFaceVertexes.Add(CubeVertexesVector[7]);	BottomFaceVertexes.Add(CubeVertexesVector[0]);
}

void AProceduralCubeActor::UpdateVsSpheresLocations()
{
	V0Sphere->SetRelativeLocation(CubePointsVector[0]);	V1Sphere->SetRelativeLocation(CubePointsVector[1]);
	V2Sphere->SetRelativeLocation(CubePointsVector[2]);	V3Sphere->SetRelativeLocation(CubePointsVector[3]);
	V4Sphere->SetRelativeLocation(CubePointsVector[4]);	V5Sphere->SetRelativeLocation(CubePointsVector[5]);
	V6Sphere->SetRelativeLocation(CubePointsVector[6]);	V7Sphere->SetRelativeLocation(CubePointsVector[7]);
}

void AProceduralCubeActor::UpdateFacesArrowsLocationsAndRotations()
{
	// Update Faces Arrows locations and rotations
	FVector FrontFaceArrowLocation = CalculateFaceMiddlePoint(FrontFaceVertexes);
	FrontFaceArrow->SetRelativeLocationAndRotation(FrontFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(FrontFaceArrowLocation, FrontFaceVertexes));
	
	FVector BackFaceArrowLocation = CalculateFaceMiddlePoint(BackFaceVertexes);
	BackFaceArrow->SetRelativeLocationAndRotation(BackFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(BackFaceArrowLocation, BackFaceVertexes));
	
	FVector LeftFaceArrowLocation = CalculateFaceMiddlePoint(LeftFaceVertexes);
	LeftFaceArrow->SetRelativeLocationAndRotation(LeftFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(LeftFaceArrowLocation, LeftFaceVertexes));
	
	FVector RightFaceArrowLocation = CalculateFaceMiddlePoint(RightFaceVertexes);
	RightFaceArrow->SetRelativeLocationAndRotation(RightFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(RightFaceArrowLocation, RightFaceVertexes));
	
	FVector TopFaceArrowLocation = CalculateFaceMiddlePoint(TopFaceVertexes);
	TopFaceArrow->SetRelativeLocationAndRotation(TopFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(TopFaceArrowLocation, TopFaceVertexes));

	FVector BottomFaceArrowLocation = CalculateFaceMiddlePoint(BottomFaceVertexes);
	BottomFaceArrow->SetRelativeLocationAndRotation(BottomFaceArrowLocation, GetOrtogonalFaceDirectionFromFaceVertex(BottomFaceArrowLocation, BottomFaceVertexes));
}

void AProceduralCubeActor::UpdateVertexArrowsLocationsAndRotations()
{
	// Set 'natural' directions
	FRotator XDirection = FRotator::MakeFromEuler(FVector(90, 0, 0));
	FRotator YDirection = FRotator::MakeFromEuler(FVector(0, 90, 0));
	FRotator ZDirection = FRotator::MakeFromEuler(FVector(0, 0, 90));
	
	// Update Vertex Arrows locations
	V0Sphere_Arrow0->SetRelativeLocation(CubePointsVector[0]);		V0Sphere_Arrow1->SetRelativeLocation(CubePointsVector[0]);		V0Sphere_Arrow2->SetRelativeLocation(CubePointsVector[0]);
	V0Sphere_Arrow0->SetWorldRotation(XDirection);	V0Sphere_Arrow1->SetWorldRotation(YDirection);	V0Sphere_Arrow2->SetWorldRotation(ZDirection);

	V1Sphere_Arrow0->SetRelativeLocation(CubePointsVector[1]);		V1Sphere_Arrow1->SetRelativeLocation(CubePointsVector[1]);		V1Sphere_Arrow2->SetRelativeLocation(CubePointsVector[1]);
	V1Sphere_Arrow0->SetWorldRotation(XDirection);	V1Sphere_Arrow1->SetWorldRotation(YDirection);	V1Sphere_Arrow2->SetWorldRotation(ZDirection);

	V2Sphere_Arrow0->SetRelativeLocation(CubePointsVector[2]);		V2Sphere_Arrow1->SetRelativeLocation(CubePointsVector[2]);		V2Sphere_Arrow2->SetRelativeLocation(CubePointsVector[2]);
	V2Sphere_Arrow0->SetWorldRotation(XDirection);	V2Sphere_Arrow1->SetWorldRotation(YDirection);	V2Sphere_Arrow2->SetWorldRotation(ZDirection);

	V3Sphere_Arrow0->SetRelativeLocation(CubePointsVector[3]);		V3Sphere_Arrow1->SetRelativeLocation(CubePointsVector[3]);		V3Sphere_Arrow2->SetRelativeLocation(CubePointsVector[3]);
	V3Sphere_Arrow0->SetWorldRotation(XDirection);	V3Sphere_Arrow1->SetWorldRotation(YDirection);	V3Sphere_Arrow2->SetWorldRotation(ZDirection);

	V4Sphere_Arrow0->SetRelativeLocation(CubePointsVector[4]);		V4Sphere_Arrow1->SetRelativeLocation(CubePointsVector[4]);		V4Sphere_Arrow2->SetRelativeLocation(CubePointsVector[4]);
	V4Sphere_Arrow0->SetWorldRotation(XDirection);	V4Sphere_Arrow1->SetWorldRotation(YDirection);	V4Sphere_Arrow2->SetWorldRotation(ZDirection);

	V5Sphere_Arrow0->SetRelativeLocation(CubePointsVector[5]);		V5Sphere_Arrow1->SetRelativeLocation(CubePointsVector[5]);		V5Sphere_Arrow2->SetRelativeLocation(CubePointsVector[5]);
	V5Sphere_Arrow0->SetWorldRotation(XDirection);	V5Sphere_Arrow1->SetWorldRotation(YDirection);	V5Sphere_Arrow2->SetWorldRotation(ZDirection);

	V6Sphere_Arrow0->SetRelativeLocation(CubePointsVector[6]);		V6Sphere_Arrow1->SetRelativeLocation(CubePointsVector[6]);		V6Sphere_Arrow2->SetRelativeLocation(CubePointsVector[6]);
	V6Sphere_Arrow0->SetWorldRotation(XDirection);	V6Sphere_Arrow1->SetWorldRotation(YDirection);	V6Sphere_Arrow2->SetWorldRotation(ZDirection);

	V7Sphere_Arrow0->SetRelativeLocation(CubePointsVector[7]);		V7Sphere_Arrow1->SetRelativeLocation(CubePointsVector[7]);		V7Sphere_Arrow2->SetRelativeLocation(CubePointsVector[7]);
	V7Sphere_Arrow0->SetWorldRotation(XDirection);	V7Sphere_Arrow1->SetWorldRotation(YDirection);	V7Sphere_Arrow2->SetWorldRotation(ZDirection);
}

void AProceduralCubeActor::SetCubeVColors(FColor VertexColor)
{
	CubeVertexesVector[0].Color = CubeVertexesVector[1].Color = CubeVertexesVector[2].Color = CubeVertexesVector[3].Color = CubeVertexesVector[4].Color = CubeVertexesVector[5].Color = CubeVertexesVector[6].Color = CubeVertexesVector[7].Color = VertexColor;
}

void AProceduralCubeActor::GenerateCubeFaces(TArray<FProceduralMeshTriangle> & OutTriangles)
{
	FProceduralMeshTriangle t1, t2;

	// front face
	GenerateCubeFace(CubeVertexesVector[0], CubeVertexesVector[1], CubeVertexesVector[2], CubeVertexesVector[3], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);

	// back face
	GenerateCubeFace(CubeVertexesVector[4], CubeVertexesVector[5], CubeVertexesVector[6], CubeVertexesVector[7], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);

	// left face
	GenerateCubeFace(CubeVertexesVector[7], CubeVertexesVector[6], CubeVertexesVector[1], CubeVertexesVector[0], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);

	// right face
	GenerateCubeFace(CubeVertexesVector[3], CubeVertexesVector[2], CubeVertexesVector[5], CubeVertexesVector[4], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);

	// top face
	GenerateCubeFace(CubeVertexesVector[1], CubeVertexesVector[6], CubeVertexesVector[5], CubeVertexesVector[2], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);

	// bottom face
	GenerateCubeFace(CubeVertexesVector[3], CubeVertexesVector[4], CubeVertexesVector[7], CubeVertexesVector[0], t1, t2);
	OutTriangles.Add(t1);	OutTriangles.Add(t2);
}

void AProceduralCubeActor::GenerateCubeFace(FProceduralMeshVertex GivenV0, FProceduralMeshVertex GivenV1, FProceduralMeshVertex GivenV2, FProceduralMeshVertex GivenV3, FProceduralMeshTriangle& t1, FProceduralMeshTriangle& t2)
{
	// Set vertexes components U, V
	GivenV0.U = 0.f;	GivenV1.U = 0.f;	GivenV2.U = 1.f;	GivenV3.U = 1.f;
	GivenV0.V = 0.f;	GivenV1.V = 1.f;	GivenV2.V = 1.f;	GivenV3.V = 0.f;
	
	// Face triangle t1
	t1.Vertex0 = GivenV0;	t1.Vertex1 = GivenV1;	t1.Vertex2 = GivenV2;

	// Face triangle t2
	t2.Vertex0 = GivenV0;	t2.Vertex1 = GivenV2;	t2.Vertex2 = GivenV3;
}

int32 AProceduralCubeActor::ExtrusionFromGivenFaceVertexes(AProceduralCubeActor* NewCube, TArray<FProceduralMeshVertex> FaceVertexes)
{
	this->HideAllComponents();
	
	// Set new cube location and rotation (the same that its parent cube has)
	NewCube->SetActorRotation(this->GetActorRotation());
	NewCube->SetActorLocation(this->GetActorLocation());

	// The extruded cube will have its parent cube edges sizes except one of them which will be smaller (the one in the orthogonal face direction) in order to extrude a smaller cube
	FVector ParentEdgesSizes = FVector(abs(CubePointsVector[3].X - CubePointsVector[0].X), abs(CubePointsVector[7].Y - CubePointsVector[0].Y), abs(CubePointsVector[1].Z - CubePointsVector[0].Z));
	float NewCubeSize = 10;
	FVector CrossProdVector = UnitVector(CrossProd(FaceVertexes[1].Position - FaceVertexes[0].Position, FaceVertexes[2].Position - FaceVertexes[0].Position));
	FVector NewCubeEdgesSizes = ParentEdgesSizes - ParentEdgesSizes * FVector(abs(CrossProdVector[0]), abs(CrossProdVector[1]), abs(CrossProdVector[2])) + NewCubeSize * FVector(abs(CrossProdVector[0]), abs(CrossProdVector[1]), abs(CrossProdVector[2]));

	// Integer representing which arrow will be moved after extruding (from 0 to 5)
	int32 ArrowIdToMoveAfterExtruding = IdentifyFaceFromVertexes(FaceVertexes[0], FaceVertexes[1], FaceVertexes[2], FaceVertexes[3]);

	// Calculate new cube P0 coordinates
	FVector NewCubeP0Coords;
	switch (ArrowIdToMoveAfterExtruding)
	{
	case 0: NewCubeP0Coords = FVector(CubePointsVector[0].X, CubePointsVector[0].Y - NewCubeEdgesSizes[1], CubePointsVector[0].Z);		break;
	case 1: NewCubeP0Coords = CubePointsVector[7];	break;
	case 2: NewCubeP0Coords = FVector(CubePointsVector[0].X - NewCubeEdgesSizes[0], CubePointsVector[0].Y, CubePointsVector[0].Z);		break;
	case 3: NewCubeP0Coords = CubePointsVector[3];	break;
	case 4: NewCubeP0Coords = CubePointsVector[1];	break;
	case 5: NewCubeP0Coords = FVector(CubePointsVector[0].X, CubePointsVector[0].Y, CubePointsVector[0].Z - NewCubeEdgesSizes[2]);		break;
	default:	break;
	}
	
	// Generate new cube
	NewCube->GenerateCube(NewCubeP0Coords, NewCubeEdgesSizes[0], NewCubeEdgesSizes[1], NewCubeEdgesSizes[2], FColor::Red, CustomPController);
	NewCube->InitFacesMovements();

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
	UpdateVertexArrowsLocationsAndRotations();
	UpdateFacesArrowsLocationsAndRotations();
	return NewVertexPosition;
}

void AProceduralCubeActor::UpdateCubeVertexLocation(FProceduralMeshVertex VertexToUpdate)
{
	if (CubeVertexesVector[0].Id == VertexToUpdate.Id)		{ CubeVertexesVector[0].Position = VertexToUpdate.Position; CubePointsVector[0] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[1].Id == VertexToUpdate.Id) { CubeVertexesVector[1].Position = VertexToUpdate.Position; CubePointsVector[1] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[2].Id == VertexToUpdate.Id) { CubeVertexesVector[2].Position = VertexToUpdate.Position; CubePointsVector[2] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[3].Id == VertexToUpdate.Id) { CubeVertexesVector[3].Position = VertexToUpdate.Position; CubePointsVector[3] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[4].Id == VertexToUpdate.Id) { CubeVertexesVector[4].Position = VertexToUpdate.Position; CubePointsVector[4] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[5].Id == VertexToUpdate.Id) { CubeVertexesVector[5].Position = VertexToUpdate.Position; CubePointsVector[5] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[6].Id == VertexToUpdate.Id) { CubeVertexesVector[6].Position = VertexToUpdate.Position; CubePointsVector[6] = VertexToUpdate.Position; }
	else if (CubeVertexesVector[7].Id == VertexToUpdate.Id) { CubeVertexesVector[7].Position = VertexToUpdate.Position; CubePointsVector[7] = VertexToUpdate.Position; }
	
	UpdateVsSpheresLocations();
	UpdateVertexArrowsLocationsAndRotations();
}

int32 AProceduralCubeActor::IdentifyFaceFromVertexes(FProceduralMeshVertex FVertex0, FProceduralMeshVertex FVertex1, FProceduralMeshVertex FVertex2, FProceduralMeshVertex FVertex3)
{
	// Set cube faces vertexes arrays
	TArray<int32> FrontFace;	FrontFace.Add(CubeVertexesVector[0].Id);	FrontFace.Add(CubeVertexesVector[1].Id);	FrontFace.Add(CubeVertexesVector[2].Id);	FrontFace.Add(CubeVertexesVector[3].Id);
	TArray<int32> BackFace;		BackFace.Add(CubeVertexesVector[4].Id);		BackFace.Add(CubeVertexesVector[5].Id);		BackFace.Add(CubeVertexesVector[6].Id);		BackFace.Add(CubeVertexesVector[7].Id);
	TArray<int32> LeftFace;		LeftFace.Add(CubeVertexesVector[7].Id);		LeftFace.Add(CubeVertexesVector[6].Id);		LeftFace.Add(CubeVertexesVector[1].Id);		LeftFace.Add(CubeVertexesVector[0].Id);
	TArray<int32> RightFace;	RightFace.Add(CubeVertexesVector[3].Id);	RightFace.Add(CubeVertexesVector[2].Id);	RightFace.Add(CubeVertexesVector[5].Id);	RightFace.Add(CubeVertexesVector[4].Id);
	TArray<int32> TopFace;		TopFace.Add(CubeVertexesVector[1].Id);		TopFace.Add(CubeVertexesVector[6].Id);		TopFace.Add(CubeVertexesVector[5].Id);		TopFace.Add(CubeVertexesVector[2].Id);
	TArray<int32> BottomFace;	BottomFace.Add(CubeVertexesVector[3].Id);	BottomFace.Add(CubeVertexesVector[4].Id);	BottomFace.Add(CubeVertexesVector[7].Id);	BottomFace.Add(CubeVertexesVector[0].Id);

	if		(FrontFace.Find(FVertex0.Id)	&& FrontFace.Find(FVertex1.Id)	&& FrontFace.Find(FVertex2.Id)	&& FrontFace.Find(FVertex3.Id))		{ return 0; }
	else if (BackFace.Find(FVertex0.Id)		&& BackFace.Find(FVertex1.Id)	&& BackFace.Find(FVertex2.Id)	&& BackFace.Find(FVertex3.Id))		{ return 1; }
	else if (LeftFace.Find(FVertex0.Id)		&& LeftFace.Find(FVertex1.Id)	&& LeftFace.Find(FVertex2.Id)	&& LeftFace.Find(FVertex3.Id))		{ return 2; }
	else if (RightFace.Find(FVertex0.Id)	&& RightFace.Find(FVertex1.Id)	&& RightFace.Find(FVertex2.Id)	&& RightFace.Find(FVertex3.Id))		{ return 3; }
	else if (TopFace.Find(FVertex0.Id)		&& TopFace.Find(FVertex1.Id)	&& TopFace.Find(FVertex2.Id)	&& TopFace.Find(FVertex3.Id))		{ return 4; }
	else if (BottomFace.Find(FVertex0.Id)	&& BottomFace.Find(FVertex1.Id) && BottomFace.Find(FVertex2.Id) && BottomFace.Find(FVertex3.Id))	{ return 5; }
	else{ return -1;  }
}

void AProceduralCubeActor::MoveVertexAlongWorldAxis(FVector AxisOfMovement, UStaticMeshComponent* ClickedSphere, FProceduralMeshVertex VToMove, float MovementSign)
{
	if (MovementSign != 0 && (AxisOfMovement.Equals(FVector(1, 0, 0)) || AxisOfMovement.Equals(FVector(0, 1, 0)) || AxisOfMovement.Equals(FVector(0, 0, 1))))
	{
		if (MovementSign < 0) { MovementSign = -1; }
		else { MovementSign = 1; }

		ClickedSphere->SetWorldLocation(ClickedSphere->GetComponentLocation() + AxisOfMovement * MovementSign);
		
		TArray<FProceduralMeshTriangle> CurrentTriangles = mesh->GetProceduralMeshTriangles();

		for (int i = 0; i < CurrentTriangles.Num(); i++)
		{
			if (CurrentTriangles[i].Vertex0.Id == VToMove.Id)		{	CurrentTriangles[i].Vertex0.Position = ClickedSphere->RelativeLocation;	}
			else if (CurrentTriangles[i].Vertex1.Id == VToMove.Id)	{	CurrentTriangles[i].Vertex1.Position = ClickedSphere->RelativeLocation;	}
			else if (CurrentTriangles[i].Vertex2.Id == VToMove.Id)	{	CurrentTriangles[i].Vertex2.Position = ClickedSphere->RelativeLocation;	}
		}

		VToMove.Position = ClickedSphere->RelativeLocation;
		UpdateCubeVertexLocation(VToMove);
		UpdateVsSpheresLocations();
		UpdateVertexArrowsLocationsAndRotations();
		mesh->SetProceduralMeshTriangles(CurrentTriangles);
	}
}

void AProceduralCubeActor::MoveVertexAlongRotatedAxis(FVector AxisOfMovement, UStaticMeshComponent* ClickedSphere, FProceduralMeshVertex VToMove, float MovementSign)
{
	if (MovementSign != 0 && (AxisOfMovement.Equals(FVector(1, 0, 0)) || AxisOfMovement.Equals(FVector(0, 1, 0)) || AxisOfMovement.Equals(FVector(0, 0, 1))))
	{
		if (MovementSign < 0) { MovementSign = -1; }
		else { MovementSign = 1; }

		ClickedSphere->SetRelativeLocation(ClickedSphere->RelativeLocation + AxisOfMovement * MovementSign);

		TArray<FProceduralMeshTriangle> CurrentTriangles = mesh->GetProceduralMeshTriangles();

		for (int i = 0; i < CurrentTriangles.Num(); i++)
		{
			if (CurrentTriangles[i].Vertex0.Id == VToMove.Id)		{ CurrentTriangles[i].Vertex0.Position = ClickedSphere->RelativeLocation; }
			else if (CurrentTriangles[i].Vertex1.Id == VToMove.Id)	{ CurrentTriangles[i].Vertex1.Position = ClickedSphere->RelativeLocation; }
			else if (CurrentTriangles[i].Vertex2.Id == VToMove.Id)	{ CurrentTriangles[i].Vertex2.Position = ClickedSphere->RelativeLocation; }
		}

		VToMove.Position = ClickedSphere->RelativeLocation;
		UpdateCubeVertexLocation(VToMove);
		UpdateVsSpheresLocations();
		UpdateVertexArrowsLocationsAndRotations();
		mesh->SetProceduralMeshTriangles(CurrentTriangles);
	}
}

// ------------------------------------------------- End - VERTEX FUNCTIONS ------------------------------------------------- \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - FACES and ARROWS FUNCTIONS ------------------------------------------------- \\

TArray<FProceduralMeshVertex> AProceduralCubeActor::MoveFace(float MovementSign, TArray<FProceduralMeshVertex> VertexesArray, UStaticMeshComponent* FaceArrow)
{
	// Calculate movement direction (orthogonal vector)
	FVector v0v1 = VertexesArray[1].Position - VertexesArray[0].Position;
	FVector v1v2 = VertexesArray[2].Position - VertexesArray[1].Position;
	FVector Direction = UnitVector(v0v1.CrossProduct(v0v1, v1v2));
	
	//FVector Direction;
	FRotator ArrowRot = FaceArrow->RelativeRotation;
	if (85 < abs(ArrowRot.Yaw) && abs(ArrowRot.Yaw) < 95) { Direction = FVector(1, 0, 0); }
	else if (85 < abs(ArrowRot.Pitch) && abs(ArrowRot.Pitch) < 95) { Direction = FVector(0, 1, 0); }
	else if (85 < abs(ArrowRot.Roll) && abs(ArrowRot.Roll) < 95) {  Direction = FVector(0, 0, 1); }

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
	FProceduralMeshVertex CubeVertexes[] = { CubeVertexesVector[0], CubeVertexesVector[1], CubeVertexesVector[2], CubeVertexesVector[3], CubeVertexesVector[4], CubeVertexesVector[5], CubeVertexesVector[6], CubeVertexesVector[7] };
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

AProceduralCubeActor* AProceduralCubeActor::ExtrudeFaceOfCube(UStaticMeshComponent* ClickedArrow)
{
	AProceduralCubeActor* NewCube = GetWorld()->SpawnActor<AProceduralCubeActor>(AProceduralCubeActor::StaticClass(), GetActorLocation(), GetActorRotation());
	EnableInput(CustomPController);
	
	int32 ArrowToExtrIndex = ExtrusionFromGivenFaceVertexes(NewCube, FindFaceVertexesFromArrowLocation(ClickedArrow->RelativeLocation));
	
	// Generate extruded cube and delete original cube clicked arrow to extrude
	switch (ArrowToExtrIndex)
	{
		case 0: NewCube->ArrowOnClickEvent(FrontFaceArrow, false, true, 0, 0);		FrontFaceArrow->SetHiddenInGame(true);		FrontFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	FrontFaceArrow->SetActive(false);	break;
		case 1: NewCube->ArrowOnClickEvent(BackFaceArrow, false, true, 0, 0);		BackFaceArrow->SetHiddenInGame(true);		BackFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	BackFaceArrow->SetActive(false);	break;
		case 2: NewCube->ArrowOnClickEvent(LeftFaceArrow, false, true, 0, 0);		LeftFaceArrow->SetHiddenInGame(true);		LeftFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	LeftFaceArrow->SetActive(false);	break;
		case 3: NewCube->ArrowOnClickEvent(RightFaceArrow, false, true, 0, 0);		RightFaceArrow->SetHiddenInGame(true);		RightFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	RightFaceArrow->SetActive(false);	break;
		case 4: NewCube->ArrowOnClickEvent(TopFaceArrow, false, true, 0, 0);		TopFaceArrow->SetHiddenInGame(true);		TopFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	TopFaceArrow->SetActive(false);		break;
		case 5: NewCube->ArrowOnClickEvent(BottomFaceArrow, false, true, 0, 0);		BottomFaceArrow->SetHiddenInGame(true);		BottomFaceArrow->SetWorldScale3D(FVector(0, 0, 0));	BottomFaceArrow->SetActive(false);	break;
	}
	return NewCube;
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





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - CUSTOM EVENTS ------------------------------------------------- \\

void AProceduralCubeActor::ArrowOnClickEvent_Implementation(UStaticMeshComponent* ClickedArrow, bool KeepMoving, bool KeepExtrMov, float LMBMovDir, float RMBMovDir)
{
	// Set arrow movement
	float Movement = 0.0;	
	if (KeepMoving) { Movement = LMBMovDir; }
	else if (KeepExtrMov) { Movement = RMBMovDir; }

	// Move Arrow
	MoveFace(Movement, FindFaceVertexesFromArrowLocation(ClickedArrow->RelativeLocation),ClickedArrow);
}

void AProceduralCubeActor::PlayerOnHover()
{
	FHitResult HitRes;
	CustomPController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);
	UStaticMeshComponent* SelectedComponent = Cast<UStaticMeshComponent>(HitRes.GetComponent());
	AProceduralCubeActor* SelectedCube = Cast<AProceduralCubeActor>(HitRes.GetActor());

	if (SelectedCube != NULL && !VertexMovementState)
	{
		KeepOnHover = true;
		HideVertexSpheres();

		if (SelectedComponent != NULL && SelectedComponent->ComponentHasTag(TEXT("VertexSphere")))
		{
			// Selected sphere at v0
			if (SelectedComponent->GetName() == V0Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V0Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v1
			else if (SelectedComponent->GetName() == V1Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V1Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v2
			else if (SelectedComponent->GetName() == V2Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V2Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v3
			else if (SelectedComponent->GetName() == V3Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V3Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v4
			else if (SelectedComponent->GetName() == V4Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V4Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v5
			else if (SelectedComponent->GetName() == V5Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V5Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v6
			else if (SelectedComponent->GetName() == V6Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V6Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}

			// Selected sphere at v7
			else if (SelectedComponent->GetName() == V7Sphere->GetName())
			{
				HideFacesArrows();
				KeepOnHover = true;
				V7Sphere->SetHiddenInGame(false);
				CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::KeepingOnHover);
				KeepOnHover = false;
			}
		}
		else if (SelectedCube != NULL && SelectedCube == this)
		{
			HideAllComponents();
			KeepOnHover = true;
			CustomPController->InputComponent->BindAction("LeftMB", IE_Pressed, this, &AProceduralCubeActor::InitFacesMovements);
			KeepOnHover = false;
		}
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AProceduralCubeActor::PlayerOnHover);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 0.05f, false);
	}
}

void AProceduralCubeActor::PlayerOnExitHover()
{
	// Check that system is not waiting to move a vertex
	KeepOnHover = false;
	if (!VertexMovementState)
	{ 
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("Exit hover!"));
		HideAllComponents();
	}
}

void AProceduralCubeActor::KeepingOnHover()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("keeping on hover"));
	HideFacesArrows();
	FHitResult HitRes;
	CustomPController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitRes);
	UStaticMeshComponent* SelectedSphere = Cast<UStaticMeshComponent>(HitRes.GetComponent());

	if (SelectedSphere == NULL) { return; }

	if (SelectedSphere->GetName() == V0Sphere->GetName() && !VertexMovementState)
	{
		V0Sphere->SetHiddenInGame(true);
		V0Sphere_Arrow0->SetHiddenInGame(false); V0Sphere_Arrow1->SetHiddenInGame(false); V0Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V1Sphere->GetName() && !VertexMovementState)
	{
		V1Sphere->SetHiddenInGame(true);
		V1Sphere_Arrow0->SetHiddenInGame(false); V1Sphere_Arrow1->SetHiddenInGame(false); V1Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V2Sphere->GetName() && !VertexMovementState)
	{
		V2Sphere->SetHiddenInGame(true);
		V2Sphere_Arrow0->SetHiddenInGame(false); V2Sphere_Arrow1->SetHiddenInGame(false); V2Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V3Sphere->GetName() && !VertexMovementState)
	{
		V3Sphere->SetHiddenInGame(true);
		V3Sphere_Arrow0->SetHiddenInGame(false); V3Sphere_Arrow1->SetHiddenInGame(false); V3Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V4Sphere->GetName() && !VertexMovementState)
	{
		V4Sphere->SetHiddenInGame(true);
		V4Sphere_Arrow0->SetHiddenInGame(false); V4Sphere_Arrow1->SetHiddenInGame(false); V4Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V5Sphere->GetName() && !VertexMovementState)
	{
		V5Sphere->SetHiddenInGame(true);
		V5Sphere_Arrow0->SetHiddenInGame(false); V5Sphere_Arrow1->SetHiddenInGame(false); V5Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V6Sphere->GetName() && !VertexMovementState)
	{
		V6Sphere->SetHiddenInGame(true);
		V6Sphere_Arrow0->SetHiddenInGame(false); V6Sphere_Arrow1->SetHiddenInGame(false); V6Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
	else if (SelectedSphere->GetName() == V7Sphere->GetName() && !VertexMovementState)
	{
		V7Sphere->SetHiddenInGame(true);
		V7Sphere_Arrow0->SetHiddenInGame(false); V7Sphere_Arrow1->SetHiddenInGame(false); V7Sphere_Arrow2->SetHiddenInGame(false);
		InitVertexMovementState(SelectedSphere);
	}
}

void AProceduralCubeActor::InitFacesMovements()
{
	VertexMovementState = true;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("Moving faces"));

	FrontFaceArrow->SetHiddenInGame(false);
	BackFaceArrow->SetHiddenInGame(false);
	LeftFaceArrow->SetHiddenInGame(false);
	RightFaceArrow->SetHiddenInGame(false);
	TopFaceArrow->SetHiddenInGame(false);
	BottomFaceArrow->SetHiddenInGame(false);

	QuitVertexMovementState();
}

// ------------------------------------------------ End - CUSTOM EVENTS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - HIDE FUNCTIONS ------------------------------------------------- \\

void AProceduralCubeActor::HideVertexSpheres()
{
	V0Sphere->SetHiddenInGame(true);	V1Sphere->SetHiddenInGame(true);	V2Sphere->SetHiddenInGame(true);	V3Sphere->SetHiddenInGame(true);
	V4Sphere->SetHiddenInGame(true);	V5Sphere->SetHiddenInGame(true);	V6Sphere->SetHiddenInGame(true);	V7Sphere->SetHiddenInGame(true);
}

void AProceduralCubeActor::HideFacesArrows()
{
	FrontFaceArrow->SetHiddenInGame(true);	BackFaceArrow->SetHiddenInGame(true);	LeftFaceArrow->SetHiddenInGame(true);
	RightFaceArrow->SetHiddenInGame(true);	TopFaceArrow->SetHiddenInGame(true);	BottomFaceArrow->SetHiddenInGame(true);
}

void AProceduralCubeActor::HideSpheresArrows()
{
	V0Sphere_Arrow0->SetHiddenInGame(true);	V0Sphere_Arrow1->SetHiddenInGame(true);	V0Sphere_Arrow2->SetHiddenInGame(true);
	V1Sphere_Arrow0->SetHiddenInGame(true);	V1Sphere_Arrow1->SetHiddenInGame(true);	V1Sphere_Arrow2->SetHiddenInGame(true);
	V2Sphere_Arrow0->SetHiddenInGame(true);	V2Sphere_Arrow1->SetHiddenInGame(true);	V2Sphere_Arrow2->SetHiddenInGame(true);
	V3Sphere_Arrow0->SetHiddenInGame(true);	V3Sphere_Arrow1->SetHiddenInGame(true);	V3Sphere_Arrow2->SetHiddenInGame(true);
	V4Sphere_Arrow0->SetHiddenInGame(true);	V4Sphere_Arrow1->SetHiddenInGame(true);	V4Sphere_Arrow2->SetHiddenInGame(true);
	V5Sphere_Arrow0->SetHiddenInGame(true);	V5Sphere_Arrow1->SetHiddenInGame(true);	V5Sphere_Arrow2->SetHiddenInGame(true);
	V6Sphere_Arrow0->SetHiddenInGame(true);	V6Sphere_Arrow1->SetHiddenInGame(true);	V6Sphere_Arrow2->SetHiddenInGame(true);
	V7Sphere_Arrow0->SetHiddenInGame(true);	V7Sphere_Arrow1->SetHiddenInGame(true);	V7Sphere_Arrow2->SetHiddenInGame(true);
}

void AProceduralCubeActor::HideAllComponents()
{
	HideVertexSpheres();
	HideFacesArrows();
	HideSpheresArrows();
}

void AProceduralCubeActor::InitVertexMovementState(UStaticMeshComponent* SelectedSphere)
{
	VertexMovementState = true;
	QuitVertexMovementState();
}

void AProceduralCubeActor::QuitVertexMovementState()
{
	CustomPController->InputComponent->BindAction("ExitQ", IE_Pressed, this, &AProceduralCubeActor::UnsetVertexMovementState);
	if (!VertexMovementState)
	{
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AProceduralCubeActor::QuitVertexMovementState);
		GetWorldTimerManager().SetTimer(Handle, Delegate, 0.05f, false);
	}
}

void AProceduralCubeActor::UnsetVertexMovementState()
{
	VertexMovementState = false;
	PlayerOnExitHover();
	HideAllComponents();
}

// ------------------------------------------------ End - HIDE FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\ ------------------------------------------------ End - HIDE FUNCTIONS ------------------------------------------------ \\





// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\
// ------------------------------------------------- Begin - OVERRIDE FUNCTIONS ------------------------------------------------- \\

void AProceduralCubeActor::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

}

// ------------------------------------------------ End - OVERRIDE FUNCTIONS ------------------------------------------------ \\
// ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- o ------------------- \\