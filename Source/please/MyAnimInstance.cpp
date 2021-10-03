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

//매 틱마다 들고옴
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto pawn = TryGetPawnOwner(); //주인을 찾아 보겠다.
	if (IsValid(pawn)) //주인을 찾았으면
	{
		Speed = pawn->GetVelocity().Size(); //속도값을 speed에 매 초 갱신

		auto Character = Cast<AMyCharacter>(pawn);
		if (Character) //캐릭터 Null 체크
		{
			isFalling = Character->GetMovementComponent()->IsFalling();

			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
		}
	}
}

//클릭한 순간에만 들고옴
void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UMyAnimInstance::JumpToSection(int32 SectionIndex)
{
	// 0 1 2 될 때마다 어떤 섹션을 해줄것인지 바꾸게 됨
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

	OnAttackHit.Broadcast(); //공격일어날 때 델레게이트 선언한거로 메세지 받아라, 구독한 사람한테 Attack이 호출 되는거임
}
