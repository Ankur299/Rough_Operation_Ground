// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAmmoProjectile.generated.h"

UCLASS()
class AMAmmoProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMAmmoProjectile();
	void SelfDestroy();
	void CallDestroy();

	UFUNCTION(BlueprintCallable)
	void HandleHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class UStaticMeshComponent* Bulletmesh;

	FTimerHandle DestroyBullet;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

};
