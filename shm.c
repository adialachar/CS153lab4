#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "stdbool.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

//you write this


int i;
int idExists = 0;
	struct proc *curproc = myproc();
	
	uint a;
	a = PGROUNDUP(curproc->sz);
	acquire(&(shm_table.lock));
for (i = 0; i < 64; ++i){
	

	

	
	
	
	if(shm_table.shm_pages[i].id == id){
		//found it
		
		//1. find physical address of page
		if(mappages(curproc->pgdir,(char*)a,PGSIZE,V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U) < 0){
			
			cprintf("ERROR");
		}
		shm_table.shm_pages[i].refcnt = shm_table.shm_pages[i].refcnt + 1;
		*pointer = (char*)a;
		curproc->sz = a + PGSIZE;
		idExists = 1;
		break;
		
		
		}
	
			
			
	}
		
		
		
	if(!idExists){
		for (i = 0; i < 64; ++i){
			
			if(shm_table.shm_pages[i].id == 0){ //finds first empty 
				
				shm_table.shm_pages[i].id = id;
				shm_table.shm_pages[i].frame = kalloc();
				shm_table.shm_pages[i].refcnt = 1;
				memset(shm_table.shm_pages[i].frame,0,PGSIZE);
				
				if (mappages(curproc->pgdir, (char*)a, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U) < 0){
					cprintf("error");
				}
				*pointer=(char *)a;
				curproc->sz = a + PGSIZE;
				break;
				
			
			}
		}
	}




release(&(shm_table.lock));




return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {

//you write this too!
	int i;
	//loop through
  acquire(&(shm_table.lock));
	for(i=0; i<64; i++){
		if (shm_table.shm_pages[i].id == id){
			shm_table.shm_pages[i].refcnt = shm_table.shm_pages[i].refcnt - 1;  
			if (shm_table.shm_pages[i].refcnt == 0) { //if refcnt is 0
				//clears the shm_table
				shm_table.shm_pages[i].id = 0;
				shm_table.shm_pages[i].frame = 0;
			}
			break;
		}
	}
	release(&(shm_table.lock));









return 0; //added to remove compiler warning -- you should decide what to return
}
