// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedSkeletalMeshComponent.h"
#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCharacter.h"

FMatrix UExtendedSkeletalMeshComponent::GetRenderMatrix() const
{
	const FMatrix ModelMatrix = GetComponentTransform().ToMatrixWithScale();
	FMatrix InverseOldViewProjectonMatrix;
	FMatrix NewViewProjectionMatrix;

	AExtendedCharacter* ExCharacter = Cast<AExtendedCharacter>(GetOwner());
	APlayerController* PlController = Cast<APlayerController>(ExCharacter->GetController());

	if (PlController)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlController->Player);

		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			FSceneViewFamilyContext ViewFamily
			(
				FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, GetWorld()->Scene,
					LocalPlayer->ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true)
			);

			FVector ViewLocation;
			FRotator ViewRotation;
			FSceneView* SceneView = LocalPlayer->
				CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

			if (!SceneView)
			{
				return ModelMatrix;
			}

			const float DesiredFOV = 90.f;
			const float DesiredHalfFovRad = DesiredFOV * PI / 360.f;

			const auto ViewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();
			const float Width = ViewportSize.X;
			const float Height = ViewportSize.Y;

			const float NearClippingPlaneDistance = SceneView->NearClippingDistance;

			FMatrix NewProjectionMatrix =
				FReversedZPerspectiveMatrix(DesiredHalfFovRad, Width, Height, NearClippingPlaneDistance);

			const FMatrix ViewMatrix = SceneView->ViewMatrices.GetViewMatrix();
			const FMatrix InverseViewMatrix = SceneView->ViewMatrices.GetInvViewMatrix();
			const FMatrix ProjectionMatrix = SceneView->ViewMatrices.GetProjectionMatrix();
			const FMatrix InverseProjectionMatrix = SceneView->ViewMatrices.GetInvProjectionMatrix();

			NewViewProjectionMatrix = ViewMatrix * NewProjectionMatrix;
			InverseOldViewProjectonMatrix = InverseProjectionMatrix * InverseViewMatrix;
		}
	}

	return ModelMatrix * NewViewProjectionMatrix * InverseOldViewProjectonMatrix ;
}
