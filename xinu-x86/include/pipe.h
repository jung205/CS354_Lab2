#define NPIPE 5
#define PIPE_BS 256
#define PIPE_AVAL 0
#define PIPE_USED 1
struct pipeent {
  char buffer[PIPE_BS];
  uint16 pipe_status;
  int32 curr_len;
  sid32 sem_id;
};

int32 pipeopen();
syscall pipeclose(int32 pipe);
syscall pipesend(int32 pipe, int32 len, char *c);
syscall piperecv(int32 pipe, int32 len, char *c);
syscall pipestate(int32 pipe, int32 *nc, int32 *ns);

extern struct pipeent pipetab[];
