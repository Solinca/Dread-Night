#include "IA/Tasks/AITask_Base.h"

bool UAITask_Base::HasFailed() const
{
	return bHasFailed;
}

void UAITask_Base::FinishTask()
{
	EndTask();
	Clean();
}

void UAITask_Base::Restart()
{
	Clean();
	Activate();
}

void UAITask_Base::FinishFailedTask()
{
	bHasFailed = true;
	
	EndTask();
	Clean();
}

void UAITask_Base::ExternalCancel()
{
	Super::ExternalCancel();

	// Ensure resources are cleaned up on external cancellation.
	Clean();
}
