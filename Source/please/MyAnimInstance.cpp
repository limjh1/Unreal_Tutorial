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

//매 틱마다 들고옴
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto pawn = TryGetPawnOwner(); //주인을 찾아 보겠다.
	if (IsValid(pawn)) //주인을 찾았으면
	{
		Speed = pawn->GetVelocity().Size(); //속도값을 speed에 매 초 갱신

		auto Character = Cast<ACharacter>(pawn);
		if (Character) //캐릭터 Null 체크
		{
			isFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

//클릭한 순간에만 들고옴
void UMyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage)) //플레이 중이라면 중복해서 플레이 제한
	{
		Montage_Play(AttackMontage, 1.f);
	}
}
