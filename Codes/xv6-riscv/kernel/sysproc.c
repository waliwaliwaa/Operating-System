#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64 sys_sysinfo(void)
{
    uint64 addr;
    argaddr(0, &addr);
    // if (argaddr(0, &addr) < 0)
    // {
    //     return -1;
    // }
    struct sysinfo info;
    struct proc *p=myproc();
    info.freemem=get_freemem();
    info.nproc=get_nproc();
 
    if(copyout(p->pagetable, addr, (char*)&info, sizeof(info))) {
    return -1;
  }
  return 0;
}

int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
    uint64 va,dst;
    int n;
    argint(1, &n);
    argaddr(0, &va);
    argaddr(2, &dst);
    if(n > 64 || n < 0)
        return -1;
    uint64 bitmask = 0,mask = 1;
    pte_t *pte;
    pagetable_t pagetable = myproc()->pagetable;
    while(n > 0){
        pte = walk(pagetable,va,1);
        if(pte){
            if(*pte & PTE_A)
                bitmask |= mask;
            *pte = *pte & (~PTE_A);
        }
        mask <<= 1;
        va = (uint64)((char*)(va)+PGSIZE);
        n--;
    }
    if(copyout(pagetable,dst,(char *)&bitmask,sizeof(bitmask)) < 0)
        return -1;
    return 0;
}
