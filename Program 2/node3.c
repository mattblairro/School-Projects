#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };
  
struct rtpkt pkt3;

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt3;

void rtinit3() 
{
	// Initialize distance table
	dt3.costs[0][0] = 0; dt3.costs[0][1] = 1; dt3.costs[0][2] = 2; dt3.costs[0][3] = 3;
	dt3.costs[1][0] = 0; dt3.costs[1][1] = 1; dt3.costs[1][2] = 1; dt3.costs[1][3] = 4;
	dt3.costs[2][0] = 3; dt3.costs[2][1] = 1; dt3.costs[2][2] = 0; dt3.costs[2][3] = 2;
	dt3.costs[3][0] = 9; dt3.costs[3][1] = 20; dt3.costs[3][2] = 4; dt3.costs[3][3] = 1;
	
	
	// Print original distance table
	printdt3(dt3);
	
	
	// Initialize routing packet
	pkt3.sourceid = 3;
	pkt3.mincost[0] = dt3.costs[3][0];
	pkt3.mincost[1] = dt3.costs[3][1];
	pkt3.mincost[2] = dt3.costs[3][2];
	pkt3.mincost[3] = dt3.costs[3][3];
	
	
	// Send packet to each neighbor
	pkt3.destid = 0;
	tolayer2(pkt3);
	pkt3.destid = 2;
	tolayer2(pkt3);
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
	int i = 0;
	int j = 0;

	// Take distance vector[i] amd compare ot to the recived packet
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dt3.costs[i][j] > (dt3.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j]))
			{
				// Update distance table and print out dt3
				dt3.costs[i][j] = dt3.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j];
				printdt3(dt3);
				
				if (pkt3.mincost[j] > dt3.costs[i][j])
					pkt3.mincost[j] = dt3.costs[i][j];
				
				// transmit packet to other nodes
				pkt3.destid = 0;
				tolayer2(pkt3);
				pkt3.destid = 2;
				tolayer2(pkt3);
			}
		}
	}
}


printdt3(dtptr)
  struct distance_table dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr.costs[0][0], dtptr.costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr.costs[1][0], dtptr.costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr.costs[2][0], dtptr.costs[2][2]);

}






