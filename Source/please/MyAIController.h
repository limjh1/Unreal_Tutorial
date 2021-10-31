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
	AMyAIController(); //�⺻������ ������ ���ֱ�

	virtual void OnPossess(APawn* InPawn) override; //�� Ŭ���� ���Ǹ� �� �� ȣ��Ǵ� �Լ�
	virtual void OnUnPossess() override; // ���ǰ� ���� �� ȣ��Ǵ� �Լ�
	
private:
	void RandomMove();

private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
