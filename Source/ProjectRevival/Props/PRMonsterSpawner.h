// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRMonsterSpawner.generated.h"

UCLASS()
class PROJECTREVIVAL_API APRMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APRMonsterSpawner();

protected:
	// 게임 시작 시 호출될 함수.
	virtual void BeginPlay() override;

	// 플레이어가 영역에 들어왔을 때 호출될 함수.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 스폰 로직을 실행하는 함수.
	void SpawnMonster();

private:
	// 플레이어 감지를 위한 트리거 볼륨.
	UPROPERTY(VisibleAnywhere, Category = Spawn)
	TObjectPtr<class UBoxComponent> TriggerVolume;

	// 에디터에서 스폰할 몬스터의 종류를 지정할 변수.
	UPROPERTY(EditAnywhere, Category = Spawn)
	TSubclassOf<class APawn> MonsterToSpawn;

	// 에디터에서 몬스터가 스폰될 위치들을 지정할 변수 (배열).
	UPROPERTY(EditAnywhere, Category = Spawn, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class ATargetPoint>> SpawnPoints;

	// 스폰이 한 번만 실행되도록 막는 플래그.
	bool bHasSpawned = false;
};
