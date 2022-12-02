/***************************************************************************************************
* File:  MVD_AlgoParamDefine.h
* Note:  Algorithm-dependent common data struct definition in namespace VisionDesigner.
*
* Version:  V3.2.0.3
****************************************************************************************************/

#ifndef _MVD_ALGO_PARAM_DEFINE_H__
#define _MVD_ALGO_PARAM_DEFINE_H__

#include "MVD_ShapeParamDefine.h"

namespace VisionDesigner
{
    /************************************************************************/
    /*                               �궨��                                */
    /************************************************************************/
    /** @struct _MVD_CALIBBOARD_TYPE__
     *  @brief  Calibboard type - enum type
     */
    enum _MVD_CALIBBOARD_TYPE__
    {
        MVD_CALIBBOARD_CHECKER = 1, // Checker Board
        MVD_CALIBBOARD_CIRCLE = 2, // Circle Board
        MVD_CALIBBOARD_HKA_SPARSE_I = 7,   // ����I�ͱ궨��
        MVD_CALIBBOARD_HKA_DENSE_II = 6,   // ����II�ͱ궨��
        MVD_CALIBBOARD_RSCODE_SIGN_FOUR = 3, // ����I�ͱ궨��(V3.3�������ΪMVD_CALIBBOARD_TYPE_HKA_SPARSE_I)
        MVD_CALIBBOARD_RSCODE_SIGN = 4 // ����II�ͱ궨��(V3.3�������ΪMVD_CALIBBOARD_TYPE_HKA_DENSE_II)
    };

    /** @struct _MVD_CALIBBOARD_IMAGE_TYPE__
     *  @brief  Calibboard image type - enum type
     */
    enum _MVD_CALIBBOARD_IMAGE_TYPE__
    {
        MVD_CALIBBOARD_OFFSET_IMAGE = 1, // ƽ��ͼ��
        MVD_CALIBBOARD_ROTATE_IMAGE = 2 // ��תͼ��
    };

    /** @struct _MVD_CALIBBOARD_COORDINATE_MODE__
     *  @brief  Calibboard coordinate mode - enum type
     */
    enum _MVD_CALIBBOARD_COORDINATE_MODE__
    {
        MVD_COORD_MODE_LEFT_HANDED = 1, // Left-handed coordinate system
        MVD_COORD_MODE_RIGHT_HANDED = 2 // Right-handed coordinate system
    };

    /** @enum _MVD_CAMERA_MODE__
     *  @brief  camera mode - enum type
     */
    enum _MVD_CAMERA_MODE__
    {
        MVD_CAMERA_MODE_STATIC_UP = 1,
        MVD_CAMERA_MODE_STATIC_DOWN = 2,
        MVD_CAMERA_MODE_MOVE = 3
    };

    /** @enum _MVD_MAP_CALIB_MODE__
    *  @brief  map calib mode - enum type
    */
    enum _MVD_MAP_CALIB_MODE__
    {
        MVD_MAP_CALIB_MODE_TWO_IMAGE = 1,
        MVD_MAP_CALIB_MODE_TWO_FILE = 2,
    };
    /** @enum _MVD_CALIB_MOVE_TYPE__
    *  @brief  map calib mode - enum type
    */
    enum _MVD_CALIB_MOVE_TYPE__
    {
        MVD_MAP_CALIB_MOVE_TYPE_OFFSET = 1,
        MVD_MAP_CALIB_MOVE_TYPE_ROTATE = 2,
    };
    /************************************************************************/
    /*                               ƴ����                                */
    /************************************************************************/
    /** @struct _MVD_STITCH_CHECKERBOARD_SUBPIXEL_WIN_AUTO_TYPE_
    *  @brief  subpixel window auto type - enum type
    */
    enum _MVD_STITCH_CHECKERBOARD_SUBPIXEL_WIN_AUTO_TYPE__
    {
        MVD_STITCH_CHECKERBOARD_SUBP_WIN_AUTO = 1, // ����Ӧ
        MVD_STITCH_CHECKERBOARD_SUBP_WIN_MANUAL = 2 // ����ֵ
    };

    /** @struct _MVD_STITCH_PATTERN_CREATE_PARAM__
    *  @brief  Pattern create param - struct type
    */
    struct _MVD_STITCH_PATTERN_CREATE_PARAM__
    {
        bool bFilterFlag; // ��ֵ�˲���ǣ�Ĭ��ֵtrue
        _MVD_STITCH_CHECKERBOARD_SUBPIXEL_WIN_AUTO_TYPE__ enSubPixelWinMode; // �����ش���ģʽ,Ĭ��ֵ1
        int nSubPixelWinSize; // �����ش��ڴ�С[3,100], Ĭ��ֵ7
        int nGrayContrast; // �ҶȶԱȶ�[1,255], Ĭ��ֵ15
        int nReserved[8]; // �����ֽ�
    };

    /************************************************************************/
    /*                               ��λ��                                */
    /************************************************************************/
    /** @enum  _MVD_EDGEPOINT_STATUS__
     *  @brief Find status of the edge point
     */
    enum _MVD_EDGEPOINT_STATUS__
    {
        MVD_EDGEPOINT_STATUS_USED = 0x0001,    // Successfully found and used, default value
        MVD_EDGEPOINT_STATUS_NO_USED = 0x0002,    // Successfully found but not used
        MVD_EDGEPOINT_STATUS_NO_FIND = 0x0003     // Fail to find
    };

    /** @enum  _MVD_EDGE_POLARITY__
     *  @brief Type of the polarity of the edge point
     */
    enum _MVD_EDGE_POLARITY__
    {
        MVD_EDGE_POLARITY_DARK_TO_BRIGHT = 0x0001,    // From black to white, default value
        MVD_EDGE_POLARITY_BRIGHT_TO_DARK = 0x0002,    // From white to black
        MVD_EDGE_POLARITY_BOTH = 0x0003     // both
    };

    /** @enum  _MVD_EDGE_POLARITY_EX__
    *  @brief Type of the polarity of the edge point
    */
    enum _MVD_EDGE_POLARITY_EX__
    {
        MVD_EDGE_POLARITY_DARK_TO_BRIGHT_EX = 0x0001,    // �ɺڵ���
        MVD_EDGE_POLARITY_BRIGHT_TO_DARK_EX = 0x0002,    // �ɰ׵���
        MVD_EDGE_POLARITY_MIXED_EX = 0x0003,     // �ɺڵ��� & �ɰ׵���(ͬһ���߶��е㼯�����ּ��Ե�)
        MVD_EDGE_POLARITY_EITHER_EX = 0x0004     // �ɺڵ��� | �ɰ׵���(ͬһ���߶��е㼯��һ�ּ��Ե�)
    };

    /** @enum  _MVD_POSITION__
     *  @brief ��λ
     */
    enum _MVD_POSITION__
    {
        MVD_POSITION_TOP = 0x0001,    // Top
        MVD_POSITION_RIGHT = 0x0002,    // Right
        MVD_POSITION_BOTTOM = 0x0003,    // Bottom
        MVD_POSITION_LEFT = 0x0004,    // Left
    };

    /** @enum  MVD_CALIPERTOOL_EDGEMODE
     *  @brief Type of the edge mode for the caliper
     */
    typedef enum _MVD_CALIPERTOOL_EDGEMODE__
    {
        MVD_CALIPERTOOL_EDGEMODE_SINGLE_EDGE = 0x0001,    // Caliper Tool Work mode : SingleEdge, default value
        MVD_CALIPERTOOL_EDGEMODE_EDGE_PAIR = 0x0002,    // Caliper Tool Work mode : EdgePair
    };

    /** @struct _MVD_FIDUCIAL_POINT_F__
    *  @brief  fiducial pose point Info - float type
    */
    struct _MVD_FIDUCIAL_POINT_F__
    {
        MVD_POINT_F stPosition;
        float fAngle;
        float fScaleX;
        float fScaleY;

        int nReserved[6];
    };

    /** @struct _MVD_POSFIX_MODE__
     *  @brief  Position Fix Mode - enum type
     */
    enum _MVD_POSFIX_MODE__
    {
        MVD_POSFIX_MODE_NONE = 0x0001,  // Do not make position fixture
        MVD_POSFIX_MODE_HORI = 0x0002,  // Position fixture horizontally
        MVD_POSFIX_MODE_VERT = 0x0003,  // Position fixture vertically
        MVD_POSFIX_MODE_HV = 0x0004,  // Position fixture both horizontally and vertically
        MVD_POSFIX_MODE_HVA = 0x0005,  // Angle fixture both horizontally and vertically
        MVD_POSFIX_MODE_HVAS = 0x0006,  // Angle fixture both horizontally and vertically, support scale
    };

    /** @struct _MVD_POSFIX_MODE__
    *  @brief  Position Fix Mode - enum type
    */
    enum _MVD_MARKFIND_ROI_TYPE__
    {
        MVD_MARK_TYPE_CROSS = 0x0000,  // ����ʮ��
        MVD_MARK_TYPE_RECT = 0x0001,  // ���ľ���
    };

    /** @struct _MVD_MARK_WEIGHT_TYPE__
    *  @brief  Weight Mode - enum type
    */
    enum _MVD_MARKFIND_WEIGHT_TYPE__
    {
        MVD_MARK_WEIGHT_TYPE_IMPORTANT = 0x0000,  // ��Ҫ
        MVD_MARK_WEIGHT_TYPE_COMMON = 0x0001,  // һ��
        MVD_MARK_WEIGHT_TYPE_WEAK = 0x0002,  // ��
    };

    /** @struct _MVD_MARK_PAT_TYPE__
    *  @brief  Pat type - enum type
    */
    enum _MVD_MARKFIND_PAT_TYPE__
    {
        MVD_MARK_PAT_TYPE_FAST = 0x0000,  // ��������ƥ��
        MVD_MARK_PAT_TYPE_HP = 0x0001,    // �߾�������ƥ��
    };

    /** @struct _MVD_BOXROIFIND_PROC_TYPE__
    *  @brief  Process Type - enum type
    */
    enum _MVD_BOXROIFIND_PROC_TYPE__
    {
        MVD_BOXROI_PROC_TYPE_LINE = 0x0000,           // ���ֱ��
        MVD_BOXROI_PROC_TYPE_DISTANCE = 0x0001,       // �������ֱ�ߵľ���
    };


    /************************************************************************/
    /*                               ������                                */
    /************************************************************************/
    /** @enum  _MVD_MEASURE_C2C_TYPES__
     *  @brief Positional relationship between circle and circle
     */
    enum _MVD_MEASURE_C2C_TYPES__
    {
        MVD_MEASURE_C2C_UNDEFINED = 0x00000000,     // Undefined
        MVD_MEASURE_C2C_CONTAIN = 0x00000001,     // Contain
        MVD_MEASURE_C2C_INSCRIBE = 0x00000002,     // Inscribe
        MVD_MEASURE_C2C_INTERSECT = 0x00000003,     // Intersect
        MVD_MEASURE_C2C_EXCIRCLE = 0x00000004,     // External cutting
        MVD_MEASURE_C2C_DEVIATION = 0x00000005,     // Deviation
    };


    /************************************************************************/
    /*                             Ԥ������                             */
    /************************************************************************/
    /** @enum _MVD_IMGFIX_MODE__
     *  @brief  Image Fix Mode
     */
    enum _MVD_IMGFIX_MODE__
    {
        MVD_IMGFIX_MODE_NONE = 0x0001,  // Do not make fixture
        MVD_IMGFIX_MODE_HORI = 0x0002,  // Fixture horizontally
        MVD_IMGFIX_MODE_VERT = 0x0003,  // Fixture vertically
        MVD_IMGFIX_MODE_HV = 0x0004,  // Fixture both horizontally and vertically
        MVD_IMGFIX_MODE_HVA = 0x0005,  // Angle fixture both horizontally and vertically
        MVD_IMGFIX_MODE_HVAS = 0x0006,  // Angle fixture both horizontally and vertically, support scale
    };

    /** @struct _MVD_PREPRO_FIDUCIAL_POINT_F__
     *  @brief  fiducial pose point Info - float type
     */
    struct _MVD_PREPRO_FIDUCIAL_POINT_F__
    {
        MVD_POINT_F stBasisPoint;
        float fBasisAngle;
        float fScaleX;
        float fScaleY;

        int nReserved[6];
    };


    /** @struct _MVD_PREPRO_FRAME_MEAN_PROC_MODE
    *  @brief  Steps to create a frame average model 
    */
    enum _MVD_PREPRO_FRAME_MEAN_PROC_MODE
    {
        MVD_FRAME_MEAN_TRAIN_STEP = 1,   // ����
        MVD_FRAME_MEAN_CALCU_STEP = 2,   // ����
        MVD_FRAME_MEAN_ABORT_STEP = 3    // ����
    };

    /************************************************************************/
    /*                              ʶ����                             */
    /************************************************************************/
    /** @enum _MVD_1DCODE_TYPE__
     *  @brief  1D Code Type - enum type
     */
    enum _MVD_1DCODE_TYPE__
    {
        MVD_1DCODE_NONE = 0, // unsupport
        MVD_1DCODE_EAN8 = 1, // EAN8 Code
        MVD_1DCODE_UPCE = 2, // UPCE Code
        MVD_1DCODE_UPCA = 3, // UPCA Code
        MVD_1DCODE_EAN13 = 4, // EAN13 Code
        MVD_1DCODE_ISBN13 = 5, // ISBN13 Code
        MVD_1DCODE_CODABAR = 6, // codabar Code
        MVD_1DCODE_ITF25 = 7, // itf 25 Code
        MVD_1DCODE_39 = 8, // Code 39
        MVD_1DCODE_93 = 9, // Code 93
        MVD_1DCODE_128 = 10, // Code 128
    };

    /** @enum _MVD_TDCODE_TYPE__
     *  @brief  2DCode Type - enum type
     */
    enum _MVD_TDCODE_TYPE__
    {
        MVD_2DCODE_NONE = 0, // �޿�ʶ������
        MVD_2DCODE_DM = 1, // DM��
        MVD_2DCODE_QR = 2, // QR��
    };

    /** @enum _MVD_SYMBOL_VERIFY_STANDARD__
    *  @brief  Grade Standard
    */
    enum _MVD_SYMBOL_VERIFY_STANDARD__
    {
        MVD_SYMBOL_VERIFY_ISO_STANDARD_15415 = 1,         // iso 15415��׼
        MVD_SYMBOL_VERIFY_ISO_STANDARD_29158 = 2,          // iso 29158��׼
        MVD_SYMBOL_VERIFY_ISO_STANDARD_15416 = 3,         // iso 15416��׼
    };

    /** @enum _MVD_SYMBOL_VERIFY_LABEL__
    *  @brief  Verify label
    */
    enum _MVD_SYMBOL_VERIFY_LABEL__
    {
        MVD_SYMBOL_VERIFY_LABEL_STANDARD = 1,         // ��������ȫ����iso��׼
        MVD_SYMBOL_VERIFY_LABEL_CUSTOM = 2,         // ����ָ����ö���
    };

    enum _MVD_SYMBOL_VERIFY_PROCESS_TYPE__
    {
        MVD_SYMBOL_VERIFY_PROCESS_TYPE_I = 1,   // ֻ֧��DM��,Ч������
        MVD_SYMBOL_VERIFY_PROCESS_TYPE_II = 2,   // ֧��DM���QR��
    };

    /** @enum  _MVDOCR_FILTER_TYPE__
     *  @brief �ַ���������
     */
    typedef enum _MVDOCR_FILTER_TYPE__
    {
        MVDOCR_FILTER_NO = 0,    // ������
        MVDOCR_FILTER_NUMBERONLY = 1,    // ֻ��������
        MVDOCR_FILTER_UPPERLOWERCASEBOTH = 2,    // ֻ������ĸ
        MVDOCR_FILTER_UPPERCASEONLY = 3,    // ֻ�����д��ĸ
        MVDOCR_FILTER_LOWERCASEONLY = 4,    // ֻ����Сд��ĸ
        MVDOCR_FILTER_SPECIALONLY = 5,    // ֻ���������ַ�
        MVDOCR_FILTER_SPACE = 6,    // ֻ����ո�,CNNOCRֻҪ����������Ϊ�ո�ʶ����ǿ��תΪ�ո񣬴�ͳOCR���ʶ������Ϊ�ո��򷵻��ʺ�
        MVDOCR_FILTER_CUSTOM = 7     // �Զ�������ַ�
    };

    /** @enum  _MVDOCR_FILTER__
     *  @brief �ַ�������
     */
    struct _MVDOCR_FILTER__
    {
        _MVDOCR_FILTER_TYPE__ enFilterType;                // �ַ���������
        char               pFilterValue[128];           // ����������йأ�Ĭ��Ϊ�գ�e.g.�Զ���ģʽ����ָ�������ַ���
        int                nReserved[7];                // �����ֽ�
    };

    /** @enum  _MVDGEOFIND_CALIPER_ROI_MODE__
    *  @brief Բ����/ֱ�߲��������еĿ�����������ģʽ
    */
    typedef enum _MVDGEOFIND_RUNNING_MODE__
    {
        MVDGEOFIND_RUNNING_MODE_CALCULATE_AND_FIND = 0x0000, //����ROI��ز�ִ�в��� 0
        MVDGEOFIND_RUNNING_MODE_ONLY_CALCULATE     = 0x0001, //������ROI������ 1
        MVDGEOFIND_RUNNING_MODE_ONLY_FIND          = 0x0002  //��ִ�в���(�����ROI�����֪)2

    };

    /************************************************************************/
    /*                            ȱ�ݼ����                            */
    /************************************************************************/
    /** @enum  _MVD_EDGEINSP_EDGE_FLAW_TYPE__
     *  @brief Type of the flaw of the edge inspection
     */
    enum _MVD_EDGEINSP_SINGLE_EDGE_FLAW_TYPE__
    {
        MVD_EDGEINSP_SINGLE_FLAW_TYPE_EDGE_OFFSET = 0x0006, // ��Եλ��ƫ��
        MVD_EDGEINSP_SINGLE_FLAW_TYPE_EDGE_ROUGH = 0x0007, // ��Ե͹��򰼿�ȱ��
        MVD_EDGEINSP_SINGLE_FLAW_TYPE_EDGE_CRACK = 0x0008, // ��Ե����ȱ��
        MVD_EDGEINSP_SINGLE_FLAW_TYPE_EDGE_GRAD = 0x0009, // ��Ե���ݲ�ȱ��
    };

    /** @enum  _MVD_EDGEINSP_EDGE_FLAW_TYPE__
     *  @brief Type of the flaw of the edge inspection
     */
    enum _MVD_EDGEINSP_EDGE_PAIR_FLAW_TYPE__
    {
        MVD_EDGEINSP_FLAW_TYPE_EDGE_WIDTH = 0x0001, // ��Ե�Կ��ȱ��
        MVD_EDGEINSP_FLAW_TYPE_EDGE_OFFSET = 0x0002, // ��Ե��λ��ƫ��
        MVD_EDGEINSP_FLAW_TYPE_EDGE_ROUGH = 0x0003, // ��Ե�Խ���ȱ��
        MVD_EDGEINSP_FLAW_TYPE_EDGE_CRACK = 0x0004, // ��Ե�Զ���ȱ��
        MVD_EDGEINSP_FLAW_TYPE_EDGE_BUBBLE = 0x000A, // ��Ե������ȱ��
    };

     /** @enum  _MVD_EDGEINSP_FLAW_DEFECT_TYPE__
     *  @brief Type of the flaw
     */
    enum _MVD_EDGEINSP_FLAW_DEFECT_TYPE__
    {
        MVD_EDGEINSP_FLAW_DEFECT_TYPE_NONE = 0, // ��ȱ��
        MVD_EDGEINSP_FLAW_DEFECT_TYPE_FLAW = 1, // ȱ��
        MVD_EDGEINSP_FLAW_DEFECT_TYPE_GAP  = 2, // ����
    };

    /** @enum  _MVD_MKINSP_METHOD_TYPE__
     *  @brief Character inspection method
     */
    enum _MVD_MKINSP_METHOD_TYPE__
    {
        MVD_MKINSP_COAR_CORRE = 1, // ��ط�
        MVD_MKINSP_FINE_MEAN_STD = 2, // ��ֵ��׼��
    };

    /** @enum  _MVD_BEAD_GUIDE_RUNNING_MODE__
     *  @brief bead guide running mode
     */
    enum _MVD_BEAD_GUIDE_RUNNING_MODE__
    {
        MVD_BEAD_GUIDE_FIX_POINT = 0, // ����·�̶���
        MVD_BEAD_GUIDE_FIND_EDGE = 1, // �̶���·���ұ�Ե
    };

    /** @enum  _MVD_EDGEINSP_PATTERN_TRAIN_MODE__
    *  @brief pattern train mode
    */
    enum _MVD_EDGEINSP_PATTERN_TRAIN_MODE__
    {
        MVD_EDGEINSP_PATTERN_TRAIN_I = 1, // ģ�ʹ���ʧ��ʱ���������ʱ��������Ե�㡢���ߵ�
        MVD_EDGEINSP_PATTERN_TRAIN_II = 2, // ģ�ʹ���ʧ��ʱ�������ʱ��������Ե�㡢���ߵ�
    };

    /************************************************************************/
    /*                            ��ɫ������                            */
    /************************************************************************/
    /** @enum  _MVD_COLOR_SPACE_TYPE__
    *  @brief Color space type
    */
    enum _MVD_COLOR_SPACE_TYPE__
    {
        MVD_COLOR_SPACE_RGB = 1,   // RGB��ɫ�ռ�
        MVD_COLOR_SPACE_HSV = 2,   // HSV��ɫ�ռ�
        MVD_COLOR_SPACE_HSI = 3,   // HSI��ɫ�ռ�
        MVD_COLOR_SPACE_YUV = 4    // YUV��ɫ�ռ�(��ɫ��ȡ��֧��)
    };

    /** @enum  _MVD_COLOR_EXTRACT_SINGLE_RANGE__
    *  @brief Color extract range param
    */
    struct _MVD_COLOR_EXTRACT_SINGLE_RANGE__
    {
        int nC1LowTolerance;        // ��һͨ��(R or H)�ĵ�����ֵ
        int nC1HighTolerance;       // ��һͨ��(R or H)�ĸ�����ֵ
        int nC2LowTolerance;        // �ڶ�ͨ��(G or S)�ĵ�����ֵ
        int nC2HighTolerance;       // �ڶ�ͨ��(G or S)�ĸ�����ֵ
        int nC3LowTolerance;        // ����ͨ��(B or V)�ĵ�����ֵ
        int nC3HighTolerance;       // ����ͨ��(B or V)�ĸ�����ֵ
        bool bReverseEnable;        // ��תʹ��   true��ʾ��ת��false��ʾ����ת
        _MVD_COLOR_SPACE_TYPE__ enColorSpace;            // ��ɫ�ռ�  //ÿ����Χ���ɵ���������ɫ�ռ�
        int nReserved[8]; // �����ֽ�
    };

	/************************************************************************/
	/*                            �汾��Ϣ��ȡ��                            */
	/************************************************************************/
	/** @enum  _MVD_SDK_TYPE__
	*  @brief sdk type
	*/
    enum _MVD_SDK_TYPE__
    {
        MVD_SDK_TYPE_MIN = 0,        ///SDK������Сֵ

        MVD_All2DVersion =0,         /// �������

        MVD_CameraMap = 1,           /// ���ӳ��
        MVD_ImageFixture = 2,        /// ͼ������
        MVD_LineAlign = 3,           /// �߶�λ
        MVD_PointSetAlign = 4,       /// �㼯��λ
        MVD_PositionFix = 5,         /// λ������
        MVD_SinglePointAlign = 6,    /// �����λ

        MVD_CalibBoardCalib = 10,    /// �궨��궨
        MVD_CalibTrans = 11,         /// �궨ת��
        MVD_ImageCalib = 12,         /// ����궨
        MVD_ImageCorrectCalib = 13,  /// �������
        MVD_MapCalib = 14,           /// ӳ��궨
        MVD_MatrixFix = 15,          /// ��������
        MVD_MatrixToParam = 16,      /// ����ת����
        MVD_NImageCalib = 17,        /// Nͼ�궨
        MVD_NPointCalib = 18,        /// N��궨

        MVD_ColorConvert = 30,       /// ��ɫת��
        MVD_ColorExtract = 31,       /// ��ɫ��ȡ
        MVD_ColorMeasure = 32,       /// ��ɫ����
        MVD_ColorRecognition = 33,   /// ��ɫʶ��

        MVD_ArcFlawInspCpp = 40,       /// Բ��ȱ�ݼ��
        MVD_ArcPairFlawInspCpp = 41,   /// Բ����ȱ�ݼ��
        MVD_BeadGuideCpp = 42,         /// ·����ȡ
        MVD_EdgeFlawInsp = 43,         /// ��Եȱ�ݼ��
        MVD_EdgePairFlawInsp = 44,     /// ��Ե��ȱ�ݼ��
        MVD_LineEdgeFlawInsp = 45,     /// ֱ�߱�Ե���
        MVD_LineEdgePairFlawInsp = 46, /// ֱ�߶Ա�Ե���

        MVD_BlobFind = 50,              /// Blob����
        MVD_CaliperTool = 51,           /// ���߹���
        MVD_CircleFind = 52,            /// Բ����
        MVD_CrossPointFind = 53,        /// ��Ե������
        MVD_EdgeFind = 54,              /// ��Ե���
        MVD_EdgeWidth = 55,             /// �����
        MVD_LineFind = 56,              /// �߲���
        MVD_MultiCategoryBlobFind = 57, /// Blob��ǩ����
        MVD_MultiLineFind = 58,         /// ��ֱ�߲���
        MVD_PairLineFind = 59,          /// ƽ���߲���
        MVD_PeakFind = 60,              /// ������
        MVD_QuadrangleFindCpp = 61,     /// �ı��β���
        MVD_RectangleFind = 62,         /// ���μ��
        MVD_BoxROIFind = 63,            /// ����ROI��ȡ
        MVD_AnnROIFind = 64,            /// Բ��ROI��ȡ
        MVD_NearestPointFind = 65,      /// ����������ȡ
        MVD_RegionProjection = 66,      /// ����ͶӰ

        MVD_C2CMeasure = 70,            /// ԲԲ����
        MVD_CircleFit = 71,             /// Բ���
        MVD_HistTool = 72,              /// ֱ��ͼ����
        MVD_IntensityMeasure = 73,      /// ���Ȳ���
        MVD_L2CMeasure = 74,            /// ��Բ����
        MVD_L2LMeasure = 75,            /// ���߲���
        MVD_LineFit = 76,               /// �����
        MVD_P2CMeasure = 77,            /// ��Բ����
        MVD_P2LMeasure = 78,            /// ���߲���
        MVD_P2PMeasure = 79,            /// ������
        MVD_PixelCount = 80,            /// ����ͳ��

        MVD_MKInsp = 90,                /// �ַ�ȱ�ݼ��

        MVD_AlmightyPatMatch = 100,     /// ȫ������ƥ��
        MVD_FastFeaturePatMatch = 101,  /// ��������ƥ��
        MVD_GrayPatMatchCpp = 102,      /// �Ҷ�����ƥ��
        MVD_HPFeaturePatMatch = 103,    /// �߾�������ƥ��

        MVD_AffineTransPrepro = 110,		/// ����任
        MVD_ArithmeticPrepro = 111,			/// ͼ������
        MVD_BinaryPrepro = 112,				/// ͼ���ֵ��
        MVD_EnhancePrepro = 113,			/// ͼ����ǿ
        MVD_FilterPrepro = 114,				/// ͼ�����
        MVD_FrameMeanPrepro = 115,			/// ֡ƽ��
        MVD_MorphPrepro = 116,				/// ��̬ѧ����
        MVD_NormalizePrepro = 117,			/// ͼ���һ��
        MVD_PolarWarpPrepro = 118,			/// Բ��չ��
        MVD_RegionCopyPrepro = 119,			/// �������
        MVD_ShadeCorrectPrepro = 120,		/// ��ӰУ��
        MVD_SharpnessPrepro = 121,			/// ����������
		MVD_WarpDistortPrepro = 122,		/// ͼ�����
		MVD_BoxOverlapPrepro = 123,			/// Box�ص���
		MVD_BoxMergePrepro = 124,			/// Box�ں�
		MVD_ImageResizePrepro = 125,		/// ͼ������
		MVD_MultiLabelFilterPrepro = 126,	/// ���ǩɸѡ
		MVD_BoxFilterPrepro = 127,			/// BOX����

        MVD_Code1DReader = 130,				/// һά��ʶ��
        MVD_Code2DReader = 131,				/// ��ά��ʶ��
        MVD_CodeVerify = 132,				/// ��������Ƕ����ʶ���У�
        MVD_OCR = 133,						/// �ַ�ʶ��

        MVD_ImageStitch = 140,				/// ͼ��ƴ��

        MVD_SDK_TYPE_MAX = 140				/// SDK�������ֵ

    };


    /** @enum  _MVD_DL_SDK_TYPE__
     *  @brief Deep learning sdk type
    */
    enum _MVD_DL_SDK_TYPE__
    {
        MVD_DL_SDK_TYPE_MIN     = 0,           /// ���SDK������Сֵ

        MVD_DL_AllVersion       = 0,           /// �������

        MVD_DL_CharDetect       = 1,           /// �ַ���λ
        MVD_DL_CharRecog        = 2,           /// �ַ�ʶ��
        MVD_DL_SingleCharDetect = 3,           /// ���ַ����

        MVD_DL_Classify        = 10,           /// ͼ�����

        MVD_DL_CodeReader      = 20,           /// ��ʶ��
        MVD_DL_CodeVerify      = 21,           /// ������ Ƕ����ʶ����

        MVD_DL_Detect          = 30,           /// Ŀ����

		MVD_DL_Inspect         = 40,           /// �쳣���
		MVD_DL_UnSupervisedSegment = 41,       /// �޼ල

        MVD_DL_InstanceSegment = 50,           /// ʵ���ָ�

        MVD_DL_OCR             = 60,           /// ���OCR

        MVD_DL_Retrieval       = 70,           /// ͼ�����

        MVD_DL_Segment         = 80,           /// ͼ��ָ�
        MVD_DL_SegmentV2       = 81,           /// ͼ��ָ�v2

        MVD_DL_SDK_TYPE_MAX    = 81,           /// ���SDK�������ֵ
    };
    /************************************************************************/
    /*                            ���������                            */
    /************************************************************************/
    /** @enum  _MVD_CNN_MODEL_TYPE__
    *  @brief cnn model type
    */
    enum _MVD_CNN_MODEL_TYPE__
    {
        MVD_CNN_MODEL_TYPE_CLASSIFY                = 0x0001, // ����ʶ��
        MVD_CNN_MODEL_TYPE_DETECT                  = 0x0002, // Ŀ����
        //MVD_CNN_MODEL_TYPE_SEMANTIC_SEGMENTATION = 0x0003, // ����ָ�ݲ�֧�֣�
        MVD_CNN_MODEL_TYPE_CHAR_DETECT             = 0x0004, // �ı���λ/���ַ����
        MVD_CNN_MODEL_TYPE_CHAR_RECOG              = 0x0005, // �ַ�ʶ��
        MVD_CNN_MODEL_TYPE_INSPECT                 = 0x0006, // �쳣���
        //MVD_CNN_MODEL_TYPE_IMAGE_DENOISE         = 0x0007, // ͼ���루�ݲ�֧�֣�
        MVD_CNN_MODEL_TYPE_RETRIEVAL               = 0x0008, // ͼ�����
        MVD_CNN_MODEL_TYPE_INSTANCE_SEGMENTATION   = 0x0009, // ʵ���ָ�
        //MVD_CNN_MODEL_TYPE_STEREO_MATCHING       = 0x000A, // ˫Ŀƥ�䣨�ݲ�֧�֣�
        //MVD_CNN_MODEL_TYPE_OCR                   = 0x000B, // OCR���ݲ�֧�֣�
        //MVD_CNN_MODEL_TYPE_SEGMENT               = 0x000C, // ͼ��ָ�ݲ�֧�֣�
        MVD_CNN_MODEL_TYPE_UASI					   = 0x000D, // �޼ල
    };

	/** @enum  _MVD_CNN_UASI_MODEL_TYPE__
	*  @brief cnn uasi model type
	*/
	enum _MVD_CNN_UASI_MODEL_TYPE__
	{
		MVD_CNN_MODEL_TYPE_UASI_FAST = 0x0001,		// 
		MVD_CNN_MODEL_TYPE_UASI_HIGH_PRECISION = 0x0002,		// 
	};
}

#endif // _MVD_ALGO_PARAM_DEFINE_H__
