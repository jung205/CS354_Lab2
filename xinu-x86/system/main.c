/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	int32 one = pipeopen();	
	int32 two = pipeopen();
	int32 three = pipeopen();

	pipeclose(two);
	
	pipesend(three, 6, "himanz");
	pipesend(one, 6, "supman");
	pipesend(three, 4, " ima");

	//pipeclose(0);

	char msg[3];
	piperecv(three, 3, msg);
	kprintf(msg);

	char msgTwo[5];
	piperecv(three, 5, msgTwo);
	kprintf("%s", msgTwo);
	
	char msgThree[2];
	piperecv(three, 2, msgThree);
	kprintf("%s\n", msgThree);
	kprintf("end receiving\n");

	pipeprint();

	int32 *nc;
	int32 *ns;
	int32 wow = pipestate(one, nc, ns);
	kprintf(wow);

	pipeclose(0);
	pipeclose(1);
	pipeclose(2);

	/* TESTING SEMAPHORES */
	kprintf("TESTING SEMAHPORES!!!!!!!\n");

	int32 four = pipeopen();
	
	char testes[] = "syscall piperecv(int32 pipe, int32 len, char *c) takes a pipe id, a message length, and a char pointer as arguments. It should put len characters from the buffer of the specified pipe to c, and return OK. If there is not enough characters in the buffer, it should put all them";
	kprintf("%s\n", testes);
	pipesend(four, 276, testes);

	char testMsg[20];
	piperecv(four, 20, testMsg);
	kprintf("%s", testMsg);	

	char testMsgTwo[256];
	piperecv(four, 256, testMsgTwo);
	kprintf("%s", testMsgTwo);

	kprintf("\n");

	return OK;
}
