#include "rtos.h"
#include <reg51.h>
#include <intrins.h>
#include <string.h>

#define _s(s) #s

/** @author Vinicius Kursancew 
 * @addtogroup rtos 
 * @{
 * @publicsection
 * @defgroup internal Documentation of data and functions that are used only within rtos.c
 */

//Possible task states
//!@ingroup internal
#define READY 0	
//!@ingroup internal
#define BLOCKED 1
//!@ingroup internal
#define SUSPENDED 2
//!@ingroup internal
#define SUSPENDED_BLOCKED 3

/** Holds which tasks are ready in an array of bitmasks, each bit representing a task
@ingroup internal
*/
PRIORITY_BITMASK_t(ready_masks);
/**
Holds info of which task on each priority level was run the last time.
@ingroup internal
*/
PRIORITY_BITMASK_t(rr_masks);

/** Structure that holds information about a task.
It is statically defined on the array 'tasks' and cannot hold more tasks than defined
on MAX_TASK_COUNT macro.
@ingroup internal
*/
struct task {
	byte_t stack_copy[MAX_STACK_SIZE]; //!< Copy of the stack for the task
	char name[MAX_NAME+1]; //!< A name representing the task
	byte_t prio; //!< Priority, 0=lowest, highest is defined on the MAX_PRIORITIES macro
	byte_t state; //!< Current state of the task, may be: READY, BLOCKED, SUSPENDED or SUSPENDED_BLOCKED
	byte_t sp; //!< Count-1 of bytes that were on the stack when context switched (SP-STACK_START)
};

/**
Definition of the tasks structure. The index of this array is the corresponding's task id.
@ingroup internal
*/
XRAM_t(struct task, tasks)[MAX_TASK_COUNT];

/** Pointer to currently running task. It is stored on the internal ram so access is faster.
@ingroup internal
*/
IDATA_TO_XRAM_PTR_t(struct task, current_task);

/** Index of the currently running task on the tasks[] array
@ingroup internal
*/
IDATA_t(byte_t, current_task_index) = 0;

/** Count of tasks created by calling k_task_create.
@ingroup internal
*/
IDATA_t(byte_t, task_count) = 0;

/** Byte used for saving and restoring EA on calling CBEGIN and CEND macros
@ingroup internal
*/
volatile IDATA_t(byte_t, ea_save) = 0;

/** Defined in low_level.a51, assembly routine to save R0-R7 on the stack
@ingroup internal
*/
extern void push_bank();

/** Defined in low_level.a51, assembly routine to restore R0-R7 from the stack
@ingroup internal
*/
extern void pop_bank();

/** Defined in low_level.a51, assembly routine to allow calling RETI from C code.
This is a hack for Keil C51 because defining a function as interrupt on
Keil compiler unavoidably inserts a preamble on the ISR which
manipulates the stack, but in this case (conext-switch) the OS is
manipulating the stack.
@ingroup internal
*/
extern void returni();

/** Save context to stack.
@ingroup internal
*/
#define SAVE 		\
	_push_(ACC);	\
	_push_(IE);		\
	_push_(B);	   	\
	_push_(DPL);	\
	_push_(DPH);	\
	push_bank();	\
	_push_(PSW);

/** Restores context.
@ingroup internal
*/
#define RESTORE	\
	_pop_(PSW);	\
	pop_bank();	\
	_pop_(DPH);	\
	_pop_(DPL);	\
	_pop_(B);	\
	_pop_(IE);	\
	_pop_(ACC);

/** Writes bytes in the range STACK_START -> SP to the external ram
into the current task stack context.
@ingroup internal
*/
#define TO_XRAM { \
	byte_t idata * idata ram = (byte_t idata *)STACK_START; \
	IDATA_TO_XRAM_PTR_t(byte_t,  xram) = current_task->stack_copy;	\
	current_task->sp = SP - STACK_START; \
	while((byte_t)ram <= SP) *(xram++) = *(ram++); }

/** Writes bytes from the current task saved context back to the stack
region in the IDATA ram.
@ingroup internal
*/				
#define TO_STACK { \
	byte_t idata * idata ram = (byte_t idata *)STACK_START; \
	IDATA_TO_XRAM_PTR_t(byte_t,  xram) = current_task->stack_copy;	\
	SP = STACK_START + current_task->sp; \
	while((byte_t)ram <= SP) *(ram++) = *(xram++); }

/**
Macro used to mark a task as ready, parameter is the task index from the tasks[] array
@ingroup internal
*/
#define TASK_SET_READY_MASK(index) ready_masks[tasks[(index)].prio] |= 1<<((index));

/** Macro used to mark a task as not ready. Parameter is the task index from the tasks[] array
After this the task cannod be scheduled to run until TASK_SET_READY_MASK is called again on it
@ingroup internal
*/
#define TASK_CLR_READY_MASK(index) ready_masks[tasks[(index)].prio] &= ~(1<<((index)));

/** Finds out which is the next task to run.
This is an internal function for the kernel.
@ingroup internal
*/
static void schedule()
{
	register unsigned char i;
	register unsigned char j;
	for(i = MAX_PRIORITIES; i >= 0; --i) {
		//Looks up which priority level is the highest to have some task ready
		if(ready_masks[i]) {
			do {
				//Loops in the bitmask to find which is the next task
				rr_masks[i] >>= 1;
				if(!rr_masks[i]) rr_masks[i] |= 1<<(task_count-1);
			} while(!(ready_masks[i] & rr_masks[i]));
			break;	
		}
	}
	current_task_index = 0;
	//with the mask we need to map back to the task number
	j = rr_masks[i];
	while(j >>= 1) {
		++current_task_index;
	}
	current_task = tasks + current_task_index;	
}

#if !defined(noSEMAPHORE) || !defined(noMUTEX)
/** Wakes up a task that is blocked putting it in the correct state.
Also properly calling TASK_SET_READY_MASK.
This is an internal function for the kernel.
@param t task to wake up
@ingroup internal
*/
static void wake_task(byte_t t)
{
	switch(tasks[t].state) {
		case SUSPENDED:
		case SUSPENDED_BLOCKED:
			tasks[t].state = SUSPENDED;
			break;
		default:
			tasks[t].state = READY;
			TASK_SET_READY_MASK(t);
	}
}
#endif

/** Creates a task. The task is marked as ready to run.
@param fun function with prototype void fun(void) when control is passed to the task
@param name a string that holds a name that represents the task
@param prio priority of the task, 0 being the lowest, macro MAX_PRIORITIES defines the maximum
@return task number
*/
char k_task_create( voidf fun, const char* name, byte_t prio )
{
	XRAM_PTR_t(struct task, mtask = &tasks[task_count]);
	IDATA_t(unsigned short, fun_addr) = (unsigned short)fun; //casts the function address to a ushort so it can be assigned to the stack
	IDATA_TO_XRAM_PTR_t(byte_t, stk) = XRAM_PTR_cast(byte_t, mtask);

	mtask->prio = prio;

	//copies task name to the task structure
	strncpy(mtask->name, name, MAX_NAME);
	//mtask->prio = prio;
	mtask->state = READY;

	*stk = fun_addr&0xff; //Low-byte first on the stack
	++stk;
	*stk = (fun_addr>>8)&0xff; //High-byte of return address
	++stk;
	*stk = 1; //A
	++stk;
	ET0 = 1; 
	*stk = IE;
	++stk;
	for(; stk != XRAM_PTR_cast(byte_t, mtask)+15; ++stk)
		*stk = 0xaa; //DPTR, B and R0-R7

	*stk = PSW; //IE
	
	//16 bytes initially on stack(SP points to the last item, instead of next, so its 1 less than 16)
	//RETURN(2)+ACC(1)+IE(1)+B(1)+DPTR(2)+R0-7(8)+PSW(1)
	mtask->sp = 15;
	return task_count++;
}

/** Starts the scheduler and never returns.
It is very important to notice that the user must provide the IDLE task.
The idle task is a task that is always ready to execute, a simple definition would be:

@code
void idle()
{
	for(;;) k_yield();
}
@endcode

and then, before calling k_start():
@code
k_create_task(idle, "idl", 0);
@endcode

*/
void k_start()
{
	char i;
	EA = 0;

	//Initializing the masks that represent ready tasks
	for(i = 0; i < task_count; ++i) {
		ready_masks[tasks[i].prio] |= 1 << i;
	}
	//An array holds information of which task is next on each priority level
	for(i = 0; i < MAX_PRIORITIES; ++i) {
		rr_masks[i] = 1;
	}
	SETUP_TIMER;

	current_task_index = 0;
	current_task = tasks;

	//Now we go!
	schedule();
    TO_STACK
	RESTORE
	EA = 1;
	return; //returning here will go to the beginning of the task selected by schedule()
}

/** Causes a context switch.
*/
void k_yield()
{
	EA = 0;
	SAVE
	TO_XRAM
	schedule();
	TO_STACK
	RESTORE
	EA = 1;
}

/** Interrupt for timer0, in case the system is using preemption
@ingroup internal
*/
void timer_isr()
{
	EA = 0;
	TR0 = 0;
	TL0 = 0xb0;
	TH0 = 0x3c;
	TR0 = 1;
#ifndef noPREEMPTION
	SAVE
	TO_XRAM
	schedule();
	TO_STACK
	RESTORE
#endif
	TF0 = 0;
	EA = 1;
	returni();
	EA = 1; //this avoids compiler optimization to LJMP
}


#ifndef noSUSPEND
/** Suspends current task, it will only be ready again if k_resume is called on the task
@param t task number that should be suspended, passing TASK_NULL will suspend currently running task
*/
void k_suspend(byte_t t)
{
	CBEGIN
	if(t == TASK_NULL)
		t = current_task_index;
	tasks[t].state = SUSPENDED;
	TASK_CLR_READY_MASK(t);
	if(t == current_task_index) {
		CEND
		k_yield();
	} else {
		CEND
	}

}

/** Resumes execution of a task, making it ready to run iff its not BLOCKED
@param t task to resume
*/
void k_resume(byte_t t)
{
	CBEGIN
	switch(tasks[t].state) {
		case SUSPENDED_BLOCKED:
		case BLOCKED:
			tasks[t].state = BLOCKED;
			break;
		default:
			tasks[t].state = READY;
			TASK_SET_READY_MASK(t);
	}
	CEND
}
#endif

#ifndef noSEMAPHORE
/** Initializes a semaphore. It is initially unavailable.
@param sem pointer to a generic_sync structure that will hold the semaphore
*/
void k_create_semaphore(XRAM_PTR_t(struct generic_sync, sem))
{
	sem->blocked = 1;
	sem->wake_me.single = 0xFF;
}

/** Waits on a semaphopre until it becomes available. Returns immediately if it's already available.
@param sem pointer to semaphore to waiton
*/
void k_waiton(XRAM_PTR_t(struct generic_sync, sem))
{
	CBEGIN
	if(sem->blocked) {
		sem->wake_me.single = current_task_index;
		current_task->state = BLOCKED;
		TASK_CLR_READY_MASK(current_task_index);
		CEND
		k_yield();
	} else {
		sem->wake_me.single = 0xFF;
		sem->blocked = 1;
		CEND
	}
}

/** Signals a semaphore and wakes the task sleeping on it (if any).
@param sem pointer to semaphore to signal
*/
void k_signalto(XRAM_PTR_t(struct generic_sync, sem))
{
	CBEGIN
	// is a semaphore is blocked and someone tried to waiton we should wake
	if(sem->blocked && (sem->wake_me.single != 0xFF) ) {
		wake_task(sem->wake_me.single);
		sem->wake_me.single = 0xFF;		
	} else {
		sem->blocked = 0;
	}
	CEND
}
#endif

#ifndef noMUTEX
/** Initializes a mutex object.
@param mut pointer to generic_sync structure that will represent the mutex
*/
void k_create_mutex(XRAM_PTR_t(struct generic_sync, mut))
{
	for(mut->blocked = 0; mut->blocked < MAX_MUTEX_SHARE; ++mut->blocked)
		mut->wake_me.array[mut->blocked] = 0xFF;
	mut->blocked = 0;
}

/** Tryes to acquire a mutex. If the mutex is already acquired, blocks until it is released.
@param mut pointer to mutex to try to acquire
*/
void k_acquire(XRAM_PTR_t(struct generic_sync, mut))
{
	register byte_t i;
	CBEGIN
	while(mut->blocked) {
		i = 0;
		while(mut->wake_me.array[i] != 0xFF) {
			++i;
		}
		mut->wake_me.array[i] = current_task_index;
		current_task->state = BLOCKED;
		TASK_CLR_READY_MASK(current_task_index);
		CEND
		k_yield();
		CBEGIN
	}
	mut->blocked = 1;
	CEND
}

/** Releases an acquired mutex and wake tasks pending on it.
Notice that releasing an un-acquired mutex, or a mutex acquired by another task is illegal.
@param mut pointer to mutex to be release
*/
void k_release(XRAM_PTR_t(struct generic_sync, mut))
{
	register byte_t i;
	CBEGIN
#ifndef noTRAP
	if(mut->blocked == 0) {
		//BUG:
		//RELEASING UNAQUIRED MUTEX IS ILLEGAL
		k_user_trap(__FILE__","_s(__LINE__)": unaquired mutex");
	}
#endif
	for(i = 0; i < MAX_MUTEX_SHARE; ++i) {
		//wake up
		if(mut->wake_me.array[i] != 0xFF) {
			wake_task(mut->wake_me.array[i]);
			mut->wake_me.array[i] = 0xFF;
		}
	}
	mut->blocked = 0; //restore attribute value
	CEND
}
/** @} */
#endif
