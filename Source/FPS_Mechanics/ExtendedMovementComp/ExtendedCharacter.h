// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ExtendedCharacter.generated.h"

class UExtendedCharacterMovementComp;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class FPS_MECHANICS_API AExtendedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** FPS camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPSCamera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FPSCameraRoot;

	/** FPS Root Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FPSRootCameraBoom;

	/** FPS mesh/arms */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FPS_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FPS_Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FPSRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* OffsetRoot;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	AExtendedCharacter(const FObjectInitializer& ObjectInitializer);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;
	virtual void StopJumping() override;

	bool bPressedExtendedJump;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) bool bFirstPerson;

	// For Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float MoveX = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float LookX = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float LookY = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement)
	UExtendedCharacterMovementComp* ExtendedCharacterMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void FPSPlayTransitionTallMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void FPSPlayTransitionShortMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void FPSPlayTransitionQueuedMontage();

	FCollisionQueryParams GetIgnoreCharacterParams() const;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UCameraComponent* GetFPSCamera() const { return FPSCamera; }
	FORCEINLINE USkeletalMeshComponent* GetFPSMesh() const { return FPS_Mesh; }
	FORCEINLINE USceneComponent* GetFPSRoot() const { return FPSRoot; }
	FORCEINLINE UExtendedCharacterMovementComp* GetExtendedCharacterMovement() 
		const { return ExtendedCharacterMovementComponent; }
};
