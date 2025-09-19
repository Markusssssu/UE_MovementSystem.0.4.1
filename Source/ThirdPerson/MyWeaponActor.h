// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeaponActor.generated.h"

UCLASS()
class THIRDPERSON_API AMyWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyWeaponActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal");
	class USkeletalMeshComponent* SkeletalMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
