// Copyright Epic Games, Inc. All Rights Reserved.


#include "PcgGameModeBase.h"

#include "EngineUtils.h"
#include "boost/random/mersenne_twister.hpp"
#include "Landscape.h"
#include "fstream"
#include "LandscapeEditorUtils.h"
#include "LandscapeInfo.h"
#include "FastNoise/FastNoise.h"

void APcgGameModeBase::StartPlay()
{
	Super::StartPlay();
	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hello World!"));
}

TArray<ALandscape*> APcgGameModeBase::GetLandscapes() const
{
	TArray<ALandscape*> Landscapes;
	ULevel* Level = GetLevel();
	for(int i = 0; i < Level->Actors.Num(); i++)
		if(ALandscape* Land = Cast<ALandscape>(Level->Actors[i]))
			Landscapes.Push(Land);
	return Landscapes;	
}

void APcgGameModeBase::GenerateLandscape() const
{
	typedef boost::mt19937 FRNGEngine;
	
	FRNGEngine Generator(static_cast<int32>(FDateTime::Now().GetTicks() % INT_MAX));
	const int32 Seed = Generator();
	FString String = FString::FromInt(Seed);
	const auto Simplex = FastNoise::New<FastNoise::Simplex>();
	const auto FractalFBm = FastNoise::New<FastNoise::FractalFBm>();
	
	FractalFBm->SetSource(Simplex);
	FractalFBm->SetOctaveCount(5);
	
	UE_LOG(LogTemp, Warning,TEXT("FastNoise setup complete."));

	for (TActorIterator<ALandscape> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ALandscape* Landscape = *ActorItr;
		if (IsValid(Landscape))
		{
			ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
			if(!IsValid(LandscapeInfo))
			{
				UE_LOG(LogTemp, Warning,TEXT("Landscape Info is not valid!"));
				return;
			}
			LandscapeInfo->LandscapeGuid = Landscape->GetLandscapeGuid();
			LandscapeInfo->ComponentSizeQuads = Landscape->ComponentSizeQuads;
			LandscapeInfo->ComponentNumSubsections = Landscape->NumSubsections;
			LandscapeInfo->SubsectionSizeQuads = Landscape->SubsectionSizeQuads;
			LandscapeInfo->DrawScale = Landscape->GetRootComponent() != nullptr ? Landscape->GetRootComponent()->GetRelativeScale3D() : FVector(100.0f);
			LandscapeInfo->LandscapeActor = Landscape->GetLandscapeActor();
			if(Landscape->GetLandscapeActor() == nullptr)
			{
				UE_LOG(LogTemp, Warning,TEXT("Landscape Actor is null!"));
				return;
			}
			FIntRect Bounds = Landscape->GetBoundingRect();
			const int32 NumHeights = (Bounds.Width()+1)*(Bounds.Height()+1);
			TArray<float> Data;
			Data.Init(0, NumHeights);
			FractalFBm->GenUniformGrid2D(Data.GetData(), 0, 0,  Bounds.Width(), Bounds.Height(), 0.2f, Seed);
			
			TArray<uint16> HeightMapData = ScaleNoiseForHeightMap(Data, -1.0f, 1.0f);
			UE_LOG(LogTemp, Warning,TEXT("%s points of noise generated"), *FString::FromInt(NumHeights));
			std::ofstream Noise;
			Noise.open("noise_scaled.txt");
			if (Noise.is_open())
			{
				UE_LOG(LogTemp, Warning,TEXT("Writing %s points of noise to file."), *FString::FromInt(NumHeights));
				for(int count = 0; count < Data.Num(); count ++){
					Noise << HeightMapData[count] << "\n" ;
				}
				Noise.close();
			}
			LandscapeEditorUtils::SetHeightmapData(Landscape, HeightMapData);
			UE_LOG(LogTemp, Warning,TEXT("Heightmap set."), *FString::FromInt(NumHeights));
		}
	}
}

