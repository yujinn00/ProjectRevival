// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PRCharacterBase.h"
#include "PRCharacterControlData.h"
#include "Weapon/PRWeaponData.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "PRAttackComboActionData.h"
#include "Physics/PRCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/PRCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/PRWidgetComponent.h"
#include "UI/PRHpBarWidget.h"

// Sets default values
APRCharacterBase::APRCharacterBase()
{
	// 컨트롤러의 회전을 받아서 설정하는 모드를 모두 해제.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 무브먼트 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PRPLAYERCOLLISION);

	// 메시 설정.
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 스켈레탈 메시 설정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// 애님 인스턴스 설정.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectRevival/Animation/Player/ABP_PRCharacter.ABP_PRCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// 숄더뷰 설정.
	static ConstructorHelpers::FObjectFinder<UPRCharacterControlData> ShoulderDataRef(TEXT("/Game/ProjectRevival/CharacterControl/PRC_Shoulder.PRC_Shoulder"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	// 쿼터뷰 설정.
	static ConstructorHelpers::FObjectFinder<UPRCharacterControlData> QuarterDataRef(TEXT("/Game/ProjectRevival/CharacterControl/PRC_Quarter.PRC_Quarter"));
	if (QuarterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

	// Roll 몽타주 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollActionMontageRef(TEXT("/Game/ProjectRevival/Animation/Player/Roll/AM_Roll.AM_Roll"));
	if (RollActionMontageRef.Object)
	{
		RollActionMontage = RollActionMontageRef.Object;
	}

	// 장착할 무기를 위한 스켈레탈 메시 컴포넌트 생성 및 부착.
	EquippedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquippedWeapon"));
	EquippedWeaponMesh->SetupAttachment(GetMesh(), FName("Weapon_Socket"));
	EquippedWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Fist 데이터 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UPRWeaponData> UnarmedDataRef(TEXT("/Game/ProjectRevival/WeaponData/DA_Fist.DA_Fist"));
	if (UnarmedDataRef.Object)
	{
		WeaponInventory.Add(EWeaponType::Fist, UnarmedDataRef.Object);
	}

	// Sword 데이터 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UPRWeaponData> SwordDataRef(TEXT("/Game/ProjectRevival/WeaponData/DA_Sword.DA_Sword"));
	if (SwordDataRef.Object)
	{
		WeaponInventory.Add(EWeaponType::Sword, SwordDataRef.Object);
	}

	// 카메라 셰이크 클래스 설정.
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> HitStopCameraShakeRef(TEXT("/Game/ProjectRevival/Props/BP_HitStopShake.BP_HitStopShake_C"));
	if (HitStopCameraShakeRef.Succeeded())
	{
		HitStopCameraShake = HitStopCameraShakeRef.Class;
	}

	// Dead 몽타주 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/ProjectRevival/Animation/Player/Dead/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// 스탯 컴포넌트 설정.
	Stat = CreateDefaultSubobject<UPRCharacterStatComponent>(TEXT("Stat"));

	// 위젯 컴포넌트 설정.
	HpBar = CreateDefaultSubobject<UPRWidgetComponent>(TEXT("Widget"));

	// 컴포넌트 계층 설정 및 상대 위치 설정.
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	// 사용할 위젯 클래스 정보 설정.
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ProjectRevival/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		// 위젯 컴포넌트는 위젯의 클래스 정보를 바탕으로 자체적으로 인스턴스를 생성함.
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		
		// 2D 모드로 그리기.
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);

		// 크기 설정.
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));

		// 콜리전 끄기.
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 콤보 공격 간 자연스러운 회전을 위한 설정.  
		bIsRotatingForCombo = false;
		ComboInterpSpeed = 15.0f;
	}
}

void APRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 비무장(주먹) 상태로 시작.
	EquipWeapon(EWeaponType::Fist);
}

void APRCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsRotatingForCombo)
	{
		// 현재 회전 값에서 목표 회전 값으로 DeltaTime에 맞춰 부드럽게 회전.
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), ComboTargetRotation, DeltaSeconds, ComboInterpSpeed);
		Super::SetActorRotation(NewRotation);

		// 목표 회전 값에 거의 도달했다면 회전을 멈춤.
		if (GetActorRotation().Equals(ComboTargetRotation, 1.0f))
		{
			bIsRotatingForCombo = false;
		}
	}
}

float APRCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 스탯 정보가 업데이트 되도록 데미지 전달.
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void APRCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 죽었을 때 발행되는 이벤트에 SetDead 함수 등록.
	Stat->OnHpZero.AddUObject(this, &APRCharacterBase::SetDead);
}

void APRCharacterBase::SetUpCharacterWidget(class UUserWidget* InUserWidget)
{
	// 필요한 위젯 정보 가져오기.
	UPRHpBarWidget* HpBarWidget = Cast<UPRHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		if (Stat)
		{
			// 최대 체력 값 설정.
			HpBarWidget->SetMaxHp(Stat->GetMaxHp());

			// HP 퍼센트가 제대로 계산되도록 현재 체력 설정.
			HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());

			// 체력 변경 이벤트(델리게이트)에 함수 및 객체 정보 등록.
			Stat->OnHpChanged.AddUObject(HpBarWidget, &UPRHpBarWidget::UpdateHpBar);
		}
	}
}

void APRCharacterBase::AttackHitCheck()
{
	// 예외 처리.
	if (!CurrentWeaponData)
	{
		return;
	}

	// 공격 사거리.
	const float AttackRange = CurrentWeaponData->AttackRange;

	// 충돌 시작 지점 계산.
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f;
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// SCENE_QUERY_STAT: 언리얼에서 지원하는 분석 툴에 태그를 추가하는 매크로.
	// 두 번째 인자: 복잡한 형태의 충돌체를 감지할지 여부.
	// 세 번째 인자: 무시할 액터 목록.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 트레이스에 사용할 구체의 반지름.
	const float AttackRadius = CurrentWeaponData->AttackRadius;

	// 트레이스를 활용해 충돌 검사.
	TArray<FHitResult> OutHitResults;
	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_PRPLAYERACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 충돌 감지된 경우의 처리.
	if (HitDetected)
	{
		// 데미지 수치.
		const float AttackDamage = CurrentWeaponData->AttackDamage;

		// 데미지 이벤트.
		FDamageEvent DamageEvent;

		// 데미지 전달.
		for (const FHitResult& HitResult : OutHitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
		}

		// 현재 무기가 칼일 때만 히트 스톱을 적용.
		if (CurrentWeaponType == EWeaponType::Sword)
		{
			// 0.05초의 히트 스톱 설정.
			CustomTimeDilation = 0.05f;
			GetWorldTimerManager().SetTimer(
				HitStopTimerHandle,
				this,
				&APRCharacterBase::EndHitStop,
				HitStopDuration,
				false
			);

			// 카메라 셰이크 설정.
			APlayerController* PC = GetController<APlayerController>();
			if (PC && HitStopCameraShake)
			{
				PC->ClientStartCameraShake(HitStopCameraShake);
			}
		}

		// 디버그 메시지 출력.
		FString DebugMessage = FString::Printf(TEXT("Damage: %f, Range: %f, Radius: %f"), AttackDamage, AttackRange, AttackRadius);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DebugMessage);
	}

	// 충돌 디버그 (시각적으로 확인할 수 있도록).
#if ENABLE_DRAW_DEBUG
	// 캡슐의 중심 위치.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	//캡슐 높이 절반 값.
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// 표시할 색상 (안 맞으면 초록, 맞으면 빨강).
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	// 캡슐 그리기.
	DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		AttackRadius,
		FRotationMatrix::MakeFromX(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		5.0f
	);
#endif
}

void APRCharacterBase::SetCharacterControlData(const class UPRCharacterControlData* CharacterControlData)
{
	// Pawn.
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// Movement.
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void APRCharacterBase::EquipWeapon(EWeaponType NewWeaponType)
{
	// 무기 인벤토리에 없는 타입이면 예외 처리.
	if (!WeaponInventory.Contains(NewWeaponType))
	{
		return;
	}

	// 이미 같은 무기를 장착하고 있다면 중복 실행 방지.
	if (CurrentWeaponType == NewWeaponType && CurrentWeaponData)
	{
		return;
	}

	// 캐릭터의 현재 무기 정보를 새로운 무기 데이터로 갱신.
	CurrentWeaponData = WeaponInventory[NewWeaponType];
	CurrentWeaponType = NewWeaponType;

	// 무기 교체 시, 진행 중이던 콤보 상태를 모두 초기화.
	CurrentCombo = 0;
	HasNextComboCommand = false;
	GetWorldTimerManager().ClearTimer(ComboTimerHandle);

	// 새로운 무기 데이터에 스켈레탈 메시가 있는지 확인.
	if (CurrentWeaponData && CurrentWeaponData->WeaponMesh)
	{
		// 있다면, 스켈레탈 메시 컴포넌트에 적용하고 보이도록 설정.
		EquippedWeaponMesh->SetSkeletalMesh(CurrentWeaponData->WeaponMesh);
		EquippedWeaponMesh->SetHiddenInGame(false);
	}
	else
	{
		// 없다면, 스켈레탈 메시 컴포넌트를 비우고 숨기도록 설정.
		EquippedWeaponMesh->SetSkeletalMesh(nullptr);
		EquippedWeaponMesh->SetHiddenInGame(true);
	}
}

void APRCharacterBase::ProcessComboCommand()
{
	// 예외 처리.
	if (!CurrentWeaponData) return;

	// 현재 재생 중인 콤보 확인.
        if (CurrentCombo == 0)
        {
        	// 콤보 액션 시작 처리 후 종료.
        	ComboActionBegin();
        	return;
        }

		// 콤보가 진행 중일 때는 해당 프레임에 맞는 타이머를 활용.
		// 이미 콤보 발동이 걸렸거나 타이밍을 놓친 경우,
		// 즉, 콤보 타이머 핸들이 유효하지 않은 경우,
		// 이러한 경우에는 콤보 처리가 필요하지 않음.
        if (!ComboTimerHandle.IsValid())
        {
            HasNextComboCommand = false;
        }
        else
        {
            HasNextComboCommand = true;
        }
}

void APRCharacterBase::ComboActionBegin()
{
	// 예외 처리.
	if (!CurrentWeaponData || !CurrentWeaponData->AttackMontage) return;

	// 콤보 상태를 1로 설정.
	CurrentCombo = 1;

	// 부드러운 회전 설정.
	ComboTargetRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	bIsRotatingForCombo = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ComboActionBegin() is called"));
		AnimInstance->Montage_Play(CurrentWeaponData->AttackMontage, 1.0f);

		// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록.
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APRCharacterBase::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeaponData->AttackMontage);

		// 콤보 확인을 위한 타이머 설정.
		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}
}

void APRCharacterBase::ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 유효성 검사.
	ensure(CurrentCombo != 0);

	// 콤보 초기화.
	CurrentCombo = 0;

	// 콤보 확인용 타이머도 중지해서 SwordComboCheck() 함수가 실행되지 않도록 클리어.
	// 이건 왜 하는가 => 이걸 하지 않으면 이전에 예약된 SwordComboCheck 타이머가 여전히 실행되어,
	// 콤보가 종료되었음에도 불구하고 CurrentSwordCombo가 다시 1로 증가해버림.
	// 즉, 구르기 등으로 콤보가 강제 종료되었을 때 정확하게 콤보를 끊기 위해 타이머를 반드시 취소해야 함.
	GetWorldTimerManager().ClearTimer(ComboTimerHandle);
}

void APRCharacterBase::SetComboCheckTimer()
{
	// 예외 처리.
	if (!CurrentWeaponData || !CurrentWeaponData->AttackComboData)
	{
		return;
	}

	// 현재 재생 중인 콤보의 인덱스.
	int32 ComboIndex = CurrentCombo - 1;

	// 예외 처리.
	if (!CurrentWeaponData->AttackComboData->EffectiveFrameCount.IsValidIndex(ComboIndex))
	{
		return;
	}

	// 콤보 시간 계산.
	float ComboEffectiveTime = (CurrentWeaponData->AttackComboData->EffectiveFrameCount[ComboIndex] / CurrentWeaponData->AttackComboData->FrameRate) / 1.0f;

	// 타이머 설정.
	if (ComboEffectiveTime > 0.0f)
	{
		// 첫 번째: 설정할 타이머 핸들.
		// 두 번째: 타이머에 설정한 시간이 모두 지났을 때 실행될 함수의 주인.
		// 세 번째: 타이머에 연동해 실행할 함수 포인터.
		// 네 번째: 타이머 시간.
		// 다섯 번째: 반복 여부.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APRCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void APRCharacterBase::ComboCheck()
{
	// 타이머 핸들 초기화.
	ComboTimerHandle.Invalidate();

	// 이전에 공격 입력이 들어왔는지 확인.
	if (HasNextComboCommand)
	{
		// 예외 처리.
		if (!CurrentWeaponData || !CurrentWeaponData->AttackMontage || !CurrentWeaponData->AttackComboData)
		{
			return;
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ComboCheck() is called"));

			// 부드러운 회전 설정.
			ComboTargetRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
			bIsRotatingForCombo = true;

			// 다음 콤보 인덱스 설정.
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentWeaponData->AttackComboData->MaxComboCount);

			// 점프할 섹션의 이름 설정.
			FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentWeaponData->AttackComboData->MontageSectionNamePrefix, CurrentCombo);

			// 섹션 점프.
			AnimInstance->Montage_JumpToSection(NextSection, CurrentWeaponData->AttackMontage);

			// 다음 콤보 공격을 위한 타이머 설정.
			SetComboCheckTimer();

			// 콤보 공격 입력 플래그 초기화.
			HasNextComboCommand = false;
		}
	}
}

void APRCharacterBase::EndHitStop()
{
	// 플레이어의 시간 흐름을 원래 속도로 되돌림.
	CustomTimeDilation = 1.0f;
}

void APRCharacterBase::SetDead()
{
	// 무브먼트 컴포넌트 끄기.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 콜리전 끄기.
	SetActorEnableCollision(false);

	// 죽는 애니메이션 재생.
	PlayDeadAnimation();

	// 죽었을 때 HpBar 사라지도록 처리.
	HpBar->SetHiddenInGame(true);
}

void APRCharacterBase::PlayDeadAnimation()
{
	// 몽타주 재생.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 이미 재생 중인 몽타주가 있다면, 모두 종료.
		AnimInstance->StopAllMontages(0.0f);

		// 죽음 몽타주 재생.
		constexpr float PlayRate = 1.0f;
		AnimInstance->Montage_Play(DeadMontage, PlayRate);
	}
}
