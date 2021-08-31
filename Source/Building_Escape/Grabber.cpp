// Copyright Maddie Abbaspour 2021

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	// Check for the physics handle component

	SetupInputComponent();

}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle)
	{
		//UE_LOG(LogTemp, Error, TEXT("Component was found on: %s"), *);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component was found on: %s"), *GetOwner()->GetName());
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Press"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Releaser Press"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
	OUT PlayerViewPointLocation,
	OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255,0),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    GetWorld() -> LineTraceSingleByObjectType(
            OUT Hit,
            PlayerViewPointLocation,
            LineTraceEnd,
            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParams
            );

	// Loging out to test
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit -> GetName()));
	}
	
	// *PlayerViewPointRotation.ToString(),
	// *PlayerViewPointLocation.ToString());
	//CurrentView = APlayerController::GetPlayerViewPoint 
	//Ray-Cast out to a certain distance(reach)
	
	//See what it hits

	// ...
}
