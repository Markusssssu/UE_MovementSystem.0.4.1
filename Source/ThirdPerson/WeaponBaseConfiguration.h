// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponBaseConfiguration.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API UWeaponBaseConfiguration : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NameWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DescriptionWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class USkeletalMesh* WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class UTexture2D* WeaponIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float DamageWeapon;
};
