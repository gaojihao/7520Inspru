
// DlgDemand.cpp : implementation file
//

#include "stdafx.h"
#include "e9101main.h"
#include "../ExTool/ex_basics.h"
#include "DlgDemand.h"

#include "../DlgConfirm.h"
#include "../DlgSetPin.h"
#include "../SMSSending.h"
#include "../../imm/IMMMng.h"
#include "../../MutiLanguage/CGDICommon.h"

#include "../E9101MainDlg.h"
#include "../BrightnessSetDlg.h"
#include "../Msg/MsgQueue.h"
#include "../E9101Main/DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDemand dialog

extern BOOL g_bLight;
extern UINT	g_nWM_MENU_GNSSTERMINAL;

BOOL CDlgDemand::m_bIsOpen = FALSE;
UINT CDlgDemand::WM_DlgDemand_Refresh = RegisterWindowMessage(_T("WM_DlgDemand_Refresh"));

CDlgDemand::CDlgDemand(int nIndex, CWnd* pParent /*=NULL*/)
	: CDialogBase(CDlgDemand::IDD, pParent)
{
	m_nInfoIndex	= nIndex;
	m_nRecordCount		= 0;
}

void CDlgDemand::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDemand)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgDemand, CDialogBase)
	//{{AFX_MSG_MAP(CDlgDemand)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgDemand::OnInitDialog()  
{
	CDialogBase::OnInitDialog();
	
	SetWindowText(_T("DlgDemand_WINDOW_NAME"));

	InitGdi();
	InitControl();
	
	OnLanguageChange();
	OnSkinChange();

	m_bIsOpen = TRUE;

	ShowPage(m_nInfoIndex);

	return TRUE;
}

//变换语言和单位
BOOL CDlgDemand::OnLanguageChange()//变皮肤
{
	m_csTitile = _T("答案信息");
	
	for(int i=0;i<6;i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
		m_Item[i].chChar = _T("");
	}

	m_Item[0].chChar = _T("上传");
	m_Item[1].chChar = _T("");

	return TRUE;
}

BOOL CDlgDemand::OnSkinChange()
{
	return TRUE;
}

void CDlgDemand::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);
	//bk_disable_dc.CreateCompatibleDC(m_pDC);

	PSKINBMP_ST  pSkin  = g_pResMng->RequestBmp(BG_SMS_CENTER_N,true);
	m_stBtnNormalBMP   = pSkin->hBitmap;
	
	pSkin = g_pResMng->RequestBmp(BG_SMS_CENTER_P, true);
	m_stBtnPressBMP = pSkin->hBitmap; 
	
	m_pold_bk_normal_bmp  = bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp    = bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));
	//m_pold_bk_disable_bmp = bk_disable_dc.SelectObject(CBitmap::FromHandle(m_stBtnDisableBMP));
	
	::ReleaseDC(m_hWnd, hDC);
}

void CDlgDemand::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);
	//bk_disable_dc.SelectObject(m_pold_bk_disable_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);
	
	//bk_disable_dc.DeleteDC();
	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();
	
	bitmap.DeleteObject();
	memDC.DeleteDC();	
}

void CDlgDemand::OnExit()
{
	m_bIsOpen = FALSE;

	ReleaseGdi();
	CDialogBase::EndDialog(0);
}

void CDlgDemand::InitControl()
{
	//回复
	m_Item[0].rect.left		= 224;
	m_Item[0].rect.top		= 396;
	m_Item[0].rect.right	= 369;
	m_Item[0].rect.bottom	= 480;

	//转发 
	m_Item[1].rect.left		= 0;
	m_Item[1].rect.top		= 0;
	m_Item[1].rect.right	= 0;
	m_Item[1].rect.bottom	= 0;

	//删除
	m_Item[2].rect.left		= 370;
	m_Item[2].rect.top		= 396;
	m_Item[2].rect.right	= 513;
	m_Item[2].rect.bottom	= 480;

	//前一个
	m_Item[3].rect.left		= 514;
	m_Item[3].rect.top		= 396;
	m_Item[3].rect.right	= 656;
	m_Item[3].rect.bottom	= 480;

	//下一个
	m_Item[4].rect.left		= 657;
	m_Item[4].rect.top		= 396;
	m_Item[4].rect.right	= 800;
	m_Item[4].rect.bottom	= 480;

	//Exit
	m_Item[5].rect.left		= 720;
	m_Item[5].rect.top		= 0;
	m_Item[5].rect.right	= 800;
	m_Item[5].rect.bottom	= 60;

	//状态
	m_Static[0].rect.left	= 104;
	m_Static[0].rect.top	= 87;
	m_Static[0].rect.right	= 396;
	m_Static[0].rect.bottom	= 125;

	//时间
	m_Static[1].rect.left	= 475;
	m_Static[1].rect.top	= 87;
	m_Static[1].rect.right	= 767;
	m_Static[1].rect.bottom	= 125;
	
	//内容
	m_Static[2].rect.left	= 35;
	m_Static[2].rect.top	= 150;
	m_Static[2].rect.right	= 758;
	m_Static[2].rect.bottom	= 376;
}

void CDlgDemand::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
{
	ASSERT(pDC);
	int x = 0;
	int y = 0;
	switch(state)
	{
	case BTN_STATE_NORMAL:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DOWN:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		//pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_disable_dc,rt.left,rt.top,SRCCOPY);
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}


void CDlgDemand::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb=pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&font);

	pDC->DrawText(str, &rt, Format );

	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CDlgDemand::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;
/*
	st_SMSCenter* pMsg = g_pSMSCenter->GetRecord(m_nCurrSMSIndex);

	if( !pMsg->m_replyType )
	{
		m_Item[0].nState = BTN_STATE_DISABLE;
		m_Item[0].chChar = _T("");
	}
*/
	for(int i=0;i<6;i++)
	{	//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect, str, CGDICommon::Instance()->bigbigfont(),
			RGB(255,255,255),DT_VCENTER|DT_CENTER);
	}

	DrawBtnText(&memDC,m_Static[0].rect,m_Static[0].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[1].rect,m_Static[1].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);
	DrawBtnText(&memDC,m_Static[2].rect,m_Static[2].chChar,CGDICommon::Instance()->smallfont(),
		RGB(255,255,255),DT_LEFT|DT_TOP|DT_WORDBREAK|DT_EDITCONTROL);

	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255),DT_VCENTER|DT_CENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CDlgDemand::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i = 0;
	for( i = 0; i < 6; i++ )
	{
		if(PtInRect(&m_Item[i].rect,point))
		{
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}

	Invalidate();
	SetCapture();	
	CDialogBase::OnLButtonDown(nFlags, point);
}

void CDlgDemand::OnLButtonUp(UINT nFlags, CPoint point) 
{
	for(int i=0;i<6;i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			m_Item[i].nState = BTN_STATE_NORMAL;
			switch( i )
			{
			case 0:	Reply();		break;
			//case 1:	SendMsg();		break;
			case 2:	OnBtnDel();		break;
			case 3:	OnBtnPageUp();	break;
			case 4: OnBtnPageDown();break;
			case 5: OnExit();		break;
			default:
				break;
			}
		}
	}
	Invalidate();
	ReleaseCapture();
	CDialogBase::OnLButtonUp(nFlags, point);
}

void  CDlgDemand::OnBtnPageUp()
{
	if(m_nInfoIndex <= 0)
	{
		m_nInfoIndex = 0;
		return;
	}
	m_nInfoIndex--;

	ShowPage(m_nInfoIndex);
	Invalidate();
}

void  CDlgDemand::OnBtnPageDown()
{
	if(m_nInfoIndex >= m_nRecordCount-1)
	{
		m_nInfoIndex = m_nRecordCount-1;
		return;
	}
	m_nInfoIndex++;

	ShowPage(m_nInfoIndex);
	Invalidate();
}

void  CDlgDemand::OnBtnDel()
{
	CResString str;
	str.LoadString( RES_BUTTON_DELETE_SEL );
	
	CDlgConfirm::m_stTitle = str;
	CDlgConfirm dlg;
	dlg.DoModal();
	if( CDlgConfirm::s_bOk )
	{
		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);	//打开数据库
		char szSql[512];
		sprintf(szSql, "delete from info_menu where UID = %d;", m_nInfoIndex);
        db.execDML( szSql );
		db.close();

		OnExit();
	}
}

void  CDlgDemand::SendMsg()
{
	return;

	int nIdx = m_pData->max_number() - m_nSelIdx - 1;
	st_SMSCenter* pMsg = m_pData->get_recourd(nIdx);
	if ( pMsg )
	{
		if (  strlen(pMsg->m_Text.m_infoContext) < 1)
			return;

		int value;
		CDlgSetPinCode dlg;
		dlg.SetType( FALSE );
		value = dlg.DoModal();

		if ( value == IDOK && dlg.m_ItemNum.chChar.GetLength() > 0 )
		{
			CSMSSending Smsdlg;
			TCHAR szBuf[300];
			memset( szBuf, 0, sizeof(szBuf) );
			ex_char2uni(pMsg->m_Text.m_infoContext, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );
			CString strName(szBuf);
			Smsdlg.SetMsgParam( dlg.m_ItemNum.chChar, strName, m_bSavedMsg );
			Smsdlg.SetDestoryHwnd(this);
			Smsdlg.DoModal();
		}
	}
}

void  CDlgDemand::ShowPage(int nRecordIndex)
{
	CppSQLite3DB db;
	db.open(PATH_SQLITE_DB_808);	//打开数据库

	char szSqlBuffer[512];
	memset(szSqlBuffer, 0, sizeof(szSqlBuffer));
	CppSQLite3Query q;

	sprintf(szSqlBuffer, "update info_menu set read_status = 1 where UID = %d;", m_nInfoIndex);

	sprintf(szSqlBuffer, "SELECT * FROM info_menu WHERE UID = '%d';", m_nInfoIndex);
	q = db.execQuery(szSqlBuffer);

	//状态
	if( q.getIntField("read_status") )	//已回
		m_Static[0].chChar = _T("状态: 已读");
	else
		m_Static[0].chChar = _T("状态: 未读");

	m_nType = q.getIntField("type"); //类型

	m_Static[1].chChar = q.fieldValue("info_datatime");	//时间

	const char* content = q.fieldValue("info_name");	//获取文本内容
	m_Static[2].chChar = content;

	q.finalize();
	db.close();	//关闭数据库

	//查询记录总数量
// 	int m_nRecordCount = db.execScalar("select count(*) from answer;");
// 
// 	//在数据库中查询第nRecordIndex条数据
// 	char szSqlBuffer[512];
// 	sprintf(szSqlBuffer, "Select * From answer ORDER BY text_datatime DESC limit %d, 1;", nRecordIndex);
// 	CppSQLite3Query q = db.execQuery(szSqlBuffer);
// 
// 	if ( !q.eof() )
// 	{
// 		//sprintf(m_szSMS_ID, "%s", q.fieldValue("sms_ID"));		//获取当前短信ID
// 		m_nSMS_ID = q.getIntField("UID");		//获取当前短信ID
// 		TCHAR  szBuf[300];
// 		//状态
// 		if( q.getIntField("read_status") )	//已回复
// 			m_Static[0].chChar = _T("状态: 已读");
// 		else
// 			m_Static[0].chChar = _T("状态: 未读");
// 		
// 		//短信内容
// 		memset(szBuf, 0, sizeof(szBuf) );
// 		ex_char2uni( q.fieldValue("text_content"), szBuf,300 );
// 		m_Static[2].chChar.Format(_T("%s"), szBuf);
// 		
// 		//时间
// 		m_Static[1].chChar = q.fieldValue("text_datatime");
// 	}
// 	//释放statement
// 	q.finalize();
// 
// 	db.close();	//关闭数据库
// 	return;
}

void CDlgDemand::Reply()
{
//	CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, enMSG_ID::MSG_TMN_QUESTION_ACK, m_nAnswerIndex);
// 	CppSQLite3DB db;
// 	db.open(PATH_SQLITE_DB_808);	//打开数据库
// 	char szSql[512];
// 	//update sms_center set reply_status = 0, reply_content = 'reply_test3' where sms_ID = 1001;
// 	sprintf(szSql, "update question set read_status = 1 where serial_num = %d;", m_nQuestionIndex);
// 
// 	db.execDML(szSql);
// 	db.close();

	//更新界面显示状态
	m_Static[0].chChar = _T("状态: 已读");

	unCOMMON_DEMAND common_demand;
	common_demand.demand_result.wMsgType = m_nType;	//类型
	common_demand.demand_result.wMsgTag	 = 1;		//标志

	if (CMsgQueue::Instance().PostMsgQueue(g_nWM_MENU_GNSSTERMINAL, MSG_TMN_INFO_DEMAND_CANCEL, common_demand.dwDemand_result))
	{
		::PlaySound( _T("\\Flashdrv Storage\\menu\\Style\\SMS_Center_SND.wav\0"), NULL, SND_ASYNC | SND_NODEFAULT );
	}

	return;

	CString strSMS;
	if ( LoadSMSContext(strSMS) )
	{
		//将strSMS中的每个单引号变成两个单引号，解决sql语句的bug
		int nStartPos = strSMS.Find(_T("'"));
		while( nStartPos != -1 )//-1为没有找到单引号，循环搜索每一个
		{
			strSMS.Insert(nStartPos, _T("'"));//在每一个“'”前面都插入一个“'”
			nStartPos = strSMS.Find(_T("'"), nStartPos+2);//插入一个“'”，开始查找下一个“'”
		}
		
		char szReplyContent[300];
		memset(szReplyContent, NULL, sizeof(szReplyContent));
		ex_uni2char(strSMS.GetBuffer(strSMS.GetLength()), 
			szReplyContent, sizeof(szReplyContent));
		
		CppSQLite3DB db;
		db.open(PATH_SQLITE_DB_808);	//打开数据库
		char szSql[512];
		//update sms_center set reply_status = 0, reply_content = 'reply_test3' where sms_ID = 1001;
		sprintf(szSql, "update text_info set text_type = 1, reply_content = '%s' where UID = %d;", 
			szReplyContent, 
			m_nSMS_ID);

        db.execDML(szSql);
		db.close();

		//更新界面显示状态
		m_Static[0].chChar = _T("状态: 已回复");

		if (CMsgQueue::Instance().PostMsgQueue(WM_COMMAND_TIANZE, WPARAM_SMS_CENTER, m_nSMS_ID))
		{
			::PlaySound( _T("\\Flashdrv Storage\\menu\\Style\\SMS_Center_SND.wav\0"), NULL, SND_ASYNC | SND_NODEFAULT );
		}
	}
	return;
}

BOOL  CDlgDemand::LoadSMSContext(CString& strContext)
{
	//进入输入法不关闭背光
	//g_bLight = FALSE;	
	
	ImmReturn ret;
	IMMRESULT res;
	enSkinType enSkin = ID_SKIN2; //CSysConfig::Instance().GetSkinType();
	if(CIMMMng::Instance().Load() == TRUE)
	{
		CIMMMng::Instance().ImmSetKeySound( g_enKeySnd );
		CIMMMng::Instance().IMMInit(FourPoint3);
		if(CSysConfig::Instance().GetLanguage() == ID_ENGLISH)
			ret = CIMMMng::Instance().IMMCreateMessage(ID_ENGLISH);
		else
			ret = CIMMMng::Instance().IMMCreateMessage(ID_CHINESE);
	}
	else
	{
		MessageBox( _T("Load IMM has Failed!\n"), _T("System Tip"), MB_OK );
		return FALSE;
	}
	//恢复关闭背光
	//g_bLight = CSysConfig::Instance().GetAutoBkLight();
	CBrightnessSetDlg::SetbkliteTimeOuts();
	res = CIMMMng::Instance().IMMGetResult();
	//14 确认
	if( ret == 14 )
	{
		if( res.strName.GetLength() > 0 )
		{
			strContext = res.strName;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDlgDemand::IsOpen()
{
	return m_bIsOpen;
}

LRESULT CDlgDemand::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_DlgDemand_Refresh == message)
	{
		//m_nPageIndex = 0;
		ShowPage( lParam );
		Invalidate();
	}

	return CDialogBase::DefWindowProc(message, wParam, lParam);
}
