#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;

  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void)
{
//grab asses arg
  char *arg;
  char *stack;
  char *fcn;
  if(argptr(0, &fcn, 4) < 0 || argptr(1, &arg, 4) < 0 || argptr(2, &stack, 4) < 0)
    return -1;

  if((uint)stack % PGSIZE != 0 || (uint)stack+PGSIZE > proc->sz) 
    return -1;

  //cprintf("[sys_clone] fcn : 0x%x | arg: 0x%x | stack: 0x%x\n", fcn, arg, stack);
  int pid = clone((void *)fcn,(void *) arg,(void *) stack);
  //cprintf("SYS_CLONE-PID-%d\n", pid);
  return pid;
}

int
sys_join(void)
{
  int pid;
  if(argint(0, &pid) < 0)
    return -1;

  return join(pid);
}

int
sys_cv_sleep(void)
{
  char *cv;
  char *lock;
  argptr(0, &cv, 4);
  argptr(1, &lock, 4);


  return cv_sleep((cond_t *) cv, (lock_t *) lock);
}

int
sys_cv_wake(void)
{
  char *cv;
  argptr(0, &cv, 4);

  return cv_wake((cond_t *) cv);
}