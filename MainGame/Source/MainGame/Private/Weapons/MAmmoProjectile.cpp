// Fill out your copyright notice in the Description page of Project Settings.


#include "MAmmoProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AMAmmoProjectile::AMAmmoProjectile()
{
	Bulletmesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	Bulletmesh->SetupAttachment(RootComponent);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->InitialSpeed = 10000.f;
	Projectile->MaxSpeed = 10000.f;



}

void AMAmmoProjectile::SelfDestroy()
{
	UE_LOG(LogTemp, Error, TEXT("Destroyed"));
	GetWorldTimerManager().SetTimer(DestroyBullet, this, &AMAmmoProjectile::CallDestroy, 1.f, false, 3.f);

}

void AMAmmoProjectile::CallDestroy()
{
	this->Destroy();
	GetWorldTimerManager().ClearTimer(DestroyBullet);
}

void AMAmmoProjectile::HandleHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit1 %s"));
	if (OtherActor != NULL) {
		Bulletmesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Test1 =  Hit.BoneName;
	}
}

// Called when the game starts or when spawned
void AMAmmoProjectile::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AMAmmoProjectile::HandleHit);
}

