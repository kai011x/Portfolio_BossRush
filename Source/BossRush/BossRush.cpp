// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossRush.h"
#include "Modules/ModuleManager.h"
#include "GAS/Tags/GameplayTags.h"

class FBossRushModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();

		// C++에서 사용할 Native Gameplay Tags 초기화
		FGameplayTags::InitializeNativeGameplayTags();
	}

	virtual void ShutdownModule() override
	{
		FDefaultGameModuleImpl::ShutdownModule();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FBossRushModule, BossRush, "BossRush" );
