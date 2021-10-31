// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class PLEASE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void AttackCheck();

	static void characterDestroy();

	FOnAttackEnd OnAttackEnd;

#pragma region CAMERA
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);
	void ZoomIn();
	void ZoomOut();
#pragma endregion

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool blnterrupted);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm; //¼¿Ä«ºÀ

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category=Pawn)
	bool IsAttacking = false;

	UPROPERTY()
	class UMyAnimInstance* AnimInstance;

	UPROPERTY()
	int32 AttackIndex = 0;

public:

	UPROPERTY()
	float UpDownValue = 0;

	UPROPERTY()
	float LeftRightValue = 0;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;

	UPROPERTY()
	FVector2D cameraInput;

	UPROPERTY()
	float ZoomFactor;

	UPROPERTY()
	bool bZoomingIn;

};

