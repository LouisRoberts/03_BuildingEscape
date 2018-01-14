// Copyrighjt RobertsConsulting

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray cast and grab whats in reach
	void Grab();

	void Release();

	// Find attached physics handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// REturn hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Get start of reach line
	FVector GetReachLineStart();

	// Get end of raech line
	FVector GetReachLineEnd();
};
