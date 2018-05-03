#include "opendefs.h"
#include "cred.h"
#include "opencoap.h"
#include "packetfunctions.h"
#include "openqueue.h"
#include "leds.h"
#include "button.h"

#include "idmanager.h"
#include "IEEE802154E.h"

static const uint8_t ipAddr_Server[] = {0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};

cred_vars_t cred_vars;
const uint8_t cred_path0[] = "red";
/* byunghun */
uint8_t bCntPrint[] = "binary counter";
uint8_t* bCnt[] = {"000", "001", "010", "011",
					"100", "101", "110", "111"};
uint8_t* ott[] ={"100", "000","110","000","111","000"};
uint8_t* ott2[] ={"100", "110","111","011","001","000"};
/* jungyeon */
uint8_t ottPrint[] = "1 2 3 blink";

/* taesung */
uint8_t wavePrint[] = "wave 5 blink";


owerror_t cred_receive(OpenQueueEntry_t* msg,
	coap_header_iht* coap_header,
	coap_option_iht* coap_options);
void cred_sendDone(OpenQueueEntry_t* msg,
	owerror_t error);

int lengthOfPrint(uint8_t* print) {
	int cnt = 0;
	while (print[cnt] != '\0') cnt++;
	return cnt;
}

void binaryCounter(uint8_t i) {
	if (bCnt[i][0] == '1') leds_error_on();
	else	leds_error_off();
	if (bCnt[i][1] == '1') leds_radio_on();
	else	leds_radio_off();
	if (bCnt[i][2] == '1') leds_sync_on();
	else	leds_sync_off();
}

void foo(uint8_t i) {
	if (ott[i][0] == '1') leds_error_on();
	else	leds_error_off();
	if (ott[i][1] == '1') leds_radio_on();
	else	leds_radio_off();
	if (ott[i][2] == '1') leds_sync_on();
	else	leds_sync_off();
}

void wave(uint8_t i) {
	if (ott2[i][0] == '1') leds_error_on();
	else	leds_error_off();
	if (ott2[i][1] == '1') leds_radio_on();
	else	leds_radio_off();
	if (ott2[i][2] == '1') leds_sync_on();
	else	leds_sync_off();
}


void cb_btn() {
	leds_error_toggle();
}


void cred_init() {
	// prepare the resource descriptor for the /.well-known/core path
	cred_vars.desc.path0len		= sizeof(cred_path0) - 1;
	cred_vars.desc.path0val		= (uint8_t*)(&cred_path0);
	cred_vars.desc.path1len		= 0;
	cred_vars.desc.path1val		= NULL;
	cred_vars.desc.componentID	= COMPONENT_CRED;
	cred_vars.desc.callbackRx	= &cred_receive;
	cred_vars.desc.callbackSendDone = &cred_sendDone;

	// cred_vars.timerId = opentimers_start(
	// 	3000,
	// 	TIMER_PERIODIC,
	// 	TIME_MS,
	// 	cred_push
	// 	);
	
	// register with the CoAP module
	opencoap_register(&cred_vars.desc);
	btn_setCallbacks(cb_btn);
}

void cred_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
	openqueue_freePacketBuffer(msg);
}


void cred_push(uint8_t action) {
	OpenQueueEntry_t* pkt;
	owerror_t outcome;
	uint8_t numOptions;
	
	// don't run if not synch
	if (ieee154e_isSynch() == FALSE) return;

	// don't run on dagroot
	if (idmanager_getIsDAGroot()) {
		opentimers_stop(cred_vars.timerId);
		return;
	}

	//=== request a packetBuffer

	// create a CoAP RD packet
	pkt = openqueue_getFreePacketBuffer(COMPONENT_CRED);

	if (pkt==NULL) {
		openserial_printError(
			COMPONENT_CRED,
			ERR_NO_FREE_PACKET_BUFFER,
			(errorparameter_t)0,
			(errorparameter_t)0
		);
		openqueue_freePacketBuffer(pkt);
		return;
	}

	// take ownership over that packet
	pkt->creator = COMPONENT_CRED;
	pkt->owner = COMPONENT_CRED;

	// CoAP payload
	numOptions = 0;
	if(action == '1') {
		int len = lengthOfPrint(ottPrint);
		packetfunctions_reserveHeaderSize(pkt, len);
		memcpy(pkt->payload, ottPrint, len);
	} else if (action == '2') {
		int len = lengthOfPrint(bCntPrint);
		packetfunctions_reserveHeaderSize(pkt, len);
		memcpy(pkt->payload, bCntPrint, len);
	} else if (action == '3') {
		int len = lengthOfPrint(wavePrint);
		packetfunctions_reserveHeaderSize(pkt, len);
		memcpy(pkt->payload, wavePrint, len);
	} else {
		packetfunctions_reserveHeaderSize(pkt,3);
		pkt->payload[0] = 'o';
		pkt->payload[1] = 'f';
		pkt->payload[2] = 'f';
	}

	// payload marker
	packetfunctions_reserveHeaderSize(pkt,1);
	pkt->payload[0] = COAP_PAYLOAD_MARKER;

	// URI-path
	packetfunctions_reserveHeaderSize(pkt,sizeof(cred_path0)-1);
	memcpy(pkt->payload,cred_path0,sizeof(cred_path0)-1);
	packetfunctions_reserveHeaderSize(pkt,1);
	pkt->payload[0] = (COAP_OPTION_NUM_URIPATH) << 4 | 3;
	numOptions++;

	//=== send CoAP message
	// metadata
	pkt->l4_destination_port = 7777;
	pkt->l3_destinationAdd.type = ADDR_128B;
	memcpy(&pkt->l3_destinationAdd.addr_128b[0],&ipAddr_Server,16);

	// send
	outcome = opencoap_send(
		pkt,
		COAP_TYPE_NON,
		COAP_CODE_REQ_POST,
		numOptions,
		&cred_vars.desc );

	if (outcome==E_FAIL) {
	openqueue_freePacketBuffer(pkt);
	}
	return;
}

owerror_t cred_receive(OpenQueueEntry_t* msg,
	coap_header_iht* coap_header,
	coap_option_iht* coap_options) {
	owerror_t outcome;
	switch (coap_header->Code) {
	case COAP_CODE_REQ_GET:
		// reset packet payload
		msg->payload
			= &(msg->packet[127]);
		msg->length
			= 0;

		// add CoAP payload
		if (leds_error_isOn() == 1) {
			packetfunctions_reserveHeaderSize(msg, 2);
			msg->payload[0]
				= 'O';
			msg->payload[1]
				= 'N';
		}
		else {
			packetfunctions_reserveHeaderSize(msg, 3);
			msg->payload[0]
				= 'O';
			msg->payload[1]
				= 'F';
			msg->payload[2]
				= 'F';
		}

		// payload marker
		packetfunctions_reserveHeaderSize(msg, 1);
		msg->payload[0] = COAP_PAYLOAD_MARKER;

		// set the CoAP header
		coap_header->Code = COAP_CODE_RESP_CONTENT;

		outcome
			= E_SUCCESS;
		break;
	case COAP_CODE_REQ_POST:
		// change the LED's state
		if (msg->payload[0] == '1') {
			leds_error_on();
		}
		else if (msg->payload[0] == '0') {
			leds_error_off();
		}
		else {
			leds_error_toggle();
		}

		// reset packet payload
		msg->payload
			= &(msg->packet[127]);
		msg->length
			= 0;


		// set the CoAP header
		coap_header->Code = COAP_CODE_RESP_CHANGED;

		outcome = E_SUCCESS;
		break;

	case COAP_CODE_REQ_PUT:
		// change the owner's state
		if (msg->payload[0] == '1') {
			int i = 0, j = 0;
			volatile int delay;

			for (i = 0; i < 6; i++) {
				leds_all_off();
				foo(i);
				for (j = 0; j < 4; j++)
					for (delay = 1000000; delay > 0; delay--);
			}
			// leds_all_off();

			// // blink error led once
			// leds_error_on();
			// for (j = 0; j < 4; j++)
			// 		for (delay = 1000000; delay > 0; delay--);
			// leds_error_off();
			// for (j = 0; j < 4; j++)
			// 		for (delay = 1000000; delay > 0; delay--);

			// // blink error and radio led twice
			// for (i = 0; i < 2; i++) {
			// 	leds_error_on();
			// 	leds_radio_on();
			// 	for (j = 0; j < 4; j++)
			// 		for (delay = 1000000; delay > 0; delay--);
			// 	leds_error_off();
			// 	leds_radio_off();
			// 	for (j = 0; j < 4; j++)
			// 		for (delay = 1000000; delay > 0; delay--);
			// }

		
			cred_push('1');

		} else if (msg->payload[0] == '2') {
			int i = 0, j = 0;
			volatile int delay;
			leds_all_off();

			for (i = 0; i < 8; i++) {
				leds_all_off();
				binaryCounter(i);
				for (j = 0; j < 4; j++)
					for (delay = 1000000; delay > 0; delay--);
			}
			cred_push('2');

		} else if (msg->payload[0] == '3') {
			int i = 0, j = 0;
			volatile int delay;

			leds_all_off();

			for (i = 0; i < 6; i++) {
				leds_all_off();
				wave(i);
				for (j = 0; j < 4; j++)
					for (delay = 1000000; delay > 0; delay--);
			}
			// for (i = 0; i < 5; i++) {
			// 	leds_error_on();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_radio_on();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_sync_on();			for (delay = 0xffff; delay > 0; delay--);
			// 	leds_error_off();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_radio_off();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_sync_off();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_sync_on();			for (delay = 0xffff; delay > 0; delay--);
			// 	leds_radio_on();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_error_on();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_sync_off();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_radio_off();		for (delay = 0xffff; delay > 0; delay--);
			// 	leds_error_off();		for (delay = 0xffff; delay > 0; delay--);
			// }

			cred_push('3');
		}
		else if (msg->payload[0] == '4') {
			leds_error_off();
			cred_push('4');
		} else {
			leds_error_toggle();
		}

		// reset packet payload
		msg->payload = &(msg->packet[127]);
		msg->length = 0;

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