// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PRWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

// Data Section.
public:
	// 에디터에서 할당할 각 무기의 스켈레탈 메시 (주먹의 경우 nullptr).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Data")
	TObjectPtr<class USkeletalMesh> WeaponMesh;

	// 에디터에서 할당할 각 무기의 콤보 공격 몽타주.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Data")
	TObjectPtr<class UAnimMontage> AttackMontage;

	// 에디터에서 할당할 각 무기의 콤보 액션 데이터.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Data")
	TObjectPtr<class UPRAttackComboActionData> AttackComboData;

	// 에디터에서 할당할 각 무기의 스킬 몽타주.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Data")
	TObjectPtr<class UAnimMontage> SkillMontage;

// UI Section.
public:
	// 에디터에서 할당할 스킬 슬롯에 표시될 아이콘.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-UI")
	TObjectPtr<class UTexture2D> SkillIcon;

	// 에디터에서 할당할 각 무기의 스킬 쿨타임.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-UI")
	float SkillCooldown;

// Stat Section.
public:
	// 에디터에서 할당할 각 무기의 공격 데미지.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Stat")
	float AttackDamage;

	// 에디터에서 할당할 각 무기의 공격 사거리.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Stat")
	float AttackRange;

	// 에디터에서 할당할 각 무기의 공격 판정 반지름.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon-Stat")
	float AttackRadius;
};
