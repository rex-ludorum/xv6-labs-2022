struct run {
  struct run *next;
};

struct kmemstruct {
  struct spinlock lock;
  struct run *freelist;
};
