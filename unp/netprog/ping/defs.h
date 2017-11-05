/*
 * Includes, defines and global variables used between functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
extern int	errno;

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <sys/param.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff	/* should be in <netinet/in.h> */
#endif

#define	MAXWAIT		  10	/* max time to wait for response, sec. */
				/* used only for final receive */

/*
 * Beware that the outgoing packet starts with the ICMP header and
 * does not include the IP header (the kernel prepends that for us).
 * But, the received packet includes the IP header.
 */

#define	MAXPACKET	4096	/* max packet size */

#ifndef	MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	  64	/* should be defined in <param.h> */
#endif

#define	SIZE_ICMP_HDR	   8	/* 8-byte ICMP header */
#define	SIZE_TIME_DATA	   8	/* then the BSD timeval struct (ICMP "data") */
#define	DEF_DATALEN	  56	/* default data area after ICMP header */

int		packsize;	/* size of ICMP packets to send */
				   /* this includes the 8-byte ICMP header */
int		datalen;	/* size of data after the ICMP header */
				   /* may be 0 */
				   /* if >= SIZE_TIME_DATA, timing is done */

int		verbose;	/* enables additional error messages */

char		sendpack[MAXPACKET];	/* the packet we send */
char		recvpack[MAXPACKET];	/* the received packet */

struct sockaddr_in	dest;	/* who to ping */
int			sockfd;	/* socket file descriptor */

char		*hostname;
int		npackets;	/* max # of packets to send; 0 if no limit */
int		ident;		/* our process ID, to identify ICMP packets */
int		ntransmitted;	/* sequence # for outbound packets = #sent */
int		nreceived;	/* # of packets we got back */

int		timing;		/* true if time-stamp in each packet */
int		tmin;		/* min round-trip time */
int		tmax;		/* max round-trip time */
long		tsum;		/* sum of all round-trip times, for average */
			/* above 3 times are in milliseconds */

char		*inet_ntoa();	/* BSD library routine */

void		recv_ping();
void		sig_finish(int args);	/* our function to finish up and exit */
void		sig_alarm(int args);	/* our SIGALRM signal handler */

void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);
void err_ret(const char *fmt, ...);

void pr_pack(char *buf, int cc, struct sockaddr_in *from);
int in_cksum(register u_short *ptr, register int nbytes);
void send_ping();
