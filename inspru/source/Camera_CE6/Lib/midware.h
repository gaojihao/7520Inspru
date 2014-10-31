#ifndef __MIDWARE_H
#define __MIDWARE_H

#if defined (__cplusplus)
extern "C" {
#endif

	
////////////////////////////////////////////Camera Control
void CamPreview(BOOL bPrev, RECT *pRect);
void CamPohto(TCHAR *pPath, RECT *pRect);
	

#if defined (__cplusplus)
}
#endif

#endif /* __MIDWARE_H */ 
