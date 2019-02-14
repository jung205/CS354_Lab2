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

	//pipeclose(0);

	char msg[3];
	piperecv(three, 3, msg);
	kprintf(msg);
	kprintf("\n");

	pipeprint();

	int32 *nc;
	int32 *ns;
	int32 wow = pipestate(one, nc, ns);
	kprintf(wow);

	pipeclose(0);
	pipeclose(1);
	pipeclose(2);

	return OK;
}