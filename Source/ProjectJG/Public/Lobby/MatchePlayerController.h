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


	//class UMatcheLobbyItemUserWidget* MatcheMenuItem;

	class AMatcheLobbyGameState* MatcheLobbyGameState;

	void CheckData();

public:
	class UMatcheLobbyUserWidget* MatcheMenu;
	UFUNCTION(Client, Reliable)
		void JoinMatcheLobby();

	UFUNCTION(Server, Reliable)
		void LogoutMatcheLobby();

	UFUNCTION(Server, Reliable)
		void ClickStartButton(bool bReady);

	UFUNCTION(Client, Reliable)
		void CreateMatcheWidget();

	UFUNCTION(Client, Reliable)
		void DeleteMatcheWidget();

	void UpdateMatcheWidget();

	bool bIsReady = false;
private:
	FTimerHandle PlayerCheckDataTimerHandle;
};
