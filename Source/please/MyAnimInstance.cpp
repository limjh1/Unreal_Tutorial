// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

//�� ƽ���� ����
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto pawn = TryGetPawnOwner(); //������ ã�� ���ڴ�.
	if (IsValid(pawn)) //������ ã������
	{
		Speed = pawn->GetVelocity().Size(); //�ӵ����� speed�� �� �� ����

		auto Character = Cast<ACharacter>(pawn);
		if (Character) //ĳ���� Null üũ
		{
			isFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

//Ŭ���� �������� ����
void UMyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage)) //�÷��� ���̶�� �ߺ��ؼ� �÷��� ����
	{
		Montage_Play(AttackMontage, 1.f);
	}
}
