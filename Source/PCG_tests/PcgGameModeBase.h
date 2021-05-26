// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Landscape.h"
#include "GameFramework/GameModeBase.h"
#include "PcgGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PCG_TESTS_API APcgGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
	TArray<ALandscape*> GetLandscapes() const;
	template<class T>
	static TArray<uint16> ScaleNoiseForHeightMap(TArray<T> Input, T Min, T Max);

	UFUNCTION(BlueprintCallable)
	void GenerateLandscape() const;
};

template <class T>
TArray<uint16> APcgGameModeBase::ScaleNoiseForHeightMap(TArray<T> Input, T Min, T Max)
{
	TArray<uint16> Output;
	const uint16 MaxHeight = USHRT_MAX;
	const uint16 MinHeight = 0;
	Output.Init(USHRT_MAX/2 + 1, Input.Num());
	for(int32 i = 0; i < Input.Num(); i++)
		Output[i] = static_cast<uint16>((MaxHeight - MinHeight)*(Input[i] - Min) / (Max - Min) + MinHeight);
	return Output;
}
