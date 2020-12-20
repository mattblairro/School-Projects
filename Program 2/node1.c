#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

struct rtpkt pkt1;

extern int TRACE;
extern int YES;
extern int NO;

int connectcosts1[4] = { 1,  4,  1, 9 };

struct distance_table 
{
  int costs[4][4];
} dt1;

rtinit1() 
{
	// Initialize distance table
	dt1.costs[0][0] = 0; dt1.costs[0][1] = 1; dt1.costs[0][2] = 2; dt1.costs[0][3] = 3;
	dt1.costs[1][0] = 0; dt1.costs[1][1] = 1; dt1.costs[1][2] = 1; dt1.costs[1][3] = 4;
	dt1.costs[2][0] = 3; dt1.costs[2][1] = 1; dt1.costs[2][2] = 0; dt1.costs[2][3] = 2;
	dt1.costs[3][0] = 9; dt1.costs[3][1] = 20; dt1.costs[3][2] = 4; dt1.costs[3][3] = 1;
	
	
	// Print original distance table
	printdt1(dt1);
	
	
	// Initialize routing packet
	pkt1.sourceid = 1;
	pkt1.mincost[0] = dt1.costs[1][0];
	pkt1.mincost[1] = dt1.costs[1][1];
	pkt1.mincost[2] = dt1.costs[1][2];
	pkt1.mincost[3] = dt1.costs[1][3];
	
	
	// Send packet to each neighbor
	pkt1.destid = 0;
	tolayer2(pkt1);
	pkt1.destid = 2;
	tolayer2(pkt1);
	pkt1.destid = 3;
	tolayer2(pkt1);
}


rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
	int i = 0;
	int j = 0;

	// Take distance vector[i] amd compare ot to the recived packet
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (dt1.costs[i][j] > (dt1.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j]))
			{
				// Update distance table and print out dt1
				dt1.costs[i][j] = dt1.costs[i][rcvdpkt->sourceid] + (rcvdpkt->mincost)[j];
				printdt1(dt1);
				
				if (pkt1.mincost[j] > dt1.costs[i][j])
					pkt1.mincost[j] = dt1.costs[i][j];
				
				// transmit packet to other nodes
				pkt1.destid = 0;
				tolayer2(pkt1);
				pkt1.destid = 2;
				tolayer2(pkt1);
				pkt1.destid = 3;
				tolayer2(pkt1);
			}
		}
	}
}


printdt1(dtptr)
  struct distance_table dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr.costs[0][0], dtptr.costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr.costs[2][0], dtptr.costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr.costs[3][0], dtptr.costs[3][2]);

}



linkhandler1(linkid, newcost)   
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}


