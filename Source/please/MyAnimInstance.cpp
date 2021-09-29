// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto pawn = TryGetPawnOwner(); //주인을 찾아 보겠다.
	if (IsValid(pawn)) //주인을 찾았으면
	{
		Speed = pawn->GetVelocity().Size(); //속도값을 speed에 매 초 갱신
	}
}
