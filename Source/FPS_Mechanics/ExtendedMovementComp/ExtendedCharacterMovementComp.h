// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ExtendedCharacterMovementComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

class AExtendedCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None       UMETA(Hidden),
	CMOVE_Slide      UMETA(DisplayName = "Slide"),
	CMOVE_Prone      UMETA(DisplayName = "Prone"),
	CMOVE_Wallrun    UMETA(DisplayName = "Wallrun"),
	CMOVE_Hang       UMETA(DisplayName = "Hang"),
	CMOVE_Climb      UMETA(DisplayName = "Climb"),
	CMOVE_MAX        UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class FPS_MECHANICS_API UExtendedCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_ExtendedMoveCharacter : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

	public:
		enum CompressedFlags
		{
			FLAG_Sprint     = 0x10,
			FLAG_Dash       = 0x20,
			FLAG_Custom_2   = 0x40,
			FLAG_Custom_3   = 0x80,
			FLAG_Walk      = 0x160,
		};

		//Flag
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bWantsToWalk : 1;
		uint8 Saved_bWantsToDash : 1;
		uint8 Saved_bPressedExtendedJump : 1;

		//Other Variables
		uint8 Saved_bPrevWantsToCrouch : 1;
		uint8 Saved_bWantsToProne : 1;
		uint8 Saved_bHadAnimRootMotion : 1;
		uint8 Saved_bTransitionFinished : 1;
		uint8 Saved_bWallrunIsRight : 1;


		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_ExtendedCharacter : public FNetworkPredictionData_Client_Character
	{
	public: 
		FNetworkPredictionData_Client_ExtendedCharacter(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:

	//Params
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ground Movement") 
	float MaxSprintSpeed = 750.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ground Movement")
	float MaxWalkingSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ground Movement")
	float JumpForwardForce = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ground Movement")
	float JumpForwardModifier = 0.5f;

private:

	//Slide params
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float Slide_MinSpeed = 350;       //Min required speed to start sliding
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float Slide_MaxSpeed = 550;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float Slide_EnterImpulse = 500;   //Start impulse when entering side
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float Slide_GravityForce = 5000;  //How many gravity force applied to slide + how fast velocity changes on slope
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideFrictionFactor = 0.06f;  //How fast char loose velocity on surface
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float BrakingDecelerationSliding = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideJumpForce = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideJumpUPModifier = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideJumpFORWARDModifier = 0.35f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideJumpACCELERATIONModifier = 0.35f;

	//Prone params
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float Prone_EnterHoldDuration = 0.2f;       //How long to hold to entering the prone
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float ProneSlideEnterImpulse = 300.f;       //Dive into prone after slide
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float ProneMaxSpeed = 300.f;                //ProneMaxSpeed
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float BrakingDecelerationProning = 2500.f;  //

	//Dash
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float DashImpulse = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float DashCoolDownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float AuthDashCooldownDuration = 0.9f;

	//Mantle
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxDistance = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleReachHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MinMantleDepth = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMinWallSteepnessAngle = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxSurfaceAngle = 40.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxAlignmentAngle = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ProxyShortMantleMontage;
	//FPS
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSTransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSTransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* FPSProxyShortMantleMontage;


	//Wallrun
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float MinWallrunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float MaxWallrunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float MaxVerticalWallrunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float WallrunPullAwayAngle = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float WallrunAttractionForce = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float MinWallrunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") UCurveFloat* WallrunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Wallrun") float WallJumpOffForce = 300.f;


	//Hang
	UPROPERTY(EditDefaultsOnly, Category = "Hang") UAnimMontage* TransitionHangMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Hang") UAnimMontage* WallJumpMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Hang") float WallJumpForce = 400.f;

	//Climb
	UPROPERTY(EditDefaultsOnly, Category = "Climb") float MaxClimbSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Climb") float BrakingDecelerationClimbing = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Climb") float ClimbReachDistance = 200.f;


	//Transient
	UPROPERTY(Transient) AExtendedCharacter* ExtendedCharacterOwner;

	//FLAGS
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;
	bool Safe_bWantsToDash;

	bool Safe_bHadAnimRootMotion;
	float DashStartTime;

	FTimerHandle TimerHandle_EnterProne;
	FTimerHandle TimerHandle_DashCooldown;

	bool Safe_bTransitionFinished;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	FString TransitionName;

	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	int TransitionRMS_ID;

	bool Safe_bWallrunIsRight;

	//Replication
	UPROPERTY(ReplicatedUsing = OnRep_DashStart) bool Proxy_bDashStart;
	UPROPERTY(ReplicatedUsing = OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing = OnRep_TallMantle) bool Proxy_bTallMantle;

public:

	UPROPERTY(Transient, BlueprintReadOnly) UAnimMontage* FPSTransitionQueuedMontage;
	UPROPERTY(BlueprintReadOnly) float TransitionQueuedMontageSpeed;

	UPROPERTY(BlueprintReadOnly) bool Safe_bWantsToSprint;
	UPROPERTY(BlueprintReadOnly) bool Safe_bWantsToWalk;

	//DELEGATES
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;

	UExtendedCharacterMovementComp();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;

protected:

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void InitializeComponent() override;

private:

	//Prone
	void TryEnterProne() { Safe_bWantsToProne = true; }

	UFUNCTION(Server, Reliable) void Server_EnterProne();

	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iteration);


	//Slide
	UFUNCTION(BlueprintCallable) void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);

public:
	UFUNCTION(BlueprintCallable) void ExitSlide();

private:
	UFUNCTION(BlueprintCallable) bool CanSlide() const;

	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;


	//Dash
	void OnDashCooldownFinished();
	bool CanDash() const;

	//Mantle
	bool TryMantle();
	FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;

	//Wallrun
	bool TryWallrun();
	void PhysWallrun(float deltaTime, int32 Iterations);


	//Climb
	UFUNCTION(BlueprintCallable) bool TryHang();

	UFUNCTION(BlueprintCallable) bool TryClimb();
	void PhysClimb(float deltaTime, int32 Iterations);

	//Helpers
	bool IsServer() const;
	float CapR() const;
	float CapHH() const;

	//Replication
	UFUNCTION() void OnRep_DashStart();
	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();

public:
	//Dash
	void PerformDash();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsSliding() const { return IsCustomMovementMode(CMOVE_Slide); }
	UFUNCTION(BlueprintPure) bool IsWallrunning() const { return IsCustomMovementMode(CMOVE_Wallrun); }
	UFUNCTION(BlueprintPure) bool WallrunningIsRight() const { return Safe_bWallrunIsRight; }
	UFUNCTION(BlueprintPure) bool IsHanging() const { return IsCustomMovementMode(CMOVE_Hang); }
	UFUNCTION(BlueprintPure) bool IsClimbing() const { return IsCustomMovementMode(CMOVE_Climb); }

	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void WalkPressed();
	UFUNCTION(BlueprintCallable) void WalkReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();

	UFUNCTION(BlueprintCallable) void ClimbPressed();
	UFUNCTION(BlueprintCallable) void ClimbReleased();
	
};
