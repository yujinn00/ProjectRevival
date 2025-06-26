// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/PRCharacterStat.h"
#include "PRCharacterStatComponent.generated.h"

// 체력 값이 0이 되었을 때 발행할 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// HpBar에서 체력 변경이 발생할 때 발행할 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

// StatusBar에서 체력 변경이 발생할 때 발행할 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusBarHpChangedDelegate, float, CurrentHp, float, MaxHp);

// 스탯 정보 변경이 발생할 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPRCharacterStat& /*BaseStat*/, const FPRCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREVIVAL_API UPRCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPRCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Getter.
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	// 부가 스탯 데이터 설정 함수.
	FORCEINLINE void SetModifierStat(const FPRCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	// 전체 스탯 데이터 반환 함수.
	FORCEINLINE FPRCharacterStat GetTotalStat() const
	{
		return BaseStat + ModifierStat;
	}

	// 기본 스탯 정보가 변경될 때 사용할 함수.
	FORCEINLINE void SetBaseStat(const FPRCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	FORCEINLINE void AddBaseStat(const FPRCharacterStat& InAddBaseStat)
	{
		BaseStat = BaseStat + InAddBaseStat;
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	FORCEINLINE const FPRCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPRCharacterStat& GetModifierStat() const { return ModifierStat; }

	// 데미지 전달 함수.
	float ApplyDamage(float DamageAmount);

protected:
	// HP가 변경됐을 때 실행할 함수.
	void SetHp(float NewHp);

// Delegate Section.
public:
	// 체력을 모두 소진했을 때 발행되는 델리게이트.
	FOnHpZeroDelegate OnHpZero;

	// HpBar 체력 변경 델리게이트.
	FOnHpChangedDelegate OnHpChanged;

	// StatusBar 체력 변경 델리게이트.
	FOnStatusBarHpChangedDelegate OnStatusBarHpChanged;

	// 스탯 변경 델리게이트.
	FOnStatChangedDelegate OnStatChanged;

// Stat Section.
protected:
	// 최대 체력 값.
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	// 현재 체력 값.
	// 현재 체력 값은 게임을 진행할 때마다 바뀌는 값임.
	// Transient: 디스크에 명시적으로 저장할 필요가 없음.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 캐릭터의 기본 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta=(AllowPrivateAccess=true))
	FPRCharacterStat BaseStat;

	// 부가 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta=(AllowPrivateAccess=true))
	FPRCharacterStat ModifierStat;
};
