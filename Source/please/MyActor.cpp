// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH")); //MESH�� �ް�

	RootComponent = Mesh; //��Ģ

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Couch.SM_Couch'")); //�޽� �ϳ� ã�Ƽ� ��������

	if (SM.Succeeded()) //����������
	{
		Mesh->SetStaticMesh(SM.Object); //�޽� �����°� �Ҵ�
	}
}

// Called when the game starts or when spawned unityStart
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame  unityUpdate
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



