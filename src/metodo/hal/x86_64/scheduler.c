#include <metodo/metodo.h>
#include <metodo/core/scheduler.h>

static int scheduler_enabled = 0;

void HalSchedulerRunProcess(SchedulerProcess *proc)
{
	printf("Switch process %i\n", CoSchedulerCurProcessId());
}

void HalSchedulerEnable()
{
	scheduler_enabled = 1;
}

void HalSchedulerDisable()
{
	scheduler_enabled = 0;
}

int HalIsSchedulerEnabled()
{
	return scheduler_enabled;
}
