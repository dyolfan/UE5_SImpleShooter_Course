#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	Mesh->SetupAttachment(Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	if (CurrentAmmo > 0)
	{
		FireShot();
	}
	UE_LOG(LogTemp, Display, TEXT("Ammo %d"), CurrentAmmo);
}

void AGun::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void AGun::Unload()
{
	CurrentAmmo = 0;
}

int AGun::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int AGun::GetMaxAmmo()
{
	return MaxAmmo;
}

bool AGun::GunTrace(FHitResult &HitResult, FVector &ShotDirection)
{

	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	ShotDirection = -ViewPointRotation.Vector();
	FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(HitResult,
												ViewPointLocation,
												End,
												ECC_GameTraceChannel1,
												Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}

void AGun::FireShot()
{
	CurrentAmmo = CurrentAmmo - 1;

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;

	if (GunTrace(HitResult, ShotDirection))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);

		AActor *HitActor = HitResult.GetActor();
		if (HitActor)
		{
			AController *OwnerController = GetOwnerController();
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController->GetInstigatorController(), this);
		}
	}
}