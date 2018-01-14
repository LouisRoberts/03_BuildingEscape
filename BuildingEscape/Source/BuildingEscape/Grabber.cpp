// Copyrighjt RobertsConsulting

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

// Annotation to indicate that parameters are OUTPUTs
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
	// Line trace and  see if we need to reach out and grab actors
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		/// if found TODO attached physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release Pressed"));
	// TODO Release
	PhysicsHandle->ReleaseComponent();
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Look for attched physics handler
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing UPhysicsHandleComponent on  %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/// Get Player viewpoint this tick
	FVector PlayerViewpPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpPointLocation,
		OUT PlayerViewPointRotation);

	/// Ray-cast out to reach distance
	FVector LineTraceEnd = PlayerViewpPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Create attached input handler
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing UPhysicsHandleComponent on  %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get Player viewpoint this tick
	FVector PlayerViewpPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpPointLocation,
		OUT PlayerViewPointRotation);

	/// Ray-cast out to reach distance
	FVector LineTraceEnd = PlayerViewpPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	DrawDebugLine(
		GetWorld(),
		PlayerViewpPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	/// Set Up Query Parameters
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	// see what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *(ActorHit->GetName()));
	}
	return Hit;
}




