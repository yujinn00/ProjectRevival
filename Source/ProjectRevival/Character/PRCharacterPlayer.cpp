// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PRCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PRCharacterControlData.h"
#include "CharacterStat/PRCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/PRWeaponData.h"
#include "Engine/DamageEvents.h"
#include "Physics/PRCollision.h"
#include "Engine/OverlapResult.h"
#include "Props/PRGrenade.h"
#include "UI/PRWidgetComponent.h"
#include "UI/PRStatusBarWidget.h"
#include "UI/PRHotBarWidget.h"
#include "UI/PRScoreBarWidget.h"
#include "Game/PRMainGameMode.h"
#include "Kismet/GameplayStatics.h"

APRCharacterPlayer::APRCharacterPlayer()
{
	// SpringArm.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Camera.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 플레이어의 체력 표시를 StatusBar로 대체하기 위해 기존 HPBar는 비활성화.
	HpBar->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StartMontageRef(TEXT("/Game/ProjectRevival/Animation/Player/Start/AM_Start.AM_Start"));
	if (StartMontageRef.Object)
	{
		StartMontage = StartMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlJumpRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (InputChangeActionControlJumpRef.Object)
	{
		ChangeControlAction = InputChangeActionControlJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuarterMoveRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_QuarterMove.IA_QuarterMove"));
	if (InputActionQuarterMoveRef.Object)
	{
		QuarterMoveAction = InputActionQuarterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Jump.IA_Jump"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWalkRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Walk.IA_Walk"));
	if (InputActionWalkRef.Object)
	{
		WalkAction = InputActionWalkRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Run.IA_Run"));
	if (InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Roll.IA_Roll"));
	if (InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SkillActionRef(TEXT("/Game/ProjectRevival/Input/Actions/IA_Skill.IA_Skill"));
	if (SkillActionRef.Object)
	{
		SkillAction = SkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EquipSlot1Ref(TEXT("/Game/ProjectRevival/Input/Actions/IA_EquipSlot1.IA_EquipSlot1"));
	if (EquipSlot1Ref.Object)
	{
		EquipSlot1Action = EquipSlot1Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EquipSlot2Ref(TEXT("/Game/ProjectRevival/Input/Actions/IA_EquipSlot2.IA_EquipSlot2"));
	if (EquipSlot2Ref.Object)
	{
		EquipSlot2Action = EquipSlot2Ref.Object;
	}

	static ConstructorHelpers::FClassFinder<APRGrenade> GrenadeClassRef(TEXT("/Game/ProjectRevival/Props/BP_PRGrenade.BP_PRGrenade_C"));
	if (GrenadeClassRef.Succeeded())
	{
		GrenadeClass = GrenadeClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPRStatusBarWidget> StatusBarWidgetClassRef(TEXT("/Game/ProjectRevival/UI/WBP_StatusBar.WBP_StatusBar_C"));
	if (StatusBarWidgetClassRef.Succeeded())
	{
		StatusBarWidgetClass = StatusBarWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPRHotBarWidget> HotBarWidgetClassRef(TEXT("/Game/ProjectRevival/UI/WBP_HotBar.WBP_HotBar_C"));
	if (HotBarWidgetClassRef.Succeeded())
	{
		HotBarWidgetClass = HotBarWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPRScoreBarWidget> ScoreBarWidgetClassRef(TEXT("/Game/ProjectRevival/UI/WBP_ScoreBar.WBP_ScoreBar_C"));
	if (ScoreBarWidgetClassRef.Succeeded())
	{
		ScoreBarWidgetClass = ScoreBarWidgetClassRef.Class;
	}

	// 숄더뷰 초기 설정.
	CurrentCharacterControlType = ECharacterControlType::Shoulder;
}

void APRCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 플래그 설정.
		bIsStarting = true;

		// 시작 몽타주 재생.
		constexpr float PlayRate = 1.0f;
		AnimInstance->Montage_Play(StartMontage, PlayRate);

		// 몽타주 재생이 끝나면 OnSpawnAnimationEnded() 함수를 호출하도록 예약.
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUObject(this, &APRCharacterPlayer::OnStartAnimationEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, StartMontage);
	}

	// 이 캐릭터를 로컬 플레이어가 조종하는지 확인.
	if (IsLocallyControlled())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// 입력 활성화.
			EnableInput(PlayerController);

			if (PlayerController->PlayerCameraManager)
			{
				// 최소 각도 설정.
				PlayerController->PlayerCameraManager->ViewPitchMin = -60.0f;

				// 최대 각도 설정.
				PlayerController->PlayerCameraManager->ViewPitchMax = 40.0f;
			}

			// StatusBar 위젯 생성 및 뷰포트 추가.
			if (StatusBarWidgetClass)
			{
				StatusBarWidget = CreateWidget<UPRStatusBarWidget>(PlayerController, StatusBarWidgetClass);
				if (StatusBarWidget)
				{
					StatusBarWidget->AddToViewport();
				}
			}

			// HotBar 위젯 생성 및 뷰포트 추가.
			if (HotBarWidgetClass)
			{
				HotBarWidget = CreateWidget<UPRHotBarWidget>(PlayerController, HotBarWidgetClass);
				if (HotBarWidget)
				{
					HotBarWidget->AddToViewport();
				}
			}

			// ScoreBar 위젯 생성 및 뷰포트 추가.
			if (ScoreBarWidgetClass)
			{
				ScoreBarWidget = CreateWidget<UPRScoreBarWidget>(PlayerController, ScoreBarWidgetClass);
				if (ScoreBarWidget)
				{
					OnKillScoreChanged.AddDynamic(ScoreBarWidget, &UPRScoreBarWidget::UpdateKillScore);
					ScoreBarWidget->AddToViewport();
				}
			}
		}
	}

	// 입력 설정.
	SetCharacterControl(CurrentCharacterControlType);

	APRMainGameMode* GameMode = Cast<APRMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// OnAnyMonsterKilled 신호가 오면, IncrementKillScore() 함수를 호출해달라고 등록.
		GameMode->OnMonsterKilled.AddDynamic(this, &APRCharacterPlayer::IncrementKillScore);
	}

	// 게임 시작 시, UI에 킬 스코어의 초기값인 0을 브로드캐스트.
	OnKillScoreChanged.Broadcast(KillScore);
}

void APRCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 입력 바인딩.
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::QuarterMove);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &APRCharacterPlayer::StartWalk);
	EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &APRCharacterPlayer::StopWalkRun);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APRCharacterPlayer::StartRun);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APRCharacterPlayer::StopWalkRun);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::Roll);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &APRCharacterPlayer::Skill);
	EnhancedInputComponent->BindAction(EquipSlot1Action, ETriggerEvent::Triggered, this, &APRCharacterPlayer::EquipSlot1);
	EnhancedInputComponent->BindAction(EquipSlot2Action, ETriggerEvent::Triggered, this, &APRCharacterPlayer::EquipSlot2);
}

void APRCharacterPlayer::SetUpCharacterWidget(class UUserWidget* InUserWidget)
{
	// 플레이어 전용 UI인 StatusBar인지 확인.
	UPRStatusBarWidget* StatusBar = Cast<UPRStatusBarWidget>(InUserWidget);
	if (StatusBar)
	{
		if (Stat)
		{
			// StatusBar의 체력바를 스탯 컴포넌트의 체력 변경 델리게이트에 바인딩.
			Stat->OnStatusBarHpChanged.AddDynamic(StatusBar, &UPRStatusBarWidget::UpdateHpBar);

			// UI가 처음 뜰 때의 초기값을 설정.
			StatusBar->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		}
	}

	// 플레이어 전용 UI인 HotBar인지 확인.
	UPRHotBarWidget* HotBar = Cast<UPRHotBarWidget>(InUserWidget);
	if (HotBar)
	{
		// HotBar의 아이콘 업데이트 함수를 캐릭터의 무기 변경 델리게이트에 바인딩.
		OnWeaponTypeChanged.AddDynamic(HotBar, &UPRHotBarWidget::UpdateWeaponSlot);

		// UI가 처음 뜰 때의 초기 아이콘을 설정.
		HotBar->UpdateWeaponSlot(CurrentWeaponData, CurrentWeaponType);
	}
}

void APRCharacterPlayer::SkillHitCheck(FName EventTag)
{
	// 무기가 주먹이면 실행. 
	if (EventTag == TEXT("Fist"))
	{
		// 예외 처리.
		if (!GrenadeClass)
		{
			return;
		}

		// 스폰될 위치와 방향 계산.
		const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("Weapon_Socket"));
		const FRotator SpawnRotation = GetControlRotation();

		// 스폰에 필요한 추가 파라미터 설정.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		// 월드에 수류탄 액터 스폰 (디버그 포함).
		GetWorld()->SpawnActor<AActor>(GrenadeClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
	// 무기가 검이면 실행.
	else if (EventTag == TEXT("Sword"))
	{
		// 스킬 데미지 및 범위 설정.
		const float SkillDamage = 80.0f;
		const float SkillRadius = 300.0f;

		// 데미지 이벤트 생성.
		FDamageEvent DamageEvent;
        
		// 충돌 쿼리 파라미터 설정 (자기 자신 제외).
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Skill), false, this);

		// 광역 충돌 검사 수행.
		TArray<FOverlapResult> Overlaps;
		bool bIsHit = GetWorld()->OverlapMultiByChannel(
			Overlaps,
			GetActorLocation(),
			FQuat::Identity,
			CCHANNEL_PRPLAYERACTION,
			FCollisionShape::MakeSphere(SkillRadius),
			Params
		);

		// 감지된 모든 대상에 데미지 적용.
		if (bIsHit)
		{
			for (const auto& Overlap : Overlaps)
			{
				AActor* HitActor = Overlap.GetActor();
				if (HitActor)
				{
					HitActor->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
				}
			}
		}

		// 디버그 메시지 출력.
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("이십사수매화검법"));

		// 디버그용 판정 범위 시각화.
#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(GetWorld(), GetActorLocation(), SkillRadius, 32, FColor::Magenta, false, 0.5f);
#endif
	}
}

void APRCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// 변경할 컨트롤 타입에 대응하는 데이터 에셋 로드 (TMap으로부터).
	UPRCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	// 데이터 에셋을 사용해 관련 값 설정.
	SetCharacterControlData(NewCharacterControl);

	// Add InputMapping Context to Enhanced Input System.
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(NewCharacterControl->InputMappingContext, 0);
	}

	// 현재 사용 중인 캐릭터 컨트롤 타입 업데이트.
	CurrentCharacterControlType = NewCharacterControlType;
}

void APRCharacterPlayer::SetCharacterControlData(const class UPRCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	// SpringArm.
	SpringArm->TargetArmLength = CharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(CharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	SpringArm->SetRelativeLocation(CharacterControlData->RelativeLocation);
	SpringArm->SocketOffset = CharacterControlData->SocketOffset;
	SpringArm->bInheritPitch = CharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = CharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = CharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void APRCharacterPlayer::OnStartAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 플래그 해제.
	bIsStarting = false;
}

void APRCharacterPlayer::ChangeCharacterControl()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill) return;

	// 숄더뷰에서 쿼터뷰로 전환.
	if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
	// 쿼터뷰에서 숄더뷰로 전환.
	else if (CurrentCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
}

void APRCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가.
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APRCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill)
	{
		return;
	}

	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러의 회전 값.
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 방향 구하기.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 값 전달.
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);
}

void APRCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill)
	{
		return;
	}

	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// 두 방향으로 입력이 들어오면,
	// 이동 방향은 정규화해 크기를 1로 만들고,
	// 입력 스케일을 1로 강제 설정.
	if (MovementVectorSizeSquared > 1.0f)
	{
		Movement.Normalize();
		MovementVectorSize = 1.0f;
	}
	// 입력이 1 이하이면, 해당 입력을 스케일로 사용하기 위해 값 계산.
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	// 캐릭터가 이동하는 방향에 맞게 컨트롤러 회전 설정.
	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	// 입력에 따른 방향으로 이동하도록 입력 전달.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void APRCharacterPlayer::StartWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

void APRCharacterPlayer::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APRCharacterPlayer::StopWalkRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APRCharacterPlayer::Roll()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill)
	{
		return;
	}

	// 플래그 설정.
	bIsRolling = true;

	// 구르는 동안 다른 Pawn과의 충돌을 무시하도록 설정.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// 범용 델리게이트를 통해 Roll 태그와 함께 쿨타임 정보 브로드캐스트.
	const float Cooldown = 0.8f;
	OnActionCooldownStarted.Broadcast(TEXT("Roll"), Cooldown);

	// 부드러운 회전 설정.
	ComboTargetRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	bIsRotatingForCombo = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 구르기 몽타주 실행.
		AnimInstance->Montage_Play(RollActionMontage);
	}

	// 정해진 시간 후에 OnRollCooldownFinished() 함수를 호출하도록 타이머를 설정.
	GetWorldTimerManager().SetTimer(
		RollTimerHandle,
		this,
		&APRCharacterPlayer::OnRollCooldownFinished,
		Cooldown,
		false
	);
}

void APRCharacterPlayer::Attack()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill)
	{
		return;
	}

	// 콤보 공격 처리 함수 실행.
	ProcessComboCommand();
}

void APRCharacterPlayer::Skill()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill || CurrentCombo != 0 || SkillCooldownFlags.FindOrAdd(CurrentWeaponType))
	{
		return;
	}

	// 예외 처리.
	if (!CurrentWeaponData || !CurrentWeaponData->SkillMontage)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 해당 스킬이 이미 재생 중이 아닐 때 실행.
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(CurrentWeaponData->SkillMontage))
	{
		// 부드러운 회전 설정.
		ComboTargetRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		bIsRotatingForCombo = true;

		// 무기 타입에 따라 스킬 애니메이션 재생.
		AnimInstance->Montage_Play(CurrentWeaponData->SkillMontage, 1.0f);

		// 플래그 설정.
		bIsUsingSkill = true;

		// 몽타주가 끝났을 때 OnSkillMontageEnded() 함수를 호출하도록 바인딩.
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUObject(this, &APRCharacterPlayer::OnSkillMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, CurrentWeaponData->SkillMontage);

		// 무기 타입에 따라 준비 작업을 담당할 함수 호출 (역할 위임).
		switch (CurrentWeaponType)
		{
		case EWeaponType::Fist:
			FistSkill();
			break;
		case EWeaponType::Sword:
			SwordSkill();
			break;
		case EWeaponType::Magic:
			MagicSkill();
			break;
		}
	}
}

void APRCharacterPlayer::FistSkill()
{
	// 주먹 스킬의 쿨타임 상태를 true로 설정.
	SkillCooldownFlags.FindOrAdd(EWeaponType::Fist) = true;

	// 주먹 스킬의 쿨타임 시간을 가져옴.
	const float Cooldown = CurrentWeaponData->SkillCooldown;

	// UI에 Fist 스킬의 쿨타임 효과를 시작하라는 신호를 보냄.
	OnActionCooldownStarted.Broadcast(TEXT("Fist"), Cooldown);

	// 정해진 시간 후에 OnFistSkillCooldownFinished() 함수를 호출하도록 타이머를 설정.
	GetWorldTimerManager().SetTimer(
	   SkillCooldownTimerHandles.FindOrAdd(EWeaponType::Fist), 
	   this, 
	   &APRCharacterPlayer::OnFistSkillCooldownFinished, 
	   Cooldown, 
	   false
	);
}

void APRCharacterPlayer::SwordSkill()
{
	// 검 스킬의 쿨타임 상태를 true로 설정.
	SkillCooldownFlags.FindOrAdd(EWeaponType::Sword) = true;

	// 검 스킬의 쿨타임 시간을 가져옴.
	const float Cooldown = CurrentWeaponData->SkillCooldown;

	// UI에 Sword 스킬의 쿨타임 효과를 시작하라는 신호를 보냄.
	OnActionCooldownStarted.Broadcast(TEXT("Sword"), Cooldown);

	// 정해진 시간 후에 OnSwordSkillCooldownFinished() 함수를 호출하도록 타이머를 설정.
	GetWorldTimerManager().SetTimer(
	   SkillCooldownTimerHandles.FindOrAdd(EWeaponType::Sword),
	   this, 
	   &APRCharacterPlayer::OnSwordSkillCooldownFinished, 
	   Cooldown, 
	   false
	);
}

void APRCharacterPlayer::MagicSkill()
{
}

void APRCharacterPlayer::EquipSlot1()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill || CurrentCombo != 0)
	{
		return;
	}

	// 주먹으로 교체.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EquipSlot1() is called"));
	EquipWeapon(EWeaponType::Fist);
	OnWeaponTypeChanged.Broadcast(CurrentWeaponData, CurrentWeaponType);
}

void APRCharacterPlayer::EquipSlot2()
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill || CurrentCombo != 0)
	{
		return;
	}

	// 검으로 교체.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EquipSlot2() is called"));
	EquipWeapon(EWeaponType::Sword);
	OnWeaponTypeChanged.Broadcast(CurrentWeaponData, CurrentWeaponType);
}

void APRCharacterPlayer::OnRollCooldownFinished()
{
	// 플래그 해제.
	bIsRolling = false;

	// 구르기가 끝났으니, 다시 다른 Pawn과 충돌하도록 설정 되돌림.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnRollCooldownFinished() is called"));
}

void APRCharacterPlayer::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 플래그 해제.
	bIsUsingSkill = false;
}

void APRCharacterPlayer::OnFistSkillCooldownFinished()
{
	// 주먹 스킬의 쿨타임 상태를 false로 되돌림.
	SkillCooldownFlags.FindOrAdd(EWeaponType::Fist) = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnFistSkillCooldownFinished() is called"));
}

void APRCharacterPlayer::OnSwordSkillCooldownFinished()
{
	// 검 스킬의 쿨타임 상태를 false로 되돌림.
	SkillCooldownFlags.FindOrAdd(EWeaponType::Sword) = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnSwordSkillCooldownFinished() is called"));
}

void APRCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 비활성화.
		DisableInput(PlayerController);
	}

	APRMainGameMode* GameMode = Cast<APRMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// 게임 종료.
		GameMode->EndGame(false);
	}
}

void APRCharacterPlayer::IncrementKillScore()
{
	// 킬 스코어를 증가시키고, 변경 이벤트를 브로드캐스트.
	KillScore += 100;
	OnKillScoreChanged.Broadcast(KillScore);
}

bool APRCharacterPlayer::CanJumpInternal_Implementation() const
{
	// 예외 처리.
	if (bIsStarting || bIsRolling || bIsUsingSkill || CurrentCombo != 0)
	{
		return false;
	}

	return Super::CanJumpInternal_Implementation();
}
