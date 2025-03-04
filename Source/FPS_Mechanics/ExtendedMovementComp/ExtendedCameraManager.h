// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ExtendedCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MECHANICS_API AExtendedCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime;


public:

	AExtendedCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
};
