#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

PSP_MODULE_INFO("HelloWorld", 0, 1, 0);
// first 0 indicates user mode
// 1 0 indicates version 1.0
// following line is also for user mode:
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void* common) {
    sceKernelExitGame();
    return 0;
}

int callbackThread(SceSize args, void *argp){
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setupCallbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, NULL);
    if(thid >= 0){
        sceKernelStartThread(thid, 0, 0);
    }
    return thid;
}

int main() {
    // Use functions to make sure the user can exit the program
    setupCallbacks();
    pspDebugScreenInit();
    while(1) {
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenPrintf("\n\nHello there!");
        sceDisplayWaitVblankStart();
    }
    return(0);
}