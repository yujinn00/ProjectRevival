#pragma once

#include "CoreMinimal.h"

// 공격용 트레이스 채널 정의
#define CPROFILE_PRPLAYERCOLLISION TEXT("PRPlayerCollision")
#define CPROFILE_PRMONSTERCOLLISION TEXT("PRMonsterCollision")

// 콜리전 프리셋 정의 
#define CCHANNEL_PRPLAYERACTION ECC_GameTraceChannel1
#define CCHANNEL_PRMONSTERACTION ECC_GameTraceChannel2
#define CPROFILE_PRTRIGGER TEXT("PRTrigger")
