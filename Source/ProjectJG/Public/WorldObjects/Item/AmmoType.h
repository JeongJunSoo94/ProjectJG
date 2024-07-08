#pragma once
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_SMG UMETA(DisplayName = "SubMachinGun"),
	EAT_AR UMETA(DisplayName = "AssaultRifle"),

	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};