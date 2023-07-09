// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem {
  struct spinlock lock;
  struct run *freelist;
};

struct kmem allkmems[NCPU];

void
kinit()
{
  for (int i = 0; i < NCPU; i++) {
    initlock(&allkmems[i].lock, "kmem");
  }
  char *p;
  p = (char*)PGROUNDUP((uint64)end);
  for(; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE) {
    struct run *r;

    if(((uint64)p % PGSIZE) != 0 || (char*)p < end || (uint64)p >= PHYSTOP)
      panic("kinit");

    int idx = ((uint64)p / PGSIZE) % NCPU;
    // Fill with junk to catch dangling refs.
    memset(p, 1, PGSIZE);

    r = (struct run*)p;

    r->next = allkmems[idx].freelist;
    allkmems[idx].freelist = r;
  }
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  push_off();
  int i = cpuid();
  acquire(&allkmems[i].lock);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE) {
    // Fill with junk to catch dangling refs.
    memset(p, 1, PGSIZE);

    struct run *r = (struct run*)p;

    r->next = allkmems[i].freelist;
    allkmems[i].freelist = r;
  }
  release(&allkmems[i].lock);
  pop_off();
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  push_off();
  int i = cpuid();
  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&allkmems[i].lock);
  r->next = allkmems[i].freelist;
  allkmems[i].freelist = r;
  release(&allkmems[i].lock);
  pop_off();
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  push_off();
  int i = cpuid();
  int j = i;

  acquire(&allkmems[i].lock);
  r = allkmems[i].freelist;
  while (!r && (j+1) % NCPU != i) {
    release(&allkmems[j].lock);
    j = (j+1) % NCPU;
    acquire(&allkmems[j].lock);
    r = allkmems[j].freelist;
  }
  if(r)
    allkmems[j].freelist = r->next;
  release(&allkmems[j].lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  pop_off();
  return (void*)r;
}
