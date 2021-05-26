// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"
#include "GameFramework/GameUserSettings.h"

void URPGGameInstance::Init()
{
    Super::Init();

    if (GEngine)
    {
        UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
        MyGameSettings->SetScreenResolution(FIntPoint(720, 480));
        MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
        MyGameSettings->SetVSyncEnabled(true);
        MyGameSettings->ApplySettings(true);
    }
}
