// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h" //Ŭ���� ������ ��� ã�Ƽ� ���Խ�Ű�� ��
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h" //����� �׸� �׸���
#include "MyWeapon.h"
#include "MyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"
#include "MyAIController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent()); //��ī���� ĸ��������Ʈ�� ��
	Camera->SetupAttachment(SpringArm); //ī�޶� ��ī���� ��
	
	SpringArm->TargetArmLength = 900.f; //��ī�� ����
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn'")); //�޽� �ϳ� ã�Ƽ� ��������

	if (SM.Succeeded()) //����������
	{
		GetMesh()->SetSkeletalMesh(SM.Object); //�޽� �����°� �Ҵ�
	}

	//FName WeaponSocket(TEXT("hand_l_socket")); //hand_l_socket������ְ�
	//if (GetMesh()->DoesSocketExist(WeaponSocket)) //ã�Ƽ�
	//{
	//	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON")); //����ƽ�޽�������Ʈ ������ְ�

	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
	//	if (SW.Succeeded()) //�� �ҷ�������
	//	{
	//		Weapon->SetStaticMesh(SW.Object); //���� �߰�, �Ž� �ε��ؼ�
	//	}

	//	Weapon->SetupAttachment(GetMesh(), WeaponSocket); // ����ġ��Ʈ�� ��
	//}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh()); //��� ���� �޽��� ����
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	//2D�������� ��ũ���� �����ֱ�
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'")); //_C�ٿ�����
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	AIControllerClass = AMyAIController::StaticClass(); //�⺻���� ai�� ����ҰԿ�
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();		

	FName WeaponSocket(TEXT("hand_l_socket")); //hand_l_socket������ְ�

	//���� ���� //���Ⱑ 0,0���� ����
	/*auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator); 
	

	if (CurrentWeapon) //���� �����Ѱ� ����
	{
		CurrentWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
	}
	*/	
	
	
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance) {
		//��Ÿ�� �ִϸ��̼� ������ ȣ���ϴ� ��
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	}

	//�ʱ�ȭ
	HpBar->InitWidget();

	// TODO
	auto HpWidget = Cast<UMyCharacterWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
		HpWidget->BindHp(Stat); //BindHPȣ���� �Ǹ鼭
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region CAMERA ZOOM IN / OUT
	if (bZoomingIn)
	{
		ZoomFactor += DeltaTime * 2.0f;
	}
	else
	{
		ZoomFactor -= DeltaTime * 4.0f;
	}
	ZoomFactor = FMath::Clamp(ZoomFactor, 0.0f, 1.0f);
	Camera->FieldOfView = FMath::Lerp(90.f, 60.f, ZoomFactor);
#pragma endregion

	/*{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = cameraInput.X;
		SetActorRotation(NewRotation);
	}*/

	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + cameraInput.Y, -80.0f, -15.0f);						
		SpringArm->SetWorldRotation(NewRotation);
	}		
	
	// HP == 0
	if (&UMyStatComponent::GetHp == 0)
	{
		//Destroy();
		
		//UE_LOG(LogTemp, Log, TEXT("DEAD Actor : %s"), &AMyCharacter::GetDebugName);
		UE_LOG(LogTemp, Warning, TEXT("Actor is DEAD"));
	}
	
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AMyCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AMyCharacter::ZoomOut);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);	
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);	


}



void AMyCharacter::Attack() 
{
	if (IsAttacking) return;

	AnimInstance->PlayAttackMontage();

	AnimInstance->JumpToSection(AttackIndex); // 0 1 2
	AttackIndex = (AttackIndex + 1) % 3;

	IsAttacking = true;
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	//��ų, ��Ÿ ���� ����׷� �׸���
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;


	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 1.f);

	//�浹�� ���
	if (bResult && HitResult.Actor.IsValid()) {
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		//������ ȣ��
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}

void AMyCharacter::characterDestroy()
{
	&AActor::Destroy;
}

void AMyCharacter::UpDown(float Value)
{
	UpDownValue = Value;

	////UE_LOG(LogTemp, Warning, TEXT("UpDown %f"), Value);
	AddMovementInput(GetActorForwardVector(), Value); // return (GetActor~ * Value) 

	//AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), Value);
}

void AMyCharacter::LeftRight(float Value)
{
	LeftRightValue = Value;

	////UE_LOG(LogTemp, Warning, TEXT("LeftRight %f"), Value);
	AddMovementInput(GetActorRightVector(), Value);

	AddMovementInput(FRotationMatrix(FRotator(0.0f,GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), Value);
}

void AMyCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value / 5); // value�� ������ ������ ���콺 �ΰ��� ��������

	cameraInput.X = Value;
}

void AMyCharacter::Pitch(float Value)
{
	AddControllerPitchInput(Value);

	cameraInput.Y = Value;
}

void AMyCharacter::ZoomIn()
{
	bZoomingIn = true;
}

void AMyCharacter::ZoomOut()
{
	bZoomingIn = false;
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool blnterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast(); //���ݳ����ٰ� ����
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);

	return DamageAmount;
	
}

