// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AActor;
class USkeletalMeshComponent;

UCLASS()
class UE_PORTFOLIO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

protected:
	virtual void BeginPlay() override;
	float WeaponDamage;
};