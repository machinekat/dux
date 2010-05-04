#include <system.h>

#include <metodo/metodo.h>
#include <metodo/stop.h>

#include <metodo/hal/mm/memory.h>
#include <metodo/tests.h>

#include <multiboot.h>

#include <buildid.h>
#include <string.h>

#include <metodo/bochs.h>

#include <metodo/colpa/debug.h>

void InInitKernel(uint32_t magic, multiboot_info_t *multiboot)
{
	void *userland = NULL;

	*mbd = *multiboot;

	HalInit();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		KrnlStop(STOP_BAD_MULTIBOOT_SIGNATURE, magic, 0, 0, 0);
	}

	/* mbd->flags */
	int i;
	module_t *module;
	if (mbd->flags>>3&1) {
		module = (module_t*)mbd->mods_addr;
		printf("We have %i modules.\n", mbd->mods_count);
		for (i = 0; i < mbd->mods_count; i++, module++) {
			printf("\nFound module.\n");
			printf("Module located at 0x%x-0x%x\n", module->mod_start, module->mod_end);
			printf("Module name: %s\n", (char*)module->string);
			if (strcmp((char*)(module->string), "/System/userland.exe") == 0){
				userland = (void*) module->mod_start;
				printf("\nUserland located at: 0x%x\n\n", userland);
			}
		}
	}

	printf("userland: 0x%x\n", userland);
	SystemTests();

	/* Initialize pseudo-user mode */
	if (userland != NULL) {
		printf("Loading userland...\n");
		//HalSwitchToUserMode();
		LoadUserland(userland);
		printf("\nWhy yes, that is a black hole that flew out of userland...\n(Userland exited unexpectedly)\n");
	} else {
		printf("No userland\n");
		HalBreak();
		//KernDebug();
		//KrnlEasyStop(STOP_NO_USERLAND);
	}
	while(1);
}
