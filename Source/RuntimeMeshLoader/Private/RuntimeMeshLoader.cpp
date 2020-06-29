// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeMeshLoader.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRuntimeMeshLoaderModule"

//In fact, we don't need to do anything here.
//It's just a "environment check" makes it's easier to debug.
void FRuntimeMeshLoaderModule::StartupModule()
{
	
}

void FRuntimeMeshLoaderModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeMeshLoaderModule, RuntimeMeshLoader)
