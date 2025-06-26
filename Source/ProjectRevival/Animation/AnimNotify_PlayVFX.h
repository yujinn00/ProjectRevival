// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayVFX.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UAnimNotify_PlayVFX : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 에디터에서 재생할 나이아가라 시스템을 선택할 변수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	TObjectPtr<class UNiagaraSystem> NiagaraSystem;

	// 이펙트가 붙을 소켓의 이름을 에디터에서 입력할 변수.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	FName SocketName;

protected:
	// 애니메이션 노티파이가 발생할 때 호출되는 이벤트 함수.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
