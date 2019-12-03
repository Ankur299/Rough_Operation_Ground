// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Private/Weapons/Mweapon.h"
#include "DrawDebugHelpers.h" //TODO #remove this after  debugging

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


	SpeedMultiplier = 0.67f;
	
	

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

void AMCharacter::WeaponEquip()
{
	if (bOverlapedWithWeapon && !CurrentWeapon) {
		CurrentWeapon = Cast<AMweapon>(Weapon);
		if (CurrentWeapon) {
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		}

	}
	else if(CurrentWeapon)
	{
		CurrentWeapon = Cast<AMweapon>(Weapon);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		CurrentWeapon->SetOwner(NULL);
		CurrentWeapon = NULL;
		UE_LOG(LogTemp, Warning, TEXT("I Want to Drap The Weapon"));
	}
	else {
		return;
	}
}



void AMCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	Weapon = Cast<AMweapon>(OtherActor);
	if (Weapon) {
		DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), "Press F to PickUp" , OtherActor, FColor::White, 2.0f, false, 1.5f);
		bOverlapedWithWeapon = true;
	}
	else {
		//Not A Weapon
	}
}


void AMCharacter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bOverlapedWithWeapon = false;
	Weapon = NULL;
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
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AMCharacter::WeaponEquip);

}

