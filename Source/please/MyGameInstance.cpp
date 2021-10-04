// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	//처음 게임인스턴스가 만들어지면, 에셋불러오기
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	//불러와지면 할당	
	MyStats = DATA.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->Attack);
}

FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	//1,2,3,4같은 스텟데이터를 레벨로 찾아서 fname으로 만들어 준다음 찾늕거
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level),TEXT(""));	
}
