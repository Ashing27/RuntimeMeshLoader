#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RuntimeMeshLoader.h"

#include <string>
#include <algorithm>

THIRD_PARTY_INCLUDES_START
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/ParsingUtils.h>
#include <assimp/include/assimp/postprocess.h>
#include <assimp/include/assimp/scene.h>
THIRD_PARTY_INCLUDES_END

#include "MeshLoader.generated.h"


UENUM(BlueprintType)
enum class EPathType : uint8
{
	Absolute,
	Relative,
};

USTRUCT(BlueprintType)
struct FRtMeshes
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<int32> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector> Normals;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector2D> UV0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FLinearColor> VertexColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FProcMeshTangent> Tangents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		FTransform RelativeTransform;
};

USTRUCT(BlueprintType)
struct FReturnedData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	bool bSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	int32 NumMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	TArray<FRtMeshes> RtLoadedMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
	FString ErrorInfo;
};


/**
 * It's a loader used in blueprint for read a static mesh at runtime.
 * thanks to RuntimeMeshLoader at Github
 */
UCLASS()
class RUNTIMEMESHLOADER_API UMeshLoader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "MeshLoader")
	static FString GetPluginInfo();

	UFUNCTION(BlueprintCallable, Category = "MeshLoader")
	static FReturnedData LoadMesh(FString filePath, EPathType type = EPathType::Absolute);
	
private:
	static void FindMeshInfo(const aiScene* scene, aiNode* node, FReturnedData& result);

	static void FindMesh(const aiScene* scene, aiNode* node, FReturnedData& retdata);
};
