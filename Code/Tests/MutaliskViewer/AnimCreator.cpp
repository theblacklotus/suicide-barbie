#include <pspkernel.h>
#include <string.h>
#include <stdio.h>
#include "vram.h"
#include <pspgu.h>
#include "AnimCreator.h"

static const bool s_createAnim = (__CREATE_ANIM == 1);

static const char* s_baseFileName = "host1:Anim/out";
void saveAnimFrame(void* vram)
{
	if (!s_createAnim)
		return;
	void* screen = mapVramBufferToTexture(vram);
	static unsigned frameIndex = 0;
	char buf[255];
	if (frameIndex == 0)
	{
		char* lastSlash = strstr(s_baseFileName, "/");
		if (lastSlash)
		{
			unsigned len = lastSlash-s_baseFileName;
			strncpy(buf, s_baseFileName, lastSlash-s_baseFileName);
			buf[len] = 0;
			printf("createdir : %s\n", buf);
			sceIoMkdir(buf, 0777);
		}
	}
	sprintf(buf, "%s%05i.tga", s_baseFileName, frameIndex++);
	int fd = sceIoOpen(buf, PSP_O_CREAT|PSP_O_WRONLY, 0777);
	unsigned scrbuf[512];
	if (fd>0)
	{
		struct TGAHeader
		{
			uint16_t pad0;
			uint8_t mode;
			uint32_t pad1;
			uint8_t pad2;
			uint16_t originx;
			uint16_t originy;
			uint16_t width;
			uint16_t height;
			uint8_t bpp;
			uint8_t desc;
		} __attribute__((packed));
		TGAHeader header;
		memset(&header, 0x00, sizeof(header));
		header.mode = 0x02;
		header.width = 480; //mainRenderTarget.width;
		header.height = 272; //mainRenderTarget.height/2;
		uint32_t bpp = 3; //mainRenderTarget.format == GU_PSM_8888 ? 3 : 2;
		uint32_t scrbpp = 4; //mainRenderTarget.format == GU_PSM_8888 ? 4 : 2;
		header.bpp = bpp * 8;
		header.desc = 0x20; // write file with reveresed y set
		if (false)		// saving alpha?
		{
			bpp = scrbpp;
			header.desc |= 8; // hardcoded 8 bits alpha (RGBA32)
		}
		sceIoWrite(fd, &header, sizeof(header));
		for (unsigned y = 0; y < header.height; ++y)
		{
			memcpy(scrbuf, screen, sizeof(scrbuf));
			if (header.bpp != scrbpp)			// pack 4 guns to 3 (in-place); discarding alpha
			{
				unsigned char* bytes = (unsigned char*)scrbuf;
				for( int x = 0; x < header.width; ++x)
				{
					bytes[x*3+2] = bytes[x*4+0];
					bytes[x*3+1] = bytes[x*4+1];
					bytes[x*3+0] = bytes[x*4+2];
				}
			}
			screen = (void*)((unsigned)screen + 512/*mainRenderTarget.stride*/ * scrbpp);
			sceIoWrite(fd, scrbuf, header.width * bpp);
		}
		sceIoClose(fd);
	}
}
