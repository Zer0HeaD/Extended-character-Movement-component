// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "ExtendedSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MECHANICS_API UExtendedSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	FMatrix GetRenderMatrix() const override;
	
};
