/***************************************************************************************************
*
* 版权信息：版权所有 (c) 2020, 杭州海康机器人技术有限公司, 保留所有权利
*
* 文件名称：VmAlgModuBase.cpp
* 摘    要：算法模块基类
*
* 当前版本：
* 作    者:  视觉平台小组
* 日    期：2020-10-14
* 备    注：新建
****************************************************************************************************/

#include "VmAlgModuBase.h"

// 构造
CVmAlgModuleBase::CVmAlgModuleBase() :
m_nModuleId(MODULE_ID_INVALID),
m_pPosFixInstance(NULL),
m_pPreproMaskInstance(NULL),
m_pInputImageInstance(NULL)
{
	Init();
}

// 析构
CVmAlgModuleBase::~CVmAlgModuleBase()
{
	DeInit();
}

// 初始化
int CVmAlgModuleBase::Init()
{
	ResetMember();

	return IMVS_EC_OK;
}

//反初始化
void CVmAlgModuleBase::DeInit()
{
	if (NULL != m_pPreproMaskInstance)
	{
		DestroyPreproMaskToolInstance(m_pPreproMaskInstance);
		m_pPreproMaskInstance = NULL;
	}

	if (NULL != m_pPosFixInstance)
	{
		DestroyPositionFixToolInstance(m_pPosFixInstance);
		m_pPosFixInstance = NULL;
	}

	if (NULL != m_pInputImageInstance)
	{
		DestroyImageInstance(m_pInputImageInstance);
		m_pInputImageInstance = NULL;
	}

	for (size_t index = 0; index < IMVS_MULTIPLE_ROI_COUNT_MAX; index++)
	{
		if (NULL != m_pRectROIObj[index])
		{
			DestroyShapeInstance(m_pRectROIObj[index]);
			m_pRectROIObj[index] = NULL;
		}
		if (NULL != m_pAnnuROIObj[index])
		{
			DestroyShapeInstance(m_pAnnuROIObj[index]);
			m_pAnnuROIObj[index] = NULL;
		}
		if (NULL != m_pPolyROIObj[index])
		{
			DestroyShapeInstance(m_pPolyROIObj[index]);
			m_pPolyROIObj[index] = NULL;
		}
	}

	for (size_t index = 0; index < IMVS_ROI_POLYGOPN_COUNT_MAX; index++)
	{
		if (NULL != m_pPolyMaskObj[index])
		{
			DestroyShapeInstance(m_pPolyMaskObj[index]);
			m_pPolyMaskObj[index] = NULL;
		}
	}

	for (size_t index = 0; index < IMVS_MAX_CALIPER_NUMBER; index++)
	{
		if (NULL != m_pRectCaliperObj[index])
		{
			DestroyShapeInstance(m_pRectCaliperObj[index]);
			m_pRectCaliperObj[index] = NULL;
		}
	}
}

//重置成员变量
void CVmAlgModuleBase::ResetMember()
{
	//模块逻辑相关数据
	m_bRoiChanged = true;
	m_bShieldedChanged = true;
	m_bInheritMultipleROI = false;
	m_bModuProMask = false;
	m_nModuleId = MODULE_ID_INVALID;

	//算法处理相关数据
	memset(&m_stShieldedPolygon, 0, sizeof(m_stShieldedPolygon));
	memset(&m_stFixInit, 0, sizeof(m_stFixInit));
	memset(&m_stFixRun, 0, sizeof(m_stFixRun));
	memset(&m_stLastImageSize, 0, sizeof(m_stLastImageSize));

	//基类运行所需变量
	memset(m_pRectROIObj, 0, sizeof(m_pRectROIObj));
	memset(m_pAnnuROIObj, 0, sizeof(m_pAnnuROIObj));
	memset(m_pPolyROIObj, 0, sizeof(m_pPolyROIObj));
	memset(m_pPolyMaskObj, 0, sizeof(m_pPolyMaskObj));
	memset(m_pRectCaliperObj, 0, sizeof(m_pRectCaliperObj));
	memset(m_szMultipleROIBuf, 0, sizeof(m_szMultipleROIBuf));
	memset(m_szPolygonMaskBuf, 0, sizeof(m_szPolygonMaskBuf));
	memset(m_chszLog, 0, sizeof(m_chszLog));
	m_nMultipleROILen = 0;
	m_nPolygonMaskLen = 0;
}

// 获取算法处理输入
int CVmAlgModuleBase::GetProcessInput(IN const void * const hInput, OUT MVDSDK_BASE_MODU_INPUT_INNER *pModuInput)
{
	HKA_CHECK_ERROR((hInput == NULL || pModuInput == NULL), IMVS_EC_PARAM);

	HKA_S32     nRet = IMVS_EC_OK;
	HKA_U32     nImageInputStatus = 0;
	HKA_IMAGE   imgInput = { HKA_IMG_MONO_08, 0 };
	HKA_S32     iImageDataLen = 0;
	HKA_SIZE_I  imgSize = { 0 };
	HKA_U32     nFixInputStatus = 0;
	MVBI_FIDUCIAL_POSE   fixInit = { 0.0f };
	MVBI_FIDUCIAL_POSE   fixRun = { 0.0f };

	//获取输入图像
	nRet = VmModule_GetInputImageEx(hInput, &imgInput, iImageDataLen, &nImageInputStatus);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, IMVS_EC_ALGORITHM_INPUT_IMAGE_ERROR);
	HKA_CHECK_ERROR((imgInput.height <= 0) || (imgInput.width <= 0), IMVS_EC_PARAM);

	imgSize.height = imgInput.height;
	imgSize.width = imgInput.width;

	pModuInput->stImageIn = imgInput;
	pModuInput->nImageInputStatus = nImageInputStatus;

	//获取输入ROI
	do
	{
		//先逐类型获取订阅ROI 同时只支持一种ROI订阅 如无订阅则采用绘制ROI
		{
			//尝试获取矩形ROI
			{
				HKA_U32     nBoxROIStatus = 0;
				vector<MVBI_BOX_F> roiBox;

				nRet = VmModule_GetInputRoiBox(hInput, imgSize, roiBox, &nBoxROIStatus, m_bInheritMultipleROI);
				HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, IMVS_EC_ALGORITHM_INPUT_ROI_ERROR, "VmModule_GetInputRoiBox");

				//已订阅ROI 则采用并跳过其他
				if (IMVS_ROI_FROM_INPUT == nBoxROIStatus)
				{
					pModuInput->stRoiBox.assign(roiBox.begin(), roiBox.end());
					pModuInput->iRoiTypeIndex = IMVS_ROI_TYPE_BOX;
					pModuInput->nROIInputStatus = IMVS_ROI_FROM_INPUT;
					break;
				}
			}

			//尝试获取扇环ROI
			{
				HKA_U32     nAnnulusROIStatus = 0;
				vector<MVBI_ANNULUS_F> roiAnnulus;

				nRet = VmModule_GetInputRoiAnnulus(hInput, imgSize, roiAnnulus, &nAnnulusROIStatus, m_bInheritMultipleROI);
				HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, IMVS_EC_ALGORITHM_INPUT_ROI_ERROR, "VmModule_GetInputRoiAnnulus");

				//已订阅ROI 则采用并跳过其他
				if (IMVS_ROI_FROM_INPUT == nAnnulusROIStatus)
				{
					pModuInput->stRoiAnnulus.assign(roiAnnulus.begin(), roiAnnulus.end());
					pModuInput->iRoiTypeIndex = IMVS_ROI_TYPE_ANNULUS;
					pModuInput->nROIInputStatus = IMVS_ROI_FROM_INPUT;
					break;
				}
			}
		}

		//无订阅采用绘制ROI 区分不同类型ROI处理 同时将界面归一化数据转换像素点数据
		{
			//全图ROI或矩形ROI或混合ROI
			if ((IMVS_ROI_TYPE_IMAGE == m_stBaseModuROIData.iRoiTypeIndex)
				|| (IMVS_ROI_TYPE_BOX == m_stBaseModuROIData.iRoiTypeIndex)
				|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
			{
				if (!m_stBaseModuROIData.stRoiBox.empty())
				{
					IMVS_ROI_BOX roiUI = { 0.0f };
					MVBI_BOX_F roiBox = { 0.0f };

					pModuInput->stRoiBox.clear();

					for (size_t index = 0; index < m_stBaseModuROIData.stRoiBox.size(); index++)
					{
						roiUI = m_stBaseModuROIData.stRoiBox[index];

						roiBox.center_x = roiUI.fRoiCenterX* imgSize.width;
						roiBox.center_y = roiUI.fRoiCenterY * imgSize.height;
						roiBox.width = roiUI.fRoiWidth * imgSize.width;
						roiBox.height = roiUI.fRoiHeight * imgSize.height;
						roiBox.angle = roiUI.fRoiAngle;

						pModuInput->stRoiBox.push_back(roiBox);
					}

					pModuInput->iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
					pModuInput->nROIInputStatus = IMVS_ROI_FROM_UI;
				}
			}

			//扇环ROI
			if ((IMVS_ROI_TYPE_ANNULUS == m_stBaseModuROIData.iRoiTypeIndex)
				|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
			{
				if (!m_stBaseModuROIData.stRoiAnnulus.empty())
				{
					IMVS_ANNULUS roiUI = { 0.0f };
					MVBI_ANNULUS_F roiAnnulus = { 0.0f };

					pModuInput->stRoiAnnulus.clear();

					for (size_t index = 0; index < m_stBaseModuROIData.stRoiAnnulus.size(); index++)
					{
						roiUI = m_stBaseModuROIData.stRoiAnnulus[index];

						roiAnnulus.center.x = roiUI.center.x * imgSize.width;
						roiAnnulus.center.y = roiUI.center.y * imgSize.height;
						roiAnnulus.inner_radius = roiUI.inner_radius * imgSize.width;
						roiAnnulus.outer_radius = roiUI.outer_radius * imgSize.width;
						roiAnnulus.start_angle = roiUI.start_angle;
						roiAnnulus.angle_extend = (roiUI.end_angle > roiUI.start_angle) ? (roiUI.end_angle - roiUI.start_angle) : (360 + (roiUI.end_angle - roiUI.start_angle));

						pModuInput->stRoiAnnulus.push_back(roiAnnulus);
					}

					pModuInput->iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
					pModuInput->nROIInputStatus = IMVS_ROI_FROM_UI;
				}
			}

			//多边形ROI
			if ((IMVS_ROI_TYPE_POLYGON == m_stBaseModuROIData.iRoiTypeIndex)
				|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
			{
				if (!m_stBaseModuROIData.stRoiPolygon.empty())
				{
					IMVS_POLYGON roiUI = { 0 };
					IMVS_POLYGON roiPolygon = { 0.0f };

					pModuInput->stRoiPolygon.clear();

					for (size_t index = 0; index < m_stBaseModuROIData.stRoiPolygon.size(); index++)
					{
						roiUI = m_stBaseModuROIData.stRoiPolygon[index];

						for (size_t point = 0; point < roiUI.vertex_num; point++)
						{
							roiPolygon.vertex_points[point].x = roiUI.vertex_points[point].x* imgSize.width;
							roiPolygon.vertex_points[point].y = roiUI.vertex_points[point].y * imgSize.height;
						}

						roiPolygon.vertex_num = roiUI.vertex_num;

						pModuInput->stRoiPolygon.push_back(roiPolygon);
					}

					pModuInput->iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
					pModuInput->nROIInputStatus = IMVS_ROI_FROM_UI;
				}
			}

			//直线卡尺ROI
			if ((IMVS_ROI_TYPE_LINECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
			{
				// 填充box列表
				pModuInput->stLineCaliperBox.clear();

				for (int i = 0; i < m_stBaseModuROIData.stLineCaliperBox.nNum; i++)
				{
					MVBI_BOX_F stBox = { 0 };
					stBox.center_x = (m_stBaseModuROIData.stLineCaliperBox.fStartX*imgSize.width) + (i + 1) * ((m_stBaseModuROIData.stLineCaliperBox.fEndX - m_stBaseModuROIData.stLineCaliperBox.fStartX)*imgSize.width) / (m_stBaseModuROIData.stLineCaliperBox.nNum + 1);
					stBox.center_y = (m_stBaseModuROIData.stLineCaliperBox.fStartY*imgSize.height) + (i + 1) * ((m_stBaseModuROIData.stLineCaliperBox.fEndY - m_stBaseModuROIData.stLineCaliperBox.fStartY)*imgSize.height) / (m_stBaseModuROIData.stLineCaliperBox.nNum + 1);
					stBox.width = m_stBaseModuROIData.stLineCaliperBox.fWidth * imgSize.width;
					stBox.height = m_stBaseModuROIData.stLineCaliperBox.fHeight * imgSize.height;
					if (m_stBaseModuROIData.stLineCaliperBox.fAngle > 180.0)
					{
						stBox.angle = m_stBaseModuROIData.stLineCaliperBox.fAngle - 360.0;
					}
					else
					{
						stBox.angle = m_stBaseModuROIData.stLineCaliperBox.fAngle;
					}

					pModuInput->stLineCaliperBox.push_back(stBox);
				}

				// 计算卡尺的外接BOX作为直线查找的ROI
				MVBI_BOX_F stBox = { 0 };
				stBox.center_x = (m_stBaseModuROIData.stLineCaliperBox.fStartX + m_stBaseModuROIData.stLineCaliperBox.fEndX) / 2 * imgSize.width;
				stBox.center_y = (m_stBaseModuROIData.stLineCaliperBox.fStartY + m_stBaseModuROIData.stLineCaliperBox.fEndY) / 2 * imgSize.height;
				stBox.width = sqrt((m_stBaseModuROIData.stLineCaliperBox.fEndX * imgSize.width - m_stBaseModuROIData.stLineCaliperBox.fStartX * imgSize.width)*(m_stBaseModuROIData.stLineCaliperBox.fEndX * imgSize.width - m_stBaseModuROIData.stLineCaliperBox.fStartX * imgSize.width) + (m_stBaseModuROIData.stLineCaliperBox.fEndY * imgSize.height - m_stBaseModuROIData.stLineCaliperBox.fStartY * imgSize.height)*(m_stBaseModuROIData.stLineCaliperBox.fEndY * imgSize.height - m_stBaseModuROIData.stLineCaliperBox.fStartY * imgSize.height));
				stBox.height = m_stBaseModuROIData.stLineCaliperBox.fHeight * imgSize.height;
				if (m_stBaseModuROIData.stLineCaliperBox.fAngle > 180)
				{
					stBox.angle = m_stBaseModuROIData.stLineCaliperBox.fAngle - 360.0;
				}
				else
				{
					stBox.angle = m_stBaseModuROIData.stLineCaliperBox.fAngle;
				}

				pModuInput->stRoiBox.push_back(stBox);

				pModuInput->iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
				pModuInput->nROIInputStatus = IMVS_ROI_FROM_UI;
			}

			//圆形卡尺ROI
			if ((IMVS_ROI_TYPE_CIRCLECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
			{
				// 填充box列表
				pModuInput->stCircleCaliperBox.clear();

				float fAngleExtend = (m_stBaseModuROIData.stCircleCaliperBox.fEndAngle > m_stBaseModuROIData.stCircleCaliperBox.fStartAngle) ? (m_stBaseModuROIData.stCircleCaliperBox.fEndAngle - m_stBaseModuROIData.stCircleCaliperBox.fStartAngle) : (360 + (m_stBaseModuROIData.stCircleCaliperBox.fEndAngle - m_stBaseModuROIData.stCircleCaliperBox.fStartAngle));
				for (int i = 0; i < m_stBaseModuROIData.stCircleCaliperBox.nNum; i++)
				{

					float fAngle = m_stBaseModuROIData.stCircleCaliperBox.fStartAngle + (fAngleExtend / m_stBaseModuROIData.stCircleCaliperBox.nNum) * (0.5 + i);
					float fPointX = m_stBaseModuROIData.stCircleCaliperBox.fCenterX*imgSize.width + m_stBaseModuROIData.stCircleCaliperBox.fOutterRadius * imgSize.width * cos(fAngle * MVB_PI / 180);
					float fPointY = m_stBaseModuROIData.stCircleCaliperBox.fCenterY * imgSize.height + m_stBaseModuROIData.stCircleCaliperBox.fOutterRadius * imgSize.width * sin(fAngle * MVB_PI / 180);

					MVBI_BOX_F stBox = { 0 };
					stBox.center_x = fPointX;
					stBox.center_y = fPointY;
					stBox.width = m_stBaseModuROIData.stCircleCaliperBox.fWidth * imgSize.width;
					stBox.height = m_stBaseModuROIData.stCircleCaliperBox.fHeight * imgSize.height;
					if (fAngle > 180.0)
					{
						stBox.angle = fAngle - 360.0;
					}
					else
					{
						stBox.angle = fAngle;
					}

					pModuInput->stCircleCaliperBox.push_back(stBox);
				}

				// 计算卡尺的外接ANNULUS作为圆查找的ROI
				IMVS_ANNULUS roiUI = { 0.0f };
				MVBI_ANNULUS_F roiAnnulus = { 0.0f };

				pModuInput->stRoiAnnulus.clear();

				for (size_t index = 0; index < m_stBaseModuROIData.stRoiAnnulus.size(); index++)
				{
					roiUI = m_stBaseModuROIData.stRoiAnnulus[index];

					roiAnnulus.center.x = roiUI.center.x * imgSize.width;
					roiAnnulus.center.y = roiUI.center.y * imgSize.height;
					roiAnnulus.inner_radius = roiUI.inner_radius * imgSize.width;
					roiAnnulus.outer_radius = roiUI.outer_radius * imgSize.width;
					roiAnnulus.start_angle = roiUI.start_angle;
					roiAnnulus.angle_extend = (roiUI.end_angle > roiUI.start_angle) ? (roiUI.end_angle - roiUI.start_angle) : (360 + (roiUI.end_angle - roiUI.start_angle));

					pModuInput->stRoiAnnulus.push_back(roiAnnulus);
				}

				pModuInput->iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
				pModuInput->nROIInputStatus = IMVS_ROI_FROM_UI;
			}
		}
	}while (0);

	//屏蔽区将界面归一化数据转换像素点数据
	for (size_t polygonIndex = 0; polygonIndex < m_stShieldedPolygon.nCount; polygonIndex++)
	{
		IMVS_POLYGON stPolygon = m_stShieldedPolygon.stMaskNormalized[polygonIndex];

		for (size_t pointIndex = 0; pointIndex < stPolygon.vertex_num; pointIndex++)
		{
			m_stShieldedPolygon.stMaskPixel[polygonIndex].vertex_points[pointIndex].x = stPolygon.vertex_points[pointIndex].x*imgSize.width;
			m_stShieldedPolygon.stMaskPixel[polygonIndex].vertex_points[pointIndex].y = stPolygon.vertex_points[pointIndex].y*imgSize.height;
		}

		m_stShieldedPolygon.stMaskPixel[polygonIndex].vertex_num = stPolygon.vertex_num;
	}

	//获取修正信息
	nRet = VmModule_GetInputFixture(hInput, &fixInit, &fixRun, &nFixInputStatus);
	HKA_MODU_CHECK_ERROR(IMVS_EC_OK != nRet, IMVS_EC_ALGORITHM_INPUT_FIXTUREINFO_ERROR);

	//开启修正后更改修正信息
	if (IMVS_MODU_ENUM_STATUS_OK == nFixInputStatus)
	{
		//首次执行 如果没有绘制的基准点则更新
		if (m_stFixInit.basis_angle == 0 && m_stFixInit.basis_point.x == 0 && m_stFixInit.basis_point.y == 0)
		{
			m_stFixInit = fixInit;
		}

		//每次执行跟随图像变化更新绘制时的运行点
		m_stFixRun = fixRun;

		//绘制ROI时基准点使用原始记录
		if (IMVS_ROI_FROM_UI == pModuInput->nROIInputStatus)
		{
			fixInit = m_stFixInit;
		}

		//继承ROI时运行点同步基准点
		if (IMVS_ROI_FROM_INPUT == pModuInput->nROIInputStatus)
		{
			m_stFixRun = m_stFixInit;
		}
	}
	else
	{
		//关闭修正后更改修正信息
		m_stFixRun = m_stFixInit;
	}

	//每次执行跟随图像变化更新图像尺寸
	m_stLastImageSize.nWidth = pModuInput->stImageIn.width;
	m_stLastImageSize.nHeight = pModuInput->stImageIn.height;

	pModuInput->stFixInit = fixInit;
	pModuInput->stFixRun = fixRun;
	pModuInput->nFixInputStatus = nFixInputStatus;

	return nRet;
}

// 进行掩膜生成算法
int CVmAlgModuleBase::GenerateMaskImage(MVDSDK_BASE_MODU_INPUT* pModuInput, vector<IMvdShape*> vctROIShape, vector<IMvdShape*> vctShieldedShape, bool bCheckDiff)
{
	int nRet = IMVS_EC_OK;

	if (NULL == m_pPreproMaskInstance)
	{
		nRet = CreatePreproMaskToolInstance(&m_pPreproMaskInstance);
		HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet,"CreatePreproMaskToolInstance");
	}

	MVDSDK_TRY
	{
        bool bIsDiff = true;

        if (bCheckDiff)
        {
            unsigned int nRegionNum = 0;
            unsigned int nRoiNum = 0;
            unsigned int nShieldedNum = 0;
            IMvdShape* pRegionShape = NULL;
            bool bRegionType = true;
            size_t nRegionIndex = 0;

            nRegionNum = m_pPreproMaskInstance->GetNumOfRegions();

            for (nRegionIndex = 0; nRegionIndex < nRegionNum; nRegionIndex++)
            {
                m_pPreproMaskInstance->GetRegionAt(nRegionIndex, &pRegionShape, &bRegionType);

                if (bRegionType)
                {
                    nRoiNum++;

                    if (nRoiNum <= vctROIShape.size())
                    {
                        bool bIsShapeDiff = true;
                        MVD_SHAPE_TYPE nTypeUsed = pRegionShape->GetShapeType();
                        MVD_SHAPE_TYPE nTypeRecent = vctROIShape[nRoiNum - 1]->GetShapeType();

                        if (nTypeUsed != nTypeRecent)
                        {
                            break;
                        }

                        switch (nTypeUsed)
                        {
                        case MvdShapeRectangle:
                        {
                            IMvdRectangleF* pShapeUsed = (IMvdRectangleF*)pRegionShape;
                            IMvdRectangleF* pShapeRecent = (IMvdRectangleF*)vctROIShape[nRoiNum - 1];

                            if ((pShapeUsed->GetCenterX() == pShapeRecent->GetCenterX())
                                && (pShapeUsed->GetCenterY() == pShapeRecent->GetCenterY())
                                && (pShapeUsed->GetWidth() == pShapeRecent->GetWidth())
                                && (pShapeUsed->GetHeight() == pShapeRecent->GetHeight())
                                && (pShapeUsed->GetAngle() == pShapeRecent->GetAngle()))
                            {
                                bIsShapeDiff = false;
                            }

                            break;
                        }
                        case MvdShapeAnnularSector:
                        {
                            IMvdAnnularSectorF* pShapeUsed = (IMvdAnnularSectorF*)pRegionShape;
                            IMvdAnnularSectorF* pShapeRecent = (IMvdAnnularSectorF*)vctROIShape[nRoiNum - 1];

                            if ((pShapeUsed->GetCenter().fX == pShapeRecent->GetCenter().fX)
                                && (pShapeUsed->GetCenter().fY == pShapeRecent->GetCenter().fY)
                                && (pShapeUsed->GetInnerRadius() == pShapeRecent->GetInnerRadius())
                                && (pShapeUsed->GetOuterRadius() == pShapeRecent->GetOuterRadius())
                                && (pShapeUsed->GetStartAngle() == pShapeRecent->GetStartAngle())
                                && (pShapeUsed->GetAngleRange() == pShapeRecent->GetAngleRange()))
                            {
                                bIsShapeDiff = false;
                            }

                            break;
                        }
                        case MvdShapePolygon:
                        {
                            IMvdPolygonF* pShapeUsed = (IMvdPolygonF*)pRegionShape;
                            IMvdPolygonF* pShapeRecent = (IMvdPolygonF*)vctROIShape[nRoiNum - 1];
                            float fXUsed, fYUsed, fXRecent, fYRecent = 0.0f;
                            size_t nPointIndex = 0;

                            if ((pShapeUsed->GetVertexNum() != pShapeRecent->GetVertexNum()))
                            {
                                bIsShapeDiff = true;
                                break;
                            }

                            for (nPointIndex = 0; nPointIndex < pShapeUsed->GetVertexNum(); nPointIndex++)
                            {
                                pShapeUsed->GetVertex(nPointIndex, fXUsed, fYUsed);
                                pShapeRecent->GetVertex(nPointIndex, fXRecent, fYRecent);

                                if ((fXUsed != fXRecent) || (fYUsed != fYRecent))
                                {
                                    break;
                                }
                            }

                            if (nPointIndex == pShapeUsed->GetVertexNum())
                            {
                                bIsShapeDiff = false;
                            }

                            break;
                        }
                        default:
                        {
                            bIsShapeDiff = true;
                            break;
                        }
                        }

                        if (bIsShapeDiff)
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    nShieldedNum++;

                    if (nShieldedNum <= vctShieldedShape.size())
                    {
                        MVD_SHAPE_TYPE nTypeUsed = pRegionShape->GetShapeType();
                        MVD_SHAPE_TYPE nTypeRecent = vctShieldedShape[nShieldedNum - 1]->GetShapeType();

                        if ((nTypeUsed != MvdShapePolygon) || (nTypeRecent != MvdShapePolygon))
                        {
                            break;
                        }

                        IMvdPolygonF* pShapeUsed = (IMvdPolygonF*)pRegionShape;
                        IMvdPolygonF* pShapeRecent = (IMvdPolygonF*)vctShieldedShape[nShieldedNum - 1];
                        float fXUsed, fYUsed, fXRecent, fYRecent = 0.0f;
                        size_t nPointIndex = 0;

                        if ((pShapeUsed->GetVertexNum() != pShapeRecent->GetVertexNum()))
                        {
                            break;
                        }

                        for (nPointIndex = 0; nPointIndex < pShapeUsed->GetVertexNum(); nPointIndex++)
                        {
                            pShapeUsed->GetVertex(nPointIndex, fXUsed, fYUsed);
                            pShapeRecent->GetVertex(nPointIndex, fXRecent, fYRecent);

                            if ((fXUsed != fXRecent) || (fYUsed != fYRecent))
                            {
                                break;
                            }
                        }

                        if (nPointIndex != pShapeUsed->GetVertexNum())
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            unsigned int nWidthUsed = 0;
            unsigned int nHeightUsed = 0;
            unsigned int nWidthRecent = 0;
            unsigned int nHeightRecent = 0;

            if (m_pPreproMaskInstance->GetInputImage() != NULL)
            {
                nWidthUsed = m_pPreproMaskInstance->GetInputImage()->GetWidth();
                nHeightUsed = m_pPreproMaskInstance->GetInputImage()->GetHeight();
            }

            if (pModuInput->pImageInObj != NULL)
            {
                nWidthRecent = pModuInput->pImageInObj->GetWidth();
                nHeightRecent = pModuInput->pImageInObj->GetHeight();
            }

            if ((nRegionIndex == nRegionNum)
                && (nRoiNum == vctROIShape.size())
                && (nShieldedNum == vctShieldedShape.size())
                && (nWidthUsed == nWidthRecent)
                && (nHeightUsed == nHeightRecent))
            {
                bIsDiff = false;
            }
        }

        if (bIsDiff)
        {
            m_pPreproMaskInstance->ClearRegions();

            for (size_t index = 0; index < vctROIShape.size(); index++)
            {
                m_pPreproMaskInstance->AddRegion(vctROIShape[index], true);
            }

            for (size_t index = 0; index < vctShieldedShape.size(); index++)
            {
                m_pPreproMaskInstance->AddRegion(vctShieldedShape[index], false);
            }

            m_pPreproMaskInstance->SetInputImage(pModuInput->pImageInObj);
        }

        m_pPreproMaskInstance->Run();

        pModuInput->pImageMaskObj = m_pPreproMaskInstance->GetOutputImage();
    }
        MVDSDK_CATCH

        HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "GenerateMaskImage");

    pModuInput->bUseMask = true;

    return nRet;
}

// 初始化位置修正算子
int CVmAlgModuleBase::InitPosFixInstance(IN MVBI_FIDUCIAL_POSE stFixInit,IN MVBI_FIDUCIAL_POSE stFixRun,IN MVD_SIZE_I stImgSize)
{
	int nRet = IMVS_EC_OK;

	//创建位置修正算子
	if (NULL == m_pPosFixInstance)
	{
		nRet = CreatePositionFixToolInstance(&m_pPosFixInstance);
		HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreatePositionFixToolInstance");
	}

	MVDSDK_TRY
	{
		IPositionFixBasicParam* pBasicParam = m_pPosFixInstance->GetBasicParam();
		MVD_FIDUCIAL_POINT_F stFixPoint = { 0 };

		//位置修正参数设置
		stFixPoint.stPosition.fX = stFixInit.basis_point.x;
		stFixPoint.stPosition.fY = stFixInit.basis_point.y;
		stFixPoint.fAngle = stFixInit.basis_angle;
		stFixPoint.fScaleX = stFixInit.basis_scale_x;
		stFixPoint.fScaleY = stFixInit.basis_scale_y;
		pBasicParam->SetBasePoint(stFixPoint);

		stFixPoint.stPosition.fX = stFixRun.basis_point.x;
		stFixPoint.stPosition.fY = stFixRun.basis_point.y;
		stFixPoint.fAngle = stFixRun.basis_angle;
		stFixPoint.fScaleX = stFixRun.basis_scale_x;
		stFixPoint.fScaleY = stFixRun.basis_scale_y;
		pBasicParam->SetRunningPoint(stFixPoint);

		pBasicParam->SetFixMode(MVD_POSFIX_MODE_HVAS);

		pBasicParam->SetRunImageSize(stImgSize);
	}
		MVDSDK_CATCH

	HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "InitPosFixInstance");

	return nRet;
}

// 位置修正算法处理
int CVmAlgModuleBase::ProcessFix(IN MVDSDK_BASE_MODU_INPUT_INNER *pModuInput, OUT MVDSDK_BASE_MODU_INPUT* pFixInput)
{
	HKA_MODU_CHECK_ERROR((pModuInput == NULL || pFixInput == NULL), IMVS_EC_PARAM);

	int nRet = IMVS_EC_OK;

	//转换图像数据
	{
		MVD_IMAGE_DATA_INFO stImageData = { 0 };

		//pFixInput->struInputImg = pModuInput->stImageIn;

		if (NULL == m_pInputImageInstance)
		{
			nRet = CreateImageInstance(&m_pInputImageInstance);
			HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet,"CreateImageInstance");
		}
		for (size_t channel = 0; channel < MAX_IMAGE_CHANNEL_NUM; channel++)
		{
			stImageData.stDataChannel[channel].pData = (unsigned char*)pModuInput->stImageIn.data[channel];
			stImageData.stDataChannel[channel].nRowStep = pModuInput->stImageIn.step[channel];
			stImageData.stDataChannel[channel].nLen = pModuInput->stImageIn.height * pModuInput->stImageIn.step[channel];
			stImageData.stDataChannel[channel].nSize = pModuInput->stImageIn.height * pModuInput->stImageIn.step[channel];
		}

		MVDSDK_TRY
		{
			m_pInputImageInstance->InitImage(pModuInput->stImageIn.width, pModuInput->stImageIn.height,
			(MVD_PIXEL_FORMAT)pModuInput->stImageIn.format, stImageData);
		}
			MVDSDK_CATCH

		HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet,"ProcessFix");

		pFixInput->pImageInObj = m_pInputImageInstance;
		pFixInput->nImageInputStatus = pModuInput->nImageInputStatus;
	}

	//转换ROI数据 位置修正也需要先转换
	{
		pFixInput->vtFixRoiShapeObj.clear();
		
		//全图ROI或矩形ROI或混合ROI
		if ((IMVS_ROI_TYPE_IMAGE == pModuInput->iRoiTypeIndex)
			|| (IMVS_ROI_TYPE_BOX == pModuInput->iRoiTypeIndex)
			|| (IMVS_ROI_TYPE_MULTIPLE == pModuInput->iRoiTypeIndex))
		{
			if (!pModuInput->stRoiBox.empty())
			{
				for (size_t index = 0; index < pModuInput->stRoiBox.size(); index++)
				{
					IMvdRectangleF* roiSDK = m_pRectROIObj[index];
					MVBI_BOX_F roiImage = { 0.0f };

					if (NULL == roiSDK)
					{
						nRet = CreateRectangleInstance(&roiSDK, 1, 1, 1, 1);
						HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

						m_pRectROIObj[index] = roiSDK;
					}

					roiImage = pModuInput->stRoiBox[index];

					MVDSDK_TRY
					{
						roiSDK->SetCenterX(roiImage.center_x);
						roiSDK->SetCenterY(roiImage.center_y);
						roiSDK->SetWidth(roiImage.width);
						roiSDK->SetHeight(roiImage.height);
						roiSDK->SetAngle(roiImage.angle);

						pFixInput->vtFixRoiShapeObj.push_back(roiSDK);
					}
						MVDSDK_CATCH
				}
			}
		}

		//扇环ROI
		if ((IMVS_ROI_TYPE_ANNULUS == pModuInput->iRoiTypeIndex)
			|| (IMVS_ROI_TYPE_MULTIPLE == pModuInput->iRoiTypeIndex))
		{
			if (!pModuInput->stRoiAnnulus.empty())
			{
				for (size_t index = 0; index < pModuInput->stRoiAnnulus.size(); index++)
				{
					IMvdAnnularSectorF* roiSDK = m_pAnnuROIObj[index];
					MVBI_ANNULUS_F roiImage = { 0.0f };
					MVD_POINT_F centerPoint = { 0.0f };

					if (NULL == roiSDK)
					{
						nRet = CreateAnnularSectorInstance(&roiSDK, centerPoint, 0, 1, 0, 360);
						HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateAnnularSectorInstance");

						m_pAnnuROIObj[index] = roiSDK;
					}

					roiImage = pModuInput->stRoiAnnulus[index];

					centerPoint.fX = roiImage.center.x;
					centerPoint.fY = roiImage.center.y;

					MVDSDK_TRY
					{
						roiSDK->SetCenter(centerPoint);

						if (roiSDK->GetInnerRadius() < roiImage.outer_radius)
						{
							roiSDK->SetOuterRadius(roiImage.outer_radius);
							roiSDK->SetInnerRadius(roiImage.inner_radius);
						}
						else
						{
							roiSDK->SetInnerRadius(roiImage.inner_radius);
							roiSDK->SetOuterRadius(roiImage.outer_radius);
						}

						roiSDK->SetStartAngle(roiImage.start_angle);
						roiSDK->SetAngleRange(roiImage.angle_extend);

						pFixInput->vtFixRoiShapeObj.push_back(roiSDK);
					}
						MVDSDK_CATCH
				}
			}
		}

		//多边形ROI
		if ((IMVS_ROI_TYPE_POLYGON == pModuInput->iRoiTypeIndex)
			|| (IMVS_ROI_TYPE_MULTIPLE == pModuInput->iRoiTypeIndex))
		{
			if (!pModuInput->stRoiPolygon.empty())
			{
				for (size_t index = 0; index < pModuInput->stRoiPolygon.size(); index++)
				{
					IMvdPolygonF* roiSDK = m_pPolyROIObj[index];
					IMVS_POLYGON roiImage = { 0 };

					if (NULL == roiSDK)
					{
						nRet = CreatePolygonInstance(&roiSDK);
						HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreatePolygonInstance");

						m_pPolyROIObj[index] = roiSDK;
					}

					roiImage = pModuInput->stRoiPolygon[index];

					MVDSDK_TRY
					{
						roiSDK->ClearVertices();
						for (size_t pointIndex = 0; pointIndex < roiImage.vertex_num; pointIndex++)
						{
							roiSDK->AddVertex(roiImage.vertex_points[pointIndex].x, roiImage.vertex_points[pointIndex].y);
						}

						pFixInput->vtFixRoiShapeObj.push_back(roiSDK);
					}
						MVDSDK_CATCH
				}
			}
		}

		//直线卡尺ROI
		if ((IMVS_ROI_TYPE_LINECALIPER == pModuInput->iRoiTypeIndex))
		{
			if (pModuInput->stLineCaliperBox.empty())
			{
				nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
			}
			else
			{
				for (int i = 0; i < pModuInput->stLineCaliperBox.size(); i++)
				{
					IMvdRectangleF* roiSDK = m_pRectCaliperObj[i];
					MVBI_BOX_F roiImage = { 0.0f };

					if (NULL == roiSDK)
					{
						nRet = CreateRectangleInstance(&roiSDK, 0, 0, 0, 0);
						HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

						m_pRectCaliperObj[i] = roiSDK;
					}

					roiImage = pModuInput->stLineCaliperBox[i];

					MVDSDK_TRY
					{
						roiSDK->SetCenterX(roiImage.center_x);
						roiSDK->SetCenterY(roiImage.center_y);
						roiSDK->SetWidth(roiImage.width);
						roiSDK->SetHeight(roiImage.height);
						roiSDK->SetAngle(roiImage.angle);

						pFixInput->vctFixLineCaliperObj.push_back(roiSDK);
					}
						MVDSDK_CATCH
				}

				IMvdRectangleF* roiSDK = m_pRectROIObj[0];
				MVBI_BOX_F roiImage = { 0.0f };

				if (NULL == roiSDK)
				{
					nRet = CreateRectangleInstance(&roiSDK, 1, 1, 1, 1);
					HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

					m_pRectROIObj[0] = roiSDK;
				}

				roiImage = pModuInput->stRoiBox[0];

				MVDSDK_TRY
				{
					roiSDK->SetCenterX(roiImage.center_x);
					roiSDK->SetCenterY(roiImage.center_y);
					roiSDK->SetWidth(roiImage.width);
					roiSDK->SetHeight(roiImage.height);
					roiSDK->SetAngle(roiImage.angle);

					pFixInput->vtFixRoiShapeObj.push_back(roiSDK);
				}
					MVDSDK_CATCH
			}
		}

		//圆形卡尺ROI
		if ((IMVS_ROI_TYPE_CIRCLECALIPER == pModuInput->iRoiTypeIndex))
		{
			if (pModuInput->stCircleCaliperBox.empty())
			{
				nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
			}
			else
			{
				for (int i = 0; i < pModuInput->stCircleCaliperBox.size(); i++)
				{
					IMvdRectangleF* roiSDK = m_pRectCaliperObj[i];
					MVBI_BOX_F roiImage = { 0.0f };

					if (NULL == roiSDK)
					{
						nRet = CreateRectangleInstance(&roiSDK, 0, 0, 0, 0);
						HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

						m_pRectCaliperObj[i] = roiSDK;
					}

					roiImage = pModuInput->stCircleCaliperBox[i];

					MVDSDK_TRY
					{
						roiSDK->SetCenterX(roiImage.center_x);
						roiSDK->SetCenterY(roiImage.center_y);
						roiSDK->SetWidth(roiImage.width);
						roiSDK->SetHeight(roiImage.height);
						roiSDK->SetAngle(roiImage.angle);

						pFixInput->vctFixCircleCaliperObj.push_back(roiSDK);
					}
						MVDSDK_CATCH
				}

				IMvdAnnularSectorF* roiSDK = m_pAnnuROIObj[0];
				MVBI_ANNULUS_F roiImage = { 0.0f };
				MVD_POINT_F centerPoint = { 0.0f };

				if (NULL == roiSDK)
				{
					nRet = CreateAnnularSectorInstance(&roiSDK, centerPoint, 0, 1, 0, 360);
					HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreateAnnularSectorInstance");

					m_pAnnuROIObj[0] = roiSDK;
				}

				roiImage = pModuInput->stRoiAnnulus[0];

				centerPoint.fX = roiImage.center.x;
				centerPoint.fY = roiImage.center.y;

				MVDSDK_TRY
				{
					roiSDK->SetInnerRadius(0);
					roiSDK->SetOuterRadius(0);

					roiSDK->SetCenter(centerPoint);
					roiSDK->SetOuterRadius(roiImage.outer_radius);
					roiSDK->SetInnerRadius(roiImage.inner_radius);
					
					roiSDK->SetStartAngle(roiImage.start_angle);
					roiSDK->SetAngleRange(roiImage.angle_extend);

					pFixInput->vtFixRoiShapeObj.push_back(roiSDK);
				}
					MVDSDK_CATCH
			}
		}

		HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "ProcessFix");
		HKA_CHECK_ERROR_LOG(pFixInput->vtFixRoiShapeObj.empty(), IMVS_EC_ALGORITHM_INPUT_ROI_ERROR, "ProcessFix");
		
		pFixInput->iRoiTypeIndex = pModuInput->iRoiTypeIndex;
		pFixInput->nROIInputStatus = pModuInput->nROIInputStatus;
	}

	//转换屏蔽区数据 位置修正也需要先转换
	{
		pFixInput->vctfixShieldedPolygon.clear();

		if (0 != m_stShieldedPolygon.nCount)
		{
			for (size_t index = 0; index < m_stShieldedPolygon.nCount; index++)
			{
				IMvdPolygonF* maskSDK = m_pPolyMaskObj[index];
				IMVS_POLYGON maskImage = { 0 };

				if (NULL == maskSDK)
				{
					nRet = CreatePolygonInstance(&maskSDK);
					HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "CreatePolygonInstance");

					m_pPolyMaskObj[index] = maskSDK;
				}

				maskImage = m_stShieldedPolygon.stMaskPixel[index];

				MVDSDK_TRY
				{
					maskSDK->ClearVertices();
					for (size_t pointIndex = 0; pointIndex < maskImage.vertex_num; pointIndex++)
					{
						maskSDK->AddVertex(maskImage.vertex_points[pointIndex].x, maskImage.vertex_points[pointIndex].y);
					}

					pFixInput->vctfixShieldedPolygon.push_back(maskSDK);
				}
					MVDSDK_CATCH
			}

			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "ProcessFix");
		}
	}
	
	//若启用修正功能则进行修正
	{
		if ((IMVS_MODU_ENUM_STATUS_OK == pModuInput->nFixInputStatus)
			&& !(pModuInput->stFixInit == pModuInput->stFixRun))
		{
			//初始化修正算子
			MVD_SIZE_I imageSize = { 0 };

			imageSize.nWidth = pModuInput->stImageIn.width;
			imageSize.nHeight = pModuInput->stImageIn.height;

			nRet = InitPosFixInstance(pModuInput->stFixInit, pModuInput->stFixRun, imageSize);
			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "InitPosFixInstance");

			//修正ROI数据
			{
				MVDSDK_TRY
				{
					switch (pModuInput->iRoiTypeIndex)
					{
					case IMVS_ROI_TYPE_IMAGE:
					case IMVS_ROI_TYPE_BOX:
					case IMVS_ROI_TYPE_ANNULUS:
					case IMVS_ROI_TYPE_POLYGON:
					case IMVS_ROI_TYPE_MULTIPLE:
					{
						if (!pFixInput->vtFixRoiShapeObj.empty())
						{
							for (size_t index = 0; index < pFixInput->vtFixRoiShapeObj.size(); index++)
							{
								m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vtFixRoiShapeObj[index]);
								m_pPosFixInstance->Run();

								//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
								{
									MVD_SHAPE_TYPE shapeType = m_pPosFixInstance->GetResult()->GetCorrectedShape()->GetShapeType();

									switch (shapeType)
									{
									case MvdShapeRectangle:
									{
										IMvdRectangleF* pInitShape = (IMvdRectangleF*)pFixInput->vtFixRoiShapeObj[index];
										IMvdRectangleF* pFixShape = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();
										
										pInitShape->SetCenterX(pFixShape->GetCenterX());
										pInitShape->SetCenterY(pFixShape->GetCenterY());
										pInitShape->SetWidth(pFixShape->GetWidth());
										pInitShape->SetHeight(pFixShape->GetHeight());
										pInitShape->SetAngle(pFixShape->GetAngle());
									
										break;
									}
									case MvdShapeAnnularSector:
									{
										IMvdAnnularSectorF* pInitShape = (IMvdAnnularSectorF*)pFixInput->vtFixRoiShapeObj[index];
										IMvdAnnularSectorF* pFixShape = (IMvdAnnularSectorF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

										pInitShape->SetCenter(pFixShape->GetCenter());

										if (pInitShape->GetInnerRadius() < pFixShape->GetOuterRadius())
										{
											pInitShape->SetOuterRadius(pFixShape->GetOuterRadius());
											pInitShape->SetInnerRadius(pFixShape->GetInnerRadius());
										}
										else
										{
											pInitShape->SetInnerRadius(pFixShape->GetInnerRadius());
											pInitShape->SetOuterRadius(pFixShape->GetOuterRadius());
										}

										pInitShape->SetStartAngle(pFixShape->GetStartAngle());
										pInitShape->SetAngleRange(pFixShape->GetAngleRange());

										break;
									}
									case MvdShapePolygon:
									{
										IMvdPolygonF* pInitShape = (IMvdPolygonF*)pFixInput->vtFixRoiShapeObj[index];
										IMvdPolygonF* pFixShape = (IMvdPolygonF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();
										float fX, fY = 0.0f;

										pInitShape->ClearVertices();
										for (size_t pointIndex = 0; pointIndex < pFixShape->GetVertexNum(); pointIndex++)
										{
											pFixShape->GetVertex(pointIndex, fX, fY);
											pInitShape->AddVertex(fX,fY);
										}

										break;
									}
									default:
									{
										nRet = IMVS_EC_ALGORITHM_ROITYPE_UNKNOWN;

										break;
									}
									}
								}
								//else
								//{
								//	nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
								//}
							}
						}

						break;
					}
					case IMVS_ROI_TYPE_LINECALIPER:
					{
						if (!pFixInput->vtFixRoiShapeObj.empty())
						{
							m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vtFixRoiShapeObj[0]);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdRectangleF* pInitShape = (IMvdRectangleF*)pFixInput->vtFixRoiShapeObj[0];
								IMvdRectangleF* pFixShape = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

								pInitShape->SetCenterX(pFixShape->GetCenterX());
								pInitShape->SetCenterY(pFixShape->GetCenterY());
								pInitShape->SetWidth(pFixShape->GetWidth());
								pInitShape->SetHeight(pFixShape->GetHeight());
								pInitShape->SetAngle(pFixShape->GetAngle());
							}
						}

						if (!pFixInput->vctFixLineCaliperObj.empty())
						{
							//pFixInput->vctFixLineCaliperObj.clear();
							for (size_t index = 0; index < pFixInput->vctFixLineCaliperObj.size(); index++)
							{
								m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vctFixLineCaliperObj[index]);
								m_pPosFixInstance->Run();

								//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
								{
									//pFixInput->vctFixLineCaliperObj[index] = m_pPosFixInstance->GetResult()->GetCorrectedShape();

									IMvdRectangleF* pInitShape = (IMvdRectangleF*)pFixInput->vctFixLineCaliperObj[index];
									IMvdRectangleF* pFixShape = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

									pInitShape->SetCenterX(pFixShape->GetCenterX());
									pInitShape->SetCenterY(pFixShape->GetCenterY());
									pInitShape->SetWidth(pFixShape->GetWidth());
									pInitShape->SetHeight(pFixShape->GetHeight());
									pInitShape->SetAngle(pFixShape->GetAngle());
								}
								//else
								//{
								//	nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
								//}
							}
						}

						break;
					}
					case IMVS_ROI_TYPE_CIRCLECALIPER:
					{
						if (!pFixInput->vtFixRoiShapeObj.empty())
						{
							m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vtFixRoiShapeObj[0]);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdAnnularSectorF* pInitShape = (IMvdAnnularSectorF*)pFixInput->vtFixRoiShapeObj[0];
								IMvdAnnularSectorF* pFixShape = (IMvdAnnularSectorF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

								pInitShape->SetCenter(pFixShape->GetCenter());

								if (pInitShape->GetInnerRadius() < pFixShape->GetOuterRadius())
								{
									pInitShape->SetOuterRadius(pFixShape->GetOuterRadius());
									pInitShape->SetInnerRadius(pFixShape->GetInnerRadius());
								}
								else
								{
									pInitShape->SetInnerRadius(pFixShape->GetInnerRadius());
									pInitShape->SetOuterRadius(pFixShape->GetOuterRadius());
								}

								pInitShape->SetStartAngle(pFixShape->GetStartAngle());
								pInitShape->SetAngleRange(pFixShape->GetAngleRange());
							}

						}

						if (!pFixInput->vctFixCircleCaliperObj.empty())
						{
							//pFixInput->vctFixLineCaliperObj.clear();
							for (size_t index = 0; index < pFixInput->vctFixCircleCaliperObj.size(); index++)
							{
								m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vctFixCircleCaliperObj[index]);
								m_pPosFixInstance->Run();

								//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
								{
									//pFixInput->vctFixCircleCaliperObj[index] = m_pPosFixInstance->GetResult()->GetCorrectedShape();

									IMvdRectangleF* pInitShape = (IMvdRectangleF*)pFixInput->vctFixCircleCaliperObj[index];
									IMvdRectangleF* pFixShape = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

									pInitShape->SetCenterX(pFixShape->GetCenterX());
									pInitShape->SetCenterY(pFixShape->GetCenterY());
									pInitShape->SetWidth(pFixShape->GetWidth());
									pInitShape->SetHeight(pFixShape->GetHeight());
									pInitShape->SetAngle(pFixShape->GetAngle());
								}
								//else
								//{
								//	nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
								//}
							}
						}
						break;
					}
					default:
					{
						nRet = IMVS_EC_ALGORITHM_ROITYPE_UNKNOWN;

						break;
					}
					}
				}
				MVDSDK_CATCH

				HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "ProcessFix");
			}

			//修正屏蔽区数据
			{
				MVDSDK_TRY
				{
					if (!pFixInput->vctfixShieldedPolygon.empty())
					{
						for (size_t index = 0; index < pFixInput->vctfixShieldedPolygon.size(); index++)
						{
							m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixInput->vctfixShieldedPolygon[index]);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdPolygonF* pInitShape = (IMvdPolygonF*)pFixInput->vctfixShieldedPolygon[index];
								IMvdPolygonF* pFixShape = (IMvdPolygonF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();
								float fX, fY = 0.0f;

								pInitShape->ClearVertices();
								for (size_t pointIndex = 0; pointIndex < pFixShape->GetVertexNum(); pointIndex++)
								{
									pFixShape->GetVertex(pointIndex, fX, fY);
									pInitShape->AddVertex(fX, fY);
								}
							}
							//else
							//{
							//	nRet = IMVS_EC_ALGORITHM_INPUT_ROI_ERROR;
							//}
						}
					}
				}
				MVDSDK_CATCH

				HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "ProcessFix");
			}

			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "ProcessFix");
		}
		
		pFixInput->stFixInit.stPosition.fX = pModuInput->stFixInit.basis_point.x;
		pFixInput->stFixInit.stPosition.fY = pModuInput->stFixInit.basis_point.y;
		pFixInput->stFixInit.fAngle = pModuInput->stFixInit.basis_angle;
		pFixInput->stFixInit.fScaleX = pModuInput->stFixInit.basis_scale_x;
		pFixInput->stFixInit.fScaleY = pModuInput->stFixInit.basis_scale_y;

		pFixInput->stFixRun.stPosition.fX = pModuInput->stFixRun.basis_point.x;
		pFixInput->stFixRun.stPosition.fY = pModuInput->stFixRun.basis_point.y;
		pFixInput->stFixRun.fAngle = pModuInput->stFixRun.basis_angle;
		pFixInput->stFixRun.fScaleX = pModuInput->stFixRun.basis_scale_x;
		pFixInput->stFixRun.fScaleY = pModuInput->stFixRun.basis_scale_y;

		pFixInput->nFixInputStatus = pModuInput->nFixInputStatus;
	}
	
	//根据修正后数据生成掩膜图像
	if (!m_bModuProMask)
	{
		//正常情况下 存在屏蔽区则生成掩膜
		if (!pFixInput->vctfixShieldedPolygon.empty())
		{
			//TODO 增加生成掩膜判定条件
			if (true)
			{
                nRet = GenerateMaskImage(pFixInput, pFixInput->vtFixRoiShapeObj, pFixInput->vctfixShieldedPolygon, true);
				HKA_CHECK_ERROR_LOG(MVD_OK != nRet, nRet, "GenerateMaskImage");
			}
			else
			{
				pFixInput->pImageMaskObj = m_pPreproMaskInstance->GetOutputImage();
				pFixInput->bUseMask = true;
			}
		}
		else
		{
			pFixInput->pImageMaskObj = NULL;
			pFixInput->bUseMask = false;
		}
	}

	return nRet;
}

// 算法处理结果输出
int CVmAlgModuleBase::ProcessOutput(IN const void * const hOutput, MVDSDK_BASE_MODU_INPUT &pModuInput)
{
	HKA_MODU_CHECK_ERROR((IMVS_NULL == hOutput), IMVS_EC_PARAM);

	HKA_S32  nRet = IMVS_EC_OK;

	if (!pModuInput.vtFixRoiShapeObj.empty())
	{
		int nRectNum = 0;
		int nAnnuNum = 0;
		HKA_S32 nROIInputStatus = HKA_TRUE;

		nROIInputStatus = (IMVS_ROI_FROM_ERROR != pModuInput.nROIInputStatus);

		MVDSDK_TRY
		{
			for (size_t index = 0; index < pModuInput.vtFixRoiShapeObj.size(); index++)
			{
				MVD_SHAPE_TYPE shapeType = pModuInput.vtFixRoiShapeObj[index]->GetShapeType();

				switch (shapeType)
				{
				case MvdShapeRectangle:
				{
					IMvdRectangleF* pRoiShape = (IMvdRectangleF*)pModuInput.vtFixRoiShapeObj[index];

					pModuInput.struDetectBox.center_x = pRoiShape->GetCenterX();
					pModuInput.struDetectBox.center_y = pRoiShape->GetCenterY();
					pModuInput.struDetectBox.width = pRoiShape->GetWidth();
					pModuInput.struDetectBox.height = pRoiShape->GetHeight();
					pModuInput.struDetectBox.angle = pRoiShape->GetAngle();

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_x,
						"DetectCenterX",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_y,
						"DetectCenterY",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.width,
						"DetectWidth",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.height,
						"DetectHeight",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.angle,
						"DetectAngle",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRectNum++;

					break;
				}
				case MvdShapeAnnularSector:
				{
					IMvdAnnularSectorF* pRoiShape = (IMvdAnnularSectorF*)pModuInput.vtFixRoiShapeObj[index];

					pModuInput.struDetectBox.center_x = pRoiShape->GetCenter().fX;
					pModuInput.struDetectBox.center_y = pRoiShape->GetCenter().fY;
					pModuInput.struDetectBox.width = 2.0f * HKA_MAX(pRoiShape->GetInnerRadius(), pRoiShape->GetOuterRadius());
					pModuInput.struDetectBox.height = 2.0f * HKA_MAX(pRoiShape->GetInnerRadius(), pRoiShape->GetOuterRadius());
					pModuInput.struDetectBox.angle = 0.0f;

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_x,
						"DetectCenterX",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_y,
						"DetectCenterY",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.width,
						"DetectWidth",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.height,
						"DetectHeight",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.angle,
						"DetectAngle",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRectNum++;

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetCenter().fX,
						"DetectAnnulusCenterX",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetCenter().fY,
						"DetectAnnulusCenterY",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetInnerRadius(),
						"DetectAnnulusInnerRadius",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetOuterRadius(),
						"DetectAnnulusOuterRadius",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetStartAngle(),
						"DetectAnnulusStartAngle",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pRoiShape->GetAngleRange(),
						"DetectAnnulusAngleExtend",
						nAnnuNum,
						nAnnuNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nAnnuNum++;

					break;
				}
				case MvdShapePolygon:
				{
					IMvdPolygonF* pRoiShape = (IMvdPolygonF*)pModuInput.vtFixRoiShapeObj[index];
					float minX = INFINITE;
					float minY = INFINITE;
					float maxX = 0.0f;
					float maxY = 0.0f;
					float pointX = 0.0f;
					float pointY = 0.0f;

					for (size_t pointIndex = 0; pointIndex < pRoiShape->GetVertexNum(); pointIndex++)
					{
						pRoiShape->GetVertex(pointIndex, pointX, pointY);

						minX = min(minX, pointX);
						minY = min(minY, pointY);
						maxX = max(maxX, pointX);
						maxY = max(maxY, pointY);
					}

					pModuInput.struDetectBox.center_x = (minX + maxX) / 2.0f;
					pModuInput.struDetectBox.center_y = (minY + maxY) / 2.0;
					pModuInput.struDetectBox.width = maxX - minX;
					pModuInput.struDetectBox.height = maxY - minY;
					pModuInput.struDetectBox.angle = 0.0f;

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_x,
						"DetectCenterX",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.center_y,
						"DetectCenterY",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.width,
						"DetectWidth",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.height,
						"DetectHeight",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRet = VmModule_OutputScalar_32f(hOutput,
						nROIInputStatus,
						pModuInput.struDetectBox.angle,
						"DetectAngle",
						nRectNum,
						nRectNum);
					HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

					nRectNum++;

					break;
				}
				default:
				{
					nRet = IMVS_EC_ALGORITHM_ROITYPE_UNKNOWN;

					break;
				}
				}

				HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);
			}
		}
		MVDSDK_CATCH
		HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);
	}

	return nRet;
}

// 进行算法处理
int CVmAlgModuleBase::Process(IN void* hInput, IN void* hOutput)
{
	HKA_MODU_CHECK_ERROR((IMVS_NULL == hInput || IMVS_NULL == hOutput), IMVS_EC_PARAM);

	HKA_S32  nRet = IMVS_EC_OK;
	MVDSDK_BASE_MODU_INPUT_INNER  stModuInputInner;
	MVDSDK_BASE_MODU_INPUT stSDKModuInput; // 位置修正并转换算子SDK结构后输入模块子类

	// 获取模块输入数据
	nRet = GetProcessInput(hInput, &stModuInputInner);
	HKA_MODU_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);
	HKA_MODU_CHECK_ERROR(!stModuInputInner.nImageInputStatus, IMVS_EC_ALGORITHM_INPUT_IMAGE_ERROR);
	HKA_MODU_CHECK_ERROR(IMVS_ROI_FROM_ERROR == stModuInputInner.nROIInputStatus, IMVS_EC_ALGORITHM_INPUT_ROI_ERROR);

	do
	{
		nRet = ProcessFix(&stModuInputInner, &stSDKModuInput);
		HKA_CHECK_BREAK(IMVS_EC_OK != nRet);

		nRet = ProcessOutput(hOutput, stSDKModuInput);
		if (nRet != IMVS_EC_OK)
		{
			//MLOG_ERROR(m_nModuleId, "Base ProcessOutput return [%x]", nRet);
		}

		nRet = Process(hInput, hOutput, &stSDKModuInput);
		HKA_CHECK_BREAK(IMVS_EC_OK != nRet);
	} while (0);

	return nRet;
}

// 获取算法参数
int CVmAlgModuleBase::GetParam(IN const char* szParamName, OUT char* pBuff, IN int nBuffSize, OUT int* pDataLen)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == szParamName || HKA_NULL == pBuff), IMVS_EC_PARAM, "GetParam");

	int nRet = IMVS_EC_OK;

	_snprintf_s(m_chszLog, LOG_STRING_SIZE, LOG_STRING_SIZE - 1, "CVmAlgModuleBase::GetParamValue,\t%s, \t%s\n", szParamName, pBuff);
	OutputDebugStringA(m_chszLog);

	// 内部信息
	if ((0 == strcmp("RoiType", szParamName)))
	{
		//绘制ROI情况下的ROI数据
		if (m_stFixInit == m_stFixRun)
		{
			//无修正则返回原始图形
			nRet = GenerateMultipleROIMsg(&m_stBaseModuROIData, m_szMultipleROIBuf, sizeof(m_szMultipleROIBuf), m_nMultipleROILen);
			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "GenerateMultipleROIMsg");
		}
		else
		{
			do
			{
				//初始化修正算子
				nRet = InitPosFixInstance(m_stFixInit, m_stFixRun, m_stLastImageSize);
				HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "InitPosFixInstance");

				//修正ROI数据
				{
					BASE_MODU_ROI_DATA stBaseModuROIData;

					nRet = ClearRoiData(&stBaseModuROIData);
					HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "ClearRoiData");

					//全图ROI或矩形ROI或混合ROI
					if ((IMVS_ROI_TYPE_IMAGE == m_stBaseModuROIData.iRoiTypeIndex)
						|| (IMVS_ROI_TYPE_BOX == m_stBaseModuROIData.iRoiTypeIndex)
						|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
					{
						if (!m_stBaseModuROIData.stRoiBox.empty())
						{
							IMvdRectangleF* roiSDK = NULL;
							IMVS_ROI_BOX roiUI = { 0.0f };

							nRet = CreateRectangleInstance(&roiSDK, 1, 1, 1, 1);
							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

							for (size_t index = 0; index < m_stBaseModuROIData.stRoiBox.size(); index++)
							{
								roiUI = m_stBaseModuROIData.stRoiBox[index];

								MVDSDK_TRY
								{
									roiSDK->SetCenterX(roiUI.fRoiCenterX* m_stLastImageSize.nWidth);
									roiSDK->SetCenterY(roiUI.fRoiCenterY* m_stLastImageSize.nHeight);
									roiSDK->SetWidth(roiUI.fRoiWidth* m_stLastImageSize.nWidth);
									roiSDK->SetHeight(roiUI.fRoiHeight* m_stLastImageSize.nHeight);
									roiSDK->SetAngle(roiUI.fRoiAngle);

									m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiSDK);
									m_pPosFixInstance->Run();

									//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
									{
										IMvdRectangleF* roiFix = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

										roiUI.fRoiCenterX = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetCenterX() / m_stLastImageSize.nWidth);
										roiUI.fRoiCenterY = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetCenterY() / m_stLastImageSize.nHeight);
										roiUI.fRoiWidth = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetWidth() / m_stLastImageSize.nWidth);
										roiUI.fRoiHeight = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetHeight() / m_stLastImageSize.nHeight);
										roiUI.fRoiAngle = roiFix->GetAngle();

										if (isnan(roiUI.fRoiCenterX) || isinf(roiUI.fRoiCenterX)
											|| isnan(roiUI.fRoiCenterY) || isinf(roiUI.fRoiCenterY)
											|| isnan(roiUI.fRoiWidth) || isinf(roiUI.fRoiWidth)
											|| isnan(roiUI.fRoiHeight) || isinf(roiUI.fRoiHeight)
											|| isnan(roiUI.fRoiAngle) || isinf(roiUI.fRoiAngle))
										{
											nRet = IMVS_EC_DATA_ERROR;
										}

										stBaseModuROIData.stRoiBox.push_back(roiUI);
									}
									//else
									//{
									//	nRet = IMVS_EC_DATA_ERROR;
									//	break;
									//}
								}
									MVDSDK_CATCH
							}

							if (NULL != roiSDK)
							{
								DestroyShapeInstance(roiSDK);
								roiSDK = NULL;
							}

							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "RoiType");

							stBaseModuROIData.iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
						}
					}

					//扇环ROI
					if ((IMVS_ROI_TYPE_ANNULUS == m_stBaseModuROIData.iRoiTypeIndex)
						|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
					{
						if (!m_stBaseModuROIData.stRoiAnnulus.empty())
						{
							IMvdAnnularSectorF* roiSDK = NULL;
							IMVS_ANNULUS roiUI = { 0.0f };
							MVD_POINT_F centerPoint = { 0.0f };

							nRet = CreateAnnularSectorInstance(&roiSDK, centerPoint, 0, 1, 0, 360);
							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreateAnnularSectorInstance");

							for (size_t index = 0; index < m_stBaseModuROIData.stRoiAnnulus.size(); index++)
							{
								roiUI = m_stBaseModuROIData.stRoiAnnulus[index];

								centerPoint.fX = roiUI.center.x * m_stLastImageSize.nWidth;
								centerPoint.fY = roiUI.center.y * m_stLastImageSize.nHeight;

								MVDSDK_TRY
								{
									roiSDK->SetCenter(centerPoint);

									if (roiSDK->GetInnerRadius() < roiUI.outer_radius * m_stLastImageSize.nWidth)
									{
										roiSDK->SetOuterRadius(roiUI.outer_radius * m_stLastImageSize.nWidth);
										roiSDK->SetInnerRadius(roiUI.inner_radius * m_stLastImageSize.nWidth);
									}
									else
									{
										roiSDK->SetInnerRadius(roiUI.inner_radius * m_stLastImageSize.nWidth);
										roiSDK->SetOuterRadius(roiUI.outer_radius * m_stLastImageSize.nWidth);
									}

									roiSDK->SetStartAngle(roiUI.start_angle);
									roiSDK->SetAngleRange((roiUI.end_angle > roiUI.start_angle) ? (roiUI.end_angle - roiUI.start_angle) : (360 + (roiUI.end_angle - roiUI.start_angle)));

									m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiSDK);
									m_pPosFixInstance->Run();

									//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
									{
										IMvdAnnularSectorF* roiFix = (IMvdAnnularSectorF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

										roiUI.center.x = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetCenter().fX / m_stLastImageSize.nWidth);
										roiUI.center.y = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetCenter().fY / m_stLastImageSize.nHeight);
										roiUI.inner_radius = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetInnerRadius() / m_stLastImageSize.nWidth);
										roiUI.outer_radius = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetOuterRadius() / m_stLastImageSize.nWidth);
										roiUI.start_angle = roiFix->GetStartAngle();

										if (isnan(roiUI.start_angle) || isinf(roiUI.start_angle)
											|| isnan(roiUI.end_angle) || isinf(roiUI.end_angle)
											|| isnan(roiUI.inner_radius) || isinf(roiUI.inner_radius)
											|| isnan(roiUI.outer_radius) || isinf(roiUI.outer_radius)
											|| isnan(roiUI.center.x) || isinf(roiUI.center.x)
											|| isnan(roiUI.center.y) || isinf(roiUI.center.y))
										{
											nRet = IMVS_EC_DATA_ERROR;
										}

										if (360.0f == roiFix->GetAngleRange())
										{
											roiUI.end_angle = roiFix->GetStartAngle();
										}
										else
										{
											roiUI.end_angle = roiFix->GetStartAngle() + roiFix->GetAngleRange();
											if (roiUI.end_angle > 180.0f)
											{
												roiUI.end_angle -= 360.0f;
											}
										}

										stBaseModuROIData.stRoiAnnulus.push_back(roiUI);
									}
									//else
									//{
									//	nRet = IMVS_EC_DATA_ERROR;
									//	break;
									//}
								}
									MVDSDK_CATCH
							}

							if (NULL != roiSDK)
							{
								DestroyShapeInstance(roiSDK);
								roiSDK = NULL;
							}

							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "RoiType");

							stBaseModuROIData.iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
						}
					}

					//多边形ROI
					if ((IMVS_ROI_TYPE_POLYGON == m_stBaseModuROIData.iRoiTypeIndex)
						|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
					{
						if (!m_stBaseModuROIData.stRoiPolygon.empty())
						{
							IMvdPolygonF* roiSDK = NULL;
							IMVS_POLYGON roiUI = { 0 };
							float fX, fY = 0.0f;

							nRet = CreatePolygonInstance(&roiSDK);
							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreatePolygonInstance");

							for (size_t index = 0; index < m_stBaseModuROIData.stRoiPolygon.size(); index++)
							{
								roiUI = m_stBaseModuROIData.stRoiPolygon[index];

								MVDSDK_TRY
								{
									roiSDK->ClearVertices();
									for (size_t point = 0; point < roiUI.vertex_num; point++)
									{
										fX = roiUI.vertex_points[point].x* m_stLastImageSize.nWidth;
										fY = roiUI.vertex_points[point].y * m_stLastImageSize.nHeight;

										roiSDK->AddVertex(fX, fY);
									}

									m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiSDK);
									m_pPosFixInstance->Run();

									//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
									{
										IMvdPolygonF* roiFix = (IMvdPolygonF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

										for (size_t point = 0; point < roiFix->GetVertexNum(); point++)
										{
											roiFix->GetVertex(point, fX, fY);

											roiUI.vertex_points[point].x = (0 == m_stLastImageSize.nWidth) ? (0) : (fX / m_stLastImageSize.nWidth);
											roiUI.vertex_points[point].y = (0 == m_stLastImageSize.nHeight) ? (0) : (fY / m_stLastImageSize.nHeight);
										
											if (isnan(roiUI.vertex_points[point].x) || isinf(roiUI.vertex_points[point].x)
												|| isnan(roiUI.vertex_points[point].y) || isinf(roiUI.vertex_points[point].y))
											{
												nRet = IMVS_EC_DATA_ERROR;
											}
										}

										roiUI.vertex_num = roiFix->GetVertexNum();

										stBaseModuROIData.stRoiPolygon.push_back(roiUI);
									}
									//else
									//{
									//	nRet = IMVS_EC_DATA_ERROR;
									//	break;
									//}
								}
									MVDSDK_CATCH
							}

							if (NULL != roiSDK)
							{
								DestroyShapeInstance(roiSDK);
								roiSDK = NULL;
							}

							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "RoiType");

							stBaseModuROIData.iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
						}
					}

					//直线卡尺ROI
					if ((IMVS_ROI_TYPE_LINECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
					{
						IMvdPointSetF* pFixPointset = NULL;
						IMvdRectangleF* roiBox = NULL;
						MY_LINECALIPERBOX roiUI = { 0.0f };
						MVD_POINT_F startPoint = { 0.0f };
						MVD_POINT_F endPoint = { 0.0f };

						nRet = CreatePointSetInstance(&pFixPointset);
						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreatePointSetInstance");
						nRet = CreateRectangleInstance(&roiBox, 1, 1, 1, 1);
						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

						roiUI = m_stBaseModuROIData.stLineCaliperBox;
						startPoint.fX = roiUI.fStartX* m_stLastImageSize.nWidth;
						startPoint.fY = roiUI.fStartY* m_stLastImageSize.nHeight;
						endPoint.fX = roiUI.fEndX* m_stLastImageSize.nWidth;
						endPoint.fY = roiUI.fEndY* m_stLastImageSize.nHeight;

						MVDSDK_TRY
						{
							pFixPointset->AddPoint(startPoint.fX, startPoint.fY);
							pFixPointset->AddPoint(endPoint.fX, endPoint.fY);

							m_pPosFixInstance->GetBasicParam()->SetInitialShape(pFixPointset);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdPointSetF* pFixTemp = (IMvdPointSetF*)(m_pPosFixInstance->GetResult()->GetCorrectedShape());

								float fOutStartX = 0.0;
								float fOutStartY = 0.0;
								float fOutEndX = 0.0;
								float fOutEndY = 0.0;
								pFixTemp->GetPoint(0, fOutStartX, fOutStartY);
								pFixTemp->GetPoint(1, fOutEndX, fOutEndY);

								roiUI.fStartX = (0 == m_stLastImageSize.nWidth) ? (0) : (fOutStartX / m_stLastImageSize.nWidth);
								roiUI.fStartY = (0 == m_stLastImageSize.nHeight) ? (0) : (fOutStartY / m_stLastImageSize.nHeight);
								roiUI.fEndX = (0 == m_stLastImageSize.nWidth) ? (0) : (fOutEndX / m_stLastImageSize.nWidth);
								roiUI.fEndY = (0 == m_stLastImageSize.nHeight) ? (0) : (fOutEndY / m_stLastImageSize.nHeight);

							}
							//else
							//{
							//	nRet = IMVS_EC_DATA_ERROR;
							//}

							roiBox->SetCenterX((roiUI.fStartX + (roiUI.fEndX - roiUI.fStartX) / (roiUI.nNum + 1))* m_stLastImageSize.nWidth);
							roiBox->SetCenterY((roiUI.fStartY + (roiUI.fEndY - roiUI.fStartY) / (roiUI.nNum + 1))* m_stLastImageSize.nHeight);
							roiBox->SetWidth(roiUI.fWidth* m_stLastImageSize.nWidth);
							roiBox->SetHeight(roiUI.fHeight* m_stLastImageSize.nHeight);
							if (roiUI.fAngle > 180.0)
							{
								roiUI.fAngle -= 360;
							}
							roiBox->SetAngle(roiUI.fAngle);

							m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiBox);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdRectangleF* roiFix = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

								roiUI.fWidth = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetWidth() / m_stLastImageSize.nWidth);
								roiUI.fHeight = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetHeight() / m_stLastImageSize.nHeight);
								roiUI.fAngle = roiFix->GetAngle();
							}
							//else
							//{
							//	nRet = IMVS_EC_DATA_ERROR;
							//}

							stBaseModuROIData.stLineCaliperBox = roiUI;
						}
							MVDSDK_CATCH

							if (NULL != pFixPointset)
							{
								DestroyPointSetInstance(pFixPointset);
								pFixPointset = NULL;
							}

						if (NULL != roiBox)
						{
							DestroyShapeInstance(roiBox);
							roiBox = NULL;
						}

						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "RoiType");

						stBaseModuROIData.iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
					}

					//圆形卡尺ROI
					if ((IMVS_ROI_TYPE_CIRCLECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
					{
						IMvdAnnularSectorF* roiCircle = NULL;
						IMvdRectangleF* roiBox = NULL;
						MY_CIRCLECALIPERBOX roiUI = { 0.0f };
						MVD_POINT_F centerPoint = { 0.0f };

						nRet = CreateAnnularSectorInstance(&roiCircle, centerPoint, 0, 1, 0, 360);
						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreateAnnularSectorInstance");
						nRet = CreateRectangleInstance(&roiBox, 1, 1, 1, 1);
						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreateRectangleInstance");

						roiUI = m_stBaseModuROIData.stCircleCaliperBox;
						centerPoint.fX = roiUI.fCenterX* m_stLastImageSize.nWidth;
						centerPoint.fY = roiUI.fCenterY* m_stLastImageSize.nHeight;

						MVDSDK_TRY
						{
							roiCircle->SetCenter(centerPoint);
							roiCircle->SetInnerRadius(0);
							roiCircle->SetOuterRadius(roiUI.fOutterRadius * m_stLastImageSize.nWidth);
							roiCircle->SetStartAngle(roiUI.fStartAngle);
							roiCircle->SetAngleRange((roiUI.fEndAngle > roiUI.fStartAngle) ? (roiUI.fEndAngle - roiUI.fStartAngle) : (360 + (roiUI.fEndAngle - roiUI.fStartAngle)));

							m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiCircle);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdAnnularSectorF* roiFix = (IMvdAnnularSectorF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();


								roiUI.fCenterX = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetCenter().fX / m_stLastImageSize.nWidth);
								roiUI.fCenterY = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetCenter().fY / m_stLastImageSize.nHeight);
								roiUI.fOutterRadius = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetOuterRadius() / m_stLastImageSize.nWidth);
								roiUI.fStartAngle = roiFix->GetStartAngle();

								if (360.0f == roiFix->GetAngleRange())
								{
									roiUI.fEndAngle = roiFix->GetStartAngle();
								}
								else
								{
									roiUI.fEndAngle = roiFix->GetStartAngle() + roiFix->GetAngleRange();
									if (roiUI.fEndAngle > 180.0f)
									{
										roiUI.fEndAngle -= 180.0f;
									}
								}
							}
							//else
							//{
							//	nRet = IMVS_EC_DATA_ERROR;
							//}

							roiBox->SetCenterX(roiUI.fCenterX * m_stLastImageSize.nWidth);
							roiBox->SetCenterY((roiUI.fCenterY + roiUI.fOutterRadius)* m_stLastImageSize.nHeight);
							roiBox->SetWidth(roiUI.fWidth* m_stLastImageSize.nWidth);
							roiBox->SetHeight(roiUI.fHeight* m_stLastImageSize.nHeight);
							roiBox->SetAngle(0);

							m_pPosFixInstance->GetBasicParam()->SetInitialShape(roiBox);
							m_pPosFixInstance->Run();

							//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
							{
								IMvdRectangleF* roiFix = (IMvdRectangleF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

								roiUI.fWidth = (0 == m_stLastImageSize.nWidth) ? (0) : (roiFix->GetWidth() / m_stLastImageSize.nWidth);
								roiUI.fHeight = (0 == m_stLastImageSize.nHeight) ? (0) : (roiFix->GetHeight() / m_stLastImageSize.nHeight);
							}
							//else
							//{
							//	nRet = IMVS_EC_DATA_ERROR;
							//}

							stBaseModuROIData.stCircleCaliperBox = roiUI;
						}
							MVDSDK_CATCH

							if (NULL != roiCircle)
							{
								DestroyShapeInstance(roiCircle);
								roiCircle = NULL;
							}

						if (NULL != roiBox)
						{
							DestroyShapeInstance(roiBox);
							roiBox = NULL;
						}

						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "RoiType");

						stBaseModuROIData.iRoiTypeIndex = m_stBaseModuROIData.iRoiTypeIndex;
					}

					HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "RoiType");

					nRet = GenerateMultipleROIMsg(&stBaseModuROIData, m_szMultipleROIBuf, sizeof(m_szMultipleROIBuf), m_nMultipleROILen);
					HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "GenerateMultipleROIMsg");
				}
			} while (0);

			if (IMVS_EC_OK != nRet)
			{
				//修正失败则返回原始图形
				nRet = GenerateMultipleROIMsg(&m_stBaseModuROIData, m_szMultipleROIBuf, sizeof(m_szMultipleROIBuf), m_nMultipleROILen);
				HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "GenerateMultipleROIMsg");
			}
		}

		*pDataLen = m_nMultipleROILen;
		if (nBuffSize <= m_nMultipleROILen)
		{
			return IMVS_EC_PARAM_BUF_LEN;
		}
		memcpy_s(pBuff, nBuffSize, m_szMultipleROIBuf, m_nMultipleROILen);
	}
	else if (0 == strcmp("ExternRoiType", szParamName))
	{
		//屏蔽区数据
		if (m_stFixInit == m_stFixRun)
		{
			//无修正则返回原始图形
			nRet = GeneratePlygonShieldMsg(&m_stShieldedPolygon, m_szPolygonMaskBuf, sizeof(m_szPolygonMaskBuf), m_nPolygonMaskLen);
			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "GeneratePlygonShieldMsg");
		}
		else
		{
			//若修正则返回位置修正后图形
			if (0 != m_stShieldedPolygon.nCount)
			{
				do
				{
					//初始化修正算子
					nRet = InitPosFixInstance(m_stFixInit, m_stFixRun, m_stLastImageSize);
					HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "InitPosFixInstance");

					//修正屏蔽区数据
					{
						IMvdPolygonF* pPolygonInit = NULL;
						IMVS_POLYGON_MASK_LIST stShieldedPolygon = { 0 };

						MVDSDK_TRY
						{
							nRet = CreatePolygonInstance(&pPolygonInit);
							HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "CreatePolygonInstance");

							//逐个修正多边形
							for (size_t polygonIndex = 0; polygonIndex < m_stShieldedPolygon.nCount; polygonIndex++)
							{
								IMVS_POLYGON stPolygon = m_stShieldedPolygon.stMaskNormalized[polygonIndex];
								float fX, fY = 0.0f;

								//多边形数据格式转化
								pPolygonInit->ClearVertices();
								for (size_t pointIndex = 0; pointIndex < stPolygon.vertex_num; pointIndex++)
								{
									fX = stPolygon.vertex_points[pointIndex].x*m_stLastImageSize.nWidth;
									fY = stPolygon.vertex_points[pointIndex].y*m_stLastImageSize.nHeight;
									pPolygonInit->AddVertex(fX, fY);
								}

								//多边形位置修正
								m_pPosFixInstance->GetBasicParam()->SetInitialShape(pPolygonInit);
								m_pPosFixInstance->Run();

								//修正成功则拼接返回字符串
								//if (1 == m_pPosFixInstance->GetResult()->GetStatus())
								{
									IMvdPolygonF* pPolygonFixed = (IMvdPolygonF*)m_pPosFixInstance->GetResult()->GetCorrectedShape();

									unsigned int nVertexNum = pPolygonFixed->GetVertexNum();
									float vertexfX = 0.0f;
									float vertexfY = 0.0f;

									for (size_t pointIndex = 0; pointIndex < nVertexNum; pointIndex++)
									{
										pPolygonFixed->GetVertex(pointIndex, vertexfX, vertexfY);

										vertexfX = (0 == m_stLastImageSize.nWidth) ? (0) : (vertexfX / m_stLastImageSize.nWidth);
										vertexfY = (0 == m_stLastImageSize.nHeight) ? (0) : (vertexfY / m_stLastImageSize.nHeight);

										if (isnan(vertexfX) || isinf(vertexfX)
											|| isnan(vertexfY) || isinf(vertexfY))
										{
											nRet = IMVS_EC_DATA_ERROR;
										}

										stShieldedPolygon.stMaskNormalized[polygonIndex].vertex_points[pointIndex].x = vertexfX;
										stShieldedPolygon.stMaskNormalized[polygonIndex].vertex_points[pointIndex].y = vertexfY;
									}

									stShieldedPolygon.stMaskNormalized[polygonIndex].vertex_num = nVertexNum;
								}
								//else
								//{
								//	nRet = IMVS_EC_DATA_ERROR;
								//	break;
								//}
							}
						}
							MVDSDK_CATCH

							if (NULL != pPolygonInit)
							{
								DestroyShapeInstance(pPolygonInit);
								pPolygonInit = NULL;
							}

						HKA_CHECK_BREAK_LOG(MVD_OK != nRet, nRet, "ExternRoiType");

						stShieldedPolygon.nCount = m_stShieldedPolygon.nCount;

						nRet = GeneratePlygonShieldMsg(&stShieldedPolygon, m_szPolygonMaskBuf, sizeof(m_szPolygonMaskBuf), m_nPolygonMaskLen);
						HKA_CHECK_BREAK_LOG(IMVS_EC_OK != nRet, nRet, "GeneratePlygonShieldMsg");
					}
				} while (0);

				if (IMVS_EC_OK != nRet)
				{
					//修正失败则返回原始图形
					nRet = GeneratePlygonShieldMsg(&m_stShieldedPolygon, m_szPolygonMaskBuf, sizeof(m_szPolygonMaskBuf), m_nPolygonMaskLen);
					HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "GeneratePlygonShieldMsg");
				}
			}
			else
			{
				//无屏蔽区则返回空
				memset(m_szPolygonMaskBuf, 0, sizeof(m_szPolygonMaskBuf));
				m_nPolygonMaskLen = 0;
				
				m_szPolygonMaskBuf[m_nPolygonMaskLen] = IMVS_ROI_TYPE_NULL;
				m_nPolygonMaskLen += sizeof(byte);
				m_szPolygonMaskBuf[m_nPolygonMaskLen] = 0;
				m_nPolygonMaskLen += sizeof(byte);
			}
		}
		
		*pDataLen = m_nPolygonMaskLen;
		if (nBuffSize <= m_nPolygonMaskLen)
		{
			return IMVS_EC_PARAM_BUF_LEN;
		}
		memcpy_s(pBuff, nBuffSize, m_szPolygonMaskBuf, m_nPolygonMaskLen);
	}
	else if (0 == strcmp("FixtureInfo", szParamName))
	{
		//绘制ROI情况下的位置修正基准点数据
		sprintf_s(pBuff,
			nBuffSize,
			"Init %f %f %f %f %f",
			m_stFixInit.basis_point.x,
			m_stFixInit.basis_point.y,
			m_stFixInit.basis_angle,
			m_stFixInit.basis_scale_x,
			m_stFixInit.basis_scale_y);
	}
	else
	{
		return IMVS_EC_MODULE_PARAM_NOT_SUPPORT;
	}

	return nRet;
}

// 解析绘制ROI信息
int CVmAlgModuleBase::AnalyzeMultipleROIMsg(IN const char * pData, IN int nDataLen, OUT BASE_MODU_ROI_DATA* const pBaseModuROIData)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pData || HKA_NULL == pBaseModuROIData), IMVS_EC_PARAM, "AnalyzeMultipleROIMsg");

	int nRet = IMVS_EC_OK;
	unsigned int roiCount = 0; // ROI计数
	unsigned int offsetItem = 0; // 单个数据项偏移量
	unsigned int offsetCount = 0; // 整体数据偏移量
	char * offsetPointer = NULL; // 整体数据偏移指针
	IMVS_ROI_TYPE roiTypeTemp = IMVS_ROI_TYPE_NULL; // 当前解析ROI类型
	IMVS_ROI_TYPE roiTypeIndex = IMVS_ROI_TYPE_NULL; // 解析结果ROI类型
	vector<IMVS_ROI_BOX> stRoiBox; // 矩形ROI
	vector<IMVS_ANNULUS> stRoiAnnulus; // 扇环ROI
	vector<IMVS_POLYGON> stRoiPolygon; // 多边形ROI

	offsetPointer = (char*)pData;

	//绘制ROI界面下发信息格式 [byte ROI类型][byte 结构体版本][pointer 结构体数据] 逐个解析
	for (size_t index = 0; index < IMVS_MULTIPLE_ROI_COUNT_MAX; index++)
	{
		//解析ROI类型 结构体版本等信息
		offsetItem = sizeof(byte) + sizeof(byte);
		offsetCount += offsetItem;
		HKA_CHECK_BREAK(offsetCount > nDataLen);

		roiTypeTemp = (IMVS_ROI_TYPE)(byte)(*offsetPointer);
		offsetPointer += offsetItem;

		//解析结构体数据 区分不同类型ROI处理
		switch (roiTypeTemp)
		{
		case IMVS_ROI_TYPE_IMAGE:
		{
			IMVS_ROI_BOX roiItem = { 0.0f };

			roiItem.fRoiCenterX = 0.5f;
			roiItem.fRoiCenterY = 0.5f;
			roiItem.fRoiHeight = 1.0f;
			roiItem.fRoiWidth = 1.0f;
			roiItem.fRoiAngle = 0.0f;

			stRoiBox.push_back(roiItem);
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_BOX:
		{
			IMVS_ROI_BOX roiItem = { 0.0f };

			offsetItem = sizeof(IMVS_ROI_BOX);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&roiItem, sizeof(roiItem), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			stRoiBox.push_back(roiItem);
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_ANNULUS:
		{
			IMVS_ANNULUS roiItem = { 0.0f };

			offsetItem = sizeof(IMVS_ANNULUS);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&roiItem, sizeof(roiItem), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			stRoiAnnulus.push_back(roiItem);
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_POLYGON:
		{
			IMVS_POLYGON roiItem = { 0 };

			//多边形点数
			offsetItem = sizeof(int);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount >= nDataLen);

			roiItem.vertex_num = *((int*)offsetPointer);
			offsetPointer += offsetItem;
			HKA_CHECK_BREAK(roiItem.vertex_num > IMVS_ROI_POLYGOPN_POINT_COUNT_MAX);

			//多边形点集
			offsetItem = roiItem.vertex_num * sizeof(HKA_POINT_F);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&roiItem.vertex_points, sizeof(roiItem.vertex_points), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			stRoiPolygon.push_back(roiItem);
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_LINECALIPER:
		{
			MY_LINECALIPERBOX roiItem = { 0.0f };

			offsetItem = sizeof(MY_LINECALIPERBOX);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&roiItem, sizeof(roiItem), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			pBaseModuROIData->stLineCaliperBox = roiItem;
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_CIRCLECALIPER:
		{
			MY_CIRCLECALIPERBOX roiItem = { 0.0f };
			IMVS_ANNULUS  stUIAnnulus = { 0.0 };

			offsetItem = sizeof(MY_CIRCLECALIPERBOX);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&roiItem, sizeof(roiItem), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			stUIAnnulus.center.x = roiItem.fCenterX;
			stUIAnnulus.center.y = roiItem.fCenterY;
			stUIAnnulus.inner_radius = roiItem.fOutterRadius - roiItem.fWidth / 2;
			stUIAnnulus.outer_radius = roiItem.fOutterRadius + roiItem.fWidth / 2;
			stUIAnnulus.start_angle = roiItem.fStartAngle;
			stUIAnnulus.end_angle = roiItem.fEndAngle;

			pBaseModuROIData->stCircleCaliperBox = roiItem;
			stRoiAnnulus.push_back(stUIAnnulus);
			roiCount++;

			break;
		}
		case IMVS_ROI_TYPE_DEFAULT:
		{
			nRet = ResetDefaultRoi(pBaseModuROIData);
			HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "AnalyzeMultipleROIMsg");

			return nRet;
		}
		default:
		{
			nRet = IMVS_EC_ALGORITHM_ROITYPE_UNKNOWN;

			break;
		}
		}

		HKA_CHECK_BREAK(IMVS_EC_OK != nRet);
		HKA_CHECK_BREAK(offsetCount >= nDataLen);
	}

	HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "AnalyzeMultipleROIMsg");
	HKA_CHECK_ERROR_LOG(0 == roiCount, IMVS_EC_DATA_ERROR, "AnalyzeMultipleROIMsg");

	roiTypeIndex = (roiCount > 1) ? IMVS_ROI_TYPE_MULTIPLE : roiTypeTemp;

	pBaseModuROIData->stRoiBox.assign(stRoiBox.begin(), stRoiBox.end());
	pBaseModuROIData->stRoiAnnulus.assign(stRoiAnnulus.begin(), stRoiAnnulus.end());
	pBaseModuROIData->stRoiPolygon.assign(stRoiPolygon.begin(), stRoiPolygon.end());

	pBaseModuROIData->iRoiTypeIndex = roiTypeIndex;

	return nRet;
}

// 生成界面绘制ROI信息
int CVmAlgModuleBase::GenerateMultipleROIMsg(IN BASE_MODU_ROI_DATA* const stBaseModuROIData, OUT char* const pBuff, IN int nBuffSize, OUT unsigned int& nDataLen)
{
	HKA_CHECK_ERROR_LOG(HKA_NULL == pBuff, IMVS_EC_PARAM, "GenerateMultipleROIMsg");

	int nRet = IMVS_EC_OK;

	memset(pBuff, 0, nBuffSize);
	nDataLen = 0;

	//生成信息数据 区分不同类型ROI处理

	//全图ROI
	if (IMVS_ROI_TYPE_IMAGE == m_stBaseModuROIData.iRoiTypeIndex)
	{
		pBuff[nDataLen] = IMVS_ROI_TYPE_IMAGE;
		nDataLen += sizeof(byte);
		pBuff[nDataLen] = 0;
		nDataLen += sizeof(byte);
	}

	//矩形ROI或混合ROI
	if ((IMVS_ROI_TYPE_BOX == m_stBaseModuROIData.iRoiTypeIndex)
		|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
	{
		//逐个生成ROI
		for (size_t roiIndex = 0; roiIndex < stBaseModuROIData->stRoiBox.size(); roiIndex++)
		{
			pBuff[nDataLen] = IMVS_ROI_TYPE_BOX;
			nDataLen += sizeof(byte);
			pBuff[nDataLen] = 0;
			nDataLen += sizeof(byte);

			memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &(stBaseModuROIData->stRoiBox[roiIndex]), sizeof(IMVS_ROI_BOX));
			nDataLen += sizeof(IMVS_ROI_BOX);
		}
	}

	//扇环ROI
	if ((IMVS_ROI_TYPE_ANNULUS == m_stBaseModuROIData.iRoiTypeIndex)
		|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
	{
		//逐个生成ROI
		for (size_t roiIndex = 0; roiIndex < stBaseModuROIData->stRoiAnnulus.size(); roiIndex++)
		{
			pBuff[nDataLen] = IMVS_ROI_TYPE_ANNULUS;
			nDataLen += sizeof(byte);
			pBuff[nDataLen] = 0;
			nDataLen += sizeof(byte);

			memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &(stBaseModuROIData->stRoiAnnulus[roiIndex]), sizeof(IMVS_ANNULUS));
			nDataLen += sizeof(IMVS_ANNULUS);
		}
	}

	//多边形ROI
	if ((IMVS_ROI_TYPE_POLYGON == m_stBaseModuROIData.iRoiTypeIndex)
		|| (IMVS_ROI_TYPE_MULTIPLE == m_stBaseModuROIData.iRoiTypeIndex))
	{
		//逐个生成ROI
		for (size_t roiIndex = 0; roiIndex < stBaseModuROIData->stRoiPolygon.size(); roiIndex++)
		{
			int nVertexNum = stBaseModuROIData->stRoiPolygon[roiIndex].vertex_num;

			pBuff[nDataLen] = IMVS_ROI_TYPE_POLYGON;
			nDataLen += sizeof(byte);
			pBuff[nDataLen] = 0;
			nDataLen += sizeof(byte);

			memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &nVertexNum, sizeof(int));
			nDataLen += sizeof(int);

			memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, stBaseModuROIData->stRoiPolygon[roiIndex].vertex_points, nVertexNum*sizeof(HKA_POINT_F));
			nDataLen += nVertexNum*sizeof(HKA_POINT_F);
		}
	}

	//直线卡尺ROI
	if ((IMVS_ROI_TYPE_LINECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
	{
		pBuff[nDataLen] = IMVS_ROI_TYPE_LINECALIPER;
		nDataLen += sizeof(byte);
		pBuff[nDataLen] = 0;
		nDataLen += sizeof(byte);

		memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &(stBaseModuROIData->stLineCaliperBox), sizeof(MY_LINECALIPERBOX));
		nDataLen += sizeof(MY_LINECALIPERBOX);
	}

	//圆形卡尺ROI
	if ((IMVS_ROI_TYPE_CIRCLECALIPER == m_stBaseModuROIData.iRoiTypeIndex))
	{
		pBuff[nDataLen] = IMVS_ROI_TYPE_CIRCLECALIPER;
		nDataLen += sizeof(byte);
		pBuff[nDataLen] = 0;
		nDataLen += sizeof(byte);

		memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &(stBaseModuROIData->stCircleCaliperBox), sizeof(MY_CIRCLECALIPERBOX));
		nDataLen += sizeof(MY_CIRCLECALIPERBOX);
	}

	return nRet;
}

// 解析界面屏蔽区信息
int CVmAlgModuleBase::AnalyzePlygonShieldMsg(IN const char * pData, IN int nDataLen, OUT IMVS_POLYGON_MASK_LIST* const pShieldedPolygon)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pData || HKA_NULL == pShieldedPolygon), IMVS_EC_PARAM, "AnalyzePlygonShieldMsg");

	int nRet = IMVS_EC_OK;
	unsigned int shieldCount = 0; // 屏蔽区计数
	unsigned int offsetItem = 0; // 单个数据项偏移量
	unsigned int offsetCount = 0; // 整体数据偏移量
	char * offsetPointer = NULL; // 整体数据偏移指针
	IMVS_ROI_TYPE shieldType = IMVS_ROI_TYPE_NULL; // 当前解析形状类型
	vector<IMVS_POLYGON> stShieldPolygon; // 多边形屏蔽区

	offsetPointer = (char*)pData;

	//屏蔽区界面下发信息格式 [byte 屏蔽区类型][byte 结构体版本][pointer 结构体数据] 逐个解析
	for (size_t index = 0; index < IMVS_ROI_POLYGOPN_COUNT_MAX; index++)
	{
		//解析屏蔽区类型 结构体版本等信息
		offsetItem = sizeof(byte) + sizeof(byte);
		offsetCount += offsetItem;
		HKA_CHECK_BREAK(offsetCount >= nDataLen);

		shieldType = (IMVS_ROI_TYPE)(byte)(*offsetPointer);
		offsetPointer += offsetItem;

		if (IMVS_ROI_TYPE_POLYGON == shieldType)
		{
			IMVS_POLYGON shieldItem = { 0 };

			//多边形点数
			offsetItem = sizeof(int);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount >= nDataLen);

			shieldItem.vertex_num = *((int*)offsetPointer);
			offsetPointer += offsetItem;
			HKA_CHECK_BREAK(shieldItem.vertex_num > IMVS_ROI_POLYGOPN_POINT_COUNT_MAX);

			//多边形点集
			offsetItem = shieldItem.vertex_num * sizeof(HKA_POINT_F);
			offsetCount += offsetItem;
			HKA_CHECK_BREAK(offsetCount > nDataLen);

			memcpy_s(&shieldItem.vertex_points, sizeof(shieldItem.vertex_points), offsetPointer, offsetItem);
			offsetPointer += offsetItem;

			stShieldPolygon.push_back(shieldItem);
			shieldCount++;
		}
		else
		{
			nRet = IMVS_EC_DATA_ERROR;

			break;
		}

		HKA_CHECK_BREAK(offsetCount >= nDataLen);
	}

	HKA_CHECK_ERROR_LOG(IMVS_EC_OK != nRet, nRet, "AnalyzePlygonShieldMsg");

	for (size_t index = 0; index < shieldCount; index++)
	{
		pShieldedPolygon->stMaskNormalized[index] = stShieldPolygon[index];
	}
	pShieldedPolygon->nCount = shieldCount;

	return nRet;
}

// 生成界面屏蔽区信息
int CVmAlgModuleBase::GeneratePlygonShieldMsg(IN IMVS_POLYGON_MASK_LIST* const stShieldedPolygon, OUT char* const pBuff, IN int nBuffSize, OUT unsigned int& nDataLen)
{
	HKA_CHECK_ERROR_LOG(HKA_NULL == pBuff, IMVS_EC_PARAM, "GeneratePlygonShieldMsg");

	int nRet = IMVS_EC_OK;

	memset(pBuff, 0, nBuffSize);
	nDataLen = 0;

	//逐个生成屏蔽区
	for (size_t polygonIndex = 0; polygonIndex < stShieldedPolygon->nCount; polygonIndex++)
	{
		int nVertexNum = stShieldedPolygon->stMaskNormalized[polygonIndex].vertex_num;

		pBuff[nDataLen] = IMVS_ROI_TYPE_POLYGON;
		nDataLen += sizeof(byte);
		pBuff[nDataLen] = 0;
		nDataLen += sizeof(byte);

		memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, &nVertexNum, sizeof(int));
		nDataLen += sizeof(int);

		memcpy_s(pBuff + nDataLen, nBuffSize - nDataLen, stShieldedPolygon->stMaskNormalized[polygonIndex].vertex_points, nVertexNum*sizeof(HKA_POINT_F));
		nDataLen += nVertexNum*sizeof(HKA_POINT_F);
	}

	if (0 == nDataLen)
	{
		pBuff[nDataLen] = IMVS_ROI_TYPE_NULL;
		nDataLen += sizeof(byte);
		pBuff[nDataLen] = 0;
		nDataLen += sizeof(byte);
	}

	return nRet;
}

// 清理模块ROI数据
int CVmAlgModuleBase::ClearRoiData(OUT BASE_MODU_ROI_DATA* stBaseModuROIData)
{
	HKA_CHECK_ERROR_LOG(HKA_NULL == stBaseModuROIData, IMVS_EC_PARAM, "ClearRoiData");

	int nRet = IMVS_EC_OK;

	stBaseModuROIData->stRoiBox.clear();
	stBaseModuROIData->stRoiAnnulus.clear();
	stBaseModuROIData->stRoiPolygon.clear();
	memset(&(stBaseModuROIData->stLineCaliperBox), 0, sizeof(stBaseModuROIData->stLineCaliperBox));
	memset(&(stBaseModuROIData->stCircleCaliperBox), 0, sizeof(stBaseModuROIData->stCircleCaliperBox));
	
	stBaseModuROIData->iRoiTypeIndex = IMVS_ROI_TYPE_NULL;

	return nRet;
}

// 重置模块默认ROI
int CVmAlgModuleBase::ResetDefaultRoi(OUT BASE_MODU_ROI_DATA* stBaseModuROIData)
{
	HKA_CHECK_ERROR_LOG(HKA_NULL == stBaseModuROIData, IMVS_EC_PARAM, "ResetDefaultRoi");

	int nRet = IMVS_EC_OK;
	IMVS_ROI_BOX roiBox = { 0.0f };

	roiBox.fRoiCenterX = 0.5f;
	roiBox.fRoiCenterY = 0.5f;
	roiBox.fRoiHeight = 1.0f;
	roiBox.fRoiWidth = 1.0f;
	roiBox.fRoiAngle = 0.0f;

	nRet = ClearRoiData(stBaseModuROIData);
	HKA_CHECK_ERROR(IMVS_EC_OK != nRet, nRet);

	stBaseModuROIData->stRoiBox.push_back(roiBox);
	stBaseModuROIData->iRoiTypeIndex = IMVS_ROI_TYPE_IMAGE;
	
	return nRet;
}

// 设置算法参数
int CVmAlgModuleBase::SetParam(IN const char* szParamName, IN const char* pData, IN int nDataLen)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == szParamName || HKA_NULL == pData), IMVS_EC_PARAM, "SetParam");

	int nRet = IMVS_EC_OK;

	_snprintf_s(m_chszLog, LOG_STRING_SIZE, LOG_STRING_SIZE - 1, "CVmAlgModuleBase::SetParamValue,\t%s, \t%s\n", szParamName, pData);
	OutputDebugStringA(m_chszLog);

	// 内部信息
	if ((0 == strcmp("RoiType", szParamName)))
	{
		//绘制ROI情况下的ROI数据
		m_bRoiChanged = true;

		nRet = AnalyzeMultipleROIMsg(pData, nDataLen, &m_stBaseModuROIData);
		HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "AnalyzeMultipleROIMsg")

		if (!(m_stFixInit == m_stFixRun))
		{
			//更新绘制ROI时 同时调整屏蔽区
			int nTempLen = 0;
			char szPolygonMaskBuf[IMVS_COMMON_POLYGON_LEN_MAX * IMVS_ROI_POLYGOPN_COUNT_MAX] = { 0 };
			nRet = GetParam("ExternRoiType", szPolygonMaskBuf, sizeof(szPolygonMaskBuf), &nTempLen);
			HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "GetParam ExternRoiType")

			m_bShieldedChanged = true;

			nRet = AnalyzePlygonShieldMsg(szPolygonMaskBuf, nTempLen, &m_stShieldedPolygon);
			HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "AnalyzePlygonShieldMsg")
		}

		//更新绘制ROI时 同时调整基准点
		m_stFixInit = m_stFixRun;
	}
	else if (0 == strcmp("ExternRoiType", szParamName))
	{
		//屏蔽区数据
		m_bShieldedChanged = true;

		nRet = AnalyzePlygonShieldMsg(pData, nDataLen, &m_stShieldedPolygon);
		HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "AnalyzePlygonShieldMsg")

		if (!(m_stFixInit == m_stFixRun))
		{
			//更新屏蔽区时 同时调整绘制ROI
			int nTempLen = 0;
			char szMultipleROIBuf[IMVS_COMMON_POLYGON_LEN_MAX * IMVS_MULTIPLE_ROI_COUNT_MAX] = { 0 };
			nRet = GetParam("RoiType", szMultipleROIBuf, sizeof(szMultipleROIBuf), &nTempLen);
			HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "GetParam RoiType")

			m_bRoiChanged = true;

			nRet = AnalyzeMultipleROIMsg(szMultipleROIBuf, nTempLen, &m_stBaseModuROIData);
			HKA_CHECK_ERROR_LOG((IMVS_EC_OK != nRet), nRet, "AnalyzeMultipleROIMsg")
		}

		//更新屏蔽区时 同时调整基准点
		m_stFixInit = m_stFixRun;
	}
	else if (0 == strcmp("FixtureInfo", szParamName))
	{
		//绘制ROI情况下的位置修正数据
		memset(&m_stFixInit, 0, sizeof(m_stFixInit));
		memset(&m_stFixRun, 0, sizeof(m_stFixRun));

		sscanf_s(pData,
			"Init %f %f %f %f %f",
			&m_stFixInit.basis_point.x,
			&m_stFixInit.basis_point.y,
			&m_stFixInit.basis_angle,
			&m_stFixInit.basis_scale_x,
			&m_stFixInit.basis_scale_y);

		//m_stFixInit.basis_scale_x = (m_stFixInit.basis_scale_x == 0) ? 1 : m_stFixInit.basis_scale_x;
		//m_stFixInit.basis_scale_y = (m_stFixInit.basis_scale_y == 0) ? 1 : m_stFixInit.basis_scale_y;

		memcpy_s(&m_stFixRun, sizeof(m_stFixRun), &m_stFixInit, sizeof(m_stFixInit));
	}
	else
	{
		return IMVS_EC_MODULE_PARAM_NOT_SUPPORT;
	}

	return nRet;
}

// 获取所有参数
int CVmAlgModuleBase::GetAllParamList(OUT PARAM_VALUE_INFO_LIST* pParamValueList)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pParamValueList), IMVS_EC_PARAM, "GetAllParamList");

	// 出错时不必返回,防止部分参数异常时后续参数不再获取
	int nRet = VM_M_GetCurrentConfigByFile(m_hModule, UNICODEtoUTF8(VmModule_GetXmlPath()), pParamValueList);

	// 绘制ROI 屏蔽区等参数考虑保证复制粘贴前后一致 不通过该接口 而直接保存加载原始数据
	int nDataLen = 0;
	int nIndex = 0;

	// 单独添加FixtureInfo参数 xml无该参数
	nIndex = pParamValueList->nNum;
	HKA_MODU_CHECK_ERROR((nIndex >= MAX_PARAM_NUM), IMVS_EC_OK);
	memcpy_s(pParamValueList->paramValueList[nIndex].byParamName, MAX_KEY_NAME_LEN, "FixtureInfo", sizeof("FixtureInfo"));
	GetParam("FixtureInfo", pParamValueList->paramValueList[nIndex].byParamValue, sizeof(pParamValueList->paramValueList[nIndex].byParamValue), &nDataLen);
	pParamValueList->nNum++;

	return IMVS_EC_OK;
}

//设置所有参数
int CVmAlgModuleBase::SetAllParamList(IN const PARAM_VALUE_INFO_LIST* pParamValueList)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pParamValueList), IMVS_EC_PARAM, "SetAllParamList");

	for (size_t index = 0; index < pParamValueList->nNum && index < MAX_PARAM_NUM; index++)
	{
		const PARAM_VALUE_INFO& struParamValue = pParamValueList->paramValueList[index];
		SetParam(struParamValue.byParamName, struParamValue.byParamValue, sizeof(struParamValue.byParamValue));
	}

	return IMVS_EC_OK;
}

//保存模块数据
int CVmAlgModuleBase::SaveModuleData(OUT SAVE_MODULE_DATA_PARAM* pModuleData)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pModuleData), IMVS_EC_PARAM, "SaveModuleData");

	int nRet = IMVS_EC_OK;

	//默认调用GetAllParamList 保存xml声明的参数
	CAbstractUserModule::SaveModuleData(pModuleData);
	
	// 绘制ROI 屏蔽区等参数考虑保证复制粘贴前后一致 直接保存加载原始数据 不通过参数读写接口引入位置修正
	pModuleData->struModuleData.struCustomUserData.eUserDataType = USERDATA_BINARY;
	USER_BINARY_DATA_LIST* pDataList = &pModuleData->struModuleData.struCustomUserData.uUserData.struBinaryDataList;

	//绘制ROI数据	
	GenerateMultipleROIMsg(&m_stBaseModuROIData, m_szMultipleROIBuf, sizeof(m_szMultipleROIBuf), m_nMultipleROILen);
	memcpy_s(pDataList->struBinaryData[pDataList->nNum].byName, MAX_KEY_NAME_LEN, "RoiType", sizeof("RoiType"));
	pDataList->struBinaryData[pDataList->nNum].nDataLen = m_nMultipleROILen;
	pDataList->struBinaryData[pDataList->nNum].pUserData = m_szMultipleROIBuf;
	pDataList->nNum++;

	//屏蔽区数据
	GeneratePlygonShieldMsg(&m_stShieldedPolygon, m_szPolygonMaskBuf, sizeof(m_szPolygonMaskBuf), m_nPolygonMaskLen);
	memcpy_s(pDataList->struBinaryData[pDataList->nNum].byName, MAX_KEY_NAME_LEN, "ExternRoiType", sizeof("ExternRoiType"));
	pDataList->struBinaryData[pDataList->nNum].nDataLen = m_nPolygonMaskLen;
	pDataList->struBinaryData[pDataList->nNum].pUserData = m_szPolygonMaskBuf;
	pDataList->nNum++;

	return IMVS_EC_OK;
}

//加载模块数据
int CVmAlgModuleBase::LoadModuleData(IN const LOAD_MODULE_DATA_PARAM* pModuleData)
{
	HKA_CHECK_ERROR_LOG((HKA_NULL == pModuleData), IMVS_EC_PARAM, "LoadModuleData");

	SetAllParamList(&pModuleData->struModuleData.struParamValueList);

	if (pModuleData->struModuleData.struCustomUserData.eUserDataType == USERDATA_BINARY)
	{
		const USER_BINARY_DATA_LIST* pDataList = &pModuleData->struModuleData.struCustomUserData.uUserData.struBinaryDataList;

		for (int nIndex = 0; nIndex < pDataList->nNum; nIndex++)
		{
			//绘制ROI数据
			if (0 == strcmp(pDataList->struBinaryData[nIndex].byName, "RoiType"))
			{
				SetParam("RoiType", pDataList->struBinaryData[nIndex].pUserData, pDataList->struBinaryData[nIndex].nDataLen);
			}
			//屏蔽区数据
			else if (0 == strcmp(pDataList->struBinaryData[nIndex].byName, "ExternRoiType"))
			{
				SetParam("ExternRoiType", pDataList->struBinaryData[nIndex].pUserData, pDataList->struBinaryData[nIndex].nDataLen);
			}
		}
	}

	return IMVS_EC_OK;
}
