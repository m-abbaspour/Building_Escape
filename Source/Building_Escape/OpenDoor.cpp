// Copyright Maddie Abbaspour 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/TransformNonVectorized.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw ;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp ,Error,TEXT("%s has the open door component on it, but no PressuerPlte set"), *GetOwner() -> GetName());
	}
	ActorThatOpen =GetWorld() -> GetFirstPlayerController() -> GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate -> IsOverlappingActor(ActorThatOpen))
	{
		OpenDoor(DeltaTime);
	}

}

void UOpenDoor::OpenDoor(float DeltaTime){

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw ,DeltaTime * .5f);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);

}

