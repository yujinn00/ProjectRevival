// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRStatusBarWidget.h"
#include "Components/ProgressBar.h"

void UPRStatusBarWidget::UpdateHpBar(float CurrentHp, float MaxHp)
{
	// HP 프로그레스 바 위젯이 유효하고, MaxHp가 0보다 큰지 확인.
	if (HP && MaxHp > 0.0f)
	{
		HP->SetPercent(CurrentHp / MaxHp);
	}
}
