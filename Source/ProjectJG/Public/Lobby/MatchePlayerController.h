// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MatchePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AMatchePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> MatcheMenuClass;

	class UMatcheLobbyUserWidget* MatcheMenu;

	class UMatcheLobbyItemUserWidget* MatcheMenuItem;

	class AMatcheLobbyGameState* MatcheLobbyGameState;
public:
	UFUNCTION(Server, Reliable)
		void JoinMatcheLobby();

	UFUNCTION(Server, Reliable)
		void LogoutMatcheLobby();

	UFUNCTION(Client, Reliable)
		void UpdateMatcheWidget();

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Widget State")
		int32 SlotIdx = -1;

	UPROPERTY(ReplicatedUsing = OnRep_Ready, VisibleAnywhere, Category = "Widget State")
		bool IsReady = false;
	UFUNCTION()
		void OnRep_Ready(bool LastIsReady);
};
