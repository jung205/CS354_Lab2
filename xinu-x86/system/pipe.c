/* pipe.c - pipeopen, */

#include <xinu.h>
struct pipeent pipetab[NPIPE];

/*-------------------------------------
 * pipeopen - find pipe not in use, mark it as used, initialize pipe
 *-------------------------------------
 */
int32 pipeopen(void) {
	for (int i = 0; i < NPIPE; i++) {
		if (pipetab[i].pipe_status == PIPE_AVAL) {
			pipetab[i].pipe_status = PIPE_USED;
			strcpy(pipetab[i].buffer, "");
			pipetab[i].curr_len = 0;
			// Access semaphore; semtab[i]
			pipetab[i].sem_id = semcreate(1);
			return i;
		}
	}
	return SYSERR;
}

syscall pipeclose(
	 int32		pipe		 	/* ID of pipe to close */
	)
{
	intmask mask;				/* Saved interrupt mask */
	
	mask = disable();
	if (pipe < 0 || pipe >= NPIPE) {
		restore(mask);
		return SYSERR;
	}
	if (pipetab[pipe].pipe_status == PIPE_AVAL) {
		restore(mask);
		return SYSERR;
	}
	
	pipetab[pipe].pipe_status = PIPE_AVAL;
	strcpy(pipetab[pipe].buffer, "");	/* Discard buffer */
	pipetab[pipe].curr_len = 0;
	semdelete(pipetab[pipe].sem_id);

	restore(mask);
	return OK;
}

syscall pipesend(
	 int32		pipe,			/* ID to send to	*/
	 int32		len,			/* Message length	*/
	 char		*c			/* Message		*/
	)
{
	intmask mask;

	mask = disable();	
	if (pipe < 0 || pipe >= NPIPE) {
		restore(mask);
		return SYSERR;
	}
	if (pipetab[pipe].pipe_status == PIPE_AVAL) {
		restore(mask);
		return SYSERR;
	}

	wait(pipetab[pipe].sem_id);
	for (int i = 0; i < len; i++) {
		pipetab[pipe].buffer[i] = c[i];
	}
	//strcpy(pipetab[pipe].buffer, c);
	pipetab[pipe].curr_len += len;	
	signal(pipetab[pipe].sem_id);

	restore(mask);
	return OK;
}

syscall piperecv(
	 int32		pipe,			/* ID to send to	*/
	 int32		len,			/* Message length	*/
	 char		*c			/* Message		*/
	)
{
	intmask mask;

	mask = disable();	
	if (pipe < 0 || pipe >= NPIPE) {
		restore(mask);
		return SYSERR;
	}
	if (pipetab[pipe].pipe_status == PIPE_AVAL) {
		restore(mask);
		return SYSERR;
	}

	for (int i = 0; i < len; i++) {
		c[i] = pipetab[pipe].buffer[i];
	}
	//strcpy(c, pipetab[pipe].buffer);
	
	restore(mask);
	return OK;
}

syscall pipestate(
	 int32		pipe,			/* ID to send to	*/
	 int32		*nc,			/* Num	*/
	 int32		*ns			/* Message		*/
	)
{
	intmask mask;

	mask = disable();	
	if (pipe < 0 || pipe >= NPIPE) {
		restore(mask);
		return SYSERR;
	}
	if (pipetab[pipe].pipe_status == PIPE_AVAL) {
		restore(mask);
		return PIPE_AVAL;
	}

	nc = pipetab[pipe].curr_len;
	ns = PIPE_BS - pipetab[pipe].curr_len;
	
	restore(mask);
	return PIPE_USED;
}


void pipeprint(void) {
	kprintf("PIPE PRINTING\n");
	for (int i = 0; i < NPIPE; i++) {
		kprintf(pipetab[i].buffer);
		kprintf("\n");
		if (pipetab[i].pipe_status == PIPE_AVAL)
			kprintf("AVAILABLE\n");
		else
			kprintf("UNAVAILABLE\n");
	}
}
