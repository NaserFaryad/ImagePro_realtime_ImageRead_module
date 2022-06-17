/***************************************************************************************************
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2020, ���ݺ��������˼������޹�˾, ��������Ȩ��
*
* �ļ����ƣ�VmAlgModuBase.h
* ժ    Ҫ���㷨ģ�����
*
* ��ǰ�汾��
* ��    ��:  �Ӿ�ƽ̨С��
* ��    �ڣ�2020-10-14
* ��    ע���½�
****************************************************************************************************/

#ifndef _IMVS_MVDSDK_BASE_MODU_SDK_H_
#define _IMVS_MVDSDK_BASE_MODU_SDK_H_

#include "VmAlgModuTypes.h"
#include "VmModuleBase.h"
#include "ErrorCodeDefine.h"
#include "VmAlgModuCommon.h"
#include "VmModule_IO.h"
#include "VmModuleInternal.h"

#define LOG_STRING_SIZE (1024) // ��־��Ϣ����
//#define DEFAULT_ROI_MSG_DATA ("") // Ĭ��ROI��Ϣ���������� [0][0][0.50.51.01.00.0]
//#define DEFAULT_ROI_MSG_LENGTH (0) // Ĭ��ROI��Ϣ���ݳ���

using namespace std;
using namespace VisionDesigner;

/** @class CMyAlgorithmManager
*  @brief
*/
class CVmAlgModuleBase : public CAbstractUserModule
{
protected:

	// ģ��ROI�����ۺ� �����һ������
	typedef struct _BASE_MODU_ROI_DATA
	{
		IMVS_ROI_TYPE iRoiTypeIndex; // ROI��������

		vector<IMVS_ROI_BOX> stRoiBox; // ����ROI
		vector<IMVS_ANNULUS> stRoiAnnulus; // �Ȼ�ROI
		vector<IMVS_POLYGON> stRoiPolygon; // �����ROI
		MY_LINECALIPERBOX stLineCaliperBox;   //ֱ�߿��߿�ṹ��
		MY_CIRCLECALIPERBOX stCircleCaliperBox; //Բ���߿�ṹ��

		_BASE_MODU_ROI_DATA()
		{
			IMVS_ROI_BOX roiBox = { 0.0f };

			roiBox.fRoiCenterX = 0.5f;
			roiBox.fRoiCenterY = 0.5f;
			roiBox.fRoiHeight = 1.0f;
			roiBox.fRoiWidth = 1.0f;
			roiBox.fRoiAngle = 0.0f;

			stRoiBox.push_back(roiBox);

			iRoiTypeIndex = IMVS_ROI_TYPE_IMAGE;

			memset(&stLineCaliperBox, 0, sizeof(stLineCaliperBox));
			memset(&stCircleCaliperBox, 0, sizeof(stCircleCaliperBox));
		}
	}BASE_MODU_ROI_DATA;

	// ģ�����ԭʼ����
	typedef struct _MVDSDK_BASE_MODU_INPUT_INNER
	{
		HKA_S32 nImageInputStatus; //����ͼ���ȡ״̬
		HKA_S32 nROIInputStatus; //����ROI��ȡ״̬
		HKA_S32 nFixInputStatus; //������Ϣ��ȡ״̬

		HKA_IMAGE stImageIn; //����ͼ��

		IMVS_ROI_TYPE iRoiTypeIndex; // ROI��������
		vector<MVBI_BOX_F> stRoiBox; // ����ROI
		vector<MVBI_ANNULUS_F> stRoiAnnulus; // �Ȼ�ROI
		vector<IMVS_POLYGON> stRoiPolygon; // �����ROI
		vector<MVBI_BOX_F>  stLineCaliperBox; //ֱ�߿��߿򼯺�
		vector<MVBI_BOX_F>  stCircleCaliperBox; //Բ���߿򼯺�

		MVBI_FIDUCIAL_POSE stFixInit; // λ��������׼��
		MVBI_FIDUCIAL_POSE stFixRun; // λ���������е�

		_MVDSDK_BASE_MODU_INPUT_INNER()
		{
			nImageInputStatus = false;
			nROIInputStatus = IMVS_ROI_FROM_ERROR;
			nFixInputStatus = IMVS_MODU_ENUM_STATUS_ERROR;

			memset(&stImageIn, 0, sizeof(stImageIn));

			iRoiTypeIndex = IMVS_ROI_TYPE_NULL;

			memset(&stFixInit, 0, sizeof(stFixInit));
			memset(&stFixRun, 0, sizeof(stFixRun));
		}
	}MVDSDK_BASE_MODU_INPUT_INNER;

	// ģ����ദ������� ����λ������ ת������SDK�ṹ�ȴ���
	typedef struct _MVDSDK_BASE_MODU_INPUT
	{
		HKA_S32 nImageInputStatus; //����ͼ���ȡ״̬
		HKA_S32 nROIInputStatus; //����ROI��ȡ״̬
		HKA_S32 nFixInputStatus; //������Ϣ��ȡ״̬

		//HKA_IMAGE struInputImg; //����ͼ�� ȥ���ö��� ͳһʹ�� pImageInObj
		IMvdImage* pImageInObj; //����ͼ�����

		IMVS_ROI_TYPE iRoiTypeIndex; // ROI��������
		vector<IMvdShape*> vtFixRoiShapeObj; // �������ROI�����б�
		vector<IMvdShape*> vctfixShieldedPolygon; // ������Ķ����������
		vector<IMvdShape*> vctFixLineCaliperObj;     //������ֱ�߿��߿�BOX���󼯺�
		vector<IMvdShape*> vctFixCircleCaliperObj;   //������Բ���߿�BOX���󼯺�
		
		MVBI_BOX_F struDetectBox; // ʵ�ʼ��������Ӿ���

		MVD_FIDUCIAL_POINT_F stFixInit; // λ��������׼��
		MVD_FIDUCIAL_POINT_F stFixRun; // λ���������е�

		HKA_S32 bUseMask; // �Ƿ���������Ĥͼ��
		IMvdImage* pImageMaskObj; // ��Ĥͼ�����

		_MVDSDK_BASE_MODU_INPUT()
		{
			nImageInputStatus = false;
			nROIInputStatus = IMVS_ROI_FROM_ERROR;
			nFixInputStatus = IMVS_MODU_ENUM_STATUS_ERROR;

			//memset(&struInputImg, 0, sizeof(struInputImg));
			pImageInObj = NULL;

			iRoiTypeIndex = IMVS_ROI_TYPE_NULL;

			memset(&struDetectBox, 0, sizeof(struDetectBox));

			memset(&stFixInit, 0, sizeof(stFixInit));
			memset(&stFixRun, 0, sizeof(stFixRun));

			bUseMask = false;
			pImageMaskObj = NULL;
		}
	}MVDSDK_BASE_MODU_INPUT;

public:
	// ����
	explicit CVmAlgModuleBase();

	// ����
	virtual ~CVmAlgModuleBase();

	// �����㷨
	virtual int Process(IN void* hInput, IN void* hOutput);

	// �����㷨
	virtual int Process(IN void* hInput, IN void* hOutput, IN MVDSDK_BASE_MODU_INPUT* pModuInput) = 0;

	// ��ȡ�㷨����
	virtual int GetParam(IN const char* szParamName, OUT char* pBuff, IN int nBuffSize, OUT int* pDataLen) override;

	// �����㷨����
	virtual int SetParam(IN const char* szParamName, IN const char* pData, IN int nDataLen) override;

	// ��ȡ���в���
	virtual int GetAllParamList(OUT PARAM_VALUE_INFO_LIST* pParamValueList);

	// ����ģ������
	virtual int SaveModuleData(OUT SAVE_MODULE_DATA_PARAM* pModuleData);

	// ����ģ������
	virtual int LoadModuleData(IN const LOAD_MODULE_DATA_PARAM* pModuleData);

private:
	CVmAlgModuleBase(const CVmAlgModuleBase &);
	CVmAlgModuleBase & operator=(const CVmAlgModuleBase &);

protected:
	// ��ȡ�㷨��������
	virtual int GetProcessInput(IN const void * const hInput, OUT MVDSDK_BASE_MODU_INPUT_INNER *pModuInput);

	// ������Ĥ�����㷨
    virtual int GenerateMaskImage(IN MVDSDK_BASE_MODU_INPUT* pModuInput, IN vector<IMvdShape*> vctROIShape, IN vector<IMvdShape*> vctShieldedShape, IN bool bCheckDiff = false);

	// ����ģ��ROI����
	virtual int ClearRoiData(OUT BASE_MODU_ROI_DATA* stBaseModuROIData);

	// ����ģ��Ĭ��ROI
	virtual int ResetDefaultRoi(OUT BASE_MODU_ROI_DATA* stBaseModuROIData);

	// �������в���
	virtual int SetAllParamList(IN const PARAM_VALUE_INFO_LIST* pParamValueList);
	
private:
	// ��ʼ��
	int Init();

	// ����ʼ��
	void DeInit();

	// ���ó�Ա����
	void ResetMember();

	// ��ʼ��λ����������
	int InitPosFixInstance(IN MVBI_FIDUCIAL_POSE stFixInit, IN MVBI_FIDUCIAL_POSE stFixRun, IN MVD_SIZE_I stImgSize);

	// λ�������㷨����ת������SDK���ݽṹ
	int ProcessFix(IN MVDSDK_BASE_MODU_INPUT_INNER *pModuInput, OUT MVDSDK_BASE_MODU_INPUT* pFixInput);

	// �㷨���������
	int ProcessOutput(IN const void * const hOutput, MVDSDK_BASE_MODU_INPUT &pModuInput);

	// �����������ROI��Ϣ
	int AnalyzeMultipleROIMsg(IN const char * pData, IN int nDataLen, OUT BASE_MODU_ROI_DATA* const pBaseModuROIData);

	// ���ɽ������ROI��Ϣ
	int GenerateMultipleROIMsg(IN BASE_MODU_ROI_DATA* const stBaseModuROIData, OUT char* const pBuff, IN int nBuffSize, OUT unsigned int& nDataLen);
	
	// ����������������Ϣ
	int AnalyzePlygonShieldMsg(IN const char * pData, IN int nDataLen, OUT IMVS_POLYGON_MASK_LIST* const pShieldedPolygon);

	// ���ɽ�����������Ϣ
	int GeneratePlygonShieldMsg(IN IMVS_POLYGON_MASK_LIST* const stShieldedPolygon, OUT char* const pBuff, IN int nBuffSize, OUT unsigned int& nDataLen);
	
protected:

	//����SDK��ض���
	IPositionFixTool*       m_pPosFixInstance;              //λ����������
	IPreproMaskTool*        m_pPreproMaskInstance;           //��Ĥ���ɶ���

	//ģ���߼��������
	bool                   m_bRoiChanged; //����ROI�仯���λ True-�ı� False-δ�ı�
	bool                   m_bShieldedChanged; //�������仯���λ True-�ı� False-δ�ı�
	bool                   m_bInheritMultipleROI; //ģ��֧�ּ̳ж�ROI���λ True-֧�� False-��֧��
	bool                   m_bModuProMask; //�����ڲ�������Ĥ���ɱ��λ  True-���ദ�� False-���ദ��
	int                    m_nModuleId; // ģ��ID

	//�㷨�����������
	BASE_MODU_ROI_DATA        m_stBaseModuROIData;         //����ROI����µ�����
	IMVS_POLYGON_MASK_LIST    m_stShieldedPolygon;         // �����������
	MVBI_FIDUCIAL_POSE        m_stFixInit;                // ����ROIʱ��λ��������׼��
	MVBI_FIDUCIAL_POSE        m_stFixRun;                 // ����ROIʱ��λ���������е�
	MVD_SIZE_I               m_stLastImageSize;           // �ϴ�ִ�е�ͼ��ߴ�

private:

	//����SDK��ض���
	IMvdImage*             m_pInputImageInstance;           //����ͼ�����

	//���������������
	IMvdRectangleF*         m_pRectROIObj[IMVS_MULTIPLE_ROI_COUNT_MAX];//ROI�����������л���
	IMvdAnnularSectorF*     m_pAnnuROIObj[IMVS_MULTIPLE_ROI_COUNT_MAX];//ROI�Ȼ��������л���
	IMvdPolygonF*           m_pPolyROIObj[IMVS_MULTIPLE_ROI_COUNT_MAX];//ROI������������л���
	IMvdPolygonF*           m_pPolyMaskObj[IMVS_ROI_POLYGOPN_COUNT_MAX];//������������������л���
	IMvdRectangleF*         m_pRectCaliperObj[IMVS_MAX_CALIPER_NUMBER];//ROI�����������л���
	unsigned int            m_nMultipleROILen;//ROI���ݽ����������ݳ���
	char                   m_szMultipleROIBuf[IMVS_COMMON_POLYGON_LEN_MAX * IMVS_MULTIPLE_ROI_COUNT_MAX]; //ROI���ݽ�������
	unsigned int            m_nPolygonMaskLen;//ROI���ݽ����������ݳ���
	char                   m_szPolygonMaskBuf[IMVS_COMMON_POLYGON_LEN_MAX * IMVS_ROI_POLYGOPN_COUNT_MAX]; //��������������ݽ�������
	char                   m_chszLog[LOG_STRING_SIZE]; //��־��Ϣ
};

#endif // _IMVS_MVDSDK_BASE_MODU_SDK_H_
