// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PRMainGameMode.generated.h"

// 몬스터의 죽음 신호를 위한 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterKilledDelegate);

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APRMainGameMode();

// End Section.
public:
	// 게임이 종료될 때 호출할 함수.
	void EndGame(bool bIsClear);

// Delegate Section.
public:
	// 플레이어가 이 신호를 바인딩할 수 있도록 public으로 선언.
	FOnMonsterKilledDelegate OnMonsterKilled;

	// 몬스터가 자신의 죽음을 보고할 때 호출할 함수.
	void MonsterKilled();

// Boss Section.
protected:
	// 에디터에서 스폰할 보스 몬스터의 블루프린트 지정.
	UPROPERTY(EditAnywhere, Category = Boss)
	TSubclassOf<class APawn> BossClassToSpawn;

	// 레벨 블루프린트에서 보스가 스폰될 Target Point 지정.
	UPROPERTY(BlueprintReadWrite, Category = Boss)
	TObjectPtr<class ATargetPoint> BossSpawnPoint;

	// 보스 스폰에 필요한 킬 Score.
	UPROPERTY(EditAnywhere, Category = Boss)
	int32 RequiredKillScoreForBoss = 1500;

	// 현재 게임의 누적 킬 스코어를 저장할 변수.
	int32 CurrentKillScore = 0;

	// 보스가 이미 스폰되었는지 확인하는 플래그.
	bool bHasBossSpawned = false;

// Result Section.
private:
	// 3초 뒤에 ResultLevel로 이동시키는 함수.
	void GoToResultLevel();

	// EndGame() 함수 지연 타이머를 위한 핸들.
	FTimerHandle EndGameTimerHandle;
};
