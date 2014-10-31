// IOControl.h : main header file for the IOControl DLL
// IOControl for KP8539  210 platform

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include <Ras.h>

// CIOControlApp
// See IOControl.cpp for the implementation of this class
//
#ifdef __cplusplus
extern "C" {
#endif

#ifdef IOCONTROL_DLL
#define IOAPI __declspec(dllexport)
#else
#define IOAPI __declspec(dllimport)
#endif

#define LOW_BATTERY_VOLTAGE        65   //Too Low Voltage Value for Battery(Voltage * 10)

/*-------------------------------------------------------------/
Description:
Init  DLL

Parameters:

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_Init();

/*-------------------------------------------------------------/
Description:
 Switch On/Off GPRS Power 

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenGprsPower(BOOL bOnOff);

/*-------------------------------------------------------------/
Description:
 Switch On/Off GPS Power 

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenGpsPower(BOOL bOnOff);

/*-------------------------------------------------------------/
Description:
 Reset GPS

Parameters:

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_ResetGps();

/*-------------------------------------------------------------/
Description:
 Switch SIM card slot (Dual SIM card slots in M731)

Parameters: BOOL bFirstSIM
If bFirstSIM == TRUE,  switch to use first SIM card(default upper SIM card slot)
If bFirstSIM == FALSE,  switch to use second SIM card(lower SIM card slot)

Note:
Inside this API, our code already turn off 3G modem, and then switch SIM card,
then switch on 3G modem again.

If 3G modem initially is on, after call this API, the 3G modem remain on.
If 3G modem initially is off, after call this API, the 3G modem be turned on.

If your application is already connected to internet, please disconnect to your server,
close TCP/IP socket, call RASHangup, and then close COM8:, after that, you can run this API
to switch to use another SIM card.

Return
Return TRUE if success, return FALSE if fail
 
--------------------------------------------------------------*/
IOAPI BOOL IO_SwitchSIM(BOOL bFirstSIM);

/*-------------------------------------------------------------/
Description:
 Reset  System

Parameters:

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_ResetOS();

/*-------------------------------------------------------------/
Description:
Whether use Battery as sole power supply or not

Parameters:

Return
Yes return TRUE,  No return FALSE 
--------------------------------------------------------------*/
IOAPI BOOL IO_IsUseBattery();



/*-------------------------------------------------------------/
Description:
 Switch On/Off TTS Voice Volume Amplifer

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenTTSAmp(BOOL bOnOff);

/*-------------------------------------------------------------/
Description:
Set LCD backlight level

Parameters:
iLevel     from 0 to 7,  0 is turn off backlight

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_SetLcdBackLight(int iLevel);



/*-------------------------------------------------------------/
Description:
 Read back  Battery  Voltage 

Parameters:

Return
 Success return  Battery  Voltage  * 10
 Failed  return -1
--------------------------------------------------------------*/
IOAPI int IO_GetBatteryVoltage();


/*-------------------------------------------------------------/
Description:
 Switch  Voice Volume Amplifer

Parameters:
bIsTTSAmp        TRUE select work as TTS Voice Volume Amplifer, FALSE select to work as GSM Voice Volume Amplifer

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_ChangeAmp(BOOL bIsTTSAmp);



/*-------------------------------------------------------------/
Description:
Turn Off ARM9 Power Supply 

Parameters:
cRestartCondition       Restart Condition

Restart event,  bit is 1 means this event is Selected Restart Event 
Bit0    IO0 status change 
Bit1    IO1 status change 
Bit2    IO2 status change 
Bit3    IO3 status change 
Bit4    IO4 status change 
Bit5    IO5 status change 
Bit6    Key Pressed
Bit7    Start Voltage Level is ready

Return
Yes return TRUE,  No return FALSE
--------------------------------------------------------------*/
IOAPI BOOL IO_CloseMainCPUPower(unsigned char cRestartCondition);

/*-------------------------------------------------------------/
Description:
 Read back Outside Main Power Supply Voltage 

Parameters:

Return
 Success return  main power supply voltage * 10
 Failed  return -1
--------------------------------------------------------------*/
IOAPI int IO_GetOuterVoltage();


/*-------------------------------------------------------------/
Description:
 Switch On/Off  LED Indicator 

Parameters:
iIdx       LED Indicator Number (0 ~ 2)
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenIndicationLight(int iIdx, BOOL bOnOff);



/*-------------------------------------------------------------/
Description:
Call this API to exit init full screen Welcome.jpg LOGO which start from logo.exe 

Parameters:

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_DisappearWelcomeJPEG(void);


/*-------------------------------------------------------------/
Description:
 Set or disable the Watch Dog function

Parameters:
cVal     if cVal==0, disable the watch dog
         if cVal > 0, enable the watch dog and cVal is the watch dog time out
         value at seconds(time out value = cValue * 10).
         e.g. if cVal = 3, watch dog will reset system after 30 seconds if 
         function IO_FeedWatchDog() does NOT feed the watch dog

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_SetWatchDog(unsigned char cVal);
//Below API is workable for M731
extern IOAPI BOOL IO_SetWatchDogAction(unsigned char cVal);//

/*-------------------------------------------------------------/
Description:
 Feed the Watch Dog
 User application must call this function regularly within timeout
 to avoid watch dog reset system

Parameters:
 

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_FeedWatchDog(void);
//Below API is workable for M731
extern IOAPI BOOL IO_FeedWatchDogAction(void);//

/*-------------------------------------------------------------/
Description:
Check if battery is fully charged or not

Parameters:
 

Return:
Full return TRUE ;  Not full return FALSE
--------------------------------------------------------------*/
extern IOAPI BOOL IO_IsBatteryFull();


/*-------------------------------------------------------------/
Description:
Get MDT Device Serial ID
 
Parameters:
cDevID     Pointer to string buffer
iLen       Buffer Size, Max is 25
 
Return:
Success return TRUE,  Fail return FALSE


For example, you can call 
char szDeviceID[11];
BOOL bResult = IO_GetDeviceID(szDevID,10);
--------------------------------------------------------------*/
extern IOAPI BOOL IO_GetDeviceID(char* cDevID, int iLen);


/*-------------------------------------------------------------/
Description:
Check ACC Ignition Status On or Off
User need to connect ACC wire to car ACC signal

Parameters:


Return
      TRUE-On  FALSE-Off
--------------------------------------------------------------*/
IOAPI BOOL IO_IsACCOn();


/*-------------------------------------------------------------/
Description:
Check to see if the hard key is pressed down and hold for quite a while


Parameters:
KeyID = 0, Set key

KeyID = 1, 1 key

KeyID = 2, 2 key

KeyID = 3, Power key

KeyID = 4, 3 key

KeyID = 5, 4 key


Return
      Press and Hold This Key:  TRUE ==> Yes
	  FALSE ==> No
  
--------------------------------------------------------------*/
IOAPI BOOL IO_IsHardKeyLongTimePressAndHold(int nKeyID);

/*-------------------------------------------------------------/
Description:
Check if SET key has been pressed and hold for long time
 

Parameters:


Return
      TRUE-Yes (SET Key has been pressed and hold)  FALSE-No
--------------------------------------------------------------*/

IOAPI BOOL IO_IsSetLongPress();

/*-------------------------------------------------------------/
Description:
Turn on Key Pad Backlight (blue LED)
 

Parameters:


Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenKeyBoardLight(BOOL bOnOff);


/*-------------------------------------------------------------/
Description:
Set Two Speaker's Sound Volume


Parameters:
iVol (from 0 to 10)
iVol = 0 mute
iVol = 10 max volume

Return
      TRUE=Success  FALSE=Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_SetSpeakerVolume(int iVol);




/*-------------------------------------------------------------/
Description:
Init the camera
 

Parameters:


Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_InitCamera();
/*-------------------------------------------------------------/
Description:
Turn on Camera
 

Parameters:
dwCam    :Camera ID which you need to turn on (1 to 4) 
CanRect  :Video Area Size

Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/

IOAPI BOOL IO_OpenCam(DWORD dwCam,CRect CamRect);


/*-------------------------------------------------------------/
Description:
Take Photo by Camera
 

Parameters:
PhotoPath: What path to save the photo file

Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
 
IOAPI BOOL IO_TakePhoto(TCHAR* PhotoPath);

/*-------------------------------------------------------------/
Description:
Start to Take Video by Camera
 

Parameters:
PhotoPath: What path to save the video file

Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/

IOAPI BOOL IO_StartVideo(char* VideoPath);

/*-------------------------------------------------------------/
Description:
Stop to Take Video by Camera
 

Parameters:


Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_StopVideo();

/*-------------------------------------------------------------/
Description:
Close Camera
 

Parameters:


Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_CloseCam();



/*-------------------------------------------------------------/
Description:
Disable RasDialup the UserName Password Verify  Dialog
 

Parameters:


Return
     void
--------------------------------------------------------------*/
IOAPI void IO_DisableRASDialUserNamePasswordVerifyDialog();


/*-------------------------------------------------------------/
Description:
RAS Dial up function, for those Dial up without need to set user name 
and password.
This function already disable the dial up user name and password verify dialog
 

Parameters:
Handle to connect to 


Return
    same return value as RASDial
--------------------------------------------------------------*/
IOAPI int IO_RasDial(HRASCONN hRasConn);

/*-------------------------------------------------------------/
Description:
RAS Dial up function, for those Dial up whcih need to set user name 
and password. 

Parameters:
RASDIALPARAMS which input user name and password, HRASCONN Handle to connect to 


Return
    same return value as RASDial
--------------------------------------------------------------*/
IOAPI int IO_RasDialWithParas(RASDIALPARAMS rasPara,HRASCONN hRasConn);



/*-------------------------------------------------------------/
Description:
Hangup RAS dialup
 

Parameters:
Handle to disconnect

Return
     TRUE- Success  FALSE- Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_RasHungUp(HRASCONN hRasConn);


/*-------------------------------------------------------------/
Description:
Get the MDT device model and basic configuration
 

Parameters:
strModel(Max 40 characters) which is string contain MDT model and CPU, 3G/GPRS model
e.g. strModel will be:
M731 A8 SIM5320J 850/2100MHz

Return
     TRUE- Success  FALSE- Fail

--------------------------------------------------------------*/
IOAPI BOOL IO_GetDeviceModel(char *strModel);

 
/*-------------------------------------------------------------/
Description:
Read Main CPU WinCE OS version
 

Parameters:
OSVersion return the OS version and build date, buffsize is max 
characters for OSVersion buffer

Return
     TRUE-Read Success  FALSE-Read Fail
--------------------------------------------------------------*/

IOAPI BOOL IO_GetOSVersion(TCHAR *OSVersion, DWORD buffsize);

/*-------------------------------------------------------------/
Description:
Read this IOControl.dll version
 

Parameters:
iVersion(Max  15 characters) return this DLL build date 
Version£ºyear.month.day  e.g. Version:2012.06.04

Return
     TRUE-Read Success  FALSE-Read Fail

 
--------------------------------------------------------------*/
IOAPI BOOL IO_GetDLLVersion(char *iVersion);

/*-------------------------------------------------------------/
Description:
Read this MDT 3G/GPRS modem IMEI number
 
Parameters:
szGUID (Max  18 characters) return this 3G/GPRS modem IMEI number
 

Return
     TRUE-Read Success  FALSE-Read Fail

 
--------------------------------------------------------------*/
IOAPI BOOL IO_GetDeviceGUID(char *szGUID);

/*-------------------------------------------------------------/
Description:
Read first input status
 
Parameters:
NULL
 

Return
     TRUE-Input High Voltage  FALSE-Input Low Voltage or Floating
--------------------------------------------------------------*/
IOAPI BOOL IO_GetDigtalInputStatus();


/*-------------------------------------------------------------/
Description:
Read all input status
 
Parameters:
nPort = Port number from 0 to 2
 
nPort = 0 //Green Input Line in Picture 
nPort =1 //Acc Ignition Input Yellow Line, near red power input wire and black wire ground 
nPort =2 //Grey Input Line

Return
     TRUE-Input High Voltage  FALSE-Input Low Voltage or Floating
--------------------------------------------------------------*/
IOAPI BOOL IO_GetAllDigtalInputStatus(int nPort);



/*-------------------------------------------------------------/
Description:
Set output port status
 
Parameters:
nPort = Port number from 0  
bHighVoltage = TRUE, high voltage
bHighVoltage = FALSE, low voltage

Return
     TRUE-Set Success FALSE-Set Fail
--------------------------------------------------------------*/
IOAPI BOOL IO_SetOutputStatus(int iPort,BOOL bHighVoltage);

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//Below APIs are NOT supported in M730 and M731
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------/
Description:
Change  SIM card slot

Parameters:

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_ChangeSIM(); //Replaced by IOAPI BOOL IO_SwitchSIM(BOOL bFirstSIM);

/*-------------------------------------------------------------/
Description:
 Switch On/Off Another MDT  Power 

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenMDTPower(BOOL bOnOff);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
 Switch On/Off Beeper

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_OpenBuzzer(BOOL bOnOff);//Not Supported in M730/M731


/*-------------------------------------------------------------/
Description:
Set the key's backlight on lasting time in seconds after this key was pressed
 

Parameters:
iTime     from 0 to 0xFF
0 means turn off key's backlight
0xff means always on the key's backlight
Other value is key's backlight last time in seconds.

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_SetKeyBackLight(int iTime);
//Replaced by IOAPI BOOL IO_OpenKeyBoardLight(BOOL bOnOff);



/*-------------------------------------------------------------/
Description:
 Read back  Environment Brightness 

Parameters:

Return
 Success return  Environment Brightness 
 Failed  return -1
--------------------------------------------------------------*/
IOAPI int IO_GetBrightness();//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
 Read back  Environment Brightness 

Parameters:

Return
 Success return  Environment Brightness 
 Failed  return -1
--------------------------------------------------------------*/
//IOAPI int IO_GetEnvBrightness();//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
If there is any SMS or GSM phone call in

Parameters:

Return
Yes return TRUE,  No return FALSE
--------------------------------------------------------------*/
IOAPI BOOL IO_HavePhoneOrSms();//Not Supported in M730/M731


/*-------------------------------------------------------------/
Description:
Turn On/Off  Battery Recharge

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Yes return TRUE,  No return FALSE
--------------------------------------------------------------*/
IOAPI BOOL IO_EnableCharge(BOOL bOnOff);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
Set the Restart Voltage 

Parameters:
fVoltage       Restart Voltage Value

Return
Yes return TRUE,  No return FALSE
--------------------------------------------------------------*/
IOAPI BOOL IO_SetRestartVolatge(float fVoltage);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
Check Recharge function is On or not

Parameters:

Return
Yes return TRUE,  No return FALSE
--------------------------------------------------------------*/
IOAPI BOOL IO_IsUseCharge();//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
 Switch On/Off 5V Power  Output on Serial port AVR0

Parameters:
bOnOff     TRUE-On  FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_Open5VPower(BOOL bOnOff);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
Set Wakeup Phone Call Number

Parameters:
cPhoneNo  Phone Number less than 16 digits

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
IOAPI BOOL IO_SetWakupPhone(char* cPhoneNo);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
Switch On/ Off USB Host Power

Parameters:
bOnOff     TRUE-Open FALSE-Off

Return
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_OpenUsbPower(BOOL bOnOff);//Not Supported in M730/M731
/*-------------------------------------------------------------/
Description:
Restore All Hardware Settings from Flash_Storage backup file

Parameters:

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_RestoreHardwareSettings(void);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
Save All Hardware Settings to Flash_Storage backup file

Parameters:

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/
extern IOAPI BOOL IO_SaveHardwareSettings(void);//Not Supported in M730/M731

/*-------------------------------------------------------------/
Description:
To get WiFi module MAC address and IP address

Parameters:
objMacIp    structure of get back WiFi module parameters value

Return:
Return TRUE if success, return FALSE if fail
--------------------------------------------------------------*/

//extern IOAPI BOOL IO_GetWifiMacAndIp(MAC_IP_T* objMacIp);//Not Supported in M730/M731


/*-------------------------------------------------------------/
Description:
Check if uses the earphone 

Parameters:
 

Return:
Yes, use earphone return TRUE; No, not use earphone return FALSE
--------------------------------------------------------------*/
extern IOAPI BOOL IO_IsUseEarphone();//Not Supported in M730/M731

////////////////////////////////////////////////////////
//End of API which is Not Supported in M730/M731
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif