// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask.h"
#include "AITask_Base.generated.h"

/**
 * @brief Macro to log a message, finish the task with a failed result and return from the function.
 * @param Condition The condition to check for failure.
 * @param LogCondition The condition under which the log message should be triggered.
 * @param ReturnValue The value to return from the function.
 * @param CategoryName The log category.
 * @param Verbosity The log verbosity level.
 * @param Format The log message format.
 */
#define UE_CLOG_FAIL_TASK(Condition, LogCondition, ReturnValue, CategoryName, Verbosity, Format, ...)\
if (Condition)                                                                          \
{                                                                                       \
	UE_CLOG(LogCondition, CategoryName, Verbosity, Format, ##__VA_ARGS__)               \
	FinishFailedTask();																	\
	return ReturnValue;                                                                 \
}

/**
 * @class UAITask_Base
 * @brief Abstract base class for custom AI tasks, providing failure tracking and cleanup functionalities.
 */
UCLASS(Abstract)
class DREADNIGHT_API UAITask_Base : public UAITask
{
	GENERATED_BODY()

protected:
	// Flag to indicate if the task has failed.
	bool bHasFailed;
	
public:
	/**
	 * @brief Called when the task is externally canceled. Ensures cleanup is performed.
	 */
	virtual void ExternalCancel() override;
	
	/**
	 * @brief Checks if the task has failed.
	 * @return True if the task has failed, false otherwise.
	 */
	bool HasFailed() const;

	virtual void Restart();

protected:
	
	/**
	 * @brief Virtual method for cleaning up resources. To be overridden by child classes.
	 */
	virtual void Clean() {}
	
	/**
	 * @brief Finishes the task successfully.
	 */
	virtual void FinishTask();

	/**
	 * @brief Finishes the task with a failure state.
	 */
	virtual void FinishFailedTask();
};