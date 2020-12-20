#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

struct rtpkt pkt2;
  
extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt2;

void rtinit2() 
{	
	// Initialize distance table
	dt2.costs[0][0] = 0; dt2.costs[0][1] = 1; dt2.costs[0][2] = 2; dt2.costs[0][3] = 3;
	dt2.costs[1][0] = 0; dt2.costs[1][1] = 1; dt2.costs[1][2] = 1; dt2.costs[1][3] = 4;
	dt2.costs[2][0] = 3; dt2.costs[2][1] = 1; dt2.costs[2][2] = 0; dt2.costs[2][3] = 2;
	dt2.costs[3][0] = 9; dt2.costs[3][1] = 20; dt2.costs[3][2] = 4; dt2.costs[3][3] = 1;
	
	
	// Print original distance table
	printdt2(dt2);
	
	
	// Initialize routing packet
	pkt2.sourceid = 2;
	pkt2.mincost[0] = dt2.costs[2][0];
	pkt2.mincost[1] = dt2.costs[2][1];
	pkt2.mincost[2] = dt2.costs[2][2];
	pkt2.mincost[3] = dt2.costs[2][3];
	
	
	// Send packet to each neighbor
	pkt2.destid = 0;
	tolayer2(pkt2);
	pkt2.destid = 1;
	tolayer2(pkt2);
	pkt2.destid = 3;
	tolayer2(pkt2);
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
	int i = 0;
	int j = 0;

	// Take distance vector[i] amd compare ot to the recived packet
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dt2.costs[i][j] > (dt2.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j]))
			{
				// Update distance table and print out dt2
				dt2.costs[i][j] = dt2.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j];
				printdt2(dt2);
				
				if (pkt2.mincost[j] > dt2.costs[i][j])
					pkt2.mincost[j] = dt2.costs[i][j];
				
				// transmit packet to other nodes
				pkt2.destid = 0;
				tolayer2(pkt2);
				pkt2.destid = 1;
				tolayer2(pkt2);
				pkt2.destid = 3;
				tolayer2(pkt2);
			}
		}
	}
}


printdt2(dtptr)
  struct distance_table dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr.costs[0][0],
	 dtptr.costs[0][1],dtptr.costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr.costs[1][0],
	 dtptr.costs[1][1],dtptr.costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr.costs[3][0],
	 dtptr.costs[3][1],dtptr.costs[3][3]);
}







