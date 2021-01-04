#include <pspkernel.h>
#include <pspaudio.h>
#include <cstdio>
#include <cstring>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define INPUTBUF_SIZE	(16*1024)
#define BUF_COUNT		(4)
static char inputBuf[BUF_COUNT][INPUTBUF_SIZE] __attribute__((aligned(64)));

int wav_streamer(SceSize args, void *argp);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

static SceUID playth;

void streamWaveFile(const char *file)
{
	playth = sceKernelCreateThread("wav_streamer", wav_streamer, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);

	if (playth < 0)
	{
		printf("Error creating play_thread.\n");
		return;
	}

	sceKernelStartThread(playth, strlen(file)+1, (void*)file);
}

static volatile int nudgeOffset = 0;
static volatile int isPaused = 0;

void streamWavePause(int pause)
{
	if (isPaused != pause && !pause)
	{
		sceKernelWakeupThread(playth);
	}
	isPaused = pause;
}


void streamWaveNudge(int offset)
{
	nudgeOffset = offset;			// possible race condition :)
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//#define printf	pspDebugScreenPrintf

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

enum
{
	RIFF_ID	= 'FFIR',	// RIFF
	WAVE_ID	= 'EVAW',	// WAVE
	FMT_ID	= ' tmf',	// fmt<space>
	DATA_ID	= 'atad'	// data
};

typedef struct RIFFHeaderTag
{
	u32	riffId;
	u32	size;
	u32	waveId;
} RIFFHeader;

typedef struct FMTHeaderTag
{
	u32	fmtId;
	u32	chunkSize;
	u16	format;
	u16	numChannels;
	u32	samplesPerSec;
	u32	avgBytesPerSec;
	u16	blockAlign;
	u16	bitsPerSample;
} FMTHeader;

typedef struct DATAHeaderTag
{
	u32	dataId;
	u32	chunkSize;
} DATAHeader;

typedef struct
{
	u16 bytesPerSample;
	u16 numChannels;
	u32 bytesPerBlock;
	u32 frequency;
	u32 dataOffset;
	u32 dataLength;
} WaveInfo;

static bool seekChunk(int fd, int size, u32 id)
{

	typedef struct
	{
		u32 id;
		u32 size;
	} ChunkHeader;
	while (size > 0)
	{
		ChunkHeader header;
		sceIoRead(fd, &header, sizeof(header));
		if (header.id == id)
		{
			int offset = -sizeof(header);
			sceIoLseek(fd, offset, PSP_SEEK_CUR);
			return true;
		}

		int offset = sceIoLseek(fd, header.size, PSP_SEEK_CUR);
		if (offset >= size)
			break;
	}

	return false;
}

int wav_streamer(SceSize args, void *argp)
{

	int channel = sceAudioChReserve(0, 1024, PSP_AUDIO_FORMAT_STEREO);

	int fd = sceIoOpen((char*)argp, PSP_O_RDONLY, 0777);
	
	if (fd < 0)
	{
		printf("Error opening file %s\n",(char*)argp);
		return -1;
	}

	RIFFHeader riffHeader;
	sceIoRead(fd, &riffHeader, sizeof(riffHeader));
	if (riffHeader.riffId != RIFF_ID || riffHeader.waveId != WAVE_ID)
	{
		printf("file not riff-wave %s\n", (char*) argp);
		sceIoClose(fd);
		return -1;
	}
	if (!seekChunk(fd, riffHeader.size, FMT_ID))
	{
		printf("unable to find fmt header %s\n", (char*) argp);
		sceIoClose(fd);
		return -1;
	}
	FMTHeader fmtHeader;
	sceIoRead(fd, &fmtHeader, sizeof(fmtHeader));
	if (fmtHeader.fmtId != FMT_ID || fmtHeader.format != 0x0001)
	{
		printf("file not correct format %s\n", (char*) argp);
		printf("format = 0x%x\n", fmtHeader.format);
		sceIoClose(fd);
		return -1;
	}
	if (!seekChunk(fd, riffHeader.size, DATA_ID))
	{
		printf("unable to find data header %s\n", (char*) argp);
		sceIoClose(fd);
		return -1;
	}
	DATAHeader dataHeader;
	sceIoRead(fd, &dataHeader, sizeof(dataHeader));
	if (dataHeader.dataId != DATA_ID)
	{
		printf("could not find 'data'-tag %s\n", (char*) argp);
		sceIoClose(fd);
		return -1;
	}

	WaveInfo info;
	info.bytesPerSample = fmtHeader.bitsPerSample / 8;
	info.numChannels = fmtHeader.numChannels;
	info.bytesPerBlock = info.bytesPerSample * fmtHeader.numChannels;
	info.frequency = fmtHeader.samplesPerSec;
	info.dataOffset = sceIoLseek(fd, 0, PSP_SEEK_CUR);
	info.dataLength = dataHeader.chunkSize;

	if (info.bytesPerSample != 16 /*bits*/ / 8 /*bits per byte*/|| 
		info.numChannels != 2 /*channels stereo*/ ||
		info.bytesPerBlock != 16 /*bits*/ * 2 /*channels stereo*/ / 8 /*bits per byte*/ ||
		info.frequency != 44100 /*Hz*/)
	{
		printf("file needs to be 16 bits stereo 44.1 kHz\n");
		printf("bytesPerSample = %i\n", info.bytesPerSample);
		printf("numChannels = %i\n", info.numChannels);
		printf("bytesPerBlock = %i\n", info.bytesPerBlock);
		printf("frequency = %i\n", info.frequency);
		printf("dataOffset = %i\n", info.dataOffset);
		printf("dataLength = %i\n", info.dataLength);
		return -1;
	}

	sceAudioSetChannelDataLen(channel, 1024);
	int size = info.dataLength;
	sceIoRead(fd, inputBuf[0], INPUTBUF_SIZE);
	int buf = 0;
	SceInt64 result;
	int offset = 0;
	do
	{
		int nextbuf = (buf+1) & (BUF_COUNT-1);
		while(sceIoReadAsync(fd, inputBuf[nextbuf], INPUTBUF_SIZE))
			;

		for(int i = 0; i < INPUTBUF_SIZE; i+=1024*4)
			sceAudioOutputBlocking(channel, 0x8000, &inputBuf[buf][i]);
		buf = nextbuf;
		while(sceIoWaitAsync(fd, &result))
			;

		offset = sceIoLseek(fd, 0, PSP_SEEK_CUR);
		if (nudgeOffset)
		{
			offset += nudgeOffset;
			nudgeOffset = 0;
			offset &= ~0x4;
			if (offset < 0)
				offset = 0;
			else if(offset > (s32)info.dataLength)
				offset = info.dataLength;
			offset = sceIoLseek(fd, info.dataOffset + offset, PSP_SEEK_SET);
			offset -= info.dataOffset;
		}

		pspDebugScreenSetXY(0,2);
//		printf("offset = 0x%x (%3.1f%%)\n", offset, 100.f * offset/size);
		if (isPaused)
			sceKernelSleepThread();
	}
	while(offset != size);
	sceIoClose(fd);

	return 0;
}
