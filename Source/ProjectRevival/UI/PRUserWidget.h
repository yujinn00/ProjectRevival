// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PRUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 소유 액터를 설정할 때 사용할 함수.
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	// 위젯을 소유하는 액터 참조 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Actor)
	TObjectPtr<AActor> OwningActor;

	// 위젯이 초기화될 때 호출되는 이벤트 함수.
	virtual void NativeOnInitialized() override;
};
