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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Structure)
		class UBoxComponent*  BoxComponent;

	UPROPERTY(BlueprintReadWrite, Category = UI)
		bool bDisplayMessage;
	UPROPERTY(BlueprintReadWrite, Category = Action)
		bool bPicked;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
