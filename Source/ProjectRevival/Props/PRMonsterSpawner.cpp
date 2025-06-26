// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PRMonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "Character/PRCharacterPlayer.h"

// Sets default values
APRMonsterSpawner::APRMonsterSpawner()
{
	// 트리거 볼륨 생성 및 루트로 설정.
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	// 트리거 볼륨의 콜리전 설정.
	TriggerVolume->SetCollisionProfileName(TEXT("PRTrigger"));
	TriggerVolume->SetBoxExtent(FVector(1000.0f, 1000.0f, 1000.0f));
}

void APRMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 영역에 들어오는 이벤트를 감지하도록 바인딩.
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APRMonsterSpawner::OnOverlapBegin);
}

void APRMonsterSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 예외 처리.
	if (bHasSpawned)
	{
		return;
	}

	// 영역에 들어온 액터가 플레이어인지 확인.
	APRCharacterPlayer* PlayerCharacter = Cast<APRCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		SpawnMonster();
		bHasSpawned = true;
	}
}

void APRMonsterSpawner::SpawnMonster()
{
	// 예외 처리.
	if (!MonsterToSpawn || SpawnPoints.Num() == 0)
	{
		return;
	}

	// 지정된 모든 스폰 위치를 순회하며 몬스터를 스폰.
	for (ATargetPoint* Point : SpawnPoints)
	{
		if (Point)
		{
			// 타겟 포인트의 위치와 회전값을 가져옴.
			FVector SpawnLocation = Point->GetActorLocation();
			FRotator SpawnRotation = Point->GetActorRotation();

			// 월드에 몬스터 스폰.
			GetWorld()->SpawnActor<APawn>(MonsterToSpawn, SpawnLocation, SpawnRotation);
		}
	}
}
