// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffect.h"
#include "Kismet/GameplayStatics.h"
#include "ValorantCharacter.h"

StatusEffect::StatusEffect(float t, float r, AActor* owner) : time(t), range(r), Owner(owner) {}


/*----------------------------------------
SEFlash
-----------------------------------------*/

SEFlash::SEFlash(float t, float r, AActor* owner): StatusEffect(t, r, owner)
{
}

void SEFlash::Fire()
{
	//Actor(Parent) �޾ƿͼ� ���� ��� 
	//�ɸ� character �þ߰� ��� -> ���� ����(�ð�)
	//character camera�� flash ����
	if (Owner)
	{
		//sphere trace
		UWorld* World = Owner->GetWorld();
		FVector Center = Owner->GetActorLocation();

		if (World == nullptr) return;

		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams(NAME_None, false, Owner);

		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_EngineTraceChannel2,
			FCollisionShape::MakeSphere(range),
			QueryParams);

		if (bResult)
		{
			for (auto result : OverlapResults)
			{
				AValorantCharacter* victim = Cast<AValorantCharacter>(result.GetActor());
				
				if (victim == nullptr)
				{
					continue;
				}

				auto Foward = victim->GetActorForwardVector();
				auto ToFlash = Center - victim->GetActorLocation();
				ToFlash.Normalize();

				auto dotResult = Foward.Dot(ToFlash);
				float theta = FMath::RadiansToDegrees(FMath::Acos(dotResult));

				auto Cam = victim->GetFirstPersonCameraComponent();
				if (Cam)
				{
					//4�ܰ�� �з�
					if (theta <= FOV / 5.f)
					{
						// time * 1. ����
						UE_LOG(LogTemp, Warning, TEXT("baaaaaam"));
					}
					else if (theta <= FOV / 3.f)
					{
						// time * .7 ����

					}
					else if (theta <= FOV / 2.f)
					{
						// time * .3 ����

					}
					else
					{
						// time * .15 ����

					}
				}
			}
		}
	}
}




/*----------------------------------------
SEConcussion
-----------------------------------------*/

SEConcussion::SEConcussion(float t, float r, AActor* owner): StatusEffect(t, r, owner)
{
}

void SEConcussion::Fire()
{
}
