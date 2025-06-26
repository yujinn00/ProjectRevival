// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/PRCharacterStatComponent.h"

// Sets default values for this component's properties
UPRCharacterStatComponent::UPRCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UPRCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay()에서 SetHp(MaxHp)를 다시 호출하지 않으면,
	// 블루프린트에서 바꾼 MaxHp 값이 CurrentHp에 반영되지 않아 버그가 발생할 수 있음.
	// 즉, 아래 코드는 언리얼 객체 초기화 순서에 따른 안전장치 같은 역할임.
	SetHp(MaxHp);
}

float UPRCharacterStatComponent::ApplyDamage(float DamageAmount)
{
	// 기존 체력 값.
	const float PrevHp = CurrentHp;

	// 데미지 보정 값.
	// 잘못된 계산으로 음수 데미지가 전달되면 0으로 보정함.
	const float ActualDamage = FMath::Clamp<float>(DamageAmount, 0.0f, DamageAmount);

	// 데미지 계산.
	SetHp(PrevHp - ActualDamage);

	// 체력을 모두 소진했는지 확인.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// 이벤트 발행.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UPRCharacterStatComponent::SetHp(float NewHp)
{
	// 체력 보정 값.
	// 잘못된 계산으로 체력이 음수가 되면 0으로 보정함.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	// 브로드캐스트.
	OnHpChanged.Broadcast(CurrentHp);
	OnStatusBarHpChanged.Broadcast(CurrentHp, MaxHp);
}
