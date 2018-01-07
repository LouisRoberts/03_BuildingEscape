// Copyrighjt RobertsConsulting

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = GetOwner();
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	// Set the Door Rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	// Set the Door Rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float CurrentDoorTime = GetWorld()->GetTimeSeconds();
	// Poll trigger Volume
	if(PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// If the actor that opens is in the volume
		OpenDoor();
		LastDoorOpenTime = CurrentDoorTime;
	}
	// check if time to close door
	
	if(CurrentDoorTime>(LastDoorOpenTime+ DoorCloseDelay))
	{
		CloseDoor();
	}
}

