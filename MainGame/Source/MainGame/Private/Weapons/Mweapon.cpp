// Fill out your copyright notice in the Description page of Project Settings.


#include "Mweapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Private/Weapons/MAmmoProjectile.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h" //TODO #remove this after  debugging


// Sets default values
AMweapon::AMweapon()
{


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap_Box"));

	bPicked = false;
	bOverlapAllowed = true;
	Type = 1; // # 1 for primary # 2 for secondary and # anything else for Explopsive/etc
	

	LastFireTime = 0;
	FireRate = 1;
	bAutomaticFire = false;
	AmmoType = 5.5;
	AvailableAmmo = 150;
	AmmoOnWeapon = 30;

}

// Called when the game starts or when spawned
void AMweapon::BeginPlay()
{ 
	Super::BeginPlay();
	WeaponMesh->SetSimulatePhysics(true);

}

void AMweapon::Tick(float DeltaTime)
{
	/*if (Bullet) {
		UE_LOG(LogTemp, Error, TEXT("initial speed %f"), Bullet->Projectile->InitialSpeed);
	}*/

	//TODO tick to be disabled
	
}

void AMweapon::PlayEffects() {
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, "Muzzle");
	}
	if (FireSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, FVector::ZeroVector);
	}
}

void AMweapon::FireProjectile()
{
	AActor* MyOwner;
	MyOwner = GetOwner();
	if (BulletClass && MyOwner) {

		FVector EyeLocation;
		FRotator EyeRotation;
		FVector MuzzleLocation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");


		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		UE_LOG(LogTemp, Error, TEXT("Spawn Here Bullet"));

		//TODO spawn bullet here
		Bullet = GetWorld()->SpawnActor<AMAmmoProjectile>(BulletClass, MuzzleLocation, EyeRotation, SpawnParams);
		
		Bullet->SelfDestroy();
	}
}

void AMweapon::Fire()
{
	if (AmmoOnWeapon > 0) {

		PlayEffects();
		FireProjectile();

		UE_LOG(LogTemp, Warning, TEXT("Firing"));
		UE_LOG(LogTemp, Warning, TEXT("%d /  %d"), AmmoOnWeapon, AvailableAmmo);
		AmmoOnWeapon--;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Reload"));
	}

}

void AMweapon::StartFire()
{
	if (bAutomaticFire) {

		float FirstDelay = FMath::Max(LastFireTime + (60 / FireRate) - GetWorld()->TimeSeconds, 0.f);
		GetWorldTimerManager().SetTimer(TimeBetweenShots, this, &AMweapon::Fire, (60 / FireRate), true, FirstDelay);		
	}
	else {
		Fire();
	}

	LastFireTime = GetWorld()->TimeSeconds;
}

void AMweapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeBetweenShots);
}

bool AMweapon::bAvailableAmmo()
{
	if (AvailableAmmo <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Available Ammo"));
		return false;
	}
	else {
		return true;
	}
}



void AMweapon::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloaded"));
	if (AvailableAmmo >= 30) {
		AmmoOnWeapon = 30;
		AvailableAmmo = AvailableAmmo - 30;
	}
	else {
		AmmoOnWeapon = AvailableAmmo;
		AvailableAmmo = 0;
	}
}

