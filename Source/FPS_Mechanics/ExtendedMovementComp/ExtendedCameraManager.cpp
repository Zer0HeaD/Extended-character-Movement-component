// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCameraManager.h"
#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCharacter.h"
#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCharacterMovementComp.h"
#include "Components/CapsuleComponent.h"



AExtendedCameraManager::AExtendedCameraManager()
{

}

void AExtendedCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	//if (AExtendedCharacter* ExCharacter = Cast<AExtendedCharacter>(GetOwningPlayerController()->GetPawn()))
	//{
	//	UExtendedCharacterMovementComp* EMC = ExCharacter->GetExtendedCharacterMovement();

	//	FVector TargetCrouchOffset = FVector(
	//		0,
	//		0,
	//		EMC->GetCrouchedHalfHeight() - ExCharacter->
	//		GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	//	FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset,
	//		FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));
	//	
	//	if (EMC->IsCrouching())
	//	{
	//		CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
	//		Offset -= TargetCrouchOffset;
	//	}
	//	else
	//	{
	//		CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
	//	}

	//	if (EMC->IsMovingOnGround())
	//	{
	//		OutVT.POV.Location += Offset;
	//		//ExCharacter->GetFPSRoot()->AddRelativeLocation(Offset);
	//	}
	//}
}
