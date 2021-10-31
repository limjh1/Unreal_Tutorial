// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLEASE_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override; //컴포넌트 조립하고 여기다 실행

public:
	//레벨바뀔때
	void SetLevel(int32 Level);
	void SetHp(int32 NewHp); //hp를 피격시에만 관여하는게 아닌, 힐, 부활에도 관여해야함
	//피격할때
	void OnAttacked(float DamageAmount);


	//외부에서 사용할 일이 있을테니까
	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	float GetHpRatio() { return Hp / (float)MaxHp; }
	int32 GetAttack() { return Attack; }	

private:
	//현재 레벨,HP,공격력
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack;

public:
	FOnHpChanged OnHpChanged;
		
};
