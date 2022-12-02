#include "VmModule_IO.h"
#include "iMVS-6000PixelFormatDefine.h"
#include "VmModuleBase.h"
#include "ErrorCodeDefine.h"
#include "hka_defs.h"

HKA_S32 VmModule_iMVSFormatToAlgFormat(HKA_S32           formatIn,
                                       HKA_IMAGE_FORMAT *formatOut)
{
    HKA_S32 status = HKA_TRUE;

    switch(formatIn)
    {
    case IMVS_PIX_MONO8:
    case IMVS_PIX_MONO:
        *formatOut = HKA_IMG_MONO_08;
        break;
    case IMVS_PIX_RGB8_PACKED:
        *formatOut = HKA_IMG_RGB_RGB24_C3;
        break;
    case IMVS_PIX_RGB8_PLANAR:
        *formatOut = HKA_IMG_RGB_RGB24_P3;
        break;
    default:
        status = HKA_FALSE;
    }

    return status;
}


void ErrorCodeToStatus(int nErrCode, HKA_U32 *status)
{
	if (status == NULL)
	{
		return;
	}

	switch (nErrCode)
	{
	case IMVS_EC_OK:
		*status = IMVS_MODU_ENUM_STATUS_OK;
		break;
	case IMVS_EC_MODULE_SUB_RECORD_NOT_FOUND:
		*status = IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE;
		break;
	case IMVS_EC_MODULE_SUB_PARAM_ERROR:
		*status = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
		break;
	default:
		*status = IMVS_MODU_ENUM_STATUS_ERROR;
		break;
	}
}


HKA_S32 VmModule_GetInputScalar_32f(IN const void * const hInput,
                                    const char          *xmlName,
                                    HKA_F32       *val,
                                    HKA_U32       *status)
{
    HKA_S32 nCount = 0;
	HKA_S32 nRet   = IMVS_EC_UNKNOWN;
  
    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);

	nRet = VM_M_GetFloat(hInput, xmlName, 0, val, &nCount);

	ErrorCodeToStatus(nRet, status);

    return nRet;
}


HKA_S32 VmModule_GetInputScalar_32i(IN const void * const hInput,
                                    const char          *xmlName,
                                    HKA_S32       *val,
                                    HKA_U32       *status)
{
    HKA_S32 nCount      = 0;
    HKA_S32 nRet        = IMVS_EC_UNKNOWN;

    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);
  
	nRet = VM_M_GetInt(hInput, xmlName, 0, val, &nCount);

	ErrorCodeToStatus(nRet, status);

    return nRet;
}


HKA_S32 VmModule_GetInputImageAddress(IN const void * const hInput,
                                      const char          *xmlName,
                                      char*       *val,
                                      HKA_U32       *status)
{
    HKA_S32 nRet          = IMVS_EC_UNKNOWN;
    HKA_S32 nCount        = 0;

    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);

	IMAGE_DATA struImageData = { 0 };
	memset(&struImageData, 0, sizeof(struImageData));

	nRet = VM_M_GetImage(hInput, xmlName, 0, &struImageData, &nCount);

	ErrorCodeToStatus(nRet, status);

    if (nRet == IMVS_EC_OK)
	{
		*val = (struImageData.pImgData);
	}

    return nRet;
}

HKA_S32 VmModule_GetInputImageAddressAndDataSize(IN const void * const hInput,
    const char          *xmlName,
    char*       *val,
    int&        iImageDataLen,
    HKA_U32       *status)
{
    HKA_S32 nRet = IMVS_EC_UNKNOWN;
    HKA_S32 nCount = 0;

    HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status, IMVS_EC_PARAM);
    iImageDataLen = 0;
    IMAGE_DATA struImageData = { 0 };
    memset(&struImageData, 0, sizeof(struImageData));

    nRet = VM_M_GetImage(hInput, xmlName, 0, &struImageData, &nCount);

    ErrorCodeToStatus(nRet, status);

    if (nRet == IMVS_EC_OK)
    {
        *val = (struImageData.pImgData);
        iImageDataLen = struImageData.nImgDataLen;
    }

    return nRet;
}


/***************************************************************************************************
* 函数名：iMVS6000_GetInputVectorCount
* 功  能：算法平台获取输入数组元素个数
* 参  数：*
*         hInput      - I 模块输入
*         xmlName     - I 对应XML里面的名称
*         count       - O 数组个数
*         status      - O 状态
*         
* 返回值：0：正确；其他：错误
* 备  注：
***************************************************************************************************/
HKA_S32 VmModule_GetInputVectorCount(IN const void * const hInput,
                                     const char          *xmlName,
                                     HKA_U32       *count,
                                     HKA_U32       *status)
{
    char    sValue[256] = {0};
    HKA_U32 nCount      = 0;
    HKA_U32 nStatus     = 0;
    HKA_S32 nRet        = IMVS_EC_OK;

    HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == count, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status, IMVS_EC_PARAM);

    float fValue = 0;
    nRet = VM_M_GetFloat(hInput, 
        xmlName, 
        0, 
        &fValue, 
        (int*)&nCount);

    *count  = nCount;

    ErrorCodeToStatus(nRet, status);

    return IMVS_EC_OK;
}
HKA_S32 VmModule_GetInputVector_32f(IN const void * const hInput,
                                    const char          *xmlName,
                                    HKA_F32       *val,
                                    HKA_U32        num,
                                    HKA_U32       *status)
{
    HKA_S32 nCount      = 0;
    HKA_S32 nRet        = IMVS_EC_UNKNOWN;

    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);

	do 
	{
		float fValue = 0;
		nRet = VM_M_GetFloat(hInput, xmlName, 0, &fValue, &nCount);
		if (nRet != IMVS_EC_OK)
		{
			break;
		}

		if (num > HKA_U32(nCount))
		{
			nRet = IMVS_EC_PARAM;
			break;
		}

		val[0] = fValue;

		for(HKA_U32 i = 1; i < num; i++)
		{
			nRet = VM_M_GetFloat(hInput, xmlName, i, &val[i], &nCount);
			if (nRet != IMVS_EC_OK)
			{
				break;
			}
		}

	} while (false);

	ErrorCodeToStatus(nRet, status);

	return nRet;
}


HKA_S32 VmModule_GetInputVector_32i(IN const void * const hInput,
                                    const char          *xmlName,
                                    HKA_S32       *val,
                                    HKA_U32        num,
                                    HKA_U32       *status)
{
    HKA_S32 nCount      = 0;
    HKA_S32 nRet        = IMVS_EC_UNKNOWN;

    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);

	do 
	{
		int nValue = 0;
		nRet = VM_M_GetInt(hInput, xmlName, 0, &nValue, &nCount);
		if (nRet != IMVS_EC_OK)
		{
			break;
		}

		if (num > HKA_U32(nCount))
		{
			nRet = IMVS_EC_PARAM;
			break;
		}

		val[0] = nValue;

		for(HKA_U32 i = 1; i < num; i++)
		{
			nRet = VM_M_GetInt(hInput, xmlName, i, &val[i], &nCount);
			if (nRet != IMVS_EC_OK)
			{
				break;
			}
		}

	} while (false);

	ErrorCodeToStatus(nRet, status);

	return nRet;
}


HKA_S32 VmModule_GetInputImageEx(IN const void * const hInput,
    HKA_IMAGE     *image,
    HKA_S32&      iImageDataLen,
    HKA_S32       *imageStatus)
{
    HKA_S32          nRet = IMVS_EC_UNKNOWN;
    HKA_S32          format = 0;
    HKA_IMAGE_FORMAT formatAlg = HKA_IMG_MONO_08;
    HKA_U32          nStatusImage = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusWidth = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusHeight = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusFormat = IMVS_MODU_ENUM_STATUS_ERROR;
    char*            addr = 0;

    HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == image, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == imageStatus, IMVS_EC_PARAM);


    nRet = VmModule_GetInputImageAddressAndDataSize(hInput, "InImage", &addr,iImageDataLen, &nStatusImage);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InImageWidth", &(image->width), &nStatusWidth);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InImageHeight", &(image->height), &nStatusHeight);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InImagePixelFormat", &format, &nStatusFormat);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    if (image->height <= 0)
    {
        nStatusHeight = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    if (image->width <= 0)
    {
        nStatusWidth = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    *imageStatus = (IMVS_MODU_ENUM_STATUS_OK == nStatusImage)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusWidth)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusHeight)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusFormat);

    nRet = VmModule_iMVSFormatToAlgFormat(format, &formatAlg);
    HKA_CHECK_ERROR(HKA_TRUE != nRet, nRet);

    image->format = formatAlg;
    image->data[0] = addr;
    image->step[0] = (HKA_IMG_RGB_RGB24_C3 == formatAlg) ? (3 * image->width) : image->width;


    return IMVS_EC_OK;
}

HKA_S32 VmModule_GetMaskImage(IN const void * const hInput,
    HKA_IMAGE     *image,
    HKA_S32&      iImageDataLen,
    HKA_S32       *imageStatus)
{
    HKA_S32          nRet = IMVS_EC_UNKNOWN;
    HKA_S32          format = 0;
    HKA_IMAGE_FORMAT formatAlg = HKA_IMG_MONO_08;
    HKA_U32          nStatusImage = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusWidth = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusHeight = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusFormat = IMVS_MODU_ENUM_STATUS_ERROR;
    char*            addr = 0;

    HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == image, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == imageStatus, IMVS_EC_PARAM);


    nRet = VmModule_GetInputImageAddressAndDataSize(hInput, "InMask", &addr, iImageDataLen, &nStatusImage);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InMaskWidth", &(image->width), &nStatusWidth);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InMaskHeight", &(image->height), &nStatusHeight);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


    nRet = VmModule_GetInputScalar_32i(hInput, "InMaskPixelFormat", &format, &nStatusFormat);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    if (image->height <= 0)
    {
        nStatusHeight = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    if (image->width <= 0)
    {
        nStatusWidth = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    *imageStatus = (IMVS_MODU_ENUM_STATUS_OK == nStatusImage)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusWidth)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusHeight)
        && (IMVS_MODU_ENUM_STATUS_OK == nStatusFormat);

    nRet = VmModule_iMVSFormatToAlgFormat(format, &formatAlg);
    HKA_CHECK_ERROR(HKA_TRUE != nRet, nRet);

    image->format = formatAlg;
    image->data[0] = addr;
    image->step[0] = (HKA_IMG_RGB_RGB24_C3 == formatAlg) ? (3 * image->width) : image->width;


    return IMVS_EC_OK;
}


HKA_S32 VmModule_GetInputFixture(IN const void * const hInput,
                                 MVBI_FIDUCIAL_POSE   *fixInit,
                                 MVBI_FIDUCIAL_POSE   *fixRun,
                                 HKA_S32              *fixStatus)
{
    HKA_S32  nRet                  = IMVS_EC_UNKNOWN;
    HKA_U32  nStatusFixInitPointX  = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32  nStatusFixInitPointY  = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32  nStatusFixInitScaleX   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitScaleY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitAngle = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32  nStatusFixRunPointX   = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32  nStatusFixRunPointY   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunScaleX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunScaleY = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32  nStatusFixRunAngle    = IMVS_MODU_ENUM_STATUS_ERROR;
    
    nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitPointX", &(fixInit->basis_point.x), &nStatusFixInitPointX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitPointX)
	{
		return IMVS_EC_PARAM;
	}

    nRet =  VmModule_GetInputScalar_32f(hInput, "FixtureInitPointY", &(fixInit->basis_point.y), &nStatusFixInitPointY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitPointY)
	{
		return IMVS_EC_PARAM;
	}

    nRet = VmModule_GetInputScalar_32f(hInput,  "FixtureInitAngle", &(fixInit->basis_angle), &nStatusFixInitAngle);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitAngle)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitScaleX", &(fixInit->basis_scale_x), &nStatusFixInitScaleX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitScaleX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitScaleY", &(fixInit->basis_scale_y), &nStatusFixInitScaleY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitScaleY)
	{
		return IMVS_EC_PARAM;
	}

    nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunPointX", &(fixRun->basis_point.x), &nStatusFixRunPointX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunPointX)
	{
		return IMVS_EC_PARAM;
	}

    nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunPointY", &(fixRun->basis_point.y), &nStatusFixRunPointY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunPointY)
	{
		return IMVS_EC_PARAM;
	}

    nRet =  VmModule_GetInputScalar_32f(hInput, "FixtureRunAngle", &(fixRun->basis_angle), &nStatusFixRunAngle);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunAngle)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunScaleX", &(fixRun->basis_scale_x), &nStatusFixRunScaleX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunScaleX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunScaleY", &(fixRun->basis_scale_y), &nStatusFixRunScaleY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunScaleY)
	{
		return IMVS_EC_PARAM;
	}

	//当手动输入太大的角度值时，会卡在循环减360中，需要做限制
	if (fixInit->basis_angle > 10000 || fixInit->basis_angle < -10000
		|| fixRun->basis_angle > 10000 || fixRun->basis_angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (fixInit->basis_angle > 180.0f)
	{
		fixInit->basis_angle -= 360.0f;
	}

	while (fixInit->basis_angle < -180.0f)
	{
		fixInit->basis_angle += 360.0f;
	}

	while (fixRun->basis_angle > 180.0f)
	{
		fixRun->basis_angle -= 360.0f;
	}

	while (fixRun->basis_angle < -180.0f)
	{
		fixRun->basis_angle += 360.0f;
	}

	*fixStatus = nStatusFixInitPointX & nStatusFixInitPointY & nStatusFixInitAngle
        & nStatusFixRunPointX & nStatusFixRunPointY & nStatusFixRunAngle;

	fixInit->basis_scale_x = fixInit->basis_scale_x == 0 ? 1 : fixInit->basis_scale_x;
	fixInit->basis_scale_y = fixInit->basis_scale_y == 0 ? 1 : fixInit->basis_scale_y;
	fixRun->basis_scale_x = fixRun->basis_scale_x == 0 ? 1 : fixRun->basis_scale_x;
	fixRun->basis_scale_y = fixRun->basis_scale_y == 0 ? 1 : fixRun->basis_scale_y;
    return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
								HKA_SIZE_I     imgSize,
								vector<MVBI_BOX_F> &boxList,
								HKA_S32       *boxStatus,
								bool          multipleROI)
{
	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus, IMVS_EC_PARAM);

	HKA_S32    nRet = IMVS_EC_OK;
	MVBI_BOX_F roiBox = { 0.0f };
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    countX = 0;
	HKA_U32    countY = 0;
	HKA_U32    countH = 0;
	HKA_U32    countW = 0;
	HKA_U32    countA = 0;
	HKA_U32    count = 0;
	HKA_S32    status = IMVS_ROI_FROM_ERROR;

	boxList.clear();
	*boxStatus = IMVS_ROI_FROM_ERROR;

	nRet = VmModule_GetInputVectorCount(hInput,
		"roicenterx",
		&countX,
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputVectorCount(hInput,
		"roicentery",
		&countY,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputVectorCount(hInput,
		"roiwidth",
		&countW,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputVectorCount(hInput,
		"roiheight",
		&countH,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputVectorCount(hInput,
		"roiangle",
		&countA,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	if ((IMVS_EC_OK != nRet)
		|| ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // 都没有订阅
		)
		)
	{
		status = IMVS_ROI_FROM_UI;
	}
	else
	{
		status = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}

		count = HKA_MIN(countX, countY);
		count = HKA_MIN(countH, count);
		count = HKA_MIN(countW, count);
		count = HKA_MIN(countA, count);
		count = HKA_MIN(count, IMVS_MULTIPLE_ROI_COUNT_MAX);

		for (size_t i = 0; i < count; i++)
		{
			nRet = VmModule_GetInputVectorIndex_32f(hInput,
				"roicenterx",
				&roiBox.center_x,
				i,
				&nStatusRoiCenterX);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roicentery",
				&roiBox.center_y,
				i,
				&nStatusRoiCenterY);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiwidth",
				&roiBox.width,
				i,
				&nStatusRoiWidth);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiheight",
				&roiBox.height,
				i,
				&nStatusRoiHeight);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiangle",
				&roiBox.angle,
				i,
				&nStatusRoiAngle);

			//当手动输入太大的角度值时，会卡在循环减360中，需要做限制
			if (roiBox.angle > 10000 || roiBox.angle < -10000)
			{
				return IMVS_EC_PARAM;
			}

			while (roiBox.angle > 180.0f)
			{
				roiBox.angle -= 360.0f;
			}

			while (roiBox.angle < -180.0f)
			{
				roiBox.angle += 360.0f;
			}

			if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
				) // 无效
			{
				roiBox.center_x = 0.0f;
				roiBox.center_y = 0.0f;
				roiBox.width = 0.0f;
				roiBox.height = 0.0f;
				roiBox.angle = 0.0f;

				return IMVS_EC_PARAM;
			}

			if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
			{
				roiBox.angle = 0.0f;
			}

			boxList.push_back(roiBox);

			if (!multipleROI)
			{
				//非多ROI则跳出
				break;
			}
		}
	}

	*boxStatus = status;

	return IMVS_EC_OK;
}


HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_ANNULUS_F> &annulusList,
	HKA_S32       *annulusStatus,
	bool          multipleROI)
{
	HKA_S32        nRet = IMVS_EC_OK;
	MVBI_ANNULUS_F roiAnnulus = { 0.0f };
	HKA_U32        nStatusCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusInnerRadius = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusOuterRadius = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusStartAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusAngleSpan = IMVS_MODU_ENUM_STATUS_ERROR;

	HKA_U32    count1 = 0;
	HKA_U32    count2 = 0;
	HKA_U32    count3 = 0;
	HKA_U32    count4 = 0;
	HKA_U32    count5 = 0;
	HKA_U32    count6 = 0;
	HKA_U32    count = 0;

	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == annulusStatus, IMVS_EC_PARAM);

	annulusList.clear();
	*annulusStatus = IMVS_ROI_FROM_ERROR;

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusCenterX",
		&count1,
		&nStatusCenterX);

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusCenterY",
		&count2,
		&nStatusCenterY);

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusInnerRadius",
		&count3,
		&nStatusInnerRadius);

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusOuterRadius",
		&count4,
		&nStatusOuterRadius);

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusStartAngle",
		&count5,
		&nStatusStartAngle);

	nRet = VmModule_GetInputVectorCount(hInput,
		"roiAnnulusAngleExtend",
		&count6,
		&nStatusAngleSpan);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	if ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusInnerRadius)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusOuterRadius)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusStartAngle)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusAngleSpan))
	{
		*annulusStatus = IMVS_ROI_FROM_UI;
	}
	else
	{
		*annulusStatus = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusCenterY)
			//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusOuterRadius))
			//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)
			//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan))
		{
			return IMVS_EC_PARAM;
		}

		count = HKA_MIN(count1, count2);
		count = HKA_MIN(count3, count);
		count = HKA_MIN(count4, count);
		count = HKA_MIN(count5, count);
		count = HKA_MIN(count6, count);
		count = HKA_MIN(count, IMVS_MULTIPLE_ROI_COUNT_MAX);

		for (int i = 0; i < count; i++)
		{
			nRet = VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusCenterX",
				&roiAnnulus.center.x,
				i,
				&nStatusCenterX);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusCenterY",
				&roiAnnulus.center.y,
				i,
				&nStatusCenterY);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusInnerRadius",
				&roiAnnulus.inner_radius,
				i,
				&nStatusInnerRadius);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusOuterRadius",
				&roiAnnulus.outer_radius,
				i,
				&nStatusOuterRadius);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusStartAngle",
				&roiAnnulus.start_angle,
				i,
				&nStatusStartAngle);

			nRet |= VmModule_GetInputVectorIndex_32f(hInput,
				"roiAnnulusAngleExtend",
				&roiAnnulus.angle_extend,
				i,
				&nStatusAngleSpan);

			if ((IMVS_MODU_ENUM_STATUS_OK != nStatusCenterX)
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusCenterY)
				//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusOuterRadius))
				//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)
				//|| (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan))
			{
				return IMVS_EC_PARAM;
			}

			if (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)
			{
				roiAnnulus.inner_radius = 0.0f;
			}
			if (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)
			{
				roiAnnulus.start_angle = 0.0f;
			}
			if (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan)
			{
				roiAnnulus.angle_extend = 360.0f;
			}

			//当手动输入太大的角度值时，会卡在循环减360中，需要做限制
			if (roiAnnulus.start_angle > 10000 || roiAnnulus.angle_extend > 10000 || roiAnnulus.start_angle < -10000 || roiAnnulus.angle_extend < -10000)
			{
				return IMVS_EC_PARAM;
			}
			while (roiAnnulus.start_angle > 180.0f)
			{
				roiAnnulus.start_angle -= 360.0f;
			}

			while (roiAnnulus.start_angle < -180.0f)
			{
				roiAnnulus.start_angle += 360.0f;
			}

			while (roiAnnulus.angle_extend > 360.0f)
			{
				roiAnnulus.angle_extend = 360.0f;
			}

			while (roiAnnulus.angle_extend < -360.0f)
			{
				roiAnnulus.angle_extend = -360.0f;
			}

			annulusList.push_back(roiAnnulus);

			if (!multipleROI)
			{
				//非多ROI则跳出
				break;
			}
		}
	}

	return IMVS_EC_OK;
}


HKA_S32 VmModule_OutputScalar_32f(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_F32               val,
                                  const char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
   
    int  nRet                = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetFloat(hOutput, xmlName, index, val);
    return nRet;
}

HKA_S32 VmModule_OutputScalar_32i(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_S32               val,
                                  const char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
    int  nRet = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetInt(hOutput, xmlName, index, val);
    return nRet;
}


HKA_S32 VmModule_OutputString_char(IN const void * const hOutput,
                                   HKA_U32               status,
                                   char                 *val,
                                   const char                 *xmlName,
                                   HKA_S32               index,
                                   HKA_S32               num)
{
    int nRet = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetString(hOutput, xmlName, index, val);
    return nRet;
}


HKA_S32 VmModule_OutputVector_32f(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_F32              *vector,
                                  const char           *xmlName,
                                  HKA_S32               num)
{
    int  nRet = IMVS_EC_OK;
	nRet = VM_M_BatchSetFloat(hOutput, xmlName, vector, num);
    return nRet;
}

HKA_S32 VmModule_OutputVector_8u(IN const void * const hOutput,
								  HKA_U32               status,
								  unsigned char*        vector,
                                  const char                 *xmlName,
								  HKA_S32               num)
{
	int  nRet                = IMVS_EC_OK;
	char strOutputValue[256] = {0};
	int i = 0;

	sprintf_s(strOutputValue, 
		sizeof(strOutputValue),
		"%s", 
		vector);

	nRet = VM_M_SetString(hOutput, 
		xmlName, 
		i, 
		strOutputValue);
	if(IMVS_EC_OK != nRet)
	{
		return IMVS_EC_UNKNOWN;
	}

	return nRet;
}


HKA_S32 VmModule_OutputVector_32i(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_S32              *vector,
                                  const char                 *xmlName,
                                  HKA_S32               num)
{
    int  nRet                = IMVS_EC_OK;
	nRet = VM_M_BatchSetInt(hOutput, xmlName, vector, num);
    return nRet;
}

/***************************************************************************************************
* 函数名：iMVS6000_OutputImageAddress
* 功  能：算法平台输出数组
* 参  数：*
*         hOutput     - IO 模块输出
*         status      - I  状态
*         imageData   - I  图像指针
*         dataLen     - I  数据长度
*         xmlName     - I  对应XML里面的名称
*         
* 返回值：0：正确；其他：错误
* 备  注：
***************************************************************************************************/
HKA_S32 VmModule_OutputImageAddress_8u(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_U08              *imageData,
                                       HKA_S32               dataLen,
                                       const char                 *xmlName)
{
    int  nRet                = IMVS_EC_UNKNOWN;

	IMAGE_DATA struImageData = { 0 };
	memset(&struImageData, 0, sizeof(struImageData));
	struImageData.pImgData    = (char*)imageData;
	struImageData.nImgDataLen = dataLen;
	nRet = VM_M_SetImage(hOutput, xmlName, 0, &struImageData);
   
    return nRet;
}

HKA_S32 VmModule_OutputImageByName_8u_C1R(IN const void * const hOutput,
										  HKA_U32               status,
										  const char          *strImage,
                                          const char          *strWidth,
                                          const char          *strHeight,
                                          const char          *strFormat,
										  HKA_IMAGE     *image)
{
	HKA_CHECK_ERROR(HKA_NULL == hOutput,    IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strImage,   IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strWidth,   IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strHeight,  IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strFormat,  IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == image,      IMVS_EC_PARAM);


	int     nRet                = IMVS_EC_UNKNOWN;
	HKA_S32 imgLen              = 0;

	imgLen = image->height * image->width;

	IMAGE_DATA struImageData = { 0 };
	memset(&struImageData, 0, sizeof(struImageData));
	struImageData.pImgData    = (char*)image->data[0];
	struImageData.nImgDataLen = imgLen;

	nRet = VM_M_SetImage(hOutput, strImage, 0, &struImageData);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strWidth, 0, image->width);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strHeight, 0, image->height);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strFormat, 0, IMVS_PIX_MONO8);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	return nRet;
}

//增加彩图输出---2019-08-07
HKA_S32 VmModule_OutputImageByName_8u_C3R(IN const void * const hOutput,
	HKA_U32		  status,
    const char          *strImage,
    const char          *strWidth,
    const char          *strHeight,
    const char          *strFormat,
	HKA_IMAGE     *image)
{
	HKA_CHECK_ERROR(HKA_NULL == hOutput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strImage, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strWidth, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strHeight, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strFormat, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == image, IMVS_EC_PARAM);

	int     nRet = IMVS_EC_UNKNOWN;
	HKA_S32 imgLen = 0;

	imgLen = image->height * image->width * 3;

	IMAGE_DATA struImageData = { 0 };
	memset(&struImageData, 0, sizeof(struImageData));
	struImageData.pImgData = (char*)image->data[0];
	struImageData.nImgDataLen = imgLen;

	nRet = VM_M_SetImage(hOutput, strImage, 0, &struImageData);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strWidth, 0, image->width);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strHeight, 0, image->height);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strFormat, 0, /*IMVS_PIX_BGR8_PACKED*/IMVS_PIX_RGB8_PACKED);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	return nRet;
}

/***************************************************************************************************
* 函数名：VmModule_OutputImageByName_Mvd
* 功  能：算法平台输出算子SDK格式图像数据
* 参  数：*
*         hOutput     - IO 模块输出
*         status      - I  状态
*         image       - I  图像数据
*
* 返回值：0：正确；其他：错误
* 备  注：`
***************************************************************************************************/
HKA_S32 VmModule_OutputImageByName_Mvd(IN const void * const hOutput,
	HKA_U32 status,
    const char          *strImage,
    const char          *strWidth,
    const char          *strHeight,
    const char          *strFormat,
	IMvdImage *image)
{
	HKA_CHECK_ERROR(HKA_NULL == hOutput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strImage, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strWidth, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strHeight, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == strFormat, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == image, IMVS_EC_PARAM);
	
	int     nRet = IMVS_EC_UNKNOWN;
	HKA_S32 imgLen = 0;
	int pixelFormat = IMVS_PIX_MONO8;
	int imageHeight = image->GetHeight();
	int imageWidth = image->GetWidth();
	
	IMAGE_DATA struImageData = { 0 };
	memset(&struImageData, 0, sizeof(struImageData));

	if (MVD_PIXEL_MONO_08 == image->GetPixelFormat())
	{
		struImageData.pImgData = (char*)image->GetImageData()->stDataChannel[0].pData;
		imgLen = imageHeight * imageWidth;
		pixelFormat = IMVS_PIX_MONO8;
	}
	else if (MVD_PIXEL_RGB_RGB24_C3 == image->GetPixelFormat())
	{
		struImageData.pImgData = (char*)image->GetImageData()->stDataChannel[0].pData;
		imgLen = 3 * imageHeight * imageWidth;
		pixelFormat = IMVS_PIX_RGB8_PACKED;
	}

	struImageData.nImgDataLen = imgLen;
	
	nRet = VM_M_SetImage(hOutput, strImage, 0, &struImageData);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strWidth, 0, imageWidth);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strHeight, 0, imageHeight);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	nRet = VM_M_SetInt(hOutput, strFormat, 0, pixelFormat);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	return nRet;
}

CStringA UNICODEtoUTF8(const wchar_t* strUNICODE)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = WideCharToMultiByte(CP_UTF8, NULL, strUNICODE, -1, NULL, NULL, NULL, NULL);
	char *szBuffer = new char[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, strUNICODE, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	CStringA strUTF8 = szBuffer;
	delete[] szBuffer;
	return strUTF8;
}

CString UTF8toUNICODE(const char* strUTF8)
{
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	TCHAR *szBuffer = new TCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, szBuffer, nLen);
	szBuffer[nLen] = 0;
	CString cstrUTF8 = szBuffer;
	delete[] szBuffer;
	return cstrUTF8;
}

CString VmModule_GetModuleName()
{
	wchar_t chszModuleName[MAX_PATH] = { 0 };
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&VmModule_GetModuleName, //这是函数名，强转
        &hModule);
    DWORD len = GetModuleFileName(hModule, chszModuleName, MAX_PATH);
	CString cstrModuleName = chszModuleName;
	return cstrModuleName.Right(cstrModuleName.GetLength() - cstrModuleName.ReverseFind('\\') - 1);
}

CString VmModule_GetModulePath()
{
	wchar_t chszModulePath[MAX_PATH] = { 0 };
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCWSTR)&VmModule_GetModuleName, //这是函数名，强转
		&hModule);
	DWORD len = GetModuleFileName(hModule, chszModulePath, MAX_PATH);
	CString cstrModulePath = chszModulePath;
	return cstrModulePath.Left(cstrModulePath.ReverseFind('\\') + 1);
}

CString VmModule_GetXmlPath()
{
    CString cstrModuleName = VmModule_GetModuleName();
    cstrModuleName.Replace(L".DLL",L"");
    cstrModuleName.Replace(L".dll",L"");
    return cstrModuleName + L"Algorithm.xml";
}

CString VmModule_GetModuleNameNoPostfix()
{
    CString cstrModuleName = VmModule_GetModuleName();
    cstrModuleName.Replace(L".DLL", L"");
    cstrModuleName.Replace(L".dll", L"");
    return cstrModuleName;
}

void VmModule_ReportError( void* hModule, int nRet )
{
    //TODO: 上报错误码，继续执行后续功能
    int nErrType = IMVS_EC_MODULE_ALGORITHM_INIT ;
    char chszErrMsg[256] = {0};
    _snprintf_s( chszErrMsg, sizeof(chszErrMsg), sizeof(chszErrMsg) - 1, "%d", nRet );
    VM_M_ReportData(hModule, nErrType, chszErrMsg,256);
}

/***************************************************************************************************
* 函数名：iMVS6000_OutputImage_8u_C1R
* 功  能：算法平台输出单通道图像
* 参  数：*
*         hOutput     - IO 模块输出
*         status      - I  状态
*         image       - I  图像
*         
* 返回值：0：正确；其他：错误
* 备  注：
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_8u_C1R(IN const void * const hOutput, HKA_U32 status, HKA_IMAGE *image)
{
	return VmModule_OutputImageByName_8u_C1R(hOutput,status,"OutImage","OutImageWidth","OutImageHeight","OutImagePixelFormat",image);
}

//修改为调用彩图输出---2019-08-07
HKA_S32 VmModule_OutputDstImage_8u_C3R(IN const void * const hOutput, HKA_U32 status, HKA_IMAGE *image)
{
	return VmModule_OutputImageByName_8u_C3R(hOutput, status, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", image);
}

/***************************************************************************************************
* 函数名：VmModule_OutputDstImage_Mvd
* 功  能：算法平台输出算子SDK格式图像数据
* 参  数：*
*         hOutput     - IO 模块输出
*         status      - I  状态
*         image       - I  图像数据
*
* 返回值：0：正确；其他：错误
* 备  注：`
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_Mvd(IN const void * const hOutput,
	HKA_U32 status,
	IMvdImage *image)
{
	return VmModule_OutputImageByName_Mvd(hOutput, status, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", image);
}

/***************************************************************************************************
* 函数名：VmModule_OutputMaskImage_Mvd
* 功  能：算法平台输出算子SDK格式图像数据
* 参  数：*
*         hOutput     - IO 模块输出
*         status      - I  状态
*         image       - I  图像数据
*
* 返回值：0：正确；其他：错误
* 备  注：`
***************************************************************************************************/
HKA_S32 VmModule_OutputMaskImage_Mvd(IN const void * const hOutput,
    HKA_U32 status,
    IMvdImage *image)
{
    return VmModule_OutputImageByName_Mvd(hOutput, status, "OutMask", "OutMaskWidth", "OutMaskHeight", "OutMaskPixelFormat", image);
}

HKA_S32 VmModule_GetInputVectorIndex_32f(IN const void * const hInput,
                                         const char          *xmlName,
                                         HKA_F32       *val,
                                         HKA_U32        index,
                                         HKA_U32       *status)
{
	HKA_U32 nCount      = 0;
	HKA_U32 nStatus     = 0;
	HKA_S32 nRet        = IMVS_EC_OK;
	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == val, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == status, IMVS_EC_PARAM);
	*status = IMVS_MODU_ENUM_STATUS_ERROR;
	nRet = VM_M_GetFloat(hInput, 
		xmlName, 
		index, 
		val, 
		(int*)&nCount);
	ErrorCodeToStatus(nRet, status);
	return nRet;
}
