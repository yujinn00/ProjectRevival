// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PRCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/PRAnimationSkillInterface.h"
#include "PRCharacterPlayer.generated.h"

// 무기 변경 신호를 위한 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponTypeChangedDelegate, const class UPRWeaponData*, NewWeaponData, EWeaponType, NewWeaponType);

// 모든 행동의 쿨타임 처리를 위한 범용 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionCooldownStartedDelegate, FName, ActionTag, float, CooldownDuration);

// 몬스터 킬 스코어 변경 신호를 위한 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillScoreChangedDelegate, int32, NewKillScore);

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRCharacterPlayer : public APRCharacterBase, public IPRAnimationSkillInterface
{
	GENERATED_BODY()

public:
	APRCharacterPlayer();

// Engine Override Section.
public:
	// 초기화 함수.
	virtual void BeginPlay() override;

	// 입력 컴포넌트 설정 함수.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Interface Override Section.
public:
	// 위젯 설정 함수.
	virtual void SetUpCharacterWidget(class UUserWidget* InUserWidget) override;

	// 스킬 판정 함수 (AnimNotify_SkillHitCheck).
	virtual void SkillHitCheck(FName EventTag) override;

// Character Control Section.
public:
	// 캐릭터 컨트롤 타입 변경 함수.
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	// 캐릭터 컨트롤 데이터 설정 함수.
	virtual void SetCharacterControlData(const class UPRCharacterControlData* CharacterControlData) override;

// Delegate Section.
public:
	// 캐릭터의 무기가 변경되었음을 UI에 알리는 신호.
	FOnWeaponTypeChangedDelegate OnWeaponTypeChanged;

	// 어떠한 행동의 쿨타임이 시작되었음을 UI에 알리는 신호.
	FOnActionCooldownStartedDelegate OnActionCooldownStarted;

	// 몬스터의 킬 스코어가 변경되었음을 UI에 알리는 신호.
	FOnKillScoreChangedDelegate OnKillScoreChanged;

// Camera Section.
protected:
	// 스프링암 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	// 카메라 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

// View Section.
protected:
	// 현재 사용 중인 캐릭터 시점.
	ECharacterControlType CurrentCharacterControlType;

// Start Section.
protected:
	// 에디터에서 지정할 시작 애니메이션 몽타주.
	UPROPERTY(EditAnywhere, Category = Start, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StartMontage;

	// 시작 애니메이션이 종료되었을 때 호출될 함수.
	UFUNCTION()
	void OnStartAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

// Input Section.
protected:
	// 캐릭터 시점 변경 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;
	// 슐더뷰 시점 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;
	// 슐더뷰 이동 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;
	// 쿼터뷰 이동 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuarterMoveAction;
	// 점프 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	// 걷기 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WalkAction;
	// 달리기 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;
	// 구르기 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;
	// 공격 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	// 스킬 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;
	// 1번 슬롯 장착 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipSlot1Action;
	// 2번 슬롯 장착 액션.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipSlot2Action;

	// 캐릭터 시점 변경 함수.
	void ChangeCharacterControl();
	// 슐더뷰 시점 함수.
	void ShoulderLook(const FInputActionValue& Value);
	// 숄더뷰 이동 함수.
	void ShoulderMove(const FInputActionValue& Value);
	// 쿼터뷰 이동 함수.
	void QuarterMove(const FInputActionValue& Value);
	// 걷기 시작 함수.
	void StartWalk();
	// 달리기 시작 함수.
	void StartRun();
	// 걷기/달리기 종료 함수.
	void StopWalkRun();
	// 구르기 시작 함수.
	void Roll();
	// 공격 함수.
	void Attack();
	// 스킬 함수.
	void Skill();
	// 주먹 스킬 함수.
	void FistSkill();
	// 검 스킬 함수.
	void SwordSkill();
	// 마법 스킬 함수.
	void MagicSkill();
	// 1번 슬롯 장착 함수.
	void EquipSlot1();
	// 2번 슬롯 장착 함수.
	void EquipSlot2();

// Roll/Skill Section.
protected:
	// 구르기 쿨타임 타이머 핸들.
	FTimerHandle RollTimerHandle;

	// 무기 타입별 쿨타임 상태를 저장하는 맵 (플래그).
	TMap<EWeaponType, bool> SkillCooldownFlags;

	// 무기 타입별 쿨타임 타이머 핸들을 저장하는 맵 (타이머).
	TMap<EWeaponType, FTimerHandle> SkillCooldownTimerHandles;

	// 구르기 쿨타임이 종료되었을 때 호출될 함수.
	void OnRollCooldownFinished();

	// 스킬의 몽타주가 종료되었을 때 호출될 함수.
	UFUNCTION()
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 주먹 스킬 쿨타임이 종료되었을 때 호출될 함수.
	void OnFistSkillCooldownFinished();

	// 검 스킬 쿨타임이 종료되었을 때 호출될 함수.
	void OnSwordSkillCooldownFinished();

// Grenade Section.
protected:
	// 수류탄 클래스.
	UPROPERTY(EditAnywhere, Category = Grenade)
	TSubclassOf<class APRGrenade> GrenadeClass;

// Dead Section.
public:
	// 플레이어 사망 처리 함수.
	virtual void SetDead() override;

// Widget Section.
private:
	// 생성할 StatusBar 위젯의 블루프린트 클래스.
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPRStatusBarWidget> StatusBarWidgetClass;

	// 생성된 StatusBar 위젯의 인스턴스를 저장할 변수.
	UPROPERTY()
	TObjectPtr<class UPRStatusBarWidget> StatusBarWidget;

	// 생성할 HotBar 위젯의 블루프린트 클래스.
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPRHotBarWidget> HotBarWidgetClass;

	// 생성된 HotBar 위젯의 인스턴스를 저장할 변수.
	UPROPERTY()
	TObjectPtr<class UPRHotBarWidget> HotBarWidget;

	// 생성할 ScoreBar 위젯의 블루프린트 클래스.
	UPROPERTY(EditAnywhere, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPRScoreBarWidget> ScoreBarWidgetClass;
	
	// 생성된 ScoreBar 위젯의 인스턴스를 저장할 변수.
	UPROPERTY()
	TObjectPtr<class UPRScoreBarWidget> ScoreBarWidget;

// Score Section.
private:
	// 게임 모드로부터 몬스터가 죽었다는 신호를 받았을 때 호출될 함수.
	// 몬스터가 죽을 때 호출하여 킬 스코어를 증가시키는 함수.
	UFUNCTION()
	void IncrementKillScore();

// State Check Section.
private:
	// 시작 상태 플래그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flag, meta = (AllowPrivateAccess = "true"))
	bool bIsStarting = false;

	// 구르기 상태 플래그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flag, meta = (AllowPrivateAccess = "true"))
	bool bIsRolling = false;

	// 스킬 상태 플래그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flag, meta = (AllowPrivateAccess = "true"))
	bool bIsUsingSkill = false;

	// 점프 가능 여부를 판단하는 함수.
	virtual bool CanJumpInternal_Implementation() const override;
};
