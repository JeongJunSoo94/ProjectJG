// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseSystem/GameHUD.h"
#include "WorldObjects/Item/Item.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

UCLASS()
class PROJECTJG_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//UPROPERTY(EditAnywhere, Category = "Player Stats")
	//	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	//void AddCharacterOverlay();

	//UPROPERTY()
	//	class UUserWidget* CharacterOverlay;
	//UPROPERTY()
	//AGameHUD* GameHUD;

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	//void SetHUDInventorySlot(AItem* Item);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible
	void OnMatchStateSet(FName State, bool bTeamsMatch = false);
	void HandleMatchHasStarted(bool bTeamsMatch = false);
	void HandleCooldown();

	float SingleTripTime = 0.f;

	FHighPingDelegate HighPingDelegate;

	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
	void PollInit();
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable)
		void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
		void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
		float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	UFUNCTION(Server, Reliable)
		void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
		void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);

	void ShowReturnToMainMenu();

	UFUNCTION(Client, Reliable)
		void ClientElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	FString GetInfoText(const TArray<class AInGamePlayerState*>& Players);

private:
	UPROPERTY()
		class AGameHUD* GameHUD;

	UPROPERTY()
		class ABattleGameMode* BattleGameMode;

	UPROPERTY(EditAnywhere, Category = HUD)
		TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
		class UUserWidget* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
		FName MatchState;

	UFUNCTION()
		void OnRep_MatchState();

	UPROPERTY()
		class UPlayerInGameWidget* PlayerInGameWidget;

	float HUDHealth;
	bool bInitializeHealth = false;
	float HUDMaxHealth;
	float HUDScore;
	bool bInitializeScore = false;
	int32 HUDDefeats;
	bool bInitializeDefeats = false;
	int32 HUDGrenades;
	bool bInitializeGrenades = false;
	float HUDShield;
	bool bInitializeShield = false;
	float HUDMaxShield;
	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;

	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
		float HighPingDuration = 5.f;

	float PingAnimationRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
		float CheckPingFrequency = 20.f;

	UFUNCTION(Server, Reliable)
		void ServerReportPingStatus(bool bHighPing);

	UPROPERTY(EditAnywhere)
		float HighPingThreshold = 50.f;
};
