#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "SelectCharacterDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FSelectCharacterList
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UTexture2D* image;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> SelectCharacter;
};

UCLASS()
class PROJECTJG_API USelectCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSelectCharacterList> SelectCharacters;
};
