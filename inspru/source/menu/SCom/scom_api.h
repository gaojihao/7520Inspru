
#ifndef  SCOM_API_H
#define  SCOM_API_H

#include "scom_ctrl.h"

scom_ctrl*	scom_api_ctrl();
bool		scom_api_start(unsigned n_com, unsigned n_baud);
void		scom_api_close();
void		watching_scom( void* a_param );

#endif //SCOM_API_H