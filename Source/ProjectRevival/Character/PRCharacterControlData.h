// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PRCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPRCharacterControlData();

	// 컨트롤러가 Yaw 회전에 관여할지.
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;

	// 이동 방향으로 자동 회전할지.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bOrientRotationToMovement : 1;

	// 컨트롤러 방향을 따라 움직일지.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bUseControllerDesiredRotation : 1;

	// 회전 속도.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// 사용될 입력 맵핑 컨텍스트.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// SpringArm 거리.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	// SpringArm 상대 회전값.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	// SpringArm 기준점 오프셋.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FVector RelativeLocation;

	// SpringArm 높낮이 오프셋.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FVector SocketOffset;

	// 카메라 회전 제어.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bUsePawnControlRotation : 1;

	// Pitch 회전을 부모로부터 상속할지.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritPitch : 1;

	// Yaw 회전을 부모로부터 상속할지.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritYaw : 1;

	// Roll 회전을 부모로부터 상속할지.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritRoll : 1;

	// SpringArm의 카메라 충돌 감지 여부.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bDoCollisionTest : 1;
};
