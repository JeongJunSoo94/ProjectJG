// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Murdock/Notifies/MurdockUltimateAnimNotify.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/Murdock.h"

FString UMurdockUltimateAnimNotify::GetNotifyName_Implementation() const
{
	return  "LoopUltimate";
}

void UMurdockUltimateAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	if (cachingMurdock == nullptr)
	{
		cachingMurdock = Cast<AMurdock>(MeshComp->GetOwner());
	}

	CheckNull(cachingMurdock);
	cachingMurdock->LoopUltimate();

}
