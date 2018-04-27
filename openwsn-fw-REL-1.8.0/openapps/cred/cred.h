#ifndef __CRED_H
#define __CRED_H

#include "opencoap.h"

typedef struct {
	coap_resource_desc_t desc;
} cred_vars_t;

//====================prototypes======================

void cred_init(void);

#endif