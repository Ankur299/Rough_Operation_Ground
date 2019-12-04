// Fill out your copyright notice in the Description page of Project Settings.


#include "Mweapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "DrawDebugHelpers.h" //TODO #remove this after  debugging


// Sets default values
AMweapon::AMweapon()
{


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap_Box"));

	bPicked = false;
	Type = 1; // # 1 for primary # 2 for secondary and # anything else for Explopsive/etc
}

// Called when the game starts or when spawned
void AMweapon::BeginPlay()
{ 
	Super::BeginPlay();

	WeaponMesh->SetSimulatePhysics(true);
}

void AMweapon::Tick(float DeltaTime)
{

	if (GetOwner()) {
		//DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), GetOwner()->GetName(), this, FColor::White, DeltaTime, 1.5f);
		

	}
	else {
		
		//DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), "No Owner", this, FColor::White, DeltaTime, 1.5f);
	}

}

