#include "opendefs.h"
#include "ctest.h"
#include "opencoap.h"
#include "packetfunctions.h"
#include "openqueue.h"

ctest_vars_t ctest_vars;
const uint8_t ctest_path0[] = "test";

owerror_t ctest_receive(OpenQueueEntry_t* msg,
	coap_header_iht* coap_header,
	coap_option_iht* coap_options);
void ctest_sendDone(OpenQueueEntry_t* msg,
	owerror_t error);
void ctest_init() {
	ctest_vars.desc.path0len	= sizeof(ctest_path0) - 1;
	ctest_vars.desc.path0val	= (uint8_t*)(&ctest_path0);
	ctest_vars.desc.path1len	= 0;
	ctest_vars.desc.path1val	= NULL;
	ctest_vars.desc.componentID = COMPONENT_CTEST;
	ctest_vars.desc.callbackRx = &ctest_receive;
	ctest_vars.desc.callbackSendDone = &ctest_sendDone;

	memset(ctest_vars.test_name, 0, 10);
	sprintf(ctest_vars.test_name, "%s\n", "Clicked GET"); // my name

	// register with the CoAp module
	opencoap_register(&ctest_vars.desc);
}

void ctest_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
	openqueue_freePacketBuffer(msg);
}

owerror_t ctest_receive(OpenQueueEntry_t* msg, coap_header_iht* coap_header, coap_option_iht* coap_options) {
	owerror_t outcome;
	int len = 0;
	
	switch (coap_header->Code) {
	case COAP_CODE_REQ_GET:
		// reset packet payload
		msg->payload = &(msg->packet[127]);
		msg->length = 0;
		
		// add CoAPpayload
		// == owner name
		len = strlen(ctest_vars.test_name);
		packetfunctions_reserveHeaderSize(msg, len);
		memcpy(msg->payload, ctest_vars.test_name, len);
		
		// payload marker
		packetfunctions_reserveHeaderSize(msg, 1);
		msg->payload[0] = COAP_PAYLOAD_MARKER;
		
		// set the CoAPheader
		coap_header->Code = COAP_CODE_RESP_CONTENT;
		
		outcome = E_SUCCESS;
		break;

	case COAP_CODE_REQ_PUT:
		// change the owner's state
		len = msg->length;
		memcpy(ctest_vars.test_name, msg->payload, len);
		ctest_vars.test_name[len] = 0;

		// reset packet payload
		msg->payload = &(msg->packet[127]);
		msg->length = 0;

		len = strlen(ctest_vars.test_name);
		packetfunctions_reserveHeaderSize(msg, len);
		memcpy(msg->payload, ctest_vars.test_name, len);

		// payload marker
		packetfunctions_reserveHeaderSize(msg, 1);
		msg->payload[0] = COAP_PAYLOAD_MARKER;

		// set the CoAPheader
		coap_header->Code = COAP_CODE_RESP_CHANGED;

		outcome = E_SUCCESS;
		break;

	default:
		outcome = E_FAIL;
		break;
	}

	return outcome;
}