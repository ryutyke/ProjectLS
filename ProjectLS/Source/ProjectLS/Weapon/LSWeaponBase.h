#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSWeaponBase.generated.h"

UCLASS()
class PROJECTLS_API ALSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	ALSWeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	virtual bool Attack();
	UFUNCTION()
	virtual bool Reload();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Meta = (AllowPrivateAccess = "true"))
	int Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Meta = (AllowPrivateAccess = "true"))
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Meta = (AllowPrivateAccess = "true"))
	int MagazineSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Meta = (AllowPrivateAccess = "true"))
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Option, Meta = (AllowPrivateAccess = "true"))
	uint8 bAutoAttack : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Option, Meta = (AllowPrivateAccess = "true"))
	uint8 bNeedAmmoToShoot : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Performance, Meta = (AllowPrivateAccess = "true"))
	float FireRate;

	bool CanAttack() const;
	FORCEINLINE bool HasAmmo() const { return Ammo > 0; }
	FORCEINLINE bool IsAmmoFull() const { return Ammo == MaxAmmo; }

};

/*
Todo
- Parts system

*/
