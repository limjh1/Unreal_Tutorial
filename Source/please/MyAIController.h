// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PLEASE_API AMyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AMyAIController(); //기본적으로 생성자 파주기

	virtual void OnPossess(APawn* InPawn) override; //폰 클래스 빙의를 할 때 호출되는 함수
	virtual void OnUnPossess() override; // 빙의가 끝날 때 호출되는 함수
	
private:
	void RandomMove();

private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
