#include "Utilities/Clog.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(GameObject, Warning, All)


void Clog::Print(int32 InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, FString::FromInt(InValue));
}

void Clog::Print(float InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, FString::SanitizeFloat(InValue));
}

void Clog::Print(const FString& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue);
}

void Clog::Print(const FVector& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue.ToString());
}

void Clog::Print(const FRotator& InValue, int32 InKey, float Duration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, Duration, InColor, InValue.ToString());
}

void Clog::Log(int32 InValue)
{
	UE_LOG(GameObject, Warning, L"%d", InValue);
}

void Clog::Log(float InValue)
{
	UE_LOG(GameObject, Warning, L"%f", InValue);
}

void Clog::Log(const FString& InValue)
{
	UE_LOG(GameObject, Warning, L"%s", *InValue);
}

void Clog::Log(const FVector& InValue)
{
	UE_LOG(GameObject, Warning, L"%s", *InValue.ToString());
}

void Clog::Log(const FRotator& InValue)
{
	UE_LOG(GameObject, Warning, L"%s", *InValue.ToString());
}

void Clog::Log(const UObject* InObject)
{
	FString str;
	if (!!InObject)
	{
		str.Append(InObject->GetName());
	}
	str.Append(!!InObject ? " Not Null " : "Null");
	UE_LOG(GameObject, Warning, L"%s", *str);
}

void Clog::Log(const FString& InFuncName, int32 InLineNumber)
{
	FString str;
	str.Append(InFuncName);
	str.Append(", ");
	str.Append(FString::FromInt(InLineNumber));
	UE_LOG(GameObject, Warning, L"%s", *str);
}

