// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PRMonsterAIInterface.h"
#include "Interface/PRAnimationAttackInterface.h"
#include "Interface/PRCharacterWidgetInterface.h"
#include "PRMonsterBase.generated.h"

UCLASS()
class PROJECTREVIVAL_API APRMonsterBase : public ACharacter, public IPRMonsterAIInterface, public IPRAnimationAttackInterface, public IPRCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APRMonsterBase();

	virtual void SetUpCharacterWidget(class UUserWidget* InUserWidget) override;

	// 공격 감지 함수.
	// AnimNotify_AttackHitCheck로부터 호출됨.
	virtual void AttackHitCheck() override;

	// 데미지 처리 함수.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 컴포넌트가 초기화된 이후에 호출되는 함수.
	virtual void PostInitializeComponents() override;

// AI Section.
protected:
	// SetAIAttackDelegate 함수로 전달된 델리게이트를 저장할 변수.
	FAIMonsterAttackFinished OnAttackFinished;

	// 몬스터의 추가적인 스탯을 반환하는 변수들.
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAIAttackRadius() override;
	virtual float GetAITurnSpeed() override;

	// 캐릭터에서 델리게이트를 넘길 때 사용할 함수.
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;

	// AI가 공격을 할 때 사용할 함수.
	virtual void AttackByAI() override;

// Attack Section.
protected:
	// 몬스터 공격 몽타주 에셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MonsterAttackActionMontage;

	virtual void NotifyAttackEnd(UAnimMontage* Montage, bool bInterrupted);

// Hit Stop Section.
protected:
	// 몬스터의 피격 히트 스톱 지속 시간.
	UPROPERTY(EditDefaultsOnly, Category = HitStop)
	float HitStopDuration = 0.1f;

	// 몬스터의 피격 히트 스톱 타이머를 위한 핸들.
	FTimerHandle HitStopTimerHandle;

	// 몬스터의 피격 히트 스톱 효과를 종료하고, 원래 속도로 되돌리는 함수.
	void EndHitStop();

// Dead Section.
protected:
	// 몬스터 죽음 몽타주 에셋.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MonsterDeadMontage;

	// 죽은 뒤에 액터를 제거하기 전까지 대기할 시간 값.
	float DeadEventDelayTime = 3.0f;

	// 죽음 상태 설정 함수.
	virtual void SetDead();

	// 죽는 애니메이션 재생 함수.
	void PlayDeadAnimation();

// Stat/Widget Section.
protected:
	// 스탯 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat/Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPRCharacterStatComponent> Stat;

	// 체력 위젯 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat/Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPRWidgetComponent> HpBar;
};
