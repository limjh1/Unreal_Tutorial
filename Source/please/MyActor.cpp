// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH")); //MESH로 받고

	RootComponent = Mesh; //규칙

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Couch.SM_Couch'")); //메쉬 하나 찾아서 가져오고

	if (SM.Succeeded()) //성공했으면
	{
		Mesh->SetStaticMesh(SM.Object); //메쉬 가져온거 할당
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



