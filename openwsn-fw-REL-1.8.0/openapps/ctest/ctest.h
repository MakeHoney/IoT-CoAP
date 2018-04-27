#ifndef __CTEST_H
#define __CTEST_H

#include "opencoap.h"

typedef struct {
	coap_resource_desc_t desc;
	uint8_t test_name[10];
} ctest_vars_t;

/* prototype */

void ctest_init(void);

#endif