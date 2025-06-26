// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SkillHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UAnimNotify_SkillHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 호출할 스킬 이벤트를 식별하는 고유 태그.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	FName EventTag;

protected:
	// 애니메이션 노티파이가 발생할 때 호출되는 이벤트 함수.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
