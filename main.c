/*************************************************************************
 > File Name: main.c
 > Author:jonker
 > address: 杭州图软科技 
 > Created Time: 2019年09月18日 星期三 15时26分57秒
 ************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include "IT8951_CMD.h"
#include "IT8951_USB.h"
#include "miniGUI.h"
#include <ft2build.h>
//#include FT_FREETYPE_H
//#include FT_GLYPH_H
#include <freetype/ftoutln.h>
#include <freetype/ftglyph.h>

#define GLOBAL_REF   0x02
#define PART_REF     0x01
#define INIT_REF     0x00

Byte* gpFrameBuf;
SystemInfo Sys_info;	// SystemInfo structure

static DWord gulPanelW, gulPanelH;

static void EPD_display_Area(DWord starX, DWord starY, DWord width, DWord High, int mode);
static void init_device(void) {

	// 0x12: get device name
	IT8951_Cmd_Inquiry();
	
	// 0x80: get system info
//	Sys_info=(SystemInfo*)malloc(sizeof(SystemInfo));
	IT8951_Cmd_SysInfo(&Sys_info);
	gulPanelW = Sys_info.uiWidth;
	gulPanelH = Sys_info.uiHeight;

	
	// 0x81: read memory
	
//	Byte *revBuf = (Byte*)malloc(sizeof(Byte)*16);
//	IT8951_Cmd_MemRead(Sys_info->uiImageBufBase, 16, revBuf);

	// 0x83: read register
	
//	DWord revVal;
//	IT8951_Cmd_RegRead(0x1800110C, &revVal);	// panel width
//	printf("0x%X\n", revVal);	
	
	// 0xA2 & 0x94: load & display
	
	// Mode 0: initial
	IT8951_Cmd_DisplayArea(0, 0, gulPanelW, gulPanelH, PART_REF, (Sys_info.uiImageBufBase), 1);

	// set full white
//	Byte srcW[(gulPanelW*gulPanelH)];
	gpFrameBuf = (unsigned char*)malloc(sizeof(unsigned char)*(gulPanelW*gulPanelH));
	EPD_Clear(0xF0);
//	memset((srcW), 0x00, (gulPanelW*gulPanelH));
    
    EPD_display_Area(0, 0, gulPanelW, gulPanelH, GLOBAL_REF);	
}

void EPD_display_Area(DWord starX, DWord starY, DWord width, DWord High, int mode) {
	IT8951_Cmd_LoadImageArea((Sys_info.uiImageBufBase), starX, starY, gulPanelW, gulPanelH);  // 
	IT8951_Cmd_DisplayArea(starX, starY, width, High, mode, (Sys_info.uiImageBufBase), 1);
}

int main(int argc, char * argv[]) {
	int count = 0;
	
    init_device();	
	Show_linuxfb(0, 0);
//	EPD_Text(200, 200, "Hello World!", 0xF0, 0x00);
//    sleep(5);
 //   EPD_display_Area(0, 0, gulPanelW, gulPanelH, GLOBAL_REF);
	while(1) {
	    Show_linuxfb(0, 0);
		count ++;
		if(count == 10) {
		    EPD_display_Area(0, 0, gulPanelW, gulPanelH, GLOBAL_REF);
		    count = 0;
		} else {
		    EPD_display_Area(0, 0, gulPanelW, gulPanelH, PART_REF);
		}
		sleep(1);
	}
	// 0xA4: temperature
	TempArg TempTest;
	TempTest.ucSetTemp = 0;	// 0: read, 1: write
	TempTest.ucTempVal = 126;	// set value if wanna write
	IT8951_Cmd_SetTemp(TempTest);

//error:
	//FT_Done_Glyph(glyph);
	//glyph  =  NULL;
	free(gpFrameBuf);
//	pthread_mutex_destroy(&frameBuf_mutex); // 销毁信号锁
	return EXIT_SUCCESS;
}
