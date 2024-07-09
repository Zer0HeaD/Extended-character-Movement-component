// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCharacter.h"
#include "FPS_Mechanics/ExtendedMovementComp/ExtendedCharacterMovementComp.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
AExtendedCharacter::AExtendedCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UExtendedCharacterMovementComp>
		(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExtendedCharacterMovementComponent = Cast<UExtendedCharacterMovementComp>(GetCharacterMovement());
	ExtendedCharacterMovementComponent->SetIsReplicated(true);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FPSRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FPSRoot"));
	FPSRoot->SetupAttachment(RootComponent);

	FPS_Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSLegs"));
	FPS_Legs->SetupAttachment(RootComponent);

	// Create a FPS Root camera boom (pulls in towards the player if there is a collision)
	FPSRootCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPSRootCameraBoom"));
	FPSRootCameraBoom->SetupAttachment(FPSRoot);
	FPSRootCameraBoom->TargetArmLength = 0.f; // The camera follows at this distance behind the character	

	//Offset Root
	OffsetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("OffsetRoot"));
	OffsetRoot->SetupAttachment(FPSRootCameraBoom);

	//create fps Mesh
	FPS_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FPS_Mesh->SetupAttachment(OffsetRoot);

	//FPS Camera Root
	FPSCameraRoot = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPSCameraRoot"));
	FPSCameraRoot->SetupAttachment(FPS_Mesh, "SOCKET_Camera");
	FPSCameraRoot->TargetArmLength = 0.f; // The camera follows at this distance behind the character	


	// Create a follow camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(FPSCameraRoot);
	FPSCamera->bUsePawnControlRotation = true; // Camera does rotate relative to arm
}

void AExtendedCharacter::Jump()
{
	Super::Jump();

	bPressedExtendedJump = true;
	bPressedJump = false;
}

void AExtendedCharacter::StopJumping()
{
	Super::StopJumping();

	bPressedExtendedJump = false;
}

// Called when the game starts or when spawned
void AExtendedCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AExtendedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AExtendedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExtendedCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExtendedCharacter::Look);
	}
}

void AExtendedCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		MoveX = MovementVector.X;

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AExtendedCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		LookX = LookAxisVector.X;
		LookY = LookAxisVector.Y;

		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AExtendedCharacter::FPSPlayTransitionTallMontage_Implementation()
{
}

void AExtendedCharacter::FPSPlayTransitionShortMontage_Implementation()
{
}

void AExtendedCharacter::FPSPlayTransitionQueuedMontage_Implementation()
{
}

FCollisionQueryParams AExtendedCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;
	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);

	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

