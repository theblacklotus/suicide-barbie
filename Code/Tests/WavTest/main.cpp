#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <stdio.h>

PSP_MODULE_INFO("WavTest", 0, 1, 1);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int setupCallbacks(void);

void streamWaveFile(char *file);
void streamWaveNudge(int offset);

int main(void)
{
	pspDebugScreenInit();
	SceUID thid = setupCallbacks();
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceUID thid1 = pspSdkLoadStartModule("flash0:/kd/audiocodec.prx", PSP_MEMORY_PARTITION_KERNEL);
	SceUID thid2 = pspSdkLoadStartModule("flash0:/kd/libatrac3plus.prx", PSP_MEMORY_PARTITION_USER);
	if (thid1 < 0 || thid2 < 0)
	{
		printf("unable to load kernel modules\n");
		printf("thid1 = %x\n", thid1);
		printf("thid2 = %x\n", thid2);
	}

	streamWaveFile("host1:/dumpa_mig.wav");

	while(1)
	{
		const int zones[6] = {30, 70, 100, 112, 125, 130};
		const float response[6] = {0.0f, 0.1f, 0.5f, 1.0f, 4.0f, 8.0f};
		SceCtrlData pad;
		float direction;
		int v;
		float frequency =0;

		sceCtrlReadBufferPositive(&pad, 1);

		v = pad.Lx - 128;
		if (v < 0)
		{
			direction = -1.0f;
			v = -v;
		}
		else
		{
			direction = 1.0f;
		}

		for (int i = 0; i < 6; i++)
		{
			if (v < zones[i])
			{
				frequency = response[i] * direction;
				break;
			}
		}

		int pos = (int)(frequency*20*1024);
		streamWaveNudge(pos);
	}

	sceKernelWaitThreadEnd(thid, 0);

	return 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int exitCallback(int arg1, int arg2, void *common) {
	sceKernelExitGame();
	return 0;
}

int callbackThread(SceSize args, void *argp) {
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();

	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

