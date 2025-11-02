// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimCallback/AN_OnAttackCallback.h"
#include "Interface/AttackInterface.h"

void UAN_OnAttackCallback::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,Animation,EventReference);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("End Attack"));
	if (MeshComp == nullptr) return;
	auto AttackInterface = TScriptInterface<IAttackInterface>(MeshComp -> GetOwner());
	if (AttackInterface) {
		AttackInterface->I_EndAttackMontage();
	}
}
