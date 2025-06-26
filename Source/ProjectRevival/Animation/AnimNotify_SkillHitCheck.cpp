// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SkillHitCheck.h"
#include "Interface/PRAnimationSkillInterface.h"

void UAnimNotify_SkillHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		// 애니메이션을 소유하는 액터가 인터페이스를 구현했는지 확인.
		IPRAnimationSkillInterface* SkillInterface = Cast<IPRAnimationSkillInterface>(MeshComp->GetOwner());

		// 인터페이스를 구현한 경우, 함수 호출.
		if (SkillInterface)
		{
			SkillInterface->SkillHitCheck(EventTag);
		}
	}
}
