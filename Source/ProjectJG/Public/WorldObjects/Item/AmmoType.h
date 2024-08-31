#pragma once
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_SMG UMETA(DisplayName = "SubMachineGun"),
	EAT_AR UMETA(DisplayName = "AssultRifle"),
	EAT_Pistol UMETA(DisplayName = "Pistol"),
	EAT_SR UMETA(DisplayName = "SniperRifle"),
	EAT_SG UMETA(DisplayName = "ShotGun"),
	EAT_RL UMETA(DisplayName = "RocketLauncher"),
	EAT_GL UMETA(DisplayName = "GrenadeLauncher"),
	
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};