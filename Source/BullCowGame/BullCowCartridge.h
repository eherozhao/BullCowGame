// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
    GENERATED_BODY()

public:
    void BeginPlay() override;
    void OnInput(const FString&) override;
    void SetupGame();
    void EndGame();
    void ProcessGuess(const FString&);
    void GetBullCows(const FString&, int32&, int32&) const;
    static bool IsIsogram(const FString&);

    // Your declarations go below!
private:
    FString HiddenWord;
    int32 Lives;
    bool bGameOver;
    TArray<FString> Isograms;
};
