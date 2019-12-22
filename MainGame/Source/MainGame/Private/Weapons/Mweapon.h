// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mweapon.generated.h"

UCLASS()
class AMweapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMweapon();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Structure)
		class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Structure)
		class UBoxComponent*  BoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();
	void StartFire();
	void StopFire();
	bool bAvailableAmmo();
	void Reload();

	//Effects
	void PlayEffects();

	//Projectile Movement
	void FireProjectile();



	UPROPERTY(EditDefaultsOnly, Category = Action)
		bool bPicked;
	UPROPERTY(EditAnywhere, Category = Type)
		int32 Type;

	bool bOverlapAllowed;
	float AmmoType; //Defines the  length of the ammunation required
	int32 AvailableAmmo;
	int32 AmmoOnWeapon;


	float LastFireTime;
	UPROPERTY(EditAnywhere, Category = Fire)
		bool bAutomaticFire;
	UPROPERTY(EditAnywhere, Category = Fire)
		float FireRate;

	FTimerHandle TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
		class UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = Fire)
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
		class AMAmmoProjectile* Bullet;
	

	UPROPERTY(EditDefaultsOnly, Category = Fire)
		TSubclassOf<AMAmmoProjectile> BulletClass;

};
