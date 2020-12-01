// Alexander Baekey
// CDA 5106
// Machine Problem 3

// Out of Order / Dynamic Scheduling simulator
// Implemention of Tomasulo's algorithm


//example of trace instructions
//2b6420 0 -1 29 14
//2b6424 2 14 29 -1
//2b6428 2 15 29 -1




// libs
#include<stdlib.h> 
#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>

// global variables
int regsize=128;

enum state { IF, ID, IS, EX, WB };
bool cycleflag=false;
int n;
int m;
int S;
int N;
float IPC;
int clock = 0;

//////////TODO erase if using instruction struct///////////
/*int pc;*/
/*int op;*/
/*int regdest;*/
/*int reg1;*/
/*int reg2;*/
/*int latency;*/
/*int tag;*/

// node structure
typedef struct node {
     int op;     
     int pc;
     int regdest; 
     int reg1;
     int reg2;
     int rdydest;
     int rdy1;
     int rdy2;
     int valdest;
     int valreg1;
     int valreg2;
     int latency;
     int tag;
     int stage;

     struct node *next_node;
}
node;

typedef struct instruction {
     int pc;
     int op;
     int regdest;
     int reg1;
     int reg2;
     int latency;
     int tag;
}
instruction;
     
typedef struct reg_file{
     int rdy;
     int tag;
} reg_file;

typedef struct time{
     int tag;
     int fetch_start;
     int fetch_length;
     int dispatch_start;
     int dispatch_length;
     int issue_start;
     int issue_length;
     int execute_start;
     int execute_length;
     int writeback_start;
     int writeback_length;
}
time;
     

instruction *instructionsList;
time *timeList;
int count = 0;

node *fakeROB; //suggested 1024 entries

node *dispatchList;
node *issueList;
node *executeList;

reg_file *regfile;

int IDcount;
int IScount;
int EXcount; 
// functions
void fakeRetire();
void execute();
void issue();
void dispatch();
void fetch();
void advanceCycle(int);
void node_initialize(node *);
void node_copy(node *, node *);
void reg_initialize(reg_file *, int i);
void instruction_initialize(instruction *);
void time_initialize(time *);
















int main(int argc, char *argv[]){
     char *trace;
     char *ptr;
     int i;
     int pc;
     int op;
     int regdest;
     int reg1;
     int reg2;
     FILE* fp;
     S     = strtol(argv[1],&ptr,10);
     N     = strtol(argv[2],&ptr,10);
     trace = argv[3];

          //initilizations
     //fake ROB
     instructionsList = (instruction *)malloc(sizeof(instruction) * S);
     fakeROB       = (node *)malloc(sizeof(node));
     dispatchList  = (node *)malloc(sizeof(node));
     issueList     = (node *)malloc(sizeof(node));
     executeList   = (node *)malloc(sizeof(node));
     regfile       = (reg_file *)malloc(sizeof(reg_file)*regsize);
     

     node_initialize(fakeROB);
     node_initialize(dispatchList);
     node_initialize(issueList);
     node_initialize(executeList);
     for(i=0;i<regsize;i++){
          reg_initialize(&regfile[i], i);
     }
     for(i=0;i<S;i++){
          instruction_initialize(&instructionsList[i]);
     }

     fakeROB->next_node       = fakeROB;
     dispatchList->next_node  = dispatchList;
     issueList->next_node     = issueList;
     executeList->next_node   = executeList;

     IDcount=0;
     IScount=0;
     EXcount=0;
     


     //read file and execute functions on each instruction line
     fp = fopen(trace, "r");

     while(!feof(fp)){   
          fscanf(fp, "%x %d %d %d %d\n", &pc, &op, &regdest, &reg1, &reg2);
          instructionsList[count].op = op;
          instructionsList[count].pc = pc;
          instructionsList[count].regdest = regdest;
          instructionsList[count].op = reg1;
          instructionsList[count].op = reg2;
          instructionsList[count].tag = count;
          // op type 0, 1 clock cycle
          if(instructionsList[count].op == 0)
               instructionsList[count].latency = 1;
          // op type 1, 2 clock cycles
          else if(instructionsList[count].op == 1)
               instructionsList[count].latency = 2;
          // op type 2, 5 clock cycles
          else if(instructionsList[count].op == 2)
               instructionsList[count].latency = 5;

          count++;
     }  

     fclose(fp);

     timeList = (time *)malloc(sizeof(time) * count);
     for(i=0;i<count;i++){
          time_initialize(&timeList[i]);
     }     
     n=0;
     while(cycleflag==false){
          fakeRetire();
          execute();
          issue();
          dispatch();
          m=0;
          //fetch conditions
          while(n<count && m<N && IDcount<(2*N)){
               fetch(&instructionsList[i]);
               n++;
               m++;
          }
     advanceCycle(n);
     }     
          
     //prints
     IPC = ((float)(count)/(float)(clock));  
     printf("number of instructions = %d\n", count);
     printf("number of cycles = %d\n", clock);
     printf("IPC = %.2f", IPC);

     return 0;

}


void node_initialize(node *A){
     A->op       = -INFINITY;
     A->pc       = -INFINITY;
     A->regdest  = -INFINITY;
     A->reg1     = -INFINITY;
     A->reg2     = -INFINITY;
     A->rdydest  = -INFINITY;
     A->rdy1     = -INFINITY;
     A->rdy2     = -INFINITY;
     A->latency  = -INFINITY;
     A->tag      = -INFINITY;
}
     
void reg_initialize(reg_file *A, int i){
          A->rdy = true;
          A->tag = i;
}

void instruction_initialize(instruction *A){
     A->op        = -INFINITY;
     A->pc        = -INFINITY;
     A->regdest   = -INFINITY;
     A->reg1      = -INFINITY;
     A->reg2      = -INFINITY;
     A->latency   = -INFINITY;
     A->tag       = -INFINITY;
}

void time_initialize(time *A){
     A->tag                 = -INFINITY;
     A->fetch_start         = -INFINITY;
     A->fetch_length        = -INFINITY;
     A->issue_start         = -INFINITY;
     A->issue_length        = -INFINITY;
     A->execute_start       = -INFINITY;
     A->execute_length      = -INFINITY;
     A->writeback_start     = -INFINITY;
     A->writeback_length    = -INFINITY;
}


void node_copy(node *B, node *A){
     //B is target, A is source
     B->op       = A->op;     
     B->pc       = A->pc;
     B->regdest  = A->regdest; 
     B->reg1     = A->reg1;
     B->reg2     = A->reg2;
     B->rdydest  = A->rdydest;
     B->rdy1     = A->rdy1;
     B->rdy2     = A->rdy2;
     B->latency  = A->latency;
     B->tag      = A->tag;
}







void fakeRetire(){
     node *cur;
     cur = fakeROB->next_node;
     while(cur!=fakeROB){
          if(cur->stage==WB){
               timeList[cur->tag].writeback_length=1;
          }
          cur=cur->next_node;
     }
     cur=fakeROB->next_node;
     while(cur!=fakeROB){
          if(cur->stage==WB){
               int t = cur->tag;
               fakeROB->next_node=cur->next_node;
               printf("%d fu{%d} src{%d,%d} dst{%d} IF{%d,%d} ID{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d} \n", 
                    t, 
                    cur->op, 
                    cur->reg1, 
                    cur->reg2, 
                    cur->regdest, 
                    timeList[t].fetch_start, 
                    timeList[t].fetch_length,
                    timeList[t].dispatch_start,
                    timeList[t].dispatch_length,
                    timeList[t].issue_start, 
                    timeList[t].issue_length,
                    timeList[t].execute_start,
                    timeList[t].execute_length,
                    timeList[t].writeback_start,
                    timeList[t].writeback_length);
               cur=cur->next_node;
          }
          else
               break;
     }
}

void execute(){
//part of issue/execute pipeline
;
}

void issue(){
//part of issue/execute pipeline
;
}

void dispatch(){
//part of fetch/dispatch pipeline
;
}

void fetch(instruction *instruct){
//part of fetch/dispatch pipeline
     node *cur, *temp;
     cur = (node *)malloc(sizeof(node));
     cur->op        = instruct->op;
     cur->pc        = instruct->pc;
     cur->reg1      = instruct->reg1;
     cur->reg2      = instruct->reg2;
     cur->regdest   = instruct->regdest;
     cur->tag       = instruct->tag;
     cur->rdy1      = false;
     cur->rdy2      = false;
     cur->rdydest   = false;
     cur->valreg1   = cur->reg1;
     cur->valreg2   = cur->reg2;
     cur->valdest   = cur->regdest;
     cur->latency   = instruct->latency;
     cur->stage     = IF;

     temp = fakeROB;
     while(temp->next_node!=fakeROB)
          temp = temp->next_node;
     temp->next_node=cur;
     cur->next_node=fakeROB;

     node *cur, *temp;
     cur = (node *)malloc(sizeof(node));
     cur->op        = instruct->op;
     cur->pc        = instruct->pc;
     cur->reg1      = instruct->reg1;
     cur->reg2      = instruct->reg2;
     cur->regdest   = instruct->regdest;
     cur->tag       = instruct->tag;
     cur->rdy1      = false;
     cur->rdy2      = false;
     cur->rdydest   = false;
     cur->valreg1   = cur->reg1;
     cur->valreg2   = cur->reg2;
     cur->valdest   = cur->regdest;
     cur->latency   = instruct->latency;
     cur->stage     = IF;
 
     temp = dispatchList;
     while(temp->next_node!=dispatchList)
          temp=temp.next_node;
     temp->next_node=cur;
     cur->next_node=dispatchList;
     IDcount++;
     timeList[instruct->tag].fetch_start=clock;    
}




void advanceCycle(int s){
     clock++;
     if((s>=count) && (fakeROB->next_node == fakeROB))
          cycleflag = true;
     else if((s<count) && (fakeROB->next_node == fakeROB))
          cycleflag = false;
     else if((s>=count) && (fakeROB->next_node != fakeROB))
          cycleflag = false;
     else if((s<count) && (fakeROB->next_node != fakeROB))
          cycleflag = false;
     else
          cycleflag = true; 
}






     



