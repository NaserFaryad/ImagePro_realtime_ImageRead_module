#include "stdafx.h"
#include "AlgorithmModule.h"
#include <stdlib.h>
#include <fstream>
#include "ErrorCodeDefine.h"
#include "iMVS-6000PixelFormatDefine.h"


#include <opencv2/opencv.hpp>

using namespace cv;



HKA_IMAGE MatToHKAImage(Mat mat)
{

	HKA_IMAGE image;
	image = {
	 HKA_IMG_MONO_08, 0 };
	image.width = mat.cols;
	image.height = mat.rows;
	image.format = HKA_IMG_MONO_08;
	image.step[0] = mat.cols;
	image.data[0] = mat.data;
	return image;
}

CAlgorithmModule::CAlgorithmModule()
{
}

CAlgorithmModule::~CAlgorithmModule()
{

}

int CAlgorithmModule::Init()
{

	int nRet = VM_M_GetModuleId(m_hModule, &m_nModuleId);
	if (IMVS_EC_OK != nRet)
	{
		m_nModuleId = -1;
		return nRet;
	}

	nRet = ResetDefaultParam();
	if (nRet != IMVS_EC_OK)
	{
		OutputDebugStringA("###Call ResetDefaultParam failed.");
	}

	return nRet;
}

int CAlgorithmModule::Process(IN void* hInput, IN void* hOutput, IN MVDSDK_BASE_MODU_INPUT* modu_input)
{
	int nErrCode = 0;


	OutputDebugStringA("###Call CAlgorithmModule::Proces --> do algorighm process\n");


	if (nErrCode != IMVS_EC_OK)
	{
		return IMVS_EC_PARAM;
	}

	MODULE_RUNTIME_INFO struRunInfo = { 0 };
	struRunInfo.fAlgorithmTime = 20;
	VM_M_SetModuleRuntimeInfo(m_hModule, &struRunInfo);

	OutputDebugStringA("###Call CAlgorithmModule::Proces end\n");

	cv::Mat input = cv::imread("C:\\temp\\image.jpg", cv::IMREAD_UNCHANGED);
	HKA_IMAGE image = MatToHKAImage(input);
	VmModule_OutputImageByName_8u_C1R(hOutput, 1, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", &image);
	//VmModule_OutputDstImage_8u_C3R(hOutput, 0, &image);
	VM_M_SetInt(hOutput, "ModuStatus", 0, nErrCode == 0 ? 1 : nErrCode);
	VM_M_SetModuleRuntimeInfo(m_hModule, &struRunInfo);

	OutputDebugStringA("###Call CAlgorithmModule::Proces end\n");

	return IMVS_EC_OK;
}


int CAlgorithmModule::GetParam(IN const char* szParamName, OUT char* pBuff, IN int nBuffSize, OUT int* pDataLen)
{
	OutputDebugStringA("###Call CAlgorithmModule::GetParam");

	int nErrCode = IMVS_EC_OK;
	if (szParamName == NULL || strlen(szParamName) == 0 || pBuff == NULL || nBuffSize <= 0 || pDataLen == NULL)
	{
		return IMVS_EC_PARAM;
	}
 	//memset(pBuff, 0, nBuffSize);

	else
	{
		return CVmAlgModuleBase::GetParam(szParamName, pBuff, nBuffSize, pDataLen);
	}
	
	return nErrCode;
}

int CAlgorithmModule::SetParam(IN const char* szParamName, IN const char* pData, IN int nDataLen)
{
	OutputDebugStringA("###Call CAlgorithmModule::SetParam");

	int nErrCode = IMVS_EC_OK;
	if (szParamName == NULL || strlen(szParamName) == 0 || pData == NULL || nDataLen == 0)
	{
		return IMVS_EC_PARAM;
	}

	else
	{
		return CVmAlgModuleBase::SetParam(szParamName, pData, nDataLen);
	}

	return nErrCode;
}


/////////////////////////////模块须导出的接口（实现开始）//////////////////////////////////////////

LINEMODULE_API CAbstractUserModule* __stdcall CreateModule(void* hModule)
{
	assert(hModule != NULL);
   

	// 创建用户模块，并记录实例。
	CAlgorithmModule* pUserModule = new(nothrow) CAlgorithmModule;

	if (pUserModule == NULL)
	{
		return NULL;
	}

	pUserModule->m_hModule = hModule;

	int nRet = pUserModule->Init();
	if (IMVS_EC_OK != nRet)
	{
		delete pUserModule;
		return NULL;
	}

	OutputDebugStringA("###Call CreateModule");

	return pUserModule;
}


LINEMODULE_API void __stdcall DestroyModule(void* hModule, CAbstractUserModule* pUserModule)
{
	assert(hModule != NULL);
	OutputDebugStringA("###Call DestroyModule");

	if (pUserModule != NULL)
	{
		delete pUserModule;
	}
}
