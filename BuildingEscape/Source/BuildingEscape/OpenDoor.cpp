// Copyrighjt RobertsConsulting

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

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
	if (PressurePlate == nullptr) {
		FString OwnerName = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Missing PressurePlate on %s"), *OwnerName);
		return;
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Poll trigger Volume
	if(GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		// If the actor that opens is in the volume
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequest.Broadcast();
	}
}


// Returns total mass in kilograms
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// iterate overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate){ 
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors)
	{
		FString OwnerName = *Actor->GetName();
		float ObjectWeight = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s weighs %f"), *OwnerName,ObjectWeight);
	}
	
	return TotalMass;
}

