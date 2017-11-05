/*
 * Print out statistics, and stop.
 * We're called both when a SIGINT signal is received, or if the
 * specified number of packets have been received.
 */

#include	"defs.h"

void sig_finish(int args)
{
	printf("\n----%s PING Statistics----\n", hostname );
	printf("%d packets transmitted, ", ntransmitted );
	printf("%d packets received, ", nreceived );
	if (ntransmitted)
		printf("%d%% packet loss",
			(int) (((ntransmitted-nreceived)*100) / ntransmitted) );
	printf("\n");
	if (nreceived && timing)
		printf("round-trip (ms)  min/avg/max = %d/%ld/%d\n",
				tmin, tsum / nreceived, tmax );
	fflush(stdout);
	exit(0);

	return;
}
