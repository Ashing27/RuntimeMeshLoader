//Copyright

#include "MeshLoader.h"
#include "Engine.h"

FString UMeshLoader::GetPluginInfo()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PluginFunciton_OK"));
    return FString("Runtime MeshLoader. Created by Mick. Neusoft Medical System June 2020. Ver. 0.1");
}

FReturnedData UMeshLoader::LoadMesh(FString filePath, EPathType type)
{
    FReturnedData stRet;
    stRet.bSuccess = false;
    stRet.RtLoadedMeshes.Empty();
    stRet.NumMeshes = 0;

    if (filePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("RuntimeMeshLoader: filepath is empty."));
        stRet.ErrorInfo = TEXT("filePath: Empty");
        return stRet;
    }

    std::string file;
    switch (type)
    {
    case EPathType::Absolute:
        file = TCHAR_TO_UTF8(*filePath);
        break;
    case EPathType::Relative:
        file = TCHAR_TO_UTF8(*FPaths::Combine(FPaths::ProjectContentDir(), filePath));
        break;
    }

    Assimp::Importer importer;

    unsigned int nImportConfig = 
        aiProcess_Triangulate |\
        aiProcess_MakeLeftHanded |\
        aiProcess_CalcTangentSpace |\
        aiProcess_GenSmoothNormals |\
        aiProcess_OptimizeMeshes;

    const aiScene* pScene = importer.ReadFile(file, nImportConfig);
    if (pScene == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("RuntimeMeshLoader: pScene got an nullptr."));
        stRet.ErrorInfo = TEXT("Scene: NULLPTR");
        return stRet;
    }

    if (!pScene->HasMeshes())
    {
        stRet.ErrorInfo = TEXT("Scene: No Meshes");
        return stRet;
    }
    stRet.RtLoadedMeshes.SetNum(pScene->mNumMeshes, false);

    FindMesh(pScene, pScene->mRootNode, stRet);

    for (uint32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        //Triangle number
        for (uint32 l = 0; l < pScene->mMeshes[i]->mNumFaces; ++l)
        {
            for (uint32 m = 0; m < pScene->mMeshes[i]->mFaces[l].mNumIndices; ++m)
            {
                stRet.RtLoadedMeshes[i].Triangles.Push(pScene->mMeshes[i]->mFaces[l].mIndices[m]);
            }
        }
    }

    stRet.bSuccess = true;
    return stRet;
}

void UMeshLoader::FindMeshInfo(const aiScene* scene, aiNode* node, FReturnedData& result)
{
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		std::string TestString = node->mName.C_Str();
		FString Fs = FString(TestString.c_str());
		UE_LOG(LogTemp, Warning, TEXT("FindMeshInfo. %s\n"), *Fs);
		int meshidx = *node->mMeshes;
		aiMesh* mesh = scene->mMeshes[meshidx];
		FRtMeshes& mi = result.RtLoadedMeshes[meshidx];

		//transform.
		aiMatrix4x4 tempTrans = node->mTransformation;
		FMatrix tempMatrix;
		tempMatrix.M[0][0] = tempTrans.a1; tempMatrix.M[0][1] = tempTrans.b1; tempMatrix.M[0][2] = tempTrans.c1; tempMatrix.M[0][3] = tempTrans.d1;
		tempMatrix.M[1][0] = tempTrans.a2; tempMatrix.M[1][1] = tempTrans.b2; tempMatrix.M[1][2] = tempTrans.c2; tempMatrix.M[1][3] = tempTrans.d2;
		tempMatrix.M[2][0] = tempTrans.a3; tempMatrix.M[2][1] = tempTrans.b3; tempMatrix.M[2][2] = tempTrans.c3; tempMatrix.M[2][3] = tempTrans.d3;
		tempMatrix.M[3][0] = tempTrans.a4; tempMatrix.M[3][1] = tempTrans.b4; tempMatrix.M[3][2] = tempTrans.c4; tempMatrix.M[3][3] = tempTrans.d4;
		mi.RelativeTransform = FTransform(tempMatrix);

		//vet
		for (uint32 j = 0; j < mesh->mNumVertices; ++j)
		{
			FVector vertex = FVector(
				mesh->mVertices[j].x,
				mesh->mVertices[j].y,
				mesh->mVertices[j].z);

			vertex = mi.RelativeTransform.TransformFVector4(vertex);
			mi.Vertices.Push(vertex);

			//Normal
			if (mesh->HasNormals())
			{
				FVector normal = FVector(
					mesh->mNormals[j].x,
					mesh->mNormals[j].y,
					mesh->mNormals[j].z);

				//normal = mi.RelativeTransform.TransformFVector4(normal);
				mi.Normals.Push(normal);
			}
			else
			{
				mi.Normals.Push(FVector::ZeroVector);
			}

			//UV Coordinates - inconsistent coordinates
			if (mesh->HasTextureCoords(0))
			{
				FVector2D uv = FVector2D(mesh->mTextureCoords[0][j].x, -mesh->mTextureCoords[0][j].y);
				mi.UV0.Add(uv);
			}

			//Tangent
			if (mesh->HasTangentsAndBitangents())
			{
				FProcMeshTangent meshTangent = FProcMeshTangent(
					mesh->mTangents[j].x,
					mesh->mTangents[j].y,
					mesh->mTangents[j].z
				);
				mi.Tangents.Push(meshTangent);
			}

			//Vertex color
			if (mesh->HasVertexColors(0))
			{
				FLinearColor color = FLinearColor(
					mesh->mColors[0][j].r,
					mesh->mColors[0][j].g,
					mesh->mColors[0][j].b,
					mesh->mColors[0][j].a
				);
				mi.VertexColors.Push(color);
			}

		}
	}
}

void UMeshLoader::FindMesh(const aiScene* scene, aiNode* node, FReturnedData& retdata)
{
    FindMeshInfo(scene, node, retdata);

    for (uint32 m = 0; m < node->mNumChildren; ++m)
    {
        FindMesh(scene, node->mChildren[m], retdata);
    }
}
