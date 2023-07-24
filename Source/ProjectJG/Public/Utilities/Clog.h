#pragma once

#include "CoreMinimal.h"

#define PrintLine() {Clog::Log(__FUNCTION__,__LINE__);}

class PROJECTJG_API Clog
{
public:
	static void Print(int32 InValue, int32 InKey = -1,
		float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1,
		float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1,
		float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1,
		float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1,
		float Duration = 10.0f, FColor InColor = FColor::Blue);
	// << : 인게임 화면

	// >> : 
	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);

	// << : 출력 로그창

	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);
};
