/* Project #2, Manohar Kotapati */

#include "csim.h"
#include <stdio.h>

#define SIMTIME 1000.0
#define NUM_NODES 5L
#define TIME_OUT 1.0
#define IAL 2.0
#define TRANS_TIME 0.1
#define REQUEST 1L
#define REPLY 2L
#define TRACE


//global variables to track the values
float totalDelayTime=0.0;
int successTransmisions=0;
int failedTransmisions=0;

typedef struct msg *msg_t;
struct msg {
  long type;
  long from;
  long to;
  TIME start_time;
  msg_t link;
};

msg_t msg_queue;

struct nde {
  FACILITY cpu;
  MBOX input;
};

struct nde node[NUM_NODES];
FACILITY network[NUM_NODES][NUM_NODES];
TABLE resp_tm;
FILE *fp;
float lossprob=0.0;
void init();
void proc();
void send_msg();
void form_reply();
void decode_msg();
void return_msg();
msg_t new_msg();

void sim()
{
	//Get the loss probabiliy
	printf("\nEnter the Loss probabiliy(0.1, 0.2, 0.e, 0.4, 0.5)");
	scanf("%f",&lossprob);
 create("sim");
 init();
 hold(SIMTIME);
 printf("The number of successful Transactions are %d\n",successTransmisions);
 printf("The number of failed Transactions are %d\n",failedTransmisions);
 printf("The total delay time is %f\n",totalDelayTime);
}

void init()
{
     long nodeCount1, nodeCount2;
     char str[24];
     fp=fopen("cs.out","w");
     set_output_file(fp);
     max_facilities(NUM_NODES*NUM_NODES + NUM_NODES);
     max_servers(NUM_NODES*NUM_NODES + NUM_NODES);
     max_mailboxes(NUM_NODES);
     max_events(2 * NUM_NODES*NUM_NODES);
     resp_tm = table("msg rsp tm");
     msg_queue = NIL;

     for (nodeCount1 = 0; nodeCount1 < NUM_NODES; nodeCount1++) {
         sprintf(str, "cpu.%d", nodeCount1);
         node[nodeCount1].cpu = facility(str);
         sprintf(str, "input.%d", nodeCount1);
         node[nodeCount1].input = mailbox(str);
      }
     for (nodeCount1 = 0; nodeCount1 < NUM_NODES; nodeCount1++)
      for (nodeCount2 = 0; nodeCount2 < NUM_NODES; nodeCount2++) {
         sprintf(str, "nt%d.%d", nodeCount1, nodeCount2);
         network[nodeCount1][nodeCount2] = facility(str);
      }
     for (nodeCount1 = 0; nodeCount1< NUM_NODES; nodeCount1++)
         proc(nodeCount1);
}

void proc(n)
long n;
 {
 msg_t m;
 msg_t xt;
 long s,t;
 float y;
 float x;
 float prob;
 float toNode;
 TIME sendtime;
 int reTransmiting=0;
  TIME delay;
 create("proc");

 int sent = 0;

 while (clock < SIMTIME)
 {
	 /*Don't send the message, if it already re-transmitted through TIME OUT event*/
	if(reTransmiting!=1){
      m = new_msg(n);
	 toNode=m->to;
	 sendtime=m->start_time;
	 send_msg(m);
		}

	else{

		 decode_msg("Waiting: re-send's a HELLO to", m, n);
	}

       s = timed_receive(node[n].input,(long *)&m, TIME_OUT);

      switch (s) {
         case TIMED_OUT :

			//printf("\ntime_out delay %f from %ld  to %ld",delay,m->from,m->to);
               totalDelayTime = totalDelayTime+TIME_OUT;
			if(reTransmiting==0){

              #ifdef TRACE
              decode_msg("re-send's a HELLO to", m, n);
              #endif
				reTransmiting=1;

					// printf("\nResending  from %ld  to %ld at %f",m->from,m->to,m->start_time);
					   send_msg(m);
					   //sent = 2;



					}
			else{
				  //Failed Transaction
				reTransmiting=0;
				failedTransmisions++;
				decode_msg("Failed: Message a HELLO to", m, n);
			}

           break;
         case EVENT_OCCURRED:

           #ifdef TRACE
           decode_msg("receives msg", m, n);
           #endif
           t = m->type;
            switch (t)
            {
             case REQUEST:
               form_reply(m);
               #ifdef TRACE
               decode_msg("HELLO_ACK to", m, n);
               #endif
               send_msg(m);
               break;
             case REPLY:
              successTransmisions++;
               #ifdef TRACE
               decode_msg("receives a HELLO_ACK from", m, n);
			   if(toNode==m->from && sendtime==m->start_time){
				  //  printf(" \n transmission matched %f",clock);
			   }

               delay=clock - m->start_time;
               totalDelayTime = totalDelayTime+clock-sendtime;
			   reTransmiting=0;
               #endif
               record(clock - m->start_time, resp_tm);
               return_msg(m);
               break;
             default:
               decode_msg("***unexpected type", m, n);
               break;
            }
      break;
}
}
}

void send_msg(m)
msg_t m;
{
	long prob;
long from, to;
from = m->from;
to = m->to;
use(node[from].cpu, IAL);
reserve(network[from][to]);
hold(TRANS_TIME);
m->start_time=clock;

//printf(" \nsending from %ld to %ld time %6.3f",from,to,m->start_time);

//Check the probabiliy before sending
prob=random(1,10);
if(prob>(lossprob*10)){
send(node[to].input, (long) m);
}

release(network[from][to]);
}

msg_t new_msg(from)
long from;
{
	msg_t m;
long i;



if (msg_queue == NIL)
  { m = (msg_t)do_malloc(sizeof(struct msg));}
else {
  m = msg_queue;
  msg_queue= msg_queue->link;
  }
do {
  i = random(0, NUM_NODES - 1);
  } while (i == from);
m->to = i;
m->from = from;
m->type = REQUEST;
m->start_time = clock;

return(m);
}

void return_msg(m)
msg_t m;
{
m->link = msg_queue;
msg_queue = m;
}

void form_reply(m)
msg_t m;
{
	//printf("\n form reply test");
long from, to;
from = m->from;
to = m->to;
m->from = to;
m->to = from;
m->type = REPLY;
}

void decode_msg(str, m, n)
char *str; msg_t m; long n;
{
	char statement[50];
sprintf(statement,"node.%2ld:%s node.%ld at %6.3f\n" ,n,str,m->to,
clock);
  fwrite(statement , 1 , sizeof(statement) , fp );
  printf("node.%2ld:%s node.%ld at %6.3f\n" ,n,str,m->to,clock);
}