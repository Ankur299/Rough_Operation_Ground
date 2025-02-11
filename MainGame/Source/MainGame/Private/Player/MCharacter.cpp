// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Private/Weapons/Mweapon.h"
#include "TimerManager.h"
#include "Private/Component/ActorHealthComponent.h" 

#include "DrawDebugHelpers.h" //TODO #removeku,l nijv this after  debugging

// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; //For mouse movement

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	HealthComponent = CreateDefaultSubobject<UActorHealthComponent>(TEXT("Health Component"));
	OnTakeAnyDamage.AddDynamic(HealthComponent, &UActorHealthComponent::HandleDamage);

	SpeedMultiplier = 0.67f;
	
	bOverlapedWithWeapon = false;
	
	//Animation
	Reloading = false;


}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMCharacter::OnOverlapEnd);
}

void AMCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector()*val*SpeedMultiplier);
}

void AMCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector()*val*SpeedMultiplier);
}

void AMCharacter::Crouching()
{
	Crouch();
}

void AMCharacter::UnCrouching()
{
	UnCrouch();
}

void AMCharacter::WeaponChange()
{
	//if nothing is in hand
	if (CurrentWeapon && !bOverlapedWithWeapon) {

		WeaponUnEquip(CurrentWeapon);

		if (CurrentWeaponType == 1) {
			PrimaryWeapon = NULL;
			CurrentWeapon = NULL;
			CurrentWeaponType = 0;
		}
		else {
			SecondaryWeapon = NULL;
			CurrentWeapon = NULL;
			CurrentWeaponType = 0;
		}


	}

	else if (bOverlapedWithWeapon) {
		if (CurrentWeapon) {
			ChangeToSlot(CurrentWeapon);
		}
		OverlapingWeapon = Cast<AMweapon>(OverlapingWeapon);
		WeaponEquip(OverlapingWeapon);

		if (OverlapingWeapon->Type == 1) {
			if (PrimaryWeapon) {
				WeaponUnEquip(PrimaryWeapon);
			}
			PrimaryWeapon = OverlapingWeapon;
		}
		else {
			if (SecondaryWeapon) {
				WeaponUnEquip(SecondaryWeapon);
			}
			SecondaryWeapon = OverlapingWeapon;
		}


	}

	bOverlapedWithWeapon = false;
}

void AMCharacter::WeaponEquip(AMweapon * Weapon)
{
	Weapon->SetOwner(this);
	Weapon->WeaponMesh->SetSimulatePhysics(false);
	Weapon->bPicked = true;
	Weapon->bOverlapAllowed = false;
	Weapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
	CurrentWeapon = Weapon;
	CurrentWeaponType = Weapon->Type;
}

void AMCharacter::WeaponUnEquip(AMweapon* Weapon)
{
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->WeaponMesh->SetSimulatePhysics(true);
	Weapon->bPicked = false;
	Weapon->bOverlapAllowed = true;
	Weapon->SetOwner(NULL);
}

void AMCharacter::ChangeToSlot(AMweapon * Weapon)
{
	int32 ThisWeaponType = Weapon->Type;
	if (ThisWeaponType == 1) {
		Weapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Rifle_Slot");
		PrimaryWeapon = Weapon;
	}
	else if (ThisWeaponType == 2) {
		Weapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Gun_Slot");
		SecondaryWeapon = Weapon;
	}
	else {
		//For Granades
	}
}

//For Changing to primary weapon
void AMCharacter::PrimaryWeaponSwap()
{
	if (CurrentWeapon && PrimaryWeapon) {
		if (CurrentWeapon != PrimaryWeapon) {
			ChangeToSlot(CurrentWeapon);
			WeaponEquip(PrimaryWeapon);
		}
		else {
			
			ChangeToSlot(CurrentWeapon);
			CurrentWeapon = NULL;
			CurrentWeaponType = 0;
		}
	}
	else if (!CurrentWeapon && PrimaryWeapon) {
		WeaponEquip(PrimaryWeapon);
	}
}

//For changing to secondary weapon
void AMCharacter::SecondaryWeaponSwap()
{
	if (CurrentWeapon && SecondaryWeapon) {
		if (CurrentWeapon != SecondaryWeapon) {
			ChangeToSlot(CurrentWeapon);
			WeaponEquip(SecondaryWeapon);
		}
		else {
			ChangeToSlot(CurrentWeapon);
			CurrentWeapon = NULL;
			CurrentWeaponType = 0;
		}
	}
	else if (!CurrentWeapon && SecondaryWeapon) {
		WeaponEquip(SecondaryWeapon);
	}
}

void AMCharacter::WeaponFireStart()
{
	if (CurrentWeapon) {
		if (!Reloading) {
			CurrentWeapon->StartFire();
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Weapon not equipped"));
	}
}

void AMCharacter::WeaponFireEnd()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
}

void AMCharacter::WeaponReloadEnd()
{
	CurrentWeapon->Reload();
	Reloading = false;
}

void AMCharacter::WeaponReloadStart()
{
	
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
		if (CurrentWeapon->bAvailableAmmo()) {
			Reloading = true;
			GetWorldTimerManager().SetTimer(ReloadTime, this, &AMCharacter::WeaponReloadEnd, 1.f, false, 3.f); //Delay yet to be specified
		}

		else {
			UE_LOG(LogTemp, Warning, TEXT("Returning False"));
		}
	}
}

void AMCharacter::Death()
{
	bDied = true;

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();

	if (PrimaryWeapon) {
		WeaponUnEquip(PrimaryWeapon);
	}
	if (SecondaryWeapon) {
		WeaponUnEquip(SecondaryWeapon);
	}

	UE_LOG(LogTemp, Warning, TEXT("Died"));
}

void AMCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	OverlapingWeapon = Cast<AMweapon>(OtherActor);
	if (OverlapingWeapon) {
		if (!OverlapingWeapon->bPicked && OverlapingWeapon->bOverlapAllowed) {
			//Shows only when weapon is not picked by anyone
			DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), "Press F to PickUp", OtherActor, FColor::White, 2.0f, false, 1.5f);
			bOverlapedWithWeapon = true;
		}
	}
	else {
		//Not A Weapon
	}
}


void AMCharacter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bOverlapedWithWeapon = false;
	OverlapingWeapon = NULL;
}



// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMCharacter::AddControllerPitchInput);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::Falling);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMCharacter::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMCharacter::UnCrouching);

	/* Weapon Properties*/
	//Weapon Equipping
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AMCharacter::WeaponChange);
	PlayerInputComponent->BindAction("PrimaryWeapon", IE_Pressed, this, &AMCharacter::PrimaryWeaponSwap);
	PlayerInputComponent->BindAction("SecondaryWeapon", IE_Pressed, this, &AMCharacter::SecondaryWeaponSwap);

	//WeaponAction
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMCharacter::WeaponFireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMCharacter::WeaponFireEnd);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMCharacter::WeaponReloadStart);

}

