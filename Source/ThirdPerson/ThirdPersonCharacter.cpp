
#include "ThirdPersonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


/*------------------AThirdPersonCharacter Initialization-----------------*/

AThirdPersonCharacter::AThirdPersonCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 
}

/*------------------------------------------------*/

/*---------------------Setup---------------------------*/

void AThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCharacter::Fire);
	PlayerInputComponent->BindAction("WallRuning", IE_Pressed, this, &AThirdPersonCharacter::WallRuninng);
	PlayerInputComponent->BindAction("Sliding", IE_Pressed, this, &AThirdPersonCharacter::Sliding);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCharacter::LookUpAtRate);

}

/*------------------------------------------------*/


/*--------------------Gameplay(Super::)---------------------------*/

void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CapsulaDebug();

	IKArms("upperarm_l", "upperarm_r");
	IKFoot("foot_l", "foot_r");	
}

void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayVibration();

	DebugPrint("Game Start");
}


/*---------------------InputAction--------------------------*/

void AThirdPersonCharacter::Fire()
{
	LineTraceDebug();
	PlayVibration(FireForceFeedBack);
}

void AThirdPersonCharacter::WallRuninng()
{
	SphereTraceDebug(45.f);
	SphereTraceDebug(-45.f);
}

void AThirdPersonCharacter::Sliding()
{
	UAnimInstance* AnimationInstance{ GetMesh()->GetAnimInstance() };

	AnimationMontageSliding && AnimationInstance ? 
		DebugPrint("Sliding"), PlayAnimMontage(AnimationMontageSliding) : DebugError("Animation Montage = nullptr");

	PlayVibration(SlidingForceFeedBack);
}

/*----------------------------------------------------------*/


/*------------------------Debug---------------------------------*/

void AThirdPersonCharacter::DebugPrint(const FString& Text)
{
	UKismetSystemLibrary::PrintString(
		GetWorld(),
		debugText + Text,
		true,
		true,
		FColor::Green,
		TimeDelta	
	);
}

void AThirdPersonCharacter::DebugError(const FString& Text)
{
	UKismetSystemLibrary::PrintString(
		GetWorld(),
		debugText + Text,
		true,
		true,
		FColor::Red,
		TimeDelta	
	);
}

void AThirdPersonCharacter::CapsulaDebug()
{
	FHitResult CapsulaHit;
	
	float Distance{ 0.0f };
	float HalfHeight{ GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };
	float Radius{ GetCapsuleComponent()->GetScaledCapsuleRadius() };

	FVector Direction{ GetActorForwardVector() };
	FVector Start{ GetActorLocation() };
	FVector End{ (Start + (Direction * Distance)) };
	TArray <AActor*> IgnoreActor{ this };

	
	bool isCapsula{ UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(),
		Start,
		End,
		Radius,
		HalfHeight,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoreActor,
		EDrawDebugTrace::ForOneFrame,
		CapsulaHit,
		true,
		FColor::Black,
		FColor::Green,
		TimeDelta	
	) };		
}

void AThirdPersonCharacter::LineTraceDebug()
{
	FHitResult LineHit;
	
	float Distance{ 1000.f };

	FVector Direction{ GetActorForwardVector() };
	FVector Start{ GetActorLocation() };
	FVector End{ (Start + (Direction * Distance)) };
	TArray <AActor*> IgnoreActor{ this };

	
	bool isTrace{ UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		LineHit,
		true,
		FColor::Black,
		FColor::Green,
		TimeDelta	
	) };		

	FVector EndLocationHit{ LineHit.ImpactPoint };
	
	isTrace && LineHit.bBlockingHit ? DebugPrint("Hit"), SphereHitDebug(EndLocationHit) : DebugError("Not Hit");
}

void AThirdPersonCharacter::SphereTraceDebug(float Angle)
{
	FHitResult SphereHit;
	
	float Distance{ 150.f };
	float Radius{ 10.f };
	
	FRotator Rotation(0.f, Angle, 0.f);
	FVector Direction{ Rotation.RotateVector(GetActorForwardVector())};
	FVector Start{ GetActorLocation() };
	FVector End{ (Start + (Direction * Distance)) };
	TArray <AActor*> IgnoreActor{ this };

	
	bool isTrace{ UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start,
		End,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		SphereHit,
		true,
		FColor::Black,
		FColor::Green,
		TimeDelta	
	) };		

	FVector EndLocationHit{ SphereHit.ImpactPoint };

	isTrace&& SphereHit.bBlockingHit ? DebugPrint("Hit Wall"), SphereHitDebug(EndLocationHit), PlayVibration(WallRunForceFeedBack) : DebugError("Not Hit Wall");
}

void AThirdPersonCharacter::SphereHitDebug(FVector& Location)
{
	float Radius{ 20.f };
	
	 DrawDebugSphere(
        GetWorld(),
		Location,
        Radius,               
        12,                 
        FColor::Green,        
        false,              
        2.0f,               
        0,                  
        2.0f                
    );
}


/*--------------------------------------------------------------*/


/*------------------------------InverseKinematics----------------*/

void AThirdPersonCharacter::IKFoot(const FName& BoneNameFootL, const FName& BoneNameFootR)
{
    TArray<FName> BoneNames = { BoneNameFootL, BoneNameFootR };
    
    float TraceDistance{ 200.0f };
    FVector Direction{ -GetActorUpVector() };
    TArray<AActor*> IgnoreActors{ this };
    
    for (const FName& BoneName : BoneNames)
    {
        FHitResult HitResult;
        FVector Start{ GetMesh()->GetBoneLocation(BoneName) };
        FVector End{ Start + (Direction * TraceDistance) };
        
        bool bHit = UKismetSystemLibrary::LineTraceSingle(
            GetWorld(),
            Start,
            End,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            IgnoreActors,
            EDrawDebugTrace::ForOneFrame,
            HitResult,
            true,
            FColor::Black,
            FColor::Green,
            TimeDelta
        );    
    }
}

void AThirdPersonCharacter::IKArms(const FName& BoneNameArmsL, const FName& BoneNameArmsR)
{
	TArray<FName> BoneNames = { BoneNameArmsL, BoneNameArmsR };
    
    float TraceDistance{ 60.0f };
	float Radius{ 5.f };
    FVector Direction{ GetActorForwardVector() };
    TArray<AActor*> IgnoreActors{ this };
    
    for (const FName& BoneName : BoneNames)
    {
        FHitResult HitResult;
        FVector Start{ GetMesh()->GetBoneLocation(BoneName) };
        FVector End{ Start + (Direction * TraceDistance) };
        
        bool bHit = UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            Start,
            End,
			Radius,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            IgnoreActors,
            EDrawDebugTrace::ForOneFrame,
            HitResult,
            true,
            FColor::Black,
            FColor::Green,
            TimeDelta
        );    
		FVector EndLocationPoint{ HitResult.ImpactPoint };

		if (bHit)
		{
			SphereHitDebug(EndLocationPoint);
			PlayVibration();
		}
    }
}

/*---------------------------------------------------------------*/


/*------------------------Environment-----------------------*/

void AThirdPersonCharacter::PlayVibration(UForceFeedbackEffect* ForceEffect)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (!PlayerController) return;
	
	FForceFeedbackParameters Params;
	Params.bLooping = false;
	Params.bIgnoreTimeDilation = false;
	
	ForceEffect ? PlayerController->ClientPlayForceFeedback(ForceEffect, Params), DebugPrint("Vibration") : DebugError("ForceFeedBack = nullptr");
}

/*------------------------------------------------*/

/*-----------------------BaseAction------------------------*/

void AThirdPersonCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

/*---------------------------------------------------------*/
