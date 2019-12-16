// Fill out your copyright notice in the Description page of Project Settings.


#include "MAmmoProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMAmmoProjectile::AMAmmoProjectile()
{
	Bulletmesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	Bulletmesh->SetupAttachment(RootComponent);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->InitialSpeed = 1000.f;
	Projectile->MaxSpeed = 1000.f;
}

// Called when the game starts or when spawned
void AMAmmoProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
