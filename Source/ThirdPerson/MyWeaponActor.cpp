
#include "MyWeaponActor.h"

AMyWeaponActor::AMyWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	SetRootComponent(SkeletalMesh);
}

void AMyWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

