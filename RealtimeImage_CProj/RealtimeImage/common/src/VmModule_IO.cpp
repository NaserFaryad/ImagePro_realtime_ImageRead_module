#include "VmModule_IO.h"
#include "iMVS-6000PixelFormatDefine.h"
#include "VmModuleBase.h"
#include "ErrorCodeDefine.h"
#include "hka_defs.h"
#include <string>

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
                                    char          *xmlName,
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

    //return nRet;
	return IMVS_EC_OK;
}


HKA_S32 VmModule_GetInputScalar_32i(IN const void * const hInput,
                                    char          *xmlName,
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



HKA_S32 VmModule_GetInputScalar_32u(IN const void * const hInput,
                                    char          *xmlName,
                                    HKA_U32       *val,
                                    HKA_U32       *status)
{
    HKA_S32 nCount      = 0;
    HKA_U32 nStatus     = 0;
    HKA_S32 nRet        = IMVS_EC_UNKNOWN;

    HKA_CHECK_ERROR(HKA_NULL == hInput,  IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == xmlName, IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == val,     IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == status,  IMVS_EC_PARAM);

	nRet = VM_M_GetInt(hInput, xmlName, 0, (int*)val, &nCount);

	ErrorCodeToStatus(nRet, status);

    return nRet;
}


HKA_S32 VmModule_GetInputImageAddress(IN const void * const hInput,
                                      char          *xmlName,
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
    char          *xmlName,
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
* ????????iMVS6000_GetInputVectorCount
* ??  ????????????????????????????????
* ??  ????*
*         hInput      - I ????????
*         xmlName     - I ????XML??????????
*         count       - O ????????
*         status      - O ????
*         
* ????????0??????????????????
* ??  ????
***************************************************************************************************/
HKA_S32 VmModule_GetInputVectorCount(IN const void * const hInput,
                                     char          *xmlName,
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
                                    char          *xmlName,
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
                                    char          *xmlName,
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


HKA_S32 VmModule_GetInputVector_32u(IN const void * const hInput,
                                    char          *xmlName,
                                    HKA_U32       *val,
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

	    val[0] = HKA_U32(nValue);

		for(HKA_U32 i = 1; i < num; i++)
		{
			nRet = VM_M_GetInt(hInput, xmlName, i, &nValue, &nCount);
			if (nRet != IMVS_EC_OK)
			{
				break;
			}
			val[i] = (HKA_U32)(nValue);
		}

	} while (false);

	ErrorCodeToStatus(nRet, status);

	return nRet;
}




HKA_S32 VmModule_GetInputImage(IN const void * const hInput,
                               HKA_IMAGE     *image,
							   HKA_U32       *imageStatus)
{
    int iImageDataLen = 0;
    return VmModule_GetInputImageEx(hInput, image, iImageDataLen, imageStatus);
}

HKA_S32 VmModule_GetInputImageEx(IN const void * const hInput,
    HKA_IMAGE     *image,
    HKA_S32&      iImageDataLen,
	HKA_U32       *imageStatus)
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

HKA_S32 VmModule_GetInputImageByName(IN const void * const hInput,
                                     char          *strImage,
                                     char          *strWidth,
                                     char          *strHeight,
                                     char          *strFormat,
                                     HKA_IMAGE     *image,
									 HKA_U32       *imageStatus)
{
    HKA_S32          nRet          = IMVS_EC_UNKNOWN;
    HKA_S32          format        = 0;
    HKA_IMAGE_FORMAT formatAlg     = HKA_IMG_MONO_08;
    HKA_U32          nStatusImage  = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusWidth  = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusHeight = IMVS_MODU_ENUM_STATUS_ERROR;
    HKA_U32          nStatusFormat = IMVS_MODU_ENUM_STATUS_ERROR;
    char*			 addr          = 0;

    HKA_CHECK_ERROR(HKA_NULL == hInput,      IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == image,       IMVS_EC_PARAM);
    HKA_CHECK_ERROR(HKA_NULL == imageStatus, IMVS_EC_PARAM);

    nRet = VmModule_GetInputImageAddress(hInput, strImage, &addr, &nStatusImage);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    nRet = VmModule_GetInputScalar_32i(hInput, strWidth, &(image->width), &nStatusWidth);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    nRet = VmModule_GetInputScalar_32i(hInput, strHeight, &(image->height), &nStatusHeight);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    nRet = VmModule_GetInputScalar_32i(hInput, strFormat, &format, &nStatusFormat);
    HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

    if(image->height <= 0)
    {
        nStatusHeight = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    if(image->width <= 0)
    {
        nStatusWidth = IMVS_MODU_ENUM_STATUS_INPUT_INVALID;
    }

    *imageStatus =   (IMVS_MODU_ENUM_STATUS_OK == nStatusImage)
                  && (IMVS_MODU_ENUM_STATUS_OK == nStatusWidth)
                  && (IMVS_MODU_ENUM_STATUS_OK == nStatusHeight)
                  && (IMVS_MODU_ENUM_STATUS_OK == nStatusFormat);

    nRet = VmModule_iMVSFormatToAlgFormat(format, &formatAlg);
    HKA_CHECK_ERROR(HKA_TRUE != nRet, nRet);

    image->format  = formatAlg;
    image->data[0] = addr;
    image->step[0] = (HKA_IMG_RGB_RGB24_C3 == formatAlg) ? (3 * image->width) : image->width;

    return IMVS_EC_OK;
}


HKA_S32 VmModule_GetInputFixture(IN const void * const hInput,
                                 MVBI_FIDUCIAL_POSE   *fixInit,
                                 MVBI_FIDUCIAL_POSE   *fixRun,
								 HKA_U32              *fixStatus)
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

	//??????????????????????????????????????360??????????????
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

	*fixStatus = nStatusFixInitPointX & nStatusFixInitPointY & nStatusFixInitAngle;

	fixInit->basis_scale_x = fixInit->basis_scale_x == 0 ? 1 : fixInit->basis_scale_x;
	fixInit->basis_scale_y = fixInit->basis_scale_y == 0 ? 1 : fixInit->basis_scale_y;
	fixRun->basis_scale_x = fixRun->basis_scale_x == 0 ? 1 : fixRun->basis_scale_x;
	fixRun->basis_scale_y = fixRun->basis_scale_y == 0 ? 1 : fixRun->basis_scale_y;
    return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputFixture_Mvd(IN const void * const hInput,
	MVD_FIDUCIAL_POINT_F   *fixInit,
	MVD_FIDUCIAL_POINT_F   *fixRun,
	HKA_U32              *fixStatus)
{
	HKA_S32  nRet = IMVS_EC_UNKNOWN;
	HKA_U32  nStatusFixInitPointX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitPointY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitScaleX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitScaleY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixInitAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunPointX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunPointY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunScaleX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunScaleY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32  nStatusFixRunAngle = IMVS_MODU_ENUM_STATUS_ERROR;

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitPointX", &(fixInit->stPosition.fX), &nStatusFixInitPointX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitPointX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitPointY", &(fixInit->stPosition.fY), &nStatusFixInitPointY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitPointY)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitAngle", &(fixInit->fAngle), &nStatusFixInitAngle);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitAngle)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitScaleX", &(fixInit->fScaleX), &nStatusFixInitScaleX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitScaleX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureInitScaleY", &(fixInit->fScaleY), &nStatusFixInitScaleY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixInitScaleY)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunPointX", &(fixRun->stPosition.fX), &nStatusFixRunPointX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunPointX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunPointY", &(fixRun->stPosition.fY), &nStatusFixRunPointY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunPointY)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunAngle", &(fixRun->fAngle), &nStatusFixRunAngle);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunAngle)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunScaleX", &(fixRun->fScaleX), &nStatusFixRunScaleX);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunScaleX)
	{
		return IMVS_EC_PARAM;
	}

	nRet = VmModule_GetInputScalar_32f(hInput, "FixtureRunScaleY", &(fixRun->fScaleY), &nStatusFixRunScaleY);
	if (IMVS_MODU_ENUM_STATUS_ERROR == nStatusFixRunScaleY)
	{
		return IMVS_EC_PARAM;
	}

	//??????????????????????????????????????360??????????????
	if (fixInit->fAngle > 10000 || fixInit->fAngle < -10000
		|| fixRun->fAngle > 10000 || fixRun->fAngle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (fixInit->fAngle > 180.0f)
	{
		fixInit->fAngle -= 360.0f;
	}

	while (fixInit->fAngle < -180.0f)
	{
		fixInit->fAngle += 360.0f;
	}

	while (fixRun->fAngle > 180.0f)
	{
		fixRun->fAngle -= 360.0f;
	}

	while (fixRun->fAngle < -180.0f)
	{
		fixRun->fAngle += 360.0f;
	}

	*fixStatus = nStatusFixInitPointX & nStatusFixInitPointY & nStatusFixInitAngle;

	fixInit->fScaleX = fixInit->fScaleX == 0 ? 1 : fixInit->fScaleX;
	fixInit->fScaleY = fixInit->fScaleY == 0 ? 1 : fixInit->fScaleY;
	fixRun->fScaleX = fixRun->fScaleX == 0 ? 1 : fixRun->fScaleX;
	fixRun->fScaleY = fixRun->fScaleY == 0 ? 1 : fixRun->fScaleY;
	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus,
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
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
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

			//??????????????????????????????????????360??????????????
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
				) // ????
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
				//????ROI??????
				break;
			}
		}
	}

	*boxStatus = status;

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
                                IMVS_ROI_BOX  *defaultBox,
                                HKA_SIZE_I     imgSize,
                                MVBI_BOX_F    *box,
								HKA_U32       *boxStatus)
{
	HKA_S32    nRet              = IMVS_EC_OK;
	MVBI_BOX_F roiBox            = {0.0f};
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight  = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status            = HKA_FALSE;

	HKA_CHECK_ERROR(HKA_NULL == hInput,     IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box,        IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus,  IMVS_EC_PARAM);

	*boxStatus = IMVS_ROI_FROM_ERROR;

	nRet = VmModule_GetInputScalar_32f(hInput, 
		"roicenterx", 
		&roiBox.center_x, 
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roicentery", 
		&roiBox.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiwidth", 
		&roiBox.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"roiheight", 
		&roiBox.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiangle",
		&roiBox.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
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

	if (   (IMVS_EC_OK != nRet)
		|| (   (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight) 
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status = IMVS_ROI_FROM_UI;
	}
	else
	{
		status = IMVS_ROI_FROM_INPUT;

		if(    (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}

        //xxxx111  ??????????????????????????
	}

	if (IMVS_ROI_FROM_UI == status)
	{
		roiBox.center_x  = defaultBox->fRoiCenterX * imgSize.width;
		roiBox.center_y  = defaultBox->fRoiCenterY * imgSize.height;
		roiBox.width     = defaultBox->fRoiWidth * imgSize.width;
		roiBox.height    = defaultBox->fRoiHeight * imgSize.height;
		roiBox.angle     = defaultBox->fRoiAngle;
	}
	else
	{
		if (   (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox.center_x = 0.0f;
			roiBox.center_y = 0.0f;
			roiBox.width    = 0.0f;
			roiBox.height   = 0.0f;
			roiBox.angle    = 0.0f;

			return IMVS_EC_PARAM;
		}

        //todo ??????????xxxx111????????????
		if(IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox.angle = 0.0f;
		} 
	}

	*boxStatus = status;

	*box = roiBox;

	return IMVS_EC_OK;
}


HKA_S32 VmModule_GetInputDoubleBox(IN const void * const hInput,
                                   IMVS_ROI_BOX  *defaultBox1,
                                   IMVS_ROI_BOX  *defaultBox2,
                                   HKA_SIZE_I     imgSize,
                                   MVBI_BOX_F    *box1,
                                   MVBI_BOX_F    *box2,
								   HKA_U32       *boxStatus)
{
	HKA_S32    nRet              = IMVS_EC_OK;
	MVBI_BOX_F roiBox1           = {0.0f};
	MVBI_BOX_F roiBox2           = {0.0f};
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight  = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status1           = HKA_FALSE;
	HKA_S32    status2           = HKA_FALSE;

	HKA_CHECK_ERROR(HKA_NULL == hInput,      IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox1, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box1,        IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox2, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box2,        IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus,   IMVS_EC_PARAM);

	*boxStatus = IMVS_ROI_FROM_ERROR;

	nRet = VmModule_GetInputScalar_32f(hInput, 
		"Roi1CenterX", 
		&roiBox1.center_x, 
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi1CenterY", 
		&roiBox1.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi1Width", 
		&roiBox1.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi1Height", 
		&roiBox1.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi1Angle",
		&roiBox1.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox1.angle > 10000 || roiBox1.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox1.angle > 180.0f)
	{
		roiBox1.angle -= 360.0f;
	}

	while (roiBox1.angle < -180.0f)
	{
		roiBox1.angle += 360.0f;
	}

	if (   (IMVS_EC_OK != nRet)
		|| (   (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight) 
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status1 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status1 = IMVS_ROI_FROM_INPUT;

		if(    (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status1)
	{
		roiBox1.center_x  = defaultBox1->fRoiCenterX * imgSize.width;
		roiBox1.center_y  = defaultBox1->fRoiCenterY * imgSize.height;
		roiBox1.width     = defaultBox1->fRoiWidth * imgSize.width;
		roiBox1.height    = defaultBox1->fRoiHeight * imgSize.height;
		roiBox1.angle     = defaultBox1->fRoiAngle;
	}
	else
	{
		if (   (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox1.center_x = 0.0f;
			roiBox1.center_y = 0.0f;
			roiBox1.width    = 0.0f;
			roiBox1.height   = 0.0f;
			roiBox1.angle    = 0.0f;
		}

		if(IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox1.angle = 0.0f;
		} 
	}

	nRet = VmModule_GetInputScalar_32f(hInput, 
		"Roi2CenterX", 
		&roiBox2.center_x, 
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi2CenterY", 
		&roiBox2.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi2Width", 
		&roiBox2.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi2Height", 
		&roiBox2.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"Roi2Angle",
		&roiBox2.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox2.angle > 10000 || roiBox2.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox2.angle > 180.0f)
	{
		roiBox2.angle -= 360.0f;
	}

	while (roiBox2.angle < -180.0f)
	{
		roiBox2.angle += 360.0f;
	}

	if (   (IMVS_EC_OK != nRet)
		|| (   (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight) 
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status2 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status2 = IMVS_ROI_FROM_INPUT;

		if(    (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status2)
	{
		//????????????????????????????ROI
		if(IMVS_ROI_FROM_INPUT == status1)
		{
			roiBox2 = roiBox1;
		}
		else
		{
			roiBox2.center_x  = defaultBox2->fRoiCenterX * imgSize.width;
			roiBox2.center_y  = defaultBox2->fRoiCenterY * imgSize.height;
			roiBox2.width     = defaultBox2->fRoiWidth * imgSize.width;
			roiBox2.height    = defaultBox2->fRoiHeight * imgSize.height;
			roiBox2.angle     = defaultBox2->fRoiAngle;
		}

	}
	else
	{
		if (   (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox2.center_x = 0.0f;
			roiBox2.center_y = 0.0f;
			roiBox2.width    = 0.0f;
			roiBox2.height   = 0.0f;
			roiBox2.angle    = 0.0f;
		}

		if(IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox2.angle = 0.0f;
		} 
	}

	*boxStatus = status1 | status2;

	*box1 = roiBox1;

	*box2 = roiBox2;

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputQuadraRoiBox(IN const void * const hInput,
	IMVS_ROI_BOX *defaultBox0,
	IMVS_ROI_BOX *defaultBox1,
	IMVS_ROI_BOX *defaultBox2,
	IMVS_ROI_BOX *defaultBox3,
	HKA_SIZE_I imgSize,
	MVBI_BOX_F *box0,
	MVBI_BOX_F *box1,
	MVBI_BOX_F *box2,
	MVBI_BOX_F *box3,
	HKA_U32 *boxStatus)
{
	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox0, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox1, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox2, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox3, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box0, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box1, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box2, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == box3, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus, IMVS_EC_PARAM);

	HKA_S32    nRet = IMVS_EC_OK;
	MVBI_BOX_F roiBox0 = { 0.0f };
	MVBI_BOX_F roiBox1 = { 0.0f };
	MVBI_BOX_F roiBox2 = { 0.0f };
	MVBI_BOX_F roiBox3 = { 0.0f };
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status0 = HKA_FALSE;
	HKA_S32    status1 = HKA_FALSE;
	HKA_S32    status2 = HKA_FALSE;
	HKA_S32    status3 = HKA_FALSE;

	*boxStatus = IMVS_ROI_FROM_ERROR;

	//??????roi
	nRet = VmModule_GetInputScalar_32f(hInput,
		"Roi0CenterX",
		&roiBox0.center_x,
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi0CenterY",
		&roiBox0.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi0Width",
		&roiBox0.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi0Height",
		&roiBox0.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi0Angle",
		&roiBox0.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox0.angle > 10000 || roiBox0.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox0.angle > 180.0f)
	{
		roiBox0.angle -= 360.0f;
	}

	while (roiBox0.angle < -180.0f)
	{
		roiBox0.angle += 360.0f;
	}

	if ((IMVS_EC_OK != nRet)
		|| ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status0 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status0 = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status0)
	{
		roiBox0.center_x = defaultBox0->fRoiCenterX * imgSize.width;
		roiBox0.center_y = defaultBox0->fRoiCenterY * imgSize.height;
		roiBox0.width = defaultBox0->fRoiWidth * imgSize.width;
		roiBox0.height = defaultBox0->fRoiHeight * imgSize.height;
		roiBox0.angle = defaultBox0->fRoiAngle;
	}
	else
	{
		if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox0.center_x = 0.0f;
			roiBox0.center_y = 0.0f;
			roiBox0.width = 0.0f;
			roiBox0.height = 0.0f;
			roiBox0.angle = 0.0f;
		}

		if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox0.angle = 0.0f;
		}
	}

	//??????roi
	nRet = VmModule_GetInputScalar_32f(hInput,
		"Roi1CenterX",
		&roiBox1.center_x,
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi1CenterY",
		&roiBox1.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi1Width",
		&roiBox1.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi1Height",
		&roiBox1.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi1Angle",
		&roiBox1.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox1.angle > 10000 || roiBox1.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox1.angle > 180.0f)
	{
		roiBox1.angle -= 360.0f;
	}

	while (roiBox1.angle < -180.0f)
	{
		roiBox1.angle += 360.0f;
	}

	if ((IMVS_EC_OK != nRet)
		|| ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status1 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status1 = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status1)
	{
		//????????????????????????????ROI
		if (IMVS_ROI_FROM_INPUT == status0)
		{
			roiBox1 = roiBox0;
		}
		else
		{
			roiBox1.center_x = defaultBox1->fRoiCenterX * imgSize.width;
			roiBox1.center_y = defaultBox1->fRoiCenterY * imgSize.height;
			roiBox1.width = defaultBox1->fRoiWidth * imgSize.width;
			roiBox1.height = defaultBox1->fRoiHeight * imgSize.height;
			roiBox1.angle = defaultBox1->fRoiAngle;
		}

	}
	else
	{
		if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox1.center_x = 0.0f;
			roiBox1.center_y = 0.0f;
			roiBox1.width = 0.0f;
			roiBox1.height = 0.0f;
			roiBox1.angle = 0.0f;
		}

		if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox1.angle = 0.0f;
		}
	}

	//??????roi
	nRet = VmModule_GetInputScalar_32f(hInput,
		"Roi2CenterX",
		&roiBox2.center_x,
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi2CenterY",
		&roiBox2.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi2Width",
		&roiBox2.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi2Height",
		&roiBox2.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi2Angle",
		&roiBox2.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox2.angle > 10000 || roiBox2.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox2.angle > 180.0f)
	{
		roiBox2.angle -= 360.0f;
	}

	while (roiBox2.angle < -180.0f)
	{
		roiBox2.angle += 360.0f;
	}

	if ((IMVS_EC_OK != nRet)
		|| ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status2 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status2 = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status2)
	{
		//????????????????????????????ROI
		if (IMVS_ROI_FROM_INPUT == status0)
		{
			roiBox2 = roiBox0;
		}
		else
		{
			roiBox2.center_x = defaultBox2->fRoiCenterX * imgSize.width;
			roiBox2.center_y = defaultBox2->fRoiCenterY * imgSize.height;
			roiBox2.width = defaultBox2->fRoiWidth * imgSize.width;
			roiBox2.height = defaultBox2->fRoiHeight * imgSize.height;
			roiBox2.angle = defaultBox2->fRoiAngle;
		}

	}
	else
	{
		if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox2.center_x = 0.0f;
			roiBox2.center_y = 0.0f;
			roiBox2.width = 0.0f;
			roiBox2.height = 0.0f;
			roiBox2.angle = 0.0f;
		}

		if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox2.angle = 0.0f;
		}
	}

	//??????roi
	nRet = VmModule_GetInputScalar_32f(hInput,
		"Roi3CenterX",
		&roiBox3.center_x,
		&nStatusRoiCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi3CenterY",
		&roiBox3.center_y,
		&nStatusRoiCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi3Width",
		&roiBox3.width,
		&nStatusRoiWidth);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi3Height",
		&roiBox3.height,
		&nStatusRoiHeight);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"Roi3Angle",
		&roiBox3.angle,
		&nStatusRoiAngle);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	//??????????????????????????????????????360??????????????
	if (roiBox3.angle > 10000 || roiBox3.angle < -10000)
	{
		return IMVS_EC_PARAM;
	}

	while (roiBox3.angle > 180.0f)
	{
		roiBox3.angle -= 360.0f;
	}

	while (roiBox3.angle < -180.0f)
	{
		roiBox3.angle += 360.0f;
	}

	if ((IMVS_EC_OK != nRet)
		|| ((IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiWidth)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiHeight)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status3 = IMVS_ROI_FROM_UI;
	}
	else
	{
		status3 = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiHeight))
		{
			return IMVS_EC_PARAM;
		}
	}

	if (IMVS_ROI_FROM_UI == status3)
	{
		//????????????????????????????ROI
		if (IMVS_ROI_FROM_INPUT == status0)
		{
			roiBox3 = roiBox0;
		}
		else
		{
			roiBox3.center_x = defaultBox3->fRoiCenterX * imgSize.width;
			roiBox3.center_y = defaultBox3->fRoiCenterY * imgSize.height;
			roiBox3.width = defaultBox3->fRoiWidth * imgSize.width;
			roiBox3.height = defaultBox3->fRoiHeight * imgSize.height;
			roiBox3.angle = defaultBox3->fRoiAngle;
		}

	}
	else
	{
		if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
			|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
			) // ????
		{
			roiBox3.center_x = 0.0f;
			roiBox3.center_y = 0.0f;
			roiBox3.width = 0.0f;
			roiBox3.height = 0.0f;
			roiBox3.angle = 0.0f;
		}

		if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
		{
			roiBox3.angle = 0.0f;
		}
	}

	*boxStatus = status0 | status1 | status2 | status3;

	*box0 = roiBox0;

	*box1 = roiBox1;

	*box2 = roiBox2;

	*box3 = roiBox3;

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputMultiRoiBox(IN const void * const hInput,
	IMVS_ROI_BOX  *defaultBox,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus)
{
	HKA_S32    nRet = IMVS_EC_OK;
	MVBI_BOX_F roiBox = { 0.0f };
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status = HKA_FALSE;
	HKA_U32    countX = 0;
	HKA_U32    countY = 0;
	HKA_U32    countH = 0;
	HKA_U32    countW = 0;
	HKA_U32    countA = 0;
	HKA_U32    count = 0;
	HKA_U32    i = 0;

	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus, IMVS_EC_PARAM);

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
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status = IMVS_ROI_FROM_UI;
		roiBox.center_x = defaultBox->fRoiCenterX * imgSize.width;
		roiBox.center_y = defaultBox->fRoiCenterY * imgSize.height;
		roiBox.width = defaultBox->fRoiWidth * imgSize.width;
		roiBox.height = defaultBox->fRoiHeight * imgSize.height;
		roiBox.angle = defaultBox->fRoiAngle;

		boxList.push_back(roiBox);
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


		for (i = 0; i < count; i++)
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

			//??????????????????????????????????????360??????????????
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
				) // ????
			{
				roiBox.center_x = 0.0f;
				roiBox.center_y = 0.0f;
				roiBox.width = 0.0f;
				roiBox.height = 0.0f;
				roiBox.angle = 0.0f;
			}

			if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
			{
				roiBox.angle = 0.0f;
			}

			boxList.push_back(roiBox);

		} // for (i = 0; i < count; i++)
	}

	*boxStatus = status;


	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputMultiRoiBox2(IN const void * const hInput,
	IMVS_ROI_BOX  *defaultBox,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus)
{
	HKA_S32    nRet = IMVS_EC_OK;
	MVBI_BOX_F roiBox = { 0.0f };
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status = HKA_FALSE;
	HKA_U32    countX = 0;
	HKA_U32    countY = 0;
	HKA_U32    countH = 0;
	HKA_U32    countW = 0;
	HKA_U32    countA = 0;
	HKA_U32    count = 0;
	HKA_U32    i = 0;

	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultBox, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == boxStatus, IMVS_EC_PARAM);

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
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		status = IMVS_ROI_FROM_UI;
		roiBox.center_x = defaultBox->fRoiCenterX * imgSize.width;
		roiBox.center_y = defaultBox->fRoiCenterY * imgSize.height;
		roiBox.width = defaultBox->fRoiWidth * imgSize.width;
		roiBox.height = defaultBox->fRoiHeight * imgSize.height;
		roiBox.angle = defaultBox->fRoiAngle;

		//boxList.push_back(roiBox);
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

		count = HKA_MIN(count, 16);//??????????ROI??????????16??

		for (i = 0; i < count; i++)
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

			//??????????????????????????????????????360??????????????
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
				) // ????
			{
				roiBox.center_x = 0.0f;
				roiBox.center_y = 0.0f;
				roiBox.width = 0.0f;
				roiBox.height = 0.0f;
				roiBox.angle = 0.0f;
			}

			if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
			{
				roiBox.angle = 0.0f;
			}

			boxList.push_back(roiBox);

		} // for (i = 0; i < count; i++)
	}

	*boxStatus = status;


	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputMultiRoiAnnulus(IN const void * const hInput,
										 HKA_SIZE_I     imgSize,
										 vector<MVBI_ANNULUS_F> &annulusList,
										 HKA_U32       *nStatus)
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
	HKA_CHECK_ERROR(HKA_NULL == nStatus, IMVS_EC_PARAM);

	*nStatus = IMVS_ROI_FROM_ERROR;

	annulusList.clear();

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
		

		*nStatus = IMVS_ROI_FROM_UI;
	}
	else
	{
		*nStatus = IMVS_ROI_FROM_INPUT;

		if ((IMVS_MODU_ENUM_STATUS_OK != nStatusCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusCenterY)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusOuterRadius)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan))
		{
			return IMVS_EC_PARAM;
		}

		count = HKA_MIN(count1, count2);
		count = HKA_MIN(count3, count);
		count = HKA_MIN(count4, count);
		count = HKA_MIN(count5, count);
		count = HKA_MIN(count6, count);

		count = HKA_MIN(count, 16);//??????????ROI??????????16??

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
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusOuterRadius)
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)
				|| (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan))
			{
				return IMVS_EC_PARAM;
			}

			//??????????????????????????????????????360??????????????
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

		} // for (i = 0; i < count; i++)


	}

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_ANNULUS_F> &annulusList,
	HKA_U32       *annulusStatus,
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

			//??????????????????????????????????????360??????????????
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
				//????ROI??????
				break;
			}
		}
	}

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
	MVBI_ANNULUS_F *defaultAnnulus,
                                    HKA_SIZE_I      imgSize,
                                    MVBI_ANNULUS_F *annulus,
									HKA_U32        *annulusStatus)
{
	HKA_S32        nRet                = IMVS_EC_OK;
	MVBI_ANNULUS_F roiAnnulus          = {0.0f};
	HKA_U32        nStatusCenterX      = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusCenterY      = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusInnerRadius  = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusOuterRadius  = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusStartAngle   = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32        nStatusAngleSpan    = IMVS_MODU_ENUM_STATUS_ERROR;

	HKA_CHECK_ERROR(HKA_NULL == hInput,         IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultAnnulus, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == annulus,        IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == annulusStatus,  IMVS_EC_PARAM);

	*annulusStatus = IMVS_ROI_FROM_ERROR;

	nRet = VmModule_GetInputScalar_32f(hInput, 
		"roiAnnulusCenterX", 
		&roiAnnulus.center.x, 
		&nStatusCenterX);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiAnnulusCenterY", 
		&roiAnnulus.center.y,
		&nStatusCenterY);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiAnnulusInnerRadius", 
		&roiAnnulus.inner_radius,
		&nStatusInnerRadius);

	nRet |= VmModule_GetInputScalar_32f(hInput,
		"roiAnnulusOuterRadius", 
		&roiAnnulus.outer_radius,
		&nStatusOuterRadius);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiAnnulusStartAngle",
		&roiAnnulus.start_angle,
		&nStatusStartAngle);

	nRet |= VmModule_GetInputScalar_32f(hInput, 
		"roiAnnulusAngleExtend",
		&roiAnnulus.angle_extend,
		&nStatusAngleSpan);

	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);


	if (   (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusCenterX)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusCenterY)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusInnerRadius)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusOuterRadius)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusStartAngle)
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusAngleSpan))
	{
		roiAnnulus.center.x     = defaultAnnulus->center.x * imgSize.width;
		roiAnnulus.center.y     = defaultAnnulus->center.y * imgSize.height;
		roiAnnulus.inner_radius = defaultAnnulus->inner_radius * imgSize.width;
		roiAnnulus.outer_radius = defaultAnnulus->outer_radius * imgSize.width;
		roiAnnulus.start_angle  = defaultAnnulus->start_angle;
		roiAnnulus.angle_extend = defaultAnnulus->angle_extend;

		*annulusStatus = IMVS_ROI_FROM_UI;
	}
	else
	{
		*annulusStatus = IMVS_ROI_FROM_INPUT;

		while(roiAnnulus.start_angle > 180.0f)
		{
			roiAnnulus.start_angle -= 360.0f;
		}

		while(roiAnnulus.start_angle < -180.0f)
		{
			roiAnnulus.start_angle += 360.0f;
		}

		while(roiAnnulus.angle_extend > 360.0f)
		{
			roiAnnulus.angle_extend = 360.0f;
		}

		while(roiAnnulus.angle_extend < -360.0f)
		{
			roiAnnulus.angle_extend = -360.0f;
		}


		if(    (IMVS_MODU_ENUM_STATUS_OK != nStatusCenterX)
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusCenterY)
			/*|| (IMVS_MODU_ENUM_STATUS_OK != nStatusInnerRadius)*/
			|| (IMVS_MODU_ENUM_STATUS_OK != nStatusOuterRadius)
			/*|| (IMVS_MODU_ENUM_STATUS_OK != nStatusStartAngle)*/
			/*|| (IMVS_MODU_ENUM_STATUS_OK != nStatusAngleSpan)*/)
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
	}

	*annulus = roiAnnulus;

	return IMVS_EC_OK;
}

HKA_S32 VmModule_GetInputCaliperBox(IN const void * const hInput,
									vector<MVBI_BOX_F> *defaultCaliperBoxList,
									HKA_SIZE_I      imgSize,
									vector<MVBI_BOX_F> *caliperBoxList,
									HKA_U32        *CaliperStatus)
{
	HKA_S32    nRet = IMVS_EC_OK;
	MVBI_BOX_F roiBox = { 0.0f };
	HKA_U32    nStatusRoiCenterX = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiCenterY = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiWidth = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiHeight = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_U32    nStatusRoiAngle = IMVS_MODU_ENUM_STATUS_ERROR;
	HKA_S32    status = HKA_FALSE;
	HKA_U32    countX = 0;
	HKA_U32    countY = 0;
	HKA_U32    countH = 0;
	HKA_U32    countW = 0;
	HKA_U32    countA = 0;
	HKA_U32    count = 0;
	HKA_U32    i = 0;

	HKA_CHECK_ERROR(HKA_NULL == hInput, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == defaultCaliperBoxList, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == caliperBoxList, IMVS_EC_PARAM);
	HKA_CHECK_ERROR(HKA_NULL == CaliperStatus, IMVS_EC_PARAM);

	caliperBoxList->clear();

	*CaliperStatus = IMVS_ROI_FROM_ERROR;

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
		&& (IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE == nStatusRoiAngle) // ??????????
		)
		)
	{
		*CaliperStatus = IMVS_ROI_FROM_UI;

		for (int i = 0; i < defaultCaliperBoxList->size(); i++)
		{
			MVBI_BOX_F stBox = { 0 };
			stBox.center_x = defaultCaliperBoxList->at(i).center_x/* * imgSize.width*/;
			stBox.center_y = defaultCaliperBoxList->at(i).center_y/* * imgSize.height*/;
			stBox.width = defaultCaliperBoxList->at(i).width/* * imgSize.width*/;
			stBox.height = defaultCaliperBoxList->at(i).height/* * imgSize.height*/;
			stBox.angle = defaultCaliperBoxList->at(i).angle;

			caliperBoxList->push_back(stBox);
		}
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


		for (i = 0; i < count; i++)
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

			if ((IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterX)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiCenterY)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiWidth)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiHeight)
				|| (IMVS_MODU_ENUM_STATUS_INPUT_INVALID == nStatusRoiAngle)
				) // ????
			{
				roiBox.center_x = 0.0f;
				roiBox.center_y = 0.0f;
				roiBox.width = 0.0f;
				roiBox.height = 0.0f;
				roiBox.angle = 0.0f;
			}

			if (IMVS_MODU_ENUM_STATUS_OK != nStatusRoiAngle)
			{
				roiBox.angle = 0.0f;
			}

			caliperBoxList->push_back(roiBox);

		} // for (i = 0; i < count; i++)
	}

	*CaliperStatus = status;


	return IMVS_EC_OK;
}


HKA_S32 VmModule_OutputScalar_32f(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_F32               val,
                                  char                 *xmlName,
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
                                  char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
    int  nRet = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetInt(hOutput, xmlName, index, val);
    return nRet;
}


HKA_S32 VmModule_OutputScalar_32u(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_U32               val,
                                  char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
    int  nRet                = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetInt(hOutput, xmlName, index, int(val));
    return nRet;
}


HKA_S32 VmModule_OutputString_u08(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_U08              *val,
                                  char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
    int  nRet = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetString(hOutput, xmlName, index, (char*)val);
    return nRet;
}


HKA_S32 VmModule_OutputString_s08(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_S08              *val,
                                  char                 *xmlName,
                                  HKA_S32               index,
                                  HKA_S32               num)
{
    int  nRet                = IMVS_EC_UNKNOWN;
	nRet = VM_M_SetString(hOutput, xmlName, index, (char*)val);
    return nRet;
}

HKA_S32 VmModule_OutputString_char(IN const void * const hOutput,
                                   HKA_U32               status,
                                   char                 *val,
                                   char                 *xmlName,
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
								  char                 *xmlName,
								  HKA_S32               num)
{
	int  nRet                = IMVS_EC_OK;
	char strOutputValue[256] = {0};
	int i = 0;
	//do 
	//{
	//	sprintf_s(strOutputValue, 
	//		sizeof(strOutputValue),
	//		"%d", 
	//		vector[i]);

	//	nRet = VM_M_SetString(hOutput, 
	//		xmlName, 
	//		i, 
	//		strOutputValue);
	//	if(IMVS_EC_OK != nRet)
	//	{
	//		break;
	//	}

	//	i++;
	//} while (i < num);

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

//	nRet = VM_M_BatchSetString(hOutput, xmlName, (char**)&vector, num);
//	return nRet;
//}


HKA_S32 VmModule_OutputVector_32i(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_S32              *vector,
                                  char                 *xmlName,
                                  HKA_S32               num)
{
    int  nRet                = IMVS_EC_OK;
	nRet = VM_M_BatchSetInt(hOutput, xmlName, vector, num);
    return nRet;
}


HKA_S32 VmModule_OutputVector_32u(IN const void * const hOutput,
                                  HKA_U32               status,
                                  HKA_U32              *vector,
                                  char                 *xmlName,
                                  HKA_S32               num)
{
	int  nRet                = IMVS_EC_OK;
	nRet = VM_M_BatchSetInt(hOutput, xmlName, (int*)vector, num);
	return nRet;
}

HKA_S32 VmModule_OutputVector_PointF(IN const void * const hOutput,
	HKA_U32               status,
	HKA_POINT_F          *vector,
	char                 *xmlName,
	HKA_S32               num)
{
	if (vector == NULL
		|| xmlName == NULL)
	{
		return IMVS_EC_PARAM;
	}

	int nRet = IMVS_EC_OK;
	string strXmlName = xmlName;
	HKA_F32* pfPointX = (HKA_F32*)malloc(num * sizeof(HKA_F32));
	HKA_F32* pfPointY = (HKA_F32*)malloc(num * sizeof(HKA_F32));

	try
	{
		if (pfPointX == NULL
			|| pfPointY == NULL)
		{
			nRet = IMVS_EC_RESOURCE_CREATE;
			throw;
		}

		for (int n = 0; n < num; n++)
		{
			pfPointX[n] = vector[n].x;
			pfPointY[n] = vector[n].y;
		}

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfPointX,
			(strXmlName + "PointX").c_str(),
			num);
		if (nRet != IMVS_EC_OK)
		{
			throw;
		}

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfPointY,
			(strXmlName + "PointY").c_str(),
			num);
		if (nRet != IMVS_EC_OK)
		{
			throw;
		}
	}
	catch (int nRet)
	{

	}

	if (pfPointX) free(pfPointX);
	if (pfPointY) free(pfPointY);
	return nRet;
}

HKA_S32 VmModule_OutputVector_BoxF(IN const void * const hOutput,
	HKA_U32               status,
	MVBI_BOX_F           *vector,
	char                 *xmlName,
	HKA_S32               num)
{
	if (vector == NULL
		|| xmlName == NULL)
	{
		return IMVS_EC_PARAM;
	}

	int nRet = IMVS_EC_OK;
	string strXmlName = xmlName;
	HKA_F32* pfPointX = (HKA_F32*)malloc(num * sizeof(HKA_F32));
	HKA_F32* pfPointY = (HKA_F32*)malloc(num * sizeof(HKA_F32));
	HKA_F32* pfWidth = (HKA_F32*)malloc(num * sizeof(HKA_F32));
	HKA_F32* pfHeight = (HKA_F32*)malloc(num * sizeof(HKA_F32));
	HKA_F32* pfAngle = (HKA_F32*)malloc(num * sizeof(HKA_F32));

	try
	{
		if (pfPointX == NULL
			|| pfPointY == NULL
			|| pfAngle == NULL
			|| pfHeight == NULL
			|| pfWidth == NULL)
		{
			nRet = IMVS_EC_RESOURCE_CREATE;
			throw;
		}

		for (int n = 0; n < num; n++)
		{
			pfPointX[n] = vector[n].center_x;
			pfPointY[n] = vector[n].center_y;
			pfAngle[n] = vector[n].angle;
			pfHeight[n] = vector[n].height;
			pfWidth[n] = vector[n].width;
		}

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfPointX,
			(strXmlName + "CenterX").c_str(),
			num);
		if (nRet != IMVS_EC_OK) throw;

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfPointY,
			(strXmlName + "CenterY").c_str(),
			num);
		if (nRet != IMVS_EC_OK) throw;

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfWidth,
			(strXmlName + "Width").c_str(),
			num);
		if (nRet != IMVS_EC_OK) throw;

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfHeight,
			(strXmlName + "Height").c_str(),
			num);
		if (nRet != IMVS_EC_OK) throw;

		nRet = VmModule_OutputVector_32f(hOutput,
			status,
			pfAngle,
			(strXmlName + "Angle").c_str(),
			num);
		if (nRet != IMVS_EC_OK) throw;

	}
	catch (int nRet)
	{

	}
	if (pfPointX) free(pfPointX);
	if (pfPointY) free(pfPointY);
	if (pfWidth) free(pfWidth);
	if (pfHeight) free(pfHeight);
	if (pfAngle) free(pfAngle);

	return nRet;
}

///***************************************************************************************************
//* ????????iMVS6000_OutputImage_8u_C1R
//* ??  ??????????????????????????
//* ??  ????*
//*         hOutput     - IO ????????
//*         status      - I  ????
//*         image       - I  ????
//*         
//* ????????0??????????????????
//* ??  ????
//***************************************************************************************************/
//HKA_S32 VmModule_OutputDstImage_8u_C1R(IN const void * const hOutput,
//									   HKA_U32               status,
//									   HKA_IMAGE            *image)
//{
//	return VmModule_OutputImageByName_8u_C1R(hOutput,status,"OutImage","OutImageWidth","OutImageHeight","OutImagePixelFormat",image);
//}

/***************************************************************************************************
* ????????iMVS6000_OutputImageAddress
* ??  ????????????????????
* ??  ????*
*         hOutput     - IO ????????
*         status      - I  ????
*         imageData   - I  ????????
*         dataLen     - I  ????????
*         xmlName     - I  ????XML??????????
*         
* ????????0??????????????????
* ??  ????
***************************************************************************************************/
HKA_S32 VmModule_OutputImageAddress_8u(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_U08              *imageData,
                                       HKA_S32               dataLen,
                                       char                 *xmlName)
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
										  char          *strImage,
										  char          *strWidth,
										  char          *strHeight,
										  char          *strFormat,
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

//????????????---2019-08-07
HKA_S32 VmModule_OutputImageByName_8u_C3R(IN const void * const hOutput,
	HKA_U32		  status,
	char          *strImage,
	char          *strWidth,
	char          *strHeight,
	char          *strFormat,
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
* ????????VmModule_OutputImageByName_Mvd
* ??  ????????????????????SDK????????????
* ??  ????*
*         hOutput     - IO ????????
*         status      - I  ????
*         image       - I  ????????
*
* ????????0??????????????????
* ??  ????`
***************************************************************************************************/
HKA_S32 VmModule_OutputImageByName_Mvd(IN const void * const hOutput,
	HKA_U32 status,
	char          *strImage,
	char          *strWidth,
	char          *strHeight,
	char          *strFormat,
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

CStringA UNICODEtoUTF8(const TCHAR* strUNICODE)
{
	//????????????????????????????????????????????????????
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
	//????????????????????????????????????????????????????
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	TCHAR *szBuffer = new TCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, szBuffer, nLen);
	szBuffer[nLen] = 0;
	CString cstrUTF8 = szBuffer;
	delete[] szBuffer;
	return cstrUTF8;
}

CStringA UTF8toANSI(const char* strUTF8)
{
	//????????????????????????????????????????????????????
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;

	BOOL bFail = false;
	nLen = WideCharToMultiByte(0, NULL, wszBuffer, -1, NULL, NULL, NULL, &bFail);
	char *szBuffer = new char[nLen + 1];
	nLen = WideCharToMultiByte(0, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	CStringA strANSI = szBuffer;
	//????????
	delete[] szBuffer;
	delete[] wszBuffer;
	return strANSI;
}

CStringA  ANSItoUTF8(CStringA bufRecv)
{
	long  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, bufRecv, -1, NULL, 0);
	wchar_t *pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, unicodeLen * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, bufRecv, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring rt_utf8 = pUnicode;
	delete[] pUnicode;

	UINT nLen = WideCharToMultiByte(CP_UTF8, NULL, rt_utf8.c_str(), -1, NULL, NULL, NULL, NULL);
	char *szBuffer = new char[nLen + 1];
	nLen = WideCharToMultiByte(CP_UTF8, NULL, rt_utf8.c_str(), -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	CStringA strUTF8 = szBuffer;
	delete[] szBuffer;
	return strUTF8;
}

bool IsStrUtf8(const char* str)
{
	unsigned int nBytes = 0;//UFT8????1-6??????????,ASCII??????????
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i){
		chr = *(str + i);
		//????????ASCII????,????????,????????????UTF8,ASCII??7??????,????????????0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0){
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//????????ASCII??,??????????????,??????????
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD){
					nBytes = 6;
				}
				else if (chr >= 0xF8){
					nBytes = 5;
				}
				else if (chr >= 0xF0){
					nBytes = 4;
				}
				else if (chr >= 0xE0){
					nBytes = 3;
				}
				else if (chr >= 0xC0){
					nBytes = 2;
				}
				else{
					return false;
				}
				nBytes--;
			}
		}
		else{
			//??????????????????,???? 10xxxxxx
			if ((chr & 0xC0) != 0x80){
				return false;
			}
			//????????????
			nBytes--;
		}
	}
	//????UTF8????????
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii){ //????????????ASCII, ????UTF8
		return true;
	}
	return true;
}

CString VmModule_GetModuleName()
{
	wchar_t chszModuleName[MAX_PATH] = { 0 };
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&VmModule_GetModuleName, //????????????????
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
		(LPCWSTR)&VmModule_GetModuleName, //????????????????
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
    //TODO: ????????????????????????????
    int nErrType = IMVS_EC_MODULE_ALGORITHM_INIT ;
    char chszErrMsg[256] = {0};
    _snprintf_s( chszErrMsg, sizeof(chszErrMsg), sizeof(chszErrMsg) - 1, "%d", nRet );
    VM_M_ReportData(hModule, nErrType, chszErrMsg,256);
}



/***************************************************************************************************
* ????????iMVS6000_OutputImage_8u_C1R
* ??  ??????????????????????????
* ??  ????*
*         hOutput     - IO ????????
*         status      - I  ????
*         image       - I  ????
*         
* ????????0??????????????????
* ??  ????
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_8u_C1R(IN const void * const hOutput, HKA_U32 status, HKA_IMAGE *image)
{
	return VmModule_OutputImageByName_8u_C1R(hOutput,status,"OutImage","OutImageWidth","OutImageHeight","OutImagePixelFormat",image);
}

HKA_S32 VmModule_OutputProcImage_8u_C1R(IN const void * const hProcedure, HKA_U32 status, HKA_IMAGE *image)
{
 	return VmModule_OutputImageByName_8u_C1R(hProcedure,status,"ProcImage","ProcImageWidth","ProcImageHeight","ProcImagePixelFormat",image);
}

//??????????????????---2019-08-07
HKA_S32 VmModule_OutputDstImage_8u_C3R(IN const void * const hOutput, HKA_U32 status, HKA_IMAGE *image)
{
	return VmModule_OutputImageByName_8u_C3R(hOutput, status, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", image);
}

/***************************************************************************************************
* ????????VmModule_OutputDstImage_Mvd
* ??  ????????????????????SDK????????????
* ??  ????*
*         hOutput     - IO ????????
*         status      - I  ????
*         image       - I  ????????
*
* ????????0??????????????????
* ??  ????`
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_Mvd(IN const void * const hOutput,
	HKA_U32 status,
	IMvdImage *image)
{
	return VmModule_OutputImageByName_Mvd(hOutput, status, "OutImage", "OutImageWidth", "OutImageHeight", "OutImagePixelFormat", image);
}

HKA_S32 VmModule_GetInputVectorIndex_32f(IN const void * const hInput,
                                         char          *xmlName,
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

HKA_S32 VmModule_GetInputVectorIndex_32i(IN const void * const hInput,
                                         char          *xmlName,
                                         HKA_S32       *val,
                                         HKA_U32        index,
                                         HKA_U32       *status)
{
    
    HKA_U32 nCount      = 0;
    HKA_U32 nStatus     = 0;
    HKA_S32 nRet        = IMVS_EC_OK;
    *status = IMVS_MODU_ENUM_STATUS_ERROR;
    nRet = VM_M_GetInt(hInput, 
                         xmlName, 
                         index, 
                         val, 
                         (int*)&nCount);
 	ErrorCodeToStatus(nRet, status);
    return nRet;
}

HKA_S32 VmModule_GetInputVectorIndex_32u(IN const void * const hInput,
                                         char          *xmlName,
                                         HKA_U32       *val,
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
	nRet = VM_M_GetInt(hInput, xmlName, 0, (int*)val, (int*)&nCount);
	ErrorCodeToStatus(nRet, status);
    return nRet;
}

