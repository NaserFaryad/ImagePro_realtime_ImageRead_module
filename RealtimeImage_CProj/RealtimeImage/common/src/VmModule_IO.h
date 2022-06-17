#ifndef _VMMODULE_IO_H_
#define _VMMODULE_IO_H_

#include "hka_types.h"
#include "mvb_defs.h"
#include "mvb_types.h"
#include <atlstr.h>//CString��ͷ�ļ� 
#include "VmAlgModuTypes.h"
#include "MVDImageCpp.h"
#include "PositionFixCpp.h"
#include <vector>

using namespace std;
using namespace VisionDesigner;
using namespace VisionDesigner::PositionFix;

HKA_S32 VmModule_GetInputScalar_32f(IN const void * const hInput,
									char          *xmlName,
									HKA_F32       *val,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputScalar_32i(IN const void * const hInput,
									char          *xmlName,
									HKA_S32       *val,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputScalar_32u(IN const void * const hInput,
									char          *xmlName,
									HKA_U32       *val,
									HKA_U32       *status);

HKA_S32 VmModule_GetInputImageAddress(IN const void * const hInput,
									  char          *xmlName,
									  char*			*val,
									  HKA_U32       *status);

HKA_S32 VmModule_GetInputImageAddressAndDataSize(IN const void * const hInput,
                                                char          *xmlName,
                                                char*       *val,
                                                int&        iImageDataLen,
                                                HKA_U32       *status);

HKA_S32 VmModule_GetInputVector_32f(IN const void * const hInput,
									char          *xmlName,
									HKA_F32       *val,
									HKA_U32        num,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputVector_32i(IN const void * const hInput,
									char          *xmlName,
									HKA_S32       *val,
									HKA_U32        num,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputVector_32u(IN const void * const hInput,
									char          *xmlName,
									HKA_U32       *val,
									HKA_U32        num,
									HKA_U32       *status);

/***************************************************************************************************
* ��������iMVS6000_GetInputVectorCount
* ��  �ܣ��㷨ƽ̨��ȡ��������Ԫ�ظ���
* ��  ����*
*         hInput      - I ģ������
*         xmlName     - I ��ӦXML���������
*         count       - O �������
*         status      - O ״̬
*         
* ����ֵ��0����ȷ������������
* ��  ע��
***************************************************************************************************/
HKA_S32 VmModule_GetInputVectorCount(IN const void * const hInput,
                                     char          *xmlName,
                                     HKA_U32       *count,
                                     HKA_U32       *status);
HKA_S32 VmModule_GetInputVectorIndex_32f(IN const void * const hInput,
										 char          *xmlName,
										 HKA_F32       *val,
										 HKA_U32        index,
										 HKA_U32       *status);


HKA_S32 VmModule_GetInputVectorIndex_32i(IN const void * const hInput,
										 char          *xmlName,
										 HKA_S32       *val,
										 HKA_U32        index,
										 HKA_U32       *status);


HKA_S32 VmModule_GetInputVectorIndex_32u(IN const void * const hInput,
										 char          *xmlName,
										 HKA_U32       *val,
										 HKA_U32        index,
										 HKA_U32       *status);


HKA_S32 VmModule_GetInputImage(IN const void * const hInput,
							   HKA_IMAGE     *image,
							   HKA_U32       *imageStatus);

HKA_S32 VmModule_GetInputImageEx(IN const void * const hInput,
                                    HKA_IMAGE     *image,
                                    HKA_S32&      iImageDataLen,   
									HKA_U32       *imageStatus);


HKA_S32 VmModule_GetInputImageByName(IN const void * const hInput,
									 char          *strImage,
									 char          *strWidth,
									 char          *strHeight,
									 char          *strFormat,
									 HKA_IMAGE     *image,
									 HKA_U32       *imageStatus);


HKA_S32 VmModule_GetInputFixture(IN const void * const hInput,
								 MVBI_FIDUCIAL_POSE   *fixInit,
								 MVBI_FIDUCIAL_POSE   *fixRun,
								 HKA_U32              *fixStatus);


HKA_S32 VmModule_GetInputFixture_Mvd(IN const void * const hInput,
	MVD_FIDUCIAL_POINT_F   *fixInit,
	MVD_FIDUCIAL_POINT_F   *fixRun,
	HKA_U32              *fixStatus);

HKA_S32 VmModule_GetInputDoubleBox(IN const void * const hInput,
								   IMVS_ROI_BOX  *defaultBox1,
								   IMVS_ROI_BOX  *defaultBox2,
								   HKA_SIZE_I     imgSize,
								   MVBI_BOX_F    *box1,
								   MVBI_BOX_F    *box2,
								   HKA_U32       *boxStatus);

HKA_S32 VmModule_GetInputMultiRoiBox(IN const void * const hInput,
	IMVS_ROI_BOX  *defaultBox,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus);

HKA_S32 VmModule_GetInputMultiRoiBox2(IN const void * const hInput,
	IMVS_ROI_BOX  *defaultBox,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus);

HKA_S32 VmModule_GetInputMultiRoiAnnulus(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_ANNULUS_F> &annulusList,
	HKA_U32       *nStatus);

HKA_S32 VmModule_GetInputQuadraRoiBox(IN const void * const hInput, 
									  IMVS_ROI_BOX *defaultBox0,
									  IMVS_ROI_BOX *defaultBox1, 
									  IMVS_ROI_BOX *defaultBox2, 
									  IMVS_ROI_BOX *defaultBox3, 
									  HKA_SIZE_I     imgSize, 
									  MVBI_BOX_F    *box0,
									  MVBI_BOX_F    *box1, 
									  MVBI_BOX_F    *box2,
									  MVBI_BOX_F    *box3, 
									  HKA_U32       *boxStatus);

HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_BOX_F> &boxList,
	HKA_U32       *boxStatus,
	bool          multipleROI);

HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
								IMVS_ROI_BOX  *defaultBox,
								HKA_SIZE_I     imgSize,
								MVBI_BOX_F    *box,
								HKA_U32       *boxStatus);

HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
	HKA_SIZE_I     imgSize,
	vector<MVBI_ANNULUS_F> &annulusList,
	HKA_U32       *annulusStatus,
	bool          multipleROI);

HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
	MVBI_ANNULUS_F *defaultAnnulus,
									HKA_SIZE_I      imgSize,
									MVBI_ANNULUS_F *annulus,
									HKA_U32        *annulusStatus);

HKA_S32 VmModule_GetInputCaliperBox(IN const void * const hInput,
									vector<MVBI_BOX_F> *defaultCaliperBoxList,
									HKA_SIZE_I      imgSize,
									vector<MVBI_BOX_F> *caliperBoxList,
									HKA_U32        *CaliperStatus);


HKA_S32 VmModule_OutputScalar_32f(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_F32               val,
								  char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num = 0);


HKA_S32 VmModule_OutputScalar_32i(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_S32               val,
								  char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num);


HKA_S32 VmModule_OutputScalar_32u(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_U32               val,
								  char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num);


HKA_S32 VmModule_OutputString_u08(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_U08                 *val,
								  char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num);


HKA_S32 VmModule_OutputString_s08(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_S08                 *val,
								  char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num);


HKA_S32 VmModule_OutputString_char(IN const void * const hOutput,
								   HKA_U32               status,
								   char                 *val,
								   char                 *xmlName,
								   HKA_S32               index,
								   HKA_S32               num);


HKA_S32 VmModule_OutputVector_32f(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_F32              *vector,
								  const char           *xmlName,
								  HKA_S32               num);


HKA_S32 VmModule_OutputVector_8u(IN const void * const hOutput,
								 HKA_U32               status,
								 HKA_U08              *vector,
								 char                 *xmlName,
								 HKA_S32               num);


HKA_S32 VmModule_OutputVector_32i(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_S32              *vector,
								  char                 *xmlName,
								  HKA_S32               num);


HKA_S32 VmModule_OutputVector_32u(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_U32              *vector,
								  char                 *xmlName,
								  HKA_S32               num);

HKA_S32 VmModule_OutputVector_PointF(IN const void * const hOutput,
	HKA_U32               status,
	HKA_POINT_F          *vector,
	char                 *xmlName,
	HKA_S32               num);

HKA_S32 VmModule_OutputVector_BoxF(IN const void * const hOutput,
	HKA_U32               status,
	MVBI_BOX_F           *vector,
	char                 *xmlName,
	HKA_S32               num);


/***************************************************************************************************
* ��������iMVS6000_OutputImage_8u_C1R
* ��  �ܣ��㷨ƽ̨���ͼ��
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         image       - I  ͼ��
*         
* ����ֵ��0����ȷ������������
* ��  ע��
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_8u_C1R(IN const void * const hOutput,
									   HKA_U32               status,
									   HKA_IMAGE            *image);


/***************************************************************************************************
* ��������iMVS6000_OutputImageAddress
* ��  �ܣ��㷨ƽ̨���ͼ������
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         imageData   - I  ͼ��ָ��
*         dataLen     - I  ���ݳ���
*         xmlName     - I  ��ӦXML���������
*         
* ����ֵ��0����ȷ������������
* ��  ע��
***************************************************************************************************/
HKA_S32 VmModule_OutputImageAddress_8u(IN const void * const hOutput,
									   HKA_U32               status,
									   HKA_U08              *imageData,
									   HKA_S32               dataLen,
									   char                 *xmlName);


HKA_S32 VmModule_OutputImageByName_8u_C1R(IN const void * const hOutput,
										  HKA_U32               status,
										  char          *strImage,
										  char          *strWidth,
										  char          *strHeight,
										  char          *strFormat,
										  HKA_IMAGE     *image);

//���Ӳ�ͼ���---2019-08-07
HKA_S32 VmModule_OutputImageByName_8u_C3R(IN const void * const hOutput,
										  HKA_U32		  status,
										  char          *strImage,
										  char          *strWidth,
										  char          *strHeight,
										  char          *strFormat,
										  HKA_IMAGE     *image);

/***************************************************************************************************
* ��������VmModule_OutputImageByName_Mvd
* ��  �ܣ��㷨ƽ̨�������SDK��ʽͼ������
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         image       - I  ͼ������
*
* ����ֵ��0����ȷ������������
* ��  ע��`
***************************************************************************************************/
HKA_S32 VmModule_OutputImageByName_Mvd(IN const void * const hOutput,
	HKA_U32 status,
	char          *strImage,
	char          *strWidth,
	char          *strHeight,
	char          *strFormat,
	IMvdImage *image);

CString UTF8toUNICODE(const char* strUTF8);
CStringA UNICODEtoUTF8(const TCHAR* strUNICODE);
CStringA UTF8toANSI(const char* strUTF8);
bool IsStrUtf8(const char* str);
CStringA  ANSItoUTF8(CStringA bufRecv);
// ��ȡģ������
CString VmModule_GetModuleName();

// ��ȡģ��·��
CString VmModule_GetModulePath();

// ��ȡģ��XML·��
CString VmModule_GetXmlPath();

//��ȡģ�����ƣ�������׺��
CString VmModule_GetModuleNameNoPostfix();

// �ϱ�������Ϣ
void VmModule_ReportError(void* hModule, int nRet);

/***************************************************************************************************
* ��������iMVS6000_OutputImage_8u_C1R
* ��  �ܣ��㷨ƽ̨���ͼ��
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         image       - I  ͼ��
*         
* ����ֵ��0����ȷ������������
* ��  ע��
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_8u_C1R(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_IMAGE            *image);

HKA_S32 VmModule_OutputProcImage_8u_C1R(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_IMAGE            *image);

HKA_S32 VmModule_OutputDstImage_8u_C3R(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_IMAGE            *image);

HKA_S32 VmModule_OutputSrcImage_8u_C1R(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_IMAGE            *image);

HKA_S32 VmModule_OutputSrcImage_8u_C3R(IN const void * const hOutput,
                                       HKA_U32               status,
                                       HKA_IMAGE            *image);

/***************************************************************************************************
* ��������VmModule_OutputDstImage_Mvd
* ��  �ܣ��㷨ƽ̨�������SDK��ʽͼ������
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         image       - I  ͼ������
*
* ����ֵ��0����ȷ������������
* ��  ע��`
***************************************************************************************************/
HKA_S32 VmModule_OutputDstImage_Mvd(IN const void * const hOutput,
	HKA_U32 status,
	IMvdImage *image);

#endif