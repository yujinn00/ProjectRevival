// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PRMainGameMode.h"
#include "Engine/TargetPoint.h"
#include "Game/PRGameInstance.h"
#include "Kismet/GameplayStatics.h"

APRMainGameMode::APRMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ProjectRevival/Blueprint/BP_PRCharacterPlayer.BP_PRCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/ProjectRevival/PlayerController/BP_PRMainPlayerController.BP_PRMainPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void APRMainGameMode::EndGame(bool bIsClear)
{
	UPRGameInstance* GameInstance = Cast<UPRGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		// 게임 인스턴스에 클리어 여부와 최종 점수를 저장.
		GameInstance->bIsGameCleared = bIsClear;
		GameInstance->FinalScore = CurrentKillScore;
	}

	// 3초 뒤에 GoToResultLevel() 함수를 호출하도록 타이머를 설정.
	GetWorldTimerManager().SetTimer(EndGameTimerHandle, this, &APRMainGameMode::GoToResultLevel, 3.0f, false);
}

void APRMainGameMode::MonsterKilled()
{
	// 몬스터가 죽었다고 브로드캐스트.
	OnMonsterKilled.Broadcast();

	// 예외 처리.
	if (bHasBossSpawned)
	{
		return;
	}

	// 킬 스코어가 필요 조건에 도달했는지 확인.
	if (CurrentKillScore >= RequiredKillScoreForBoss)
	{
		// 보스 클래스와 스폰 위치가 모두 유효한지 확인.
		if (BossClassToSpawn && BossSpawnPoint)
		{
			FVector SpawnLocation = BossSpawnPoint->GetActorLocation();
			FRotator SpawnRotation = BossSpawnPoint->GetActorRotation();

			// 보스 스폰.
			GetWorld()->SpawnActor<APawn>(BossClassToSpawn, SpawnLocation, SpawnRotation);

			// 디버그 메시지 출력.
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WARNING !!"));

			// 플래그 설정.
			bHasBossSpawned = true;
		}
	}
}

void APRMainGameMode::GoToResultLevel()
{
	// 3초가 지나면, ResultLevel으로 레벨을 이동.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ResultLevel"));
}
