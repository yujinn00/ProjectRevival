// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_PlayVFX.h"
#include "NiagaraFunctionLibrary.h"

void UAnimNotify_PlayVFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 예외 처리.
	if (!NiagaraSystem) return;

	// 메시 컴포넌트의 지정된 소켓에 이펙트 생성.
	if (MeshComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			MeshComp,
			SocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
}
