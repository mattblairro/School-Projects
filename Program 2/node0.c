#include <stdio.h>

// cc prog3.c node0.c node1.c node2.c node3.c

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

struct rtpkt pkt0;
  
extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt0;

void rtinit0() 
{	
	// populate distance table & print
	dt0.costs[0][0] = 0; dt0.costs[0][1] = 1; dt0.costs[0][2] = 2; dt0.costs[0][3] = 3;
	dt0.costs[1][0] = 0; dt0.costs[1][1] = 1; dt0.costs[1][2] = 1; dt0.costs[1][3] = 4;
	dt0.costs[2][0] = 3; dt0.costs[2][1] = 1; dt0.costs[2][2] = 0; dt0.costs[2][3] = 2;
	dt0.costs[3][0] = 9; dt0.costs[3][1] = 20; dt0.costs[3][2] = 4; dt0.costs[3][3] = 1;
	printdt0(dt0);
	
	
	// Initialize routing packet
	pkt0.sourceid = 0;
	pkt0.mincost[0] = dt0.costs[0][0];
	pkt0.mincost[1] = dt0.costs[0][1];
	pkt0.mincost[2] = dt0.costs[0][2];
	pkt0.mincost[3] = dt0.costs[0][3];
	
	
	// Send packet to each neighbor
	pkt0.destid = 1;
	tolayer2(pkt0);
	pkt0.destid = 2;
	tolayer2(pkt0);
	pkt0.destid = 3;
	tolayer2(pkt0);
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
	int i = 0;
	int j = 0;
	
	// Take distance vector[i] amd compare ot to the recived packet
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dt0.costs[i][j] > (dt0.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j]))
			{
				// Update distance table and print out dt0
				dt0.costs[i][j] = dt0.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j];
				printdt0(dt0);
				
				if (pkt0.mincost[j] > dt0.costs[i][j])
					pkt0.mincost[j] = dt0.costs[i][j];
				
				// transmit packet to other nodes
				pkt0.destid = 1;
				tolayer2(pkt0);
				pkt0.destid = 2;
				tolayer2(pkt0);
				pkt0.destid = 3;
				tolayer2(pkt0);
			}
		}
	}
}


printdt0(dtptr)
  struct distance_table dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr.costs[1][1],
	 dtptr.costs[1][2],dtptr.costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr.costs[2][1],
	 dtptr.costs[2][2],dtptr.costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr.costs[3][1],
	 dtptr.costs[3][2],dtptr.costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

