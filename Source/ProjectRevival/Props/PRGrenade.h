// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRGrenade.generated.h"

UCLASS()
class PROJECTREVIVAL_API APRGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APRGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Component Section.
protected:
	// 충돌체 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<class USphereComponent> Sphere;

	// 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;

	// 투사체 이동 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

// Explosion Section.
protected:
	// 폭발 시 재생될 파티클 이펙트.
	UPROPERTY(EditAnywhere, Category = "Explosion-Effect")
	TObjectPtr<class UParticleSystem> ParticleSystem;

	// 폭발 시 재생될 카메라 셰이크 클래스.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	TSubclassOf<class UCameraShakeBase> CameraShakeBase;

	// 폭발의 최대 데미지.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	float BaseDamage = 250.0f;

	// 폭발의 최소 데미지.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	float MinimumDamage = 100.0f;

	// 최대 데미지가 적용되는 내부 반경.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	float InnerRadius = 200.0f;

	// 폭발이 영향을 미치는 최대 반경.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	float OuterRadius = 500.0f;

	// 폭발까지 걸리는 시간.
	UPROPERTY(EditAnywhere, Category = "Explosion")
	float FuseTime = 2.0f;

private:
	// 폭발 로직을 처리하는 함수
	void Explode();

	// 폭발 타이머를 위한 핸들.
	FTimerHandle ExplosionTimerHandle;
};
