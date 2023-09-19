#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Components/CActionComponent.h"
#include "BaseHUDComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UBaseHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseHUDComponent();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget_CrossHair> CrossHairClass;

	class UUserWidget_CrossHair* CrossHair;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UPlayerInGameWidget> PlayerInGameClass;

	class UPlayerInGameWidget* PlayerInGameUI;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UHealthWidget> HealthClass;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HeadHealthWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		class UHealthWidget* HealthClass;

	UPROPERTY(EditDefaultsOnly, Category = "SkillWidgets")
		class UCharacterSkillWidget* LeftSkillClass;
	UPROPERTY(EditDefaultsOnly, Category = "SkillWidgets")
		class UCharacterSkillWidget* MidSkillClass;
	UPROPERTY(EditDefaultsOnly, Category = "SkillWidgets")
		class UCharacterSkillWidget* RightSkillClass;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CreatePlayerControllerAttach(APlayerController* controller);
	virtual void SetPlayerControllerAttach(UActorComponent* Weapon = nullptr, UCActionComponent* Left = nullptr, UCActionComponent* Mid = nullptr, UCActionComponent* Right = nullptr);
	virtual void CreateHeadHealthBar();
	virtual void HealthBarUpdate(float curHealth,float maxHealth);
	void SetHealthWidget(UHealthWidget* healthWidget) { HealthClass = healthWidget; }
	void SetSkillWidget(UCharacterSkillWidget* Left, UCharacterSkillWidget* Mid, UCharacterSkillWidget* Right) { LeftSkillClass= Left; MidSkillClass = Mid; RightSkillClass = Right;}
public:
	bool isHiddenHeadHealthBar=false;
	//TMap<FName, UCharacterSkillWidget*> SkillWidgets;
};
