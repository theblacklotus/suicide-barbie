#include <pspkernel.h>

PSP_MODULE_INFO("PSPSTUBTEST", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp)
{
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
    if (thid >= 0)
	sceKernelStartThread(thid, 0, 0);
    return thid;
}


int main(void)
{
	SetupCallbacks();
    sceKernelExitGame();
    sceKernelSleepThread();
	return 0;
}
