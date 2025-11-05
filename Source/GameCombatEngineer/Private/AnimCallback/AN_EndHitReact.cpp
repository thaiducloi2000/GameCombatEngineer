// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimCallback/AN_EndHitReact.h"
#include "Interface/AttackInterface.h"

void UAN_EndHitReact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr) return;
	auto AttackInterface = TScriptInterface<IAttackInterface>(MeshComp->GetOwner());
	if (AttackInterface) {
		AttackInterface->I_EndHitReact();
	}
}
