// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	//ó�� �����ν��Ͻ��� ���������, ���ºҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	//�ҷ������� �Ҵ�	
	MyStats = DATA.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->Attack);
}

FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	//1,2,3,4���� ���ݵ����͸� ������ ã�Ƽ� fname���� ����� �ش��� ã�d��
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level),TEXT(""));	
}
