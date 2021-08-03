#include "rtos.h"

struct generic_sync xdata sem;
struct generic_sync xdata mut;

struct {
	char ten;
	
} tests = { 0 };

void task_A()
{
	while(1) {
		k_acquire(&mut);
		k_yield();
		if(tests.ten == 10) {
			k_release(&mut);
			break;
		} else {
			tests.ten += 1;
			k_release(&mut);	
		}
		k_yield();
	}
	k_suspend(TASK_NULL);
	for(;;);
}

void task_B() {
	while(1) {
		k_acquire(&mut);	
		k_yield();
		if(tests.ten == 10) {
			k_release(&mut);
			break;
		} else {
			tests.ten += 1;
			k_release(&mut);	
		}
		k_yield();
	}
	
	k_waiton(&sem);
	for(;;);
}

void task_C() {
	k_signalto(&sem);
	for(;;) {
		k_yield();	
	}
}

void task_D() {
	k_acquire(&mut);
	k_yield();
	k_release(&mut);
	k_suspend(TASK_NULL);
	for(;;);
}

void k_user_trap(const char* cause)
{
	while(cause);
}

void main()
{
	k_create_semaphore(&sem);
	k_create_mutex(&mut);

	k_task_create(task_A, "a", 1);
	k_task_create(task_B, "b", 1);
	k_task_create(task_C, "c", 0);
	k_task_create(task_D, "d", 1);

	k_start();
}