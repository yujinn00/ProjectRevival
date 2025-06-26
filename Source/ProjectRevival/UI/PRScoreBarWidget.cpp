// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRScoreBarWidget.h"
#include "Components/TextBlock.h"

void UPRScoreBarWidget::UpdateKillScore(int32 NewKillScore)
{
	if (Score)
	{
		// 업데이트할 킬 스코어를 포맷팅하여 출력.
		FText FormattedText = FText::Format(NSLOCTEXT("PRScoreBarWidget", "KillScoreFormat", "Score: {0000}"), NewKillScore);
		Score->SetText(FormattedText);
	}
}
