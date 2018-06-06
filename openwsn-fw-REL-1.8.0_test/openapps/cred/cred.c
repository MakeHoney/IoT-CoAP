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
const uint8_t bCntPrint[] = "권태성,김다운,원정연,이병헌|108동,2304호|";


owerror_t cred_receive(OpenQueueEntry_t* msg,
	coap_header_iht* coap_header,
	coap_option_iht* coap_options);
void cred_sendDone(OpenQueueEntry_t* msg,
	owerror_t error);

int lengthOfPrint(uint8_t* print) {
	int cnt = 0;
	while(print[cnt] != '\0') cnt++;
	return cnt;
}


void cb_btn() {
	cred_push('1');
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

		int len = lengthOfPrint(bCntPrint);
		packetfunctions_reserveHeaderSize(pkt,len);
		memcpy(pkt->payload, bCntPrint, len);
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
