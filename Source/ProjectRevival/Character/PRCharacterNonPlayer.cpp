// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PRCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PRCollision.h"
#include "Game/PRMainGameMode.h"
#include "Kismet/GameplayStatics.h"

APRCharacterNonPlayer::APRCharacterNonPlayer()
{
	// NPC는 플레이어와 같은 플레이어 콜리전을 사용해야 하지만, 현재는 테스트를 위해 임시로 몬스터 콜리전으로 설정.
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PRMONSTERCOLLISION);
}

void APRCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 타이머를 사용해 액터 제거.
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				// 액터 제거.
				Destroy();
			}
		),
		DeadEventDelayTime, // 타이머 설정 시간.
		false // 반복 여부 설정 (반복 안함).
	);

	// @Todo: 추후 PRMonsterBase로 옮겨서 완성할 예정.
	APRMainGameMode* GameMode = Cast<APRMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// if (this->IsBoss())
		// {
			// 죽은 몬스터가 보스라면, 게임 클리어.
			GameMode->EndGame(true);
		// }
		// else
		// {
		// 	// 죽은 몬스터가 일반 몬스터라면, 계속 진행.
		// 	GameMode->OnMonsterKilled(this);
		// }
	}
}
