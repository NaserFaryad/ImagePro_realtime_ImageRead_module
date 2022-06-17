#include "stdafx.h"
#include "AlgorithmModule.h"
#include <stdlib.h>
#include <fstream>
#include "ErrorCodeDefine.h"
#include "iMVS-6000PixelFormatDefine.h"


#include <opencv2/opencv.hpp>

using namespace cv;



void logText(string text) {
	ofstream file_out;
	file_out.open("C:\\Users\\ali\\Desktop\\workspace\\log.txt", std::ios_base::app);
	file_out << text << endl;
	file_out.close();
}

CAlgorithmModule::CAlgorithmModule()
{
	
}

CAlgorithmModule::~CAlgorithmModule()
{

}

int CAlgorithmModule::Init()
{
	PARAM_VALUE_INFO_LIST stList = { 0 };
	int nRet = VM_M_GetDefaultConfigByFile(m_hModule, UNICODEtoUTF8(VmModule_GetXmlPath().GetBuffer()), &stList);
	if (nRet == IMVS_EC_OK)
	{
		for (int i = 0; i < stList.nNum; i++)
		{
			SetParam(stList.paramValueList[i].byParamName, stList.paramValueList[i].byParamValue, strlen(stList.paramValueList[i].byParamValue));
		}
	}

	return nRet;
}

//HKA_IMAGE MatToHKAImage(Mat mat)
//{
//    
//	HKA_IMAGE image;
//	if (mat.channels() == 1)
//	{
//    
//		image = {
//     HKA_IMG_MONO_08, 0 };
//		image.width = mat.cols;
//		image.height = mat.rows;
//		image.format = HKA_IMG_MONO_08;
//		image.step[0] = mat.cols;
//		image.data[0] = mat.data;
//	}
//	else if (mat.channels() == 3)
//	{
//    
//		image = {
//     HKA_IMG_RGB_RGB24_C3, 0 };
//		image.width = mat.cols;
//		image.height = mat.rows;
//		image.format = HKA_IMG_RGB_RGB24_C3;
//		image.step[0] = 3 * mat.cols;
//		image.data[0] = mat.data;
//	}
//	return image;
//}

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

int CAlgorithmModule::Process(IN void* hInput, IN void* hOutput, IN MVDSDK_BASE_MODU_INPUT* modu_input)
{
	OutputDebugStringA("###Call CAlgorithmModule::Proces -->begin\n");
	int nErrCode = 0;

// 1.获取输入
	
	/************************************************/

	//ToDo Code...............

	/************************************************/

// 2.算法处理
	OutputDebugStringA("###Call CAlgorithmModule::Proces --> do algorighm process\n");

	/************************************************/

	//ToDo Code...............

	/************************************************/


// 3.输出结果
	
	/************************************************/

	//ToDo Code...............

	/************************************************/

	if (nErrCode != IMVS_EC_OK)
	{
		return IMVS_EC_PARAM;
	}

	/************************************************/
	//默认算法时间20ms，根据实际时间计算
	MODULE_RUNTIME_INFO struRunInfo = { 0 };
	struRunInfo.fAlgorithmTime = 20;
	VM_M_SetModuleRuntimeInfo(m_hModule, &struRunInfo);

	OutputDebugStringA("###Call CAlgorithmModule::Proces end\n");

	cv::Mat input = cv::imread("C:\\temp\\image.jpg", cv::IMREAD_UNCHANGED);
	HKA_IMAGE image = MatToHKAImage(input);
	VmModule_OutputImageByName_8u_C1R(hOutput, 1, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", &image);
	//VmModule_OutputDstImage_8u_C3R(hOutput, 0, &image);
	VM_M_SetInt(hOutput, "ModuStatus", 0, nErrCode == 0 ? 1 : nErrCode);

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

	printf("[ LineModule ] CreateModule, hModule = 0x%x, pUserModule = 0x%x \n", hModule, pUserModule);

	OutputDebugStringA("###Call CreateModule");

	return pUserModule;
}


LINEMODULE_API void __stdcall DestroyModule(void* hModule, CAbstractUserModule* pUserModule)
{
	assert(hModule != NULL);

	printf("\n[ LineModule ] DestroyModule, hModule = 0x%x\n", hModule);

	OutputDebugStringA("###Call DestroyModule");

	if (pUserModule != NULL)
	{
		delete pUserModule;
	}
}
/////////////////////////////模块须导出的接口（实现结束）//////////////////////////////////////////
