/*
 * Copyright (C) 2012 Tomasz Rostanski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_NIDEBUG 0
#define LOG_TAG "test"
#include <utils/Log.h>

#include "CameraHardwareInterface.h"

#include <utils/Errors.h>
#include <utils/threads.h>
#include <binder/MemoryHeapPmem.h>
#include <utils/String16.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cutils/properties.h>
#include <math.h>
#include <linux/android_pmem.h>
#include <linux/ioctl.h>
#include <camera/CameraParameters.h>
#include <media/mediarecorder.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <utils/threads.h>
#include <stdint.h>
#include <ui/OverlayHtc.h>

#include "linux/msm_mdp.h"
#include <linux/fb.h>

extern "C" {
#include <linux/android_pmem.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/system_properties.h>
#include <sys/time.h>
#include <stdlib.h>
}

extern "C" android::sp<android::CameraHardwareInterface> openCameraHardware();

static void wrap_set_fd_hook(void *data, int fd)
{
	LOGI("%s+++: data %p, fd %d", __FUNCTION__, data, fd);
}

static void wrap_set_crop_hook(void *data,
                               uint32_t x, uint32_t y,
                               uint32_t w, uint32_t h)
{
	LOGI("%s+++: data %p", __FUNCTION__,data);
	LOGI("%s---: %i %i %i %i", __FUNCTION__, x, y, w, h);
}

static void wrap_queue_buffer_hook(void *data, void* buffer)
{
	LOGI("%s+++: dat %p, buffer %p", __FUNCTION__,data, buffer);
}

static void print_menu()
{
	printf("a. getPreviewHeap\n");
	printf("b. getRawHeap\n");
	printf("c. enableMsgType\n");
	printf("d. disableMsgType\n");
	printf("e. msgTypeEnabled\n");
	printf("f. startPreview\n");
	printf("g. useOverlay\n");
	printf("h. setOverlay\n");
	printf("i. stopPreview\n");
	printf("j. previewEnabled\n");
	printf("k. startRecording\n");
	printf("l. stopRecording()\n");
	printf("m. recordingEnabled\n");
	printf("n. releaseRecordingFrame\n");
	printf("o. autoFocus\n");
	printf("p. cancelAutoFocus\n");
	printf("q. takePicture\n");
	printf("r. cancelPicture\n");
	printf("s. setParameters\n");
	printf("t. getParameters\n");
	printf("u. sendCommand\n");
	printf("v. release\n");
#ifdef USE_GETBUFFERINFO
	printf("x. getBufferInfo\n");
#endif
#ifdef USE_ENCODEDATA
	printf("y. encodeData\n");
#endif
	printf("z. exit\n");
}

int main(int argc, char **argv)
{
	android::sp<android::CameraHardwareInterface> camera;
	android::CameraParameters params;
	android::sp<android::IMemory> mem;
	android::sp<android::IMemoryHeap> memHeap;
	size_t alignedSize = 0;
	struct timespec tp;
	int ret;

	printf("Camera Tester\n");

	android::Overlay *overlay =  new android::Overlay(wrap_set_fd_hook,
		wrap_set_crop_hook,
		wrap_queue_buffer_hook,
		NULL);

	printf("Opening camera...\n");
	camera = openCameraHardware();
	if(camera == NULL)
	{
		printf("Cannot open Camera\n");
		return -1;
	}
  
	char option = 0;
	do
	{
		print_menu();
		printf("Choose option: ");
		option = getchar();
		printf("\n");

		if (option != 'z') printf("*** Executing...\n");
		ret = 1024;
		clock_gettime(CLOCK_MONOTONIC, &tp);
		switch (option)
		{
			case 'a':
				memHeap = camera->getPreviewHeap();
				break;
			case 'b':
				memHeap = camera->getRawHeap();
				break;
			case 'c':
				camera->enableMsgType(0x200);
				break;
			case 'd':
				camera->disableMsgType(0x200);
				break;
			case 'e':
				ret = camera->msgTypeEnabled(0x200);
				break;
			case 'f':
				ret = camera->startPreview();
				break;
			case 'g':
				ret = camera->useOverlay();
				break;
			case 'h':
				ret = camera->setOverlay(overlay);
				break;
			case 'i':
				camera->stopPreview();
				break;
			case 'j':
				ret = camera->previewEnabled();
				break;
			case 'k':
				ret = camera->startRecording();
				break;
			case 'l':
				camera->stopRecording();
				break;
			case 'm':
				ret = camera->recordingEnabled();
				break;
			case 'n':
				camera->releaseRecordingFrame(mem);
				break;
			case 'o':
				ret = camera->autoFocus();
				break;
			case 'p':
				ret = camera->cancelAutoFocus();
				break;
			case 'q':
				ret = camera->takePicture();
				break;
			case 'r':
				ret = camera->cancelPicture();
				break;
			case 's':
				ret = camera->setParameters(params);
				break;
			case 't':
				params = camera->getParameters();
				params.dump();
				break;
			case 'u':
				ret = camera->sendCommand(0, 1, 2);
				break;
			case 'v':
				camera->release();
				break;
#ifdef USE_GETBUFFERINFO
			case 'x':
				ret = camera->getBufferInfo(mem, &alignedSize);
				break;
#endif
#ifdef USE_ENCODEDATA
			case 'y':
				camera->encodeData();
				break;
#endif
			case 'z':
				break;
		}
		if (ret != 1024) 
			printf("*** Function returned: %d\n", ret); 
		else 
			printf("*** Function does not return value\n");
		printf("*** Command executed at: [%5lu.%06lu]\n\n", tp.tv_sec, tp.tv_nsec);
	}
	while (option != 'z');

	printf("Exiting\n");
	camera->release();

	return 0;
}
