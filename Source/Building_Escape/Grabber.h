// Copyright Maddie Abbaspour 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	// Sets default values for this component's properties

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Return the first actor within reach with aphysics body
	FHitResult GetFirstPhysicsBodyInReach() const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	float Reach =200.f;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	//Return the Line trace end
	FVector GetPlayerReach() const;

	//Get players Position in the world
	FVector GetPlaversWorldPos() const;
};
