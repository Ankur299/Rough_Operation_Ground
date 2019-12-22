// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorHealthComponent.h"
#include "GameFrameWork/Character.h"
#include "Private/Player/MCharacter.h"

// Sets default values for this component's properties
UActorHealthComponent::UActorHealthComponent()
{
	DefaultHealth = 100.f;
	CurrentHealth = DefaultHealth;
}


// Called when the game starts
void UActorHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActorHealthComponent::HandleDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage > 0 && CurrentHealth > 0) {
		CurrentHealth -= Damage;
		if (CurrentHealth <= 0) {
			AActor* MyOwner = GetOwner();
			AMCharacter* Character = Cast<AMCharacter>(MyOwner);
			if (Character) {
				Character->Death();
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Taking Damage: %f"), CurrentHealth);
}


