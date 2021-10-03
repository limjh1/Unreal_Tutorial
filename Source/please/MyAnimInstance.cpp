// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyCharacter.h"

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

		auto Character = Cast<AMyCharacter>(pawn);
		if (Character) //ĳ���� Null üũ
		{
			isFalling = Character->GetMovementComponent()->IsFalling();

			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
		}
	}
}

//Ŭ���� �������� ����
void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	// 0 1 2 �� ������ � ������ ���ٰ����� �ٲٰ� ��
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	//Attack 0 1 2
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UMyAnimInstance::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("Anim_AttackNotify"));

	OnAttackHit.Broadcast(); //�����Ͼ �� ��������Ʈ �����Ѱŷ� �޼��� �޾ƶ�, ������ ������� Attack�� ȣ�� �Ǵ°���
}
