// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
        return Word.Len() > 4 && Word.Len() < 8 && IsIsogram(Word);
    });

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Hi, Welcome to the BullCowCartridge!"));

    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess and press enter to continue..."));
    //PrintLine(TEXT("The hidden word is %s."), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Please press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (HiddenWord == Guess)
    {
        PrintLine("Your guess is right!");
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    --Lives;
    PrintLine(TEXT("You lost a live!"));

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives."));
        PrintLine(TEXT("The Hiddenword is %s."), *HiddenWord);
        EndGame();
        return;
    }

    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);

    PrintLine(TEXT("Your guess is wrong! Now you have %i lives. Please guess again!"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex != Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex != HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                CowCount++;
            }
        }
    }
}
