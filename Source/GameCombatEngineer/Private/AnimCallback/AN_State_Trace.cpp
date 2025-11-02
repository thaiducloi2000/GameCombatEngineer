// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimCallback/AN_State_Trace.h"
#include "Interface/AttackInterface.h"

void UAN_State_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr) return;
	AttackInterface = TScriptInterface<IAttackInterface>(MeshComp->GetOwner());
	if (AttackInterface)
		AttackInterface->I_ANS_BeginTraceHit();
}

void UAN_State_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (AttackInterface)
		AttackInterface->I_ANS_TraceHit();
}
