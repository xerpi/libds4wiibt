#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <ds4wiibt.h>
#include "utils.h"

//Controller's MAC: 1C:66:6D:2A:18:C5
struct bd_addr addr = {.addr = {0xC5, 0x18, 0x2A, 0x6D, 0x66, 0x1C}};
static void print_data(struct ds4wiibt_input *inp);
static void conn_cb(void *usrdata);
static void discon_cb(void *usrdata);

struct ds4wiibt_context ctx;

int main(int argc, char *argv[])
{
	fatInitDefault();
	init();
	WPAD_Init();
	printf("ds4wiibt sample by xerpi\n");

	ds4wiibt_initialize(&ctx, &addr);
	ds4wiibt_set_userdata(&ctx, NULL);
	ds4wiibt_set_connect_cb(&ctx, conn_cb);
	ds4wiibt_set_disconnect_cb(&ctx, discon_cb);

	printf("Listening to: ");
	print_mac((struct bd_addr*)addr.addr);
	printf("Listening for an incoming connection...\n");
	ds4wiibt_listen(&ctx);

	while (run) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_HOME) run = 0;
		if (pressed & WPAD_BUTTON_B) ds4wiibt_disconnect(&ctx);
		if (pressed & WPAD_BUTTON_1) ds4wiibt_listen(&ctx);
		if (ds4wiibt_is_connected(&ctx)) {
			print_data(&ctx.input);
			if (ctx.input.PS && ctx.input.OPTIONS) ds4wiibt_disconnect(&ctx);
		}
		flip_screen();
	}
	ds4wiibt_disconnect(&ctx);
	return 0;
}

void conn_cb(void *usrdata)
{
	printf("Controller connected.\n");
}

void discon_cb(void *usrdata)
{
	printf("Controller disconnected.\n");
	struct l2cap_pcb_listen *lpcb;
	for(lpcb = l2cap_listen_pcbs; lpcb != NULL; lpcb = lpcb->next) {
		//printf("\t%d\n", lpcb->psm);
	}
	//ds4wiibt_listen(&ctx);
}

void print_data(struct ds4wiibt_input *inp)
{ 
	printf("\x1b[10;0H");
	printf("PS: %1i   OPTIONS: %1i  SHARE: %1i   /\\: %1i   []: %1i   O: %1i   X: %1i\n", \
			inp->PS, inp->OPTIONS, inp->SHARE, inp->triangle, \
			inp->square, inp->circle, inp->cross);
			
	printf("TPAD: %1i   L3: %1i   R3: %1i\n", \
			inp->TPAD, inp->L3, inp->R3);

	printf("L1: %1i   L2: %1i   R1: %1i   R2: %1i   DPAD: %1i\n", \
			inp->L1, inp->L2, inp->R1, inp->R2, \
			inp->dpad);
	printf("LX: %2X   LY: %2X   RX: %2X   RY: %2X  battery: %1X\n", \
			inp->leftX, inp->leftY, inp->rightX, inp->rightY, inp->battery);
	
	printf("headphones: %1X   microphone: %1X   usb_plugged: %1X  batt_level: %2X\n", \
			inp->headphones, inp->microphone, inp->cable, inp->batt_level);	

	printf("aX: %5hi  aY: %5hi  aZ: %5hi  roll: %5hi  yaw: %5hi  pitch: %5hi\n", \
			inp->accelX, inp->accelY, inp->accelZ, inp->roll, inp->yaw, inp->pitch);

	printf("Ltrigger: %2X   Rtrigger: %2X  trackpadpackets: %4i  packetcnt: %4i\n", \
			inp->triggerL, inp->triggerR, inp->trackpad_pkts, inp->packet_count);
			
	printf("f1active: %2X   f1ID: %2X  f1X: %4i  f1Y: %4i\n", \
			inp->finger1.active, inp->finger1.ID, inp->finger1.X, inp->finger1.Y);
	printf("f2active: %2X   f2ID: %2X  f2X: %4i  f2Y: %4i\n", \
			inp->finger2.active, inp->finger2.ID, inp->finger2.X, inp->finger2.Y);
	printf("Timestamp: %llu\n", ctx.timestamp);
}
