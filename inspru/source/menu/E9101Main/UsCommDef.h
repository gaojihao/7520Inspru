
// UsCommDef.h

#if !defined(_USCOMDEF_H_2005_12_15_)   
#define   _USCOMDEF_H_2005_12_15_


#define TIMEOUT_LOCATION_MSG	3000		//��Ϣ��ʱ

const double	Knot2Meter		    = 1.851985;			// ��->����/Сʱ
const long		SAT_NUMBER	   	    = 20;				// ����������
const long		_SAT_ID_NUMBER	    = 12;				// ��������ID��Ŀ
const double	DOP_MULTIPLE		= 10.0;				//���ȱ���

#define BUFFER_INPUT_COM	1024	//�������뻺����
#define BUFFER_OUTPUT_COM	1024	//�������������
#define RECIEVE_BUFFER_WORD	211		//�����ַ���������

//////////////////////////////////////////////////////////////////////////
/*������������
*/
enum enDataType
{
	LOCATION_GPSDATA,	//ָGPS����
	LOCATION_DOPDATA,	//ָDOP����
	LOCATION_SATDATA	//ָSAT����
};

//////////////////////////////////////////////////////////////////////////
/*��ʵ��λ����ģ�ⶨλ
*/
enum enLocation
{
	LOCATION_REAL,	//��ʵ��λ
	LOCATION_SIMUL	//ģ�ⶨλ
};

///////////////////////////////////////////////////////////////////////////////
// GPS��Ҫ���ݽṹ��
struct SGpsData
{	
	BYTE				byFixType;					// ��λ��־��0-no fixed,1-fixed.
	BOOL				bIsAzimuth;					// ��λ����Ч				
	double				dbLat;						// γ��
	double				dbLong;						// ����
	double				dbSpeed;					// ����ڵ�����ٶ�
	double				dbAzimuth;					// ��λ�� (�ȱ�ʾ)
	SYSTEMTIME			UtcTime;					// utcʱ��
	double				dbAlt;						// ���θ߶�
};

///////////////////////////////////////////////////////////////////////////////
// ��λ���㾫�ȵļ���ͼ��(DOP)����
struct SDopData
{
	int					iFixState;		//��λ״̬:1-�޶�λ��Ϣ��2-2D, 3-3D, 6-DR Fixed
	double				dbPdop;					// PDOPֵ
	double				dbHdop;					// HDOPֵ
	double				dbVdop;					// VDOPֵ
	int					nNum;					// �������Ǹ���
	int					nUsedID[SAT_NUMBER];	// ��������id
};

///////////////////////////////////////////////////////////////////////////////
// ��������
struct SSatData
{
	int					nSatID;						// ���Ǳ��
	int					nElevation;					// ����
	int					nAzimuth;					// ��λ��
	int					nSNR;						// �����.0~99,-1Ϊ��Ч
	BOOL				bUsed;
};

#endif