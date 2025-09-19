// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS(config=Game)
class AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AThirdPersonCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/*----------------------ForceFeedBack--------------*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceFeedbackEffect")
	class UForceFeedbackEffect* FireForceFeedBack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceFeedbackEffect")
	class UForceFeedbackEffect* SlidingForceFeedBack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceFeedbackEffect")
	class UForceFeedbackEffect* WallRunForceFeedBack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceFeedbackEffect")
	class UForceFeedbackEffect* JumpForceFeedBack = nullptr;

	/*------------------------------------------------*/


	/*-----------AnimationMontage-----------*/	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* AnimationMontageSliding = nullptr;

	/*------------------------------------------------*/

protected:

	/*-----------------Gameplay(Super::)-----------------*/

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/*------------------------------------------------*/


	/*-----------Axis Action--------*/

	void Fire();
	
	void WallRuninng();

	void Sliding();

	/*------------------------------------------------*/

	/*-----------Input Action--------*/

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	/*------------------------------------------------*/

	/*-----------Debug--------------*/

	void DebugPrint(const FString& Text);

	void DebugError(const FString& Text);

	void CapsulaDebug();

	void LineTraceDebug();

	void SphereTraceDebug(float Angle);

	void SphereHitDebug(FVector& Location);

	/*------------------------------------------------*/


	/*-----------Additionally--------*/

	void PlayVibration(UForceFeedbackEffect* ForceEffect = nullptr);

	/*------------------------------------------------*/


	/*-----------AdditionallyInverseKinematics--------*/	

	void IKFoot(const FName& BoneNameFootL, const FName& BoneNameFootR);

	void IKArms(const FName& BoneNameAmrsL, const FName& BoneNameArmsR);

	/*------------------------------------------------*/

public:
	/*-----------Float-----------*/	

	static constexpr float TimeDelta{ 2.0f };

	/*------------------------------------------------*/


	/*-----------FString-----------*/	

	const FString debugText{ "[DEBUG]: " };

	/*------------------------------------------------*/

	/*------------------uint32------------------------*/


	/*------------------------------------------------*/

	/*--------------------FLinearColor----------------*/

	/*------------------------------------------------*/

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

