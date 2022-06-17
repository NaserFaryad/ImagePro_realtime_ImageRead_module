/**	@file       imvs_types.h
 *	@note       HangZhou Hikrobot Technology Co., Ltd. All Right Reserved.
 *	@brief      ������д���ļ���ժҪ
 *
 *	@author		songzhenlong
 *	@date		2020/12/17   18:36
 *
 *	@note       Create
 *
 *	@warning    <No waining>
 */

#ifndef _VM_ALG_MODULE_DEFINES_H_
#define _VM_ALG_MODULE_DEFINES_H_
#include "hka_defs.h"
#include "hka_types.h"
#include "mvb_defs.h"
#include "mvb_types.h"


#ifndef IMVS_NULL
#define IMVS_NULL    0
#endif

//��Ч��ģ��ID
#define  MODULE_ID_INVALID       (-1)


#define IMVS_CHAR_SET_NUMBER         (0x00000001) // ���ּ�
#define IMVS_CHAR_SET_SMALL_ALPHABET (0x00000010) // Сд��ĸ��
#define IMVS_CHAR_SET_BIG_ALPHABET   (0x00000100) // ��д��ĸ��
#define IMVS_CHAR_SET_SPECIAL_CHAR   (0x00001000) // �����ַ���
#define IMVS_CHAR_SET_USER_STRING    (0x00010000) // �û��ַ���
#define IMVS_CHAR_SET_ALG            (0x00100000) // �㷨�ض��ַ���

//����θ���
#define IMVS_ROI_POLYGOPN_COUNT_MAX                 (8)          // ģ��֧�ֶ���θ�������ʱ��16��Ϊ8��
#define IMVS_ROI_POLYGOPN_POINT_COUNT_MAX      (32)       // ���������������
#define IMVS_COMMON_POLYGON_LEN_MAX              (1024)    // ������ַ�������

#define IMVS_MULTIPLE_ROI_COUNT_MAX              (16)    // ��ROI������

#define IMVS_MAX_CALIPER_NUMBER           (1000)  //����������

#define  MAX_FILE_PATH          (260)         // �ļ���󳤶�
#define  MAX_FILE_PATH_UTF8     (260*3)       // �ļ���󳤶�
#define  MAX_POLYGON_POINT_NUM  32


#define IMVS_ROI_FROM_ERROR (0)
#define IMVS_ROI_FROM_UI    (1)
#define IMVS_ROI_FROM_INPUT (2)

//ͼ��ṹ�����ͨ����
#define MAX_IMAGE_CHANNEL_NUM (4)

//�㷨ģ�鵼��/�������
#ifdef IMVSALGMODU_EXPORTS
#define IMVSALGMODU_API __declspec(dllexport)
#else
#define IMVSALGMODU_API __declspec(dllimport)
#endif

// ROI����
typedef enum _IMVS_ROI_TYPE {
	IMVS_ROI_TYPE_NULL = 0, // ��ROI
	IMVS_ROI_TYPE_IMAGE = 1, // ȫͼROI
    IMVS_ROI_TYPE_BOX = 2, // ����ROI
    IMVS_ROI_TYPE_ANNULUS = 3, // �Ȼ�ROI
    IMVS_ROI_TYPE_POLYGON = 4, // �����ROI
	IMVS_ROI_TYPE_LINECALIPER = 5, // ֱ�߿���ROI
	IMVS_ROI_TYPE_CIRCLECALIPER = 6, // Բ�ο���ROI
	IMVS_ROI_TYPE_MULTIPLE = 7, //������ROI
	IMVS_ROI_TYPE_DEFAULT = 255, //Ĭ��ROI
    IMVS_ROI_TYPE_NUM
}IMVS_ROI_TYPE;

/** @enum IMVS_MODU_STATUS
 *  @brief  
 */
typedef enum IMVS_MODU_STATUS__
{
    IMVS_MODU_ENUM_STATUS_ERROR = 0,            // ͨ�Ų���ȷ, ͨ����Ϣ�������ش���
    IMVS_MODU_ENUM_STATUS_OK = 1,            // ͨ����ȷ, �Ѿ�����, �һ�ȡ���ĵ�ֵ��ȷ
    IMVS_MODU_ENUM_STATUS_INPUT_INVALID = 2,            // ͨ����ȷ, �Ѿ�����, ����ȡ���ĵ�ֵ��Ч(�����ϸ�ģ��û���ҵ�Բ���)
    IMVS_MODU_ENUM_STATUS_NOSUBSCRIBE = 3,            // ͨ����ȷ, ��δ����, �����ж�ֵ,����Ӧ��������(һ��ʹ��Ĭ��ֵ�����û��Զ������ֵ)
} IMVS_MODU_STATUS;

//����У��ֱ������ѭ��
#define HKA_CHECK_BREAK(flag)        \
{                                     \
	if (flag)                         \
        	{                                 \
		break;                       \
        	}                                 \
}

#define HKA_CHECK_BREAK_EX(flag,loginfo)        \
    {                                     \
	if (flag)                         \
                	{                                 \
		break;                       \
            	}                                 \
    }

#pragma   warning(error:4003)//���������moduID����
#pragma   warning(error:4002)//���������moduID����
#define HKA_CHECK_BREAK_LOG(flag, nret, funName)        \
	    {                                     \
	if (flag)                         \
	                	{                                 \
		break;                       \
	            	}                                 \
	    }

//����У��return
#define HKA_MODU_CHECK_ERROR(flag, ret)    \
        {                                     \
	if (flag)                         \
                	{                                 \
	return ret;                 \
            	}                                 \
    }

#pragma   warning(error:4003)//���������moduID����
#pragma   warning(error:4002)//���������moduID����
#define HKA_CHECK_ERROR_LOG(flag, nret, funName)    \
    {                                     \
    if (flag)                         \
        {                                 \
        return (nret);                 \
        }                                 \
    }



#define  MVDSDK_TRY  try

#define  MVDSDK_CATCH  catch (int nErrCode)\
        {\
        nRet = nErrCode;\
        }\
    catch (IMVDException &ex)\
        {\
        nRet = ex.GetErrorCode();\
        }\
    catch (...)\
        {\
        nRet = IMVS_EC_UNKNOWN;\
        }

//����У�鷵��״̬��
#define MVDSDK_THROW_ERROR(flag, sts, fun)							\
    {																\
    if (flag)													\
            	{															\
		throw (sts);											\
            	}															\
    }



// Box
typedef struct _IMVS_ROI_BOX {
    float               fRoiCenterX;
    float               fRoiCenterY;
    float               fRoiWidth;
    float               fRoiHeight;
    float               fRoiAngle;
} IMVS_ROI_BOX;

// Բ��
typedef struct _IMVS_ANNULUS {
    HKA_POINT_F center;        // ���ĵ�
    HKA_F32     inner_radius;  // ��Բ�뾶
    HKA_F32     outer_radius;  // ��Բ�뾶
    HKA_F32     start_angle;   // Բ����ʼ�Ƕ�
    HKA_F32     end_angle;     // Բ���Ƕȷ�Χ
}IMVS_ANNULUS;

//�����
typedef struct _IMVS_POLYGON
{
	unsigned int       vertex_num;      //������
	HKA_POINT_F   vertex_points[IMVS_ROI_POLYGOPN_POINT_COUNT_MAX];   //����
}IMVS_POLYGON;

// �����Ͳ����ж�
typedef struct _IMVS_PARAM_LIMIT_F32 {
    HKA_S32        is_valid;   // �Ƿ���Ч
    HKA_F32        min_val;    // ������Сֵ
    HKA_F32        max_val;    // �������ֵ
} IMVS_PARAM_LIMIT_F32;

// �����Ͳ����ж�
typedef struct _IMVS_PARAM_LIMIT_F64 {
	HKA_S32        is_valid;   // �Ƿ���Ч
	HKA_F64        min_val;    // ������Сֵ
	HKA_F64        max_val;    // �������ֵ
} IMVS_PARAM_LIMIT_F64;

// ���Ͳ����ж�
typedef struct _IMVS_PARAM_LIMIT_S32 {
    HKA_S32        is_valid;   // �Ƿ���Ч
    HKA_S32        min_val;    // ������Сֵ
    HKA_S32        max_val;    // �������ֵ
} IMVS_PARAM_LIMIT_S32;

// �ַ��������ж�
typedef struct _IMVS_PARAM_LIMIT_STRING {
    HKA_S32        is_valid;   // �Ƿ���Ч
    char           user_char[256]; // �û������ַ���
} IMVS_PARAM_LIMIT_STRING;

// �ַ���֤��Ϣ
typedef struct _IMVS_CHAR_VERIFY {
    HKA_S32  is_valid;       // �Ƿ�ʹ��
    HKA_S32  char_set;       // �ַ���
    char     user_char[256]; // �û������ַ���
    char     alg_char[256];  // �㷨�ض��ַ���
}IMVS_CHAR_VERIFY;

//�����ROI�б�
typedef struct _IMVS_Polygon_ROI_LIST {
    int                   nCount;
    MVB_POLYGON_F         szstShieldedPolygon[IMVS_ROI_POLYGOPN_COUNT_MAX];
    MVB_POLYGON_F         szstShieldedRoiPolygon[IMVS_ROI_POLYGOPN_COUNT_MAX];
    MVB_POLYGON_F         szstShieldedFixPolygon[IMVS_ROI_POLYGOPN_COUNT_MAX];
    MVB_POLYGON_F         szstShieldedPolygonBasis[IMVS_ROI_POLYGOPN_COUNT_MAX];
    int                   nReserved[4];
}IMVS_Polygon_ROI_LIST;

//������������б�
typedef struct _IMVS_POLYGON_MASK_LIST {
	int                   nCount;
	IMVS_POLYGON         stMaskNormalized[IMVS_ROI_POLYGOPN_COUNT_MAX];
	IMVS_POLYGON         stMaskPixel[IMVS_ROI_POLYGOPN_COUNT_MAX];
	int                 nReserved[4];
}IMVS_POLYGON_MASK_LIST;

// ��������Ϣ
typedef struct _IMVS_CONTOUR_POINT_INFO
{
    HKA_F32 ContourX;
    HKA_F32 ContourY;
    HKA_F32 ContourScore;
    HKA_S32 ContourIndex;
}IMVS_CONTOUR_POINT_INFO;

typedef struct _MY_LINECALIPERBOX
{
	float      fStartX;
	float      fStartY;
	float      fEndX;
	float      fEndY;
	float      fWidth;
	float      fHeight;
	float      fAngle;
	int        nNum;
} MY_LINECALIPERBOX;

typedef struct _MY_CIRCLECALIPERBOX
{
	float      fCenterX;
	float      fCenterY;
	float      fOutterRadius;
	float      fStartAngle;
	float      fEndAngle;
	float      fWidth;
	float      fHeight;
	int        nNum;
} MY_CIRCLECALIPERBOX;

#endif//_VM_ALG_MODULE_DEFINES_H_
