#include "BaseSystem/InGamePlayerController.h"
#include "Global.h"
#include "BaseSystem/GameHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Character/BaseCharacter.h"
#include "BaseSystem/BattleGameMode.h"
#include "Character/Components/CombatComponent.h"
#include "Components/Image.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "Widgets/PlayerInGameWidget.h"
#include "Widgets/Character/ItemInfoWidget.h"
#include "Widgets/Character/InventoryBarWidget.h"
#include "WorldObjects/Item/Item.h"
#include "GameFramework/PlayerState.h"

void AInGamePlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);
}

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameHUD = Cast<AGameHUD>(GetHUD());
	ServerCheckMatchState();
}

void AInGamePlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		//if (BattleGameMode == nullptr)
		//{
		//	BattleGameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(this));
		//	LevelStartingTime = BattleGameMode->LevelStartingTime;
		//}
		//BattleGameMode = BattleGameMode == nullptr ? Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(this)) : BattleGameMode;
		//if (BattleGameMode)
		//{
		//	SecondsLeft = FMath::CeilToInt(BattleGameMode->GetCountdownTime() + LevelStartingTime);
		//}
	}

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			//SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			//SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

void AInGamePlayerController::PollInit()
{
	if (PlayerInGameWidget == nullptr)
	{
		if (GameHUD && GameHUD->GetPlayerInGameWidget())
		{
			PlayerInGameWidget = GameHUD->GetPlayerInGameWidget();
			if (PlayerInGameWidget)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeShield) SetHUDShield(HUDShield, HUDMaxShield);
				if (bInitializeScore) SetHUDScore(HUDScore);
				if (bInitializeDefeats) SetHUDDefeats(HUDDefeats);
				if (bInitializeCarriedAmmo) SetHUDCarriedAmmo(HUDCarriedAmmo);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);

				/*ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
				if (BlasterCharacter && BlasterCharacter->GetCombat())
				{
					if (bInitializeGrenades) SetHUDGrenades(BlasterCharacter->GetCombat()->GetGrenades());
				}*/
			}
		}
	}
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("ESC", EInputEvent::IE_Pressed, this, &AInGamePlayerController::ShowReturnToMainMenu);

}

void AInGamePlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}

void AInGamePlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AInGamePlayerController::ServerCheckMatchState_Implementation()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void AInGamePlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (GameHUD && MatchState == MatchState::WaitingToStart)
	{
	//	BlasterHUD->AddAnnouncement();
	}
}

void AInGamePlayerController::HighPingWarning()
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	//bool bHUDValid = GameHUD &&
	//	GameHUD->GetPlayerInGameWidget() &&
	//	GameHUD->GetPlayerInGameWidget()->HighPingImage &&
	//	GameHUD->GetPlayerInGameWidget()->HighPingAnimation;
	//if (bHUDValid)
	//{
	//	GameHUD->GetPlayerInGameWidget()->HighPingImage->SetOpacity(1.f);
	//	GameHUD->GetPlayerInGameWidget()->PlayAnimation(
	//		GameHUD->GetPlayerInGameWidget()->HighPingAnimation,
	//		0.f,
	//		5);
	//}
}

void AInGamePlayerController::StopHighPingWarning()
{
}

void AInGamePlayerController::CheckPing(float DeltaTime)
{
	if (HasAuthority()) return;
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;
		if (PlayerState)
		{
			if (PlayerState->GetPing() * 4 > HighPingThreshold) // ping is compressed; it's actually ping / 4
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				ServerReportPingStatus(false);
			}
		}
		HighPingRunningTime = 0.f;
	}
	/*bool bHighPingAnimationPlaying =
		GameHUD && GameHUD->GetCharacterOverlay() &&
		GameHUD->GetCharacterOverlay()->HighPingAnimation &&
		GameHUD->GetCharacterOverlay()->IsAnimationPlaying(GameHUD->GetCharacterOverlay()->HighPingAnimation);
	if (bHighPingAnimationPlaying)*/
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

void AInGamePlayerController::ShowReturnToMainMenu()
{
	if (ReturnToMainMenu && ReturnToMainMenu->IsInViewport())
	{
		ReturnToMainMenu->RemoveFromViewport();
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		if (!ReturnToMainMenu && ReturnToMainMenuWidget)
		{
			ReturnToMainMenu = CreateWidget<UUserWidget>(this, ReturnToMainMenuWidget);
		}

		if (ReturnToMainMenu)
		{
			ReturnToMainMenu->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AInGamePlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
}

FString AInGamePlayerController::GetInfoText(const TArray<class AInGamePlayerState*>& Players)
{
	AInGamePlayerState* BlasterPlayerState = GetPlayerState<AInGamePlayerState>();
	if (BlasterPlayerState == nullptr) return FString();
	FString InfoTextString;
	/*if (Players.Num() == 0)
	{
		InfoTextString = Announcement::ThereIsNoWinner;
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState)
	{
		InfoTextString = Announcement::YouAreTheWinner;
	}
	else if (Players.Num() == 1)
	{
		InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName());
	}
	else if (Players.Num() > 1)
	{
		InfoTextString = Announcement::PlayersTiedForTheWin;
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players)
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
	}*/

	return InfoTextString;
}

void AInGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
	CheckPing(DeltaTime);
}

void AInGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGamePlayerController, MatchState);
	//DOREPLIFETIME(AInGamePlayerController, bShowTeamScores);
}

void AInGamePlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AInGamePlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float AInGamePlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AInGamePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AInGamePlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AInGamePlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
	//if (HasAuthority()) bShowTeamScores = bTeamsMatch;
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD)
	{
		if (GameHUD->GetPlayerInGameWidget() == nullptr) GameHUD->AddPlayerInGameWidget();
		/*if (GameHUD->Announcement)
		{
			GameHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
		if (!HasAuthority()) return;
		if (bTeamsMatch)
		{
			InitTeamScores();
		}
		else
		{
			HideTeamScores();
		}*/
	}
}

void AInGamePlayerController::HandleCooldown()
{
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	//if (BlasterHUD)
	//{
	//	BlasterHUD->CharacterOverlay->RemoveFromParent();
	//	bool bHUDValid = BlasterHUD->Announcement &&
	//		BlasterHUD->Announcement->AnnouncementText &&
	//		BlasterHUD->Announcement->InfoText;

	//	if (bHUDValid)
	//	{
	//		BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
	//		FString AnnouncementText = Announcement::NewMatchStartsIn;
	//		BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

	//		ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	//		ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	//		if (BlasterGameState && BlasterPlayerState)
	//		{
	//			TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
	//			FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);

	//			BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
	//		}
	//	}
	//}
	//ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
	//if (BlasterCharacter && BlasterCharacter->GetCombat())
	//{
	//	BlasterCharacter->bDisableGameplay = true;
	//	BlasterCharacter->GetCombat()->FireButtonPressed(false);
	//}
}

void AInGamePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD&& GameHUD->GetPlayerInGameWidget())
	{
		GameHUD->GetPlayerInGameWidget()->HealthUpdate(Health, MaxHealth);
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AInGamePlayerController::SetHUDShield(float Shield, float MaxShield)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	//bool bHUDValid = GameHUD &&
	//	GameHUD->GetCharacterOverlay() &&
	//	GameHUD->GetCharacterOverlay()->ShieldBar &&
	//	GameHUD->GetCharacterOverlay()->ShieldText;
	//if (bHUDValid)
	//{
	//	const float ShieldPercent = Shield / MaxShield;
	//	GameHUD->GetCharacterOverlay()->ShieldBar->SetPercent(ShieldPercent);
	//	FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
	//	GameHUD->GetCharacterOverlay()->ShieldText->SetText(FText::FromString(ShieldText));
	//}
	//else
	//{
	//	bInitializeShield = true;
	//	HUDShield = Shield;
	//	HUDMaxShield = MaxShield;
	//}
}

void AInGamePlayerController::SetHUDScore(float Score)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	//bool bHUDValid = BlasterHUD &&
	//	GameHUD->CharacterOverlay &&
	//	GameHUD->CharacterOverlay->ScoreAmount;

	if (GameHUD && GameHUD->GetPlayerInGameWidget())
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		GameHUD->GetPlayerInGameWidget()->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeScore = true;
		HUDScore = Score;
	}
}

void AInGamePlayerController::SetHUDDefeats(int32 Defeats)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	//bool bHUDValid = BlasterHUD &&
	//	BlasterHUD->CharacterOverlay &&
	//	BlasterHUD->CharacterOverlay->DefeatsAmount;
	if (GameHUD && GameHUD->GetPlayerInGameWidget())
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		GameHUD->GetPlayerInGameWidget()->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

void AInGamePlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD && GameHUD->GetPlayerInGameWidget())
	{
		GameHUD->GetPlayerInGameWidget()->SetItemInfoCount(Ammo);
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void AInGamePlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD && GameHUD->GetPlayerInGameWidget())
	{
		GameHUD->GetPlayerInGameWidget()->SetItemInfoCarried(Ammo);
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

//void AInGamePlayerController::SetHUDInventorySlot(AItem* Item)
//{
//	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
//	if (GameHUD && GameHUD->GetPlayerInGameWidget())
//	{
//		GameHUD->GetPlayerInGameWidget()->GetInventoryBar()->GetSlotWidget(Item->GetSlotIndex())->UpdateItemSlotImage(Item->GetIconBackground(),Item->GetIconItem(), Item->GetAmmoIcon());
//	}
//
//}

void AInGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(InPawn);
	if (BaseCharacter)
	{
		SetHUDHealth(BaseCharacter->GetHealth(), BaseCharacter->GetMaxHealth());
	}
}

void AInGamePlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}