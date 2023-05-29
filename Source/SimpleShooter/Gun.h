#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	AGun();

	void PullTrigger();

	void Reload();

	void Unload();

	int GetCurrentAmmo();

	int GetMaxAmmo();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000;

	UPROPERTY(EditAnywhere)
	UParticleSystem *HitEffect;

	UPROPERTY(EditAnywhere)
	USoundBase *ImpactSound;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditAnywhere)
	int MaxAmmo = 15;

	UPROPERTY(EditAnywhere)
	int CurrentAmmo = 15;

	bool GunTrace(FHitResult &HitResult, FVector &ShotDirection);

	AController *GetOwnerController() const;

	void FireShot();
};
