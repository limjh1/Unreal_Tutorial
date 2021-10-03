// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON")); //스태틱메쉬컴포넌트 만들어주고
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
	if (SW.Succeeded()) //잘 불러왔으면
	{
		Weapon->SetStaticMesh(SW.Object); //무기 추가, 매쉬 로드해서
	}

	Weapon->SetupAttachment(RootComponent);//루트를 따라가고
	Trigger->SetupAttachment(Weapon); //웨폰을 따라가고 , 계층구조임
									  //,이거 안하면 무기가 이리저리 움직여도 trigger는 따라서 이동 안할  수도있어서 붙임
		
	Weapon->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetBoxExtent(FVector(30.f, 30.f, 30.f));

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

void AMyWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnCharacterOverlap);
}

void AMyWeapon::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OverLapped"));

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor); // 마이 캐릭터가 잘 들어오면 NULL이 아님
	if (MyCharacter) //NULL 판별
	{
		FName WeaponSocket(TEXT("hand_l_socket"));

		AttachToComponent(MyCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
	}
}
