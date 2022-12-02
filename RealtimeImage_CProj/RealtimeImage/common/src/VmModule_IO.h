#ifndef _VMMODULE_IO_H_
#define _VMMODULE_IO_H_

#include "hka_types.h"
#include "mvb_defs.h"
#include "mvb_types.h"

#include "VmAlgModuTypes.h"
#include "MVDImageCpp.h"
#include "PositionFixCpp.h"
#include "CrossPlatform/CrossPlatformDef.h"
#include "CrossPlatform/CrossPlatformFunc.h"
#include <vector>

#ifdef _WIN32
#	include <atlstr.h>//CString��ͷ�ļ� 
#	define  VM_MODULE_CALL  __stdcall
#else
#	define  VM_MODULE_CALL 
#endif // _WIN32

using namespace std;
using namespace VisionDesigner;
using namespace VisionDesigner::PositionFix;

HKA_S32 VmModule_GetInputScalar_32f(IN const void * const hInput,
                                    const char          *xmlName,
									HKA_F32       *val,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputScalar_32i(IN const void * const hInput,
                                    const char          *xmlName,
									HKA_S32       *val,
									HKA_U32       *status);

HKA_S32 VmModule_GetInputImageAddress(IN const void * const hInput,
                                      const char          *xmlName,
									  char*			*val,
									  HKA_U32       *status);

HKA_S32 VmModule_GetInputImageAddressAndDataSize(IN const void * const hInput,
                                                const char          *xmlName,
                                                char*       *val,
                                                int&        iImageDataLen,
                                                HKA_U32       *status);

HKA_S32 VmModule_GetInputVector_32f(IN const void * const hInput,
                                    const char          *xmlName,
									HKA_F32       *val,
									HKA_U32        num,
									HKA_U32       *status);


HKA_S32 VmModule_GetInputVector_32i(IN const void * const hInput,
                                    const char          *xmlName,
									HKA_S32       *val,
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
                                     const char          *xmlName,
                                     HKA_U32       *count,
                                     HKA_U32       *status);
HKA_S32 VmModule_GetInputVectorIndex_32f(IN const void * const hInput,
										 const char          *xmlName,
										 HKA_F32       *val,
										 HKA_U32        index,
										 HKA_U32       *status);



HKA_S32 VmModule_GetInputImageEx(IN const void * const hInput,
                                    HKA_IMAGE     *image,
                                    HKA_S32&      iImageDataLen,
                                    HKA_S32       *imageStatus);

HKA_S32 VmModule_GetMaskImage(IN const void * const hInput,
							 HKA_IMAGE     *image,
							 HKA_S32&      iImageDataLen,
							 HKA_S32       *imageStatus);

HKA_S32 VmModule_GetInputFixture(IN const void * const hInput,
								 MVBI_FIDUCIAL_POSE   *fixInit,
								 MVBI_FIDUCIAL_POSE   *fixRun,
								 HKA_S32              *fixStatus);




HKA_S32 VmModule_GetInputRoiBox(IN const void * const hInput,
								HKA_SIZE_I     imgSize,
								vector<MVBI_BOX_F> &boxList,
								HKA_S32       *boxStatus,
								bool          multipleROI);

HKA_S32 VmModule_GetInputRoiAnnulus(IN const void * const hInput,
									HKA_SIZE_I     imgSize,
									vector<MVBI_ANNULUS_F> &annulusList,
									HKA_S32       *annulusStatus,
									bool          multipleROI);


HKA_S32 VmModule_GetInputCaliperBox(IN const void * const hInput,
									vector<MVBI_BOX_F> *defaultCaliperBoxList,
									HKA_SIZE_I      imgSize,
									vector<MVBI_BOX_F> *caliperBoxList,
									HKA_S32        *CaliperStatus);


HKA_S32 VmModule_OutputScalar_32f(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_F32               val,
								  const char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num = 0);


HKA_S32 VmModule_OutputScalar_32i(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_S32               val,
                                  const char                 *xmlName,
								  HKA_S32               index,
								  HKA_S32               num);




HKA_S32 VmModule_OutputString_char(IN const void * const hOutput,
								   HKA_U32               status,
								   char                 *val,
                                   const char                 *xmlName,
								   HKA_S32               index,
								   HKA_S32               num);


HKA_S32 VmModule_OutputVector_32f(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_F32              *vector,
								  const char           *xmlName,
								  HKA_S32               num);


HKA_S32 VmModule_OutputVector_8u(IN const void * const hOutput,
								 HKA_U32               status,
                                 unsigned char              *vector,
								 const char                 *xmlName,
								 HKA_S32               num);


HKA_S32 VmModule_OutputVector_32i(IN const void * const hOutput,
								  HKA_U32               status,
								  HKA_S32              *vector,
                                  const char                 *xmlName,
								  HKA_S32               num);

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
									   const char                 *xmlName);


HKA_S32 VmModule_OutputImageByName_8u_C1R(IN const void * const hOutput,
										  HKA_U32               status,
                                          const char          *strImage,
                                          const char          *strWidth,
                                          const char          *strHeight,
                                          const char          *strFormat,
										  HKA_IMAGE     *image);

//���Ӳ�ͼ���---2019-08-07
HKA_S32 VmModule_OutputImageByName_8u_C3R(IN const void * const hOutput,
										  HKA_U32		  status,
                                          const char          *strImage,
                                          const char          *strWidth,
                                          const char          *strHeight,
                                          const char          *strFormat,
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
    const char          *strImage,
    const char          *strWidth,
    const char          *strHeight,
    const char          *strFormat,
	IMvdImage *image);

CString UTF8toUNICODE(const char* strUTF8);
CStringA UNICODEtoUTF8(const wchar_t* strUNICODE);

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


HKA_S32 VmModule_OutputDstImage_8u_C3R(IN const void * const hOutput,
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

/***************************************************************************************************
* ��������VmModule_OutputMaskImage_Mvd
* ��  �ܣ��㷨ƽ̨�������SDK��ʽͼ������
* ��  ����*
*         hOutput     - IO ģ�����
*         status      - I  ״̬
*         image       - I  ͼ������
*
* ����ֵ��0����ȷ������������
* ��  ע��`
***************************************************************************************************/
HKA_S32 VmModule_OutputMaskImage_Mvd(IN const void * const hOutput,
    HKA_U32 status,
    IMvdImage *image);

#endif