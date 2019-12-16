// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"



UCLASS()
class AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement
	void MoveForward(float val);
	void  MoveRight(float val);
	void Crouching();
	void UnCrouching();
	
	//Weapon Equiping
	void WeaponChange();
	void WeaponEquip(class AMweapon* Weapon);
	void WeaponUnEquip(class AMweapon* Weapon);
	void ChangeToSlot(class AMweapon * Weapon);


	//Weapon Changing
	void PrimaryWeaponSwap();  //choosing Primary Weapon Rifle, Shortgun, Snipers
	void SecondaryWeaponSwap(); //Choosing Secondary Weapons Revolver, SMG etc.

	//Weapon Action
	void WeaponFireStart();
	void WeaponFireEnd();
	void WeaponReloadStart();
	void WeaponReloadEnd();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Structure)
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Structure)
		class UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		float SpeedMultiplier;
	//TODO delete this
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		bool Test;

	bool bOverlapedWithWeapon; //Checks if Overlaping happed with an weapon or not
	
	class AMweapon* OverlapingWeapon;
	UPROPERTY(BlueprintReadOnly)
	class AMweapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	class AMweapon* PrimaryWeapon;
	UPROPERTY(BlueprintReadOnly)
	class AMweapon* SecondaryWeapon;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentWeaponType;


	//variables for animations
	UPROPERTY(BlueprintReadOnly, Category = Animation)
		bool Reloading;
	FTimerHandle ReloadTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

};
