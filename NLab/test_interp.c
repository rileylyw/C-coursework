#include "nlab.h"

void test(void);
void FreeProg(Program *prog);

int main(void){
   test();
}

void FreeProg(Program *prog){
   for(int i=0; i<CHARACTER; i++){
      if(prog->variable[i].num != NULL){
         n2dfree(prog->variable[i].num, prog->variable[i].height);
      }
   }
   free(prog);
}

void test(void){
/** 1.nlb **/
   Program* prog1 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/1.nlb", prog1);
   assert(Prog(prog1));
   assert(prog1->variable[0].num[0][0]==2); //var A contains 2
   assert(prog1->variable[1].num==NULL);
   // assert(prog1);
   FreeProg(prog1);

/** 2.nlb **/
   Program* prog2 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/2.nlb", prog2);
   assert(Prog(prog2));
   assert(prog2->variable[8].num[0][0]==11); //var I=11 at the end
   assert(!(prog2->variable[8].num[0][0]==12));
   assert(prog2->variable[5].num[0][0]==3628800); //var F=3628800
   assert(!(prog2->variable[5].num[0][0]==362880));
   assert(prog2->variable[2].num==NULL); //var C uninitialized
   assert(prog2->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog2);

/** 3.nlb **/
   Program* prog3 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/3.nlb", prog3);
   assert(Prog(prog3));
   // printf("%d", prog3->variable[8].num[0][0]);
   assert(prog3->variable[8].num[0][0]==11); //var I=11 at the end
   assert(!(prog3->variable[8].num[0][0]==1000));
   assert(prog3->variable[3].num==NULL); //var D uninitialized
   assert(prog3->variable[24].num==NULL); //var Y uninitialized
   FreeProg(prog3);

/** 4.nlb **/
   Program* prog4 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/4.nlb", prog4);
   assert(Prog(prog4));
   // printf("%d", prog4->variable[5].num[0][0]);
   assert(prog4->variable[8].num[0][0]==4); //var I=4 at the end
   assert(!(prog4->variable[8].num[0][0]==3)); 
   assert(!(prog4->variable[8].num==NULL));
   assert(prog4->variable[9].num[0][0]==3); //var J=3 at the end
   assert(!(prog4->variable[9].num[0][0]==2));
   assert(prog4->variable[5].num[0][0]==0); //var F=0 at the end
   assert(!(prog4->variable[5].num[0][0]==3)); 
   assert(!(prog4->variable[5].num==NULL));
   assert(prog4->variable[3].num==NULL); //var D uninitialized
   assert(prog4->variable[24].num==NULL); //var Y uninitialized
   assert(prog4->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog4);

/** 5.nlb **/
   Program* prog5 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/5.nlb", prog5);
   assert(Prog(prog5));
   printf("%d", prog5->variable[5].num[0][0]); //F:5, K:10, J:9, I:8
   assert(prog5->variable[10].num[0][0]==3); //var K=3 at the end
   assert(!(prog5->variable[10].num[0][0]==2));
   assert(prog5->variable[9].num[0][0]==3); //var J=3 at the end
   assert(!(prog5->variable[9].num[0][0]==2));
   assert(prog5->variable[8].num[0][0]==4); //var I=4 at the end
   assert(!(prog5->variable[8].num[0][0]==3));
   assert((prog5->variable[5].num[0][0]==2731)); //var F=2731 at the end
   assert((prog5->variable[5].num[0][2]==2731)); 
   assert((prog5->variable[5].num[1][1]==2731)); 
   assert((prog5->variable[5].num[2][3]==2731)); 
   assert((prog5->variable[5].num[1][0]==2731)); 
   assert(!(prog5->variable[5].num[0][1]==0));
   assert(!(prog5->variable[5].num[2][3]==100));
   assert(!(prog5->variable[5].num[2][2]==99999));
   assert(!(prog5->variable[8].num[0][0]==3)); 
   assert(prog5->variable[0].num==NULL); //var A uninitialized
   assert(prog5->variable[23].num==NULL); //var X uninitialized
   assert(prog5->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog5);

/** 6.nlb **/
   Program* prog6 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/6.nlb", prog6);
   assert(Prog(prog6));
   assert((prog6->variable[5].num[0][0]==2)); //var F=2 at the end
   assert((prog6->variable[5].num[0][2]==2)); 
   assert((prog6->variable[5].num[1][1]==2)); 
   assert((prog6->variable[5].num[2][3]==2)); 
   assert((prog6->variable[5].num[1][0]==2)); 
   assert(!(prog6->variable[5].num[0][1]==0));
   assert(!(prog6->variable[5].num[2][3]==100));
   assert(!(prog6->variable[5].num[2][2]==99999));
   assert(prog6->variable[0].num==NULL); //var A uninitialized
   assert(prog6->variable[7].num==NULL); //var H uninitialized
   assert(prog6->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog6);

/** 7.nlb **/
   Program* prog7 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/7.nlb", prog7);
   assert(Prog(prog7));
   printf("%d\n", prog7->variable[1].num[2][0]); //F:5, A:0, B:1
   assert((prog7->variable[0].num[0][0]==1)); //var A=1 at the end
   assert((prog7->variable[0].num[2][2]==1)); 
   assert((prog7->variable[0].num[1][1]==1)); 
   assert((prog7->variable[0].num[2][1]==1)); 
   assert((prog7->variable[0].num[1][3]==1)); 
   assert(!(prog7->variable[0].num[0][1]==123));
   assert(!(prog7->variable[0].num[2][3]==100));
   assert(!(prog7->variable[0].num[2][2]==99999));
   assert((prog7->variable[1].num[0][0]==1)); //var B=1 at the end
   assert((prog7->variable[1].num[2][2]==1)); 
   assert((prog7->variable[1].num[1][1]==1)); 
   assert((prog7->variable[1].num[2][1]==1)); 
   assert((prog7->variable[1].num[1][3]==1)); 
   assert(!(prog7->variable[1].num[0][1]==123));
   assert(!(prog7->variable[1].num[2][3]==100));
   assert(!(prog7->variable[1].num[2][2]==99999));
   assert((prog7->variable[5].num[0][0]==1)); //var F=1 at the end
   assert((prog7->variable[5].num[0][2]==1)); 
   assert((prog7->variable[5].num[1][1]==1)); 
   assert((prog7->variable[5].num[2][3]==1)); 
   assert((prog7->variable[5].num[1][0]==1)); 
   assert(!(prog7->variable[5].num[0][1]==0));
   assert(!(prog7->variable[5].num[2][3]==100));
   assert(!(prog7->variable[5].num[2][2]==99999));
   assert(prog7->variable[3].num==NULL); //var D uninitialized
   assert(prog7->variable[23].num==NULL); //var X uninitialized
   assert(prog7->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog7);

/** 8.nlb **/
   Program* prog8 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/8.nlb", prog8);
   assert(Prog(prog8));
   printf("%d\n", prog8->variable[0].num[2][3]); //A:0, B:1, E:4
   assert(prog8->variable[0].num[0][0]==0); //var A
   assert(prog8->variable[0].num[1][1]==0); 
   assert(prog8->variable[0].num[4][4]==0); 
   assert(prog8->variable[0].num[2][2]==1); 
   assert(prog8->variable[0].num[2][3]==1); 
   assert(!(prog8->variable[0].num[2][1]==0));
   assert(!(prog8->variable[0].num[0][4]==1));
   assert(!(prog8->variable[0].num[4][2]==1));
   assert(prog8->variable[1].num[0][0]==0); //var B
   assert(prog8->variable[1].num[1][1]==0); 
   assert(prog8->variable[1].num[4][4]==0); 
   assert(prog8->variable[1].num[2][2]==1); 
   assert(prog8->variable[1].num[2][3]==1); 
   assert(!(prog8->variable[1].num[2][1]==0));
   assert(!(prog8->variable[1].num[0][4]==1));
   assert(!(prog8->variable[1].num[4][2]==1));
   assert(prog8->variable[4].num[0][0]==-1); //var E
   assert(prog8->variable[4].num[1][1]==-1); 
   assert(prog8->variable[4].num[4][4]==-1); 
   assert(prog8->variable[4].num[2][2]==-1); 
   assert(prog8->variable[4].num[2][3]==-1); 
   assert(!(prog8->variable[4].num[2][1]==0));
   assert(!(prog8->variable[4].num[0][4]==1));
   assert(!(prog8->variable[4].num[4][2]==1));
   assert(!(prog8->variable[4].num[3][2]==3));
   assert(!(prog8->variable[4].num[2][2]==111));
   assert(prog8->variable[5].num==NULL); //var F uninitialized
   assert(prog8->variable[23].num==NULL); //var X uninitialized
   assert(prog8->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog8);

/** 9.nlb **/
   Program* prog9 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/9.nlb", prog9);
   assert(Prog(prog9));
   printf("%d\n", prog9->variable[0].num[1][9]); //A:0, B:1
   assert(prog9->variable[0].num[2][3]==8); //var A
   assert(prog9->variable[0].num[0][0]==3); 
   assert(prog9->variable[0].num[5][5]==8); 
   assert(prog9->variable[0].num[7][5]==8); 
   assert(prog9->variable[0].num[6][5]==8); 
   assert(prog9->variable[0].num[1][1]==8); 
   assert(prog9->variable[0].num[8][8]==8); 
   assert(prog9->variable[0].num[7][8]==8); 
   assert(prog9->variable[0].num[1][0]==5); 
   assert(prog9->variable[0].num[1][9]==5); 
   assert(prog9->variable[0].num[9][9]==3); 
   assert(prog9->variable[0].num[9][0]==3); 
   assert(!(prog9->variable[0].num[3][1]==5));
   assert(!(prog9->variable[0].num[4][5]==3));
   assert(!(prog9->variable[0].num[7][2]==5));
   assert(prog9->variable[1].num[2][3]==0); //var B
   assert(prog9->variable[1].num[0][0]==0); 
   assert(prog9->variable[1].num[5][5]==0); 
   assert(prog9->variable[1].num[7][5]==0); 
   assert(prog9->variable[1].num[6][5]==0); 
   assert(prog9->variable[1].num[1][1]==0); 
   assert(prog9->variable[1].num[8][8]==0); 
   assert(prog9->variable[1].num[7][8]==0); 
   assert(prog9->variable[1].num[9][9]==0); 
   assert(prog9->variable[1].num[9][0]==0); 
   assert(prog9->variable[1].num[1][9]==1); 
   assert(prog9->variable[1].num[1][0]==1); 
   assert(!(prog9->variable[1].num[3][1]==1));
   assert(!(prog9->variable[1].num[4][5]==1));
   assert(!(prog9->variable[1].num[7][2]==1));
   assert(!(prog9->variable[1].num[8][8]==1));
   assert(!(prog9->variable[1].num[9][1]==0));
   assert(prog9->variable[12].num==NULL); //var M uninitialized
   assert(prog9->variable[23].num==NULL); //var X uninitialized
   assert(prog9->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog9);

/** 10.nlb **/
   Program* prog10 = ncalloc(1, sizeof(Program));
   readFile("Test_interp/10.nlb", prog10);
   assert(Prog(prog10));
   printf("%d\n", prog10->variable[1].num[0][0]); //A:0, B:1, D:3
   assert(prog10->variable[0].num[0][0]==0); //var A
   assert(prog10->variable[0].num[1][1]==1); 
   assert(prog10->variable[0].num[2][2]==1); 
   assert(prog10->variable[0].num[3][3]==0); 
   assert(!(prog10->variable[0].num[2][1]==0));
   assert(!(prog10->variable[0].num[0][3]==1));
   assert(!(prog10->variable[0].num[3][2]==1));
   assert(prog10->variable[1].num[0][0]==1); //var B
   assert(prog10->variable[1].num[1][1]==0); 
   assert(prog10->variable[1].num[2][2]==0); 
   assert(prog10->variable[1].num[3][3]==1); 
   assert(!(prog10->variable[1].num[2][1]==1));
   assert(!(prog10->variable[1].num[0][3]==0));
   assert(!(prog10->variable[1].num[3][2]==0));
   assert(prog10->variable[3].num[0][0]==1); //var D
   assert(prog10->variable[3].num[1][1]==0); 
   assert(prog10->variable[3].num[2][2]==0); 
   assert(prog10->variable[3].num[3][3]==1); 
   assert(!(prog10->variable[3].num[2][1]==1));
   assert(!(prog10->variable[3].num[0][3]==0));
   assert(!(prog10->variable[3].num[3][2]==0));
   assert(!(prog10->variable[3].num==NULL));
   assert(prog10->variable[5].num==NULL); //var F uninitialized
   assert(prog10->variable[23].num==NULL); //var X uninitialized
   assert(prog10->variable[25].num==NULL); //var Z uninitialized
   FreeProg(prog10);
}
