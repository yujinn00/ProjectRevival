// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PRAnimationAttackInterface.h"
#include "Interface/PRCharacterWidgetInterface.h"
#include "PRCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};

UENUM()
enum class EWeaponType : uint8
{
	Fist,
	Sword,
	Magic
};

UCLASS()
class PROJECTREVIVAL_API APRCharacterBase : public ACharacter, public IPRAnimationAttackInterface, public IPRCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APRCharacterBase();

// Engine Override Section.
public:
	// 초기화 함수.
	virtual void BeginPlay() override;

	// 업데이트 함수.
	virtual void Tick(float DeltaSeconds) override;

	// 데미지 처리 함수.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 컴포넌트 초기화 이후 호출 함수.
	virtual void PostInitializeComponents() override;

// Interface Override Section.
public:
	// 위젯 설정 함수.
	virtual void SetUpCharacterWidget(class UUserWidget* InUserWidget) override;

	// 공격 판정 함수 (AnimNotify_AttackHitCheck).
	virtual void AttackHitCheck() override;

// Character Control Section.
protected:
	// 캐릭터 컨트롤 데이터 설정 함수.
	virtual void SetCharacterControlData(const class UPRCharacterControlData* CharacterControlData);

// View Section.
protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UPRCharacterControlData*> CharacterControlManager;

// Roll Section.
protected:
	// Roll 몽타주 에셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollActionMontage;

// Weapon Section.
protected:
	// 캐릭터 소지 무기 목록.
	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, class UPRWeaponData*> WeaponInventory;

	// 현재 장착 무기 타입.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponType;

	// 현재 장착 무기 데이터.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPRWeaponData> CurrentWeaponData;

	// 현재 장착 무기 스켈레탈 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> EquippedWeaponMesh;

	// 무기 장착/교체 함수.
	void EquipWeapon(EWeaponType NewWeaponType);

// Combo Section.
protected:
	// 현재 콤보 단계.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	int32 CurrentCombo = 0;

	// 콤보 입력을 위한 타이머 핸들.
	FTimerHandle ComboTimerHandle;

	// 다음 콤보 입력 예약 플래그.
	bool HasNextComboCommand = false;

	// 콤보 중 회전 여부 플래그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsRotatingForCombo;

	// 콤보 회전 목표 값.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	FRotator ComboTargetRotation;

	// 콤보 회전 속도.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ComboInterpSpeed;

	// 콤보 공격 처리 함수.
	void ProcessComboCommand();

	// 콤보 시작 함수.
	void ComboActionBegin();

	// 콤보 종료 함수.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 콤보 타이머 설정 함수.
	void SetComboCheckTimer();

	// 다음 콤보 연계 확인 함수.
	void ComboCheck();

// Hit Stop Section.
protected:
	// 플레이어의 공격 히트 스톱 시 재생할 카메라 셰이크 클래스.
	UPROPERTY(EditAnywhere, Category = HitStop, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> HitStopCameraShake;

	// 플레이어의 공격 히트 스톱 지속 시간.
	UPROPERTY(EditAnywhere, Category = HitStop, meta = (AllowPrivateAccess = "true"))
	float HitStopDuration = 0.1f;

	// 플레이어의 공격 히트 스톱 타이머를 위한 핸들.
	FTimerHandle HitStopTimerHandle;

	// 플레이어의 공격 히트 스톱 효과를 종료하고, 원래 속도로 되돌리는 함수.
	void EndHitStop();

// Dead Section.
protected:
	// 죽음 몽타주 에셋.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	// 죽은 액터 제거 지연 시간.
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
