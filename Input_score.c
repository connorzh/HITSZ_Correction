#include <stdio.h>
#include <stdlib.h>

#define MAX_ERROR_NUM 8     //最多错误点出现数
#define error -1
#define MAX_SMALL_TITLE_NUM 6
#define MAX_BIG_TITLE_NUM 10
#define ROOT -1
#define MAX_NUM 240
#define MAX_STUDENT_NUM 100
#define MAX_STOTAL_NUM 24

#define InfoType int //其他的信息
#define ScoreType float
#define ErrorType int
#define IdType int

//关于使用到的结构体
//<**********************树******************************>
//模式一的树的每个节点存储的信息
typedef struct child_node {
    int child;  //孩子下标索引
    struct chile_node *next;  //指向下一个孩子的指针
}Child;

typedef struct  {
    int id;     //题号
    int parent;     //双亲下表索引
    ScoreType score;
    ErrorType error_happened[MAX_ERROR_NUM];   //出现的错误
    Child *firstchild;  //第一个孩子的下标
}TreeNode;

typedef struct node {
    TreeNode *base;     //动态创建数组的首地址
    int root[MAX_BIG_TITLE_NUM];   //每个答题的根节点索引
    int capicity;   //数组容量
    int root_capicity;  //大题数
    int type;
}ScoreTree;
//<***********************end*****************************>

//<***********************前端数据*****************************>
// 关于从前端得到的数据
/*
*关于flag：
*flag=-1,表示这个位置为空，没有填入数据
*flag=0:表示是第一层级（一）
*flag=1:表示是（一.(1)）
*flag=2：表示是（一.(1).a）
*/
typedef struct {
    ScoreType score;
    int flag;  
    int parent; //表示是属于哪一题的
}SmallNode;

typedef struct {
    SmallNode small[4*MAX_SMALL_TITLE_NUM];
    int len;
}Data_front_one_title;
//<***********************end*****************************>

//<************************按键key****************************>
//关于按键的结构体
typedef struct {
    int next[MAX_BIG_TITLE_NUM];   //next按键
    int finish; //finish按键
    int mode_co;    //批改模式
    int mode_id;    //批改的对象
    int type_tree;     //不同的对象对应的不同的图的类型
}Click;
//<*************************end***************************>

//<*************************图***************************>
//title_mode1 用邻接表的形式设置初始模板 
//根据创建的树，动态创建数组就好了
//这个是ss题链表
typedef struct ssnode {
    SmallNode ssdata;
    struct ssnode *ssbase;
    int capicity_ssbase;    //ss题数量
}ssGraphNode;
//这个是s题数组
typedef struct snode {
    ssGraphNode *sbase;
    int capicity_sbase;//s题目数量

} sGraphNode;

typedef struct node {
    sGraphNode *base;
    int capicity_base;  //大题数量
} ALgraph;
//<************************end****************************>

//<*************************题目数量***************************>
//这个是计算作业大题数/小题数/小小题数结构体
typedef struct tnode{
    //每个小题对应的小小题数,下标为0表示第一小题拥有的小题数
    int ssnum[MAX_SMALL_TITLE_NUM];
    int snum_capicity;   //小题数,下标为0，表示第一题
}TitleNum;

typedef struct TNode {
    TitleNum num_s_ss[MAX_BIG_TITLE_NUM];   //每个大题对应的小题，小小题
    int capicity_big;   //大题数
}TotalTitleNum;
//<*****************************end***********************>

//<--------------------------------前端转后端--------------------------------------------->
int mode_identity;   //表示批改作业还是实验报告
int mode_correcton;  //表示作业/实验报告的批改方式


//从前端接收的数据
int get_score;  //接收从前端得到的分数。
int next;       //接收前端用户是否按了next 按键
int Finish;     //接收前端用户是否按了finish 按键

//前端接口函数
int Get_score_from_front ();    
int Get_next_front ();
int Get_finish_front ();
int Get_mode_co_front ();   //返回批改模式
int Get_mode_id_front ();   //返回批改的是作业还是实验报告
void Printf_InputAgain_front(); //在前端让用户重新选择
void Get_stu_score_from_front(int big_title_num);    //从前段获得学生成绩
Data_front_one_title *Get_one_title_score_from_front(int id);  //从前段获得某个学生的一题成绩
//<---------------------------------END-------------------------------------------->


//<---------------------------------后端函数库-------------------------------------------->

void Get_Small_SSmall_TitleNum(int *s_num,int *ss_num,Data_front_one_title *get_score);
int Choose_mode_co(int mode);    //选择批改模式
int Choose_mode_id(int mode);    //选择批改的是作业还是实验报告

void Correct_mode1_buildtree (int type);   //按模式一批改建树
void Correct_mode2_buildtree ();   //按模式二批改建树

//模式一的树
void Create_Tree_mode1_templete(ScoreTree tree,ALgraph g,TotalTitleNum totalnum);
void Get_Tree_num(ScoreTree tree,TotalTitleNum totalnum);
void Init_Tree_mode1(ScoreTree tree,int type_tree,int frequency);
int Create_Child_Tree_mode1(ScoreTree tree,Data_front_one_title *get_score,int root_pos,int root_id);
void Calculate_score_mode1(ScoreTree tree,int start,int end);

//模式二的数
int ChangeClassToStudentNum(int classStart,int classEnd);
void Init_Tree_mode2(ScoreTree tree2,int type_tree,int bigID,int stuNum);
int Create_Child_Tree_mode2(ScoreTree tree2,Data_front_one_title *getGrade,int root_pos,int root_id);
//<---------------------------------全局变量（测试数据）-------------------------------------------->
/*模式一：*/
ScoreTree Tree[MAX_STUDENT_NUM];//一颗树是一个学生的成绩
ALgraph g;
TotalTitleNum totalnum;     //得到作业的大题数/小题数/小小题数
//一个学生的成绩，如果有多个，用二维数组存储
Data_front_one_title stu_score[MAX_BIG_TITLE_NUM];
//在树的存储结构中,{0,1,0}所在的数组下标是1
//这是一位学生的测试数据
stu_score[]={//例如{0,1,0}表示的是0分，在树的第一层，父亲是0
    {{{0,1,0},{0,2,1},{2,3,2},{3,3,2},{4,2,1}},5},
    {{{0,1,0},{1,2,1},{2,2,1},{3,2,1}},4},
    {{{0,1,0},{1,2,1},{2,2,1},{0,3,1},{1,3,4},{2,3,4},{3,3,4}},7} 
    //关于怎么计算长度，后面再讨论
    //注意len所表示
};
int len_stu_score = (int)sizeof(stu_score)/(sizeof(ScoreType)+sizeof(int));
//<----------------------------------------------------------------------------->

/*
模式二：一道一道题目的改，一棵树是所有学生同一题的答题情况
*/
//下标要从1开始,TreeMode2[0]里面存所有同学的总分
ScoreTree TreeMode2[MAX_BIG_TITLE_NUM+1];


Data_front_one_title stuGrade[MAX_STUDENT_NUM];

//和模式一一样,但不同的是stuGrade[i]里面的模式是一样的，只有分数不一样
//为四个学生第一答题的答题情况
//stuGrade[i]是一个学生的某一题成绩//根据读入更新
stuGrade[] = {
    {{{0,1,0},{0,2,1},{3,3,2},{1,3,2},{3,2,1}},5},
    {{{0,1,0},{0,2,1},{2,3,2},{2,3,2},{2,2,1}},5},
    {{{0,1,0},{0,2,1},{1,3,2},{3,3,2},{1,2,1}},5},
    {{{0,1,0},{0,2,1},{0,3,2},{3,3,2},{0,2,1}},5}
};

//<---------------------------------END-------------------------------------------->

int main() 
{
    Click *key;
    
    int ret_co;
    int ret_id;
    key = (Click *)malloc(sizeof(Click));

    //如果没有得到前端的输入
    while(ret_co && ret_id)
    {
        Printf_InputAgain_front();//在前端显示
    }

    key->mode_co = Get_mode_co_front();
    key->mode_id = Get_mode_id_front();
    key->type_tree = key->mode_co;

    Choose_mode_co(key->mode_co);//根据需要创建树

    
    

}

//<---------------------------------前段借口函数-------------------------------------------->
int Get_score_from_front () 
{

}  

int Get_score_next_front ()
{
    
}

int Get_score_finish_front ()
{

}

int Get_mode_co_front ()   //返回批改模式
{

}

int Get_mode_id_front ()   //返回批改的是作业还是实验报告
{

}

void Printf_InputAgain_front() //在前端让用户重新选择
{

}

void Get_stu_score_from_front(int big_title_num)
{

}

Data_front_one_title *Get_one_title_score_from_front(int id)
{
    return stu_score+id;
}
//<---------------------------------END-------------------------------------------->


//<---------------------------------后端函数定义-------------------------------------------->
int ChangeClassToStudentNum(int classStart,int classEnd)
{
    //读取表格 然后获得人数，再相加
    //通过林大琪
}



int Choose_mode_co(int mode)
{
    switch(mode)
    {
        case 1: //模式一
            Correct_mode1_buildtree (mode);
            break;
        case 2: //模式二
            Correct_mode2_buildtree (mode);
            break;
        default:
            return error;
    }
}
/*批改模式一创建树*/

void Correct_mode1_buildtree (int type)
{
    int i;
    int n;
    n = Get_Studet_num();
    //创建有n个学生作业树
    Init_Tree_mode1(Tree[0],type,0);
    //利用tree[0]来创建模板
    Get_Tree_num(Tree[0],totalnum);

    Create_Tree_mode1_templete(Tree[0],g,totalnum);
    
    for (i = 1; i < n; i++)
    {
        Init_Tree_mode1(Tree[i],type,i);

    }
}

/*批改模式二创建树*/
void Correct_mode2_buildtree (int mode)
{
    int i;
    int start,end;
    int stuNum;
    int titleNum;
    int titleId[MAX_BIG_TITLE_NUM];
    //前端显示，错误输入，注意要在后期增强健壮性的设置
    //林大琪要实现的功能：
    printf("请输入您要批改的题目数量：\n");
    scanf("%d",&titleNum);
    printf("请输入您要批改的题号（一个一个输入回车输入下一个）");
    for (i = 0; i < titleNum; i++)
    {
        scanf("%d",&titleId[i]);
    }
    printf("请输入您要批改的班级\n");
    scanf("%d,%d",start,end); //[start,end)   
    stuNum = ChangeClassToStudentNum(start,end);

    //TreeMode2[0]是用来计算总分的额外写一个函数
    Init_Tree_mode2(TreeMode2[1],mode,1,stuNum);
    //怎么得到总共有多少个大题？
    for (i = 1; i < MAX_BIG_TITLE_NUM+1; i++)
    Init_Tree_mode2(TreeMode2[0],mode,0,stuNum);
}

/*
*功能：初始化树
*参数：frequency是指第几次创建
*/
void Init_Tree_mode1(ScoreTree tree,int type_tree,int frequency)
{
    int big_title_num=0;    //记录大题个数
    int root_pos;     //在数组中开始的下标
    Data_front_one_title *get_score;
    root_pos = 1;   //从数组的第一个开始，把第0个位置空出来，存放总分
    
    tree.base = (TreeNode *)calloc(MAX_NUM+1,sizeof(TreeNode));//动态创建了一个巨大的树
    tree.capicity = 1;
    tree.type = type_tree;

    //初始化tree.base[0]
    tree.base[0].firstchild =(Child *)malloc(sizeof(Child));
    tree.base[0].firstchild->next=NULL;
    tree.base[0].id = 0;
    tree.base[0].parent = ROOT;
    tree.base[0].score = 0;

    
    while(!Get_finish_front())//如果没有点下finish键
    {
        if(Get_next_front())//如果按下了next键
        {
            get_score = Get_one_title_score_from_front(big_title_num);
            //在邻接表中填入数据
            //TransIntoGraph(g,get_score,big_title_num);
            root_pos = Create_Child_Tree_mode1(tree,get_score,root_pos,big_title_num);
            big_title_num++;//大题数+1
            
        }
    }
    tree.root_capicity = big_title_num;

}

/*
参数：id是根节点的位置
*/

int Create_Child_Tree_mode1(ScoreTree tree,Data_front_one_title *get_score,int root_pos,int root_id)
{
    int i,j;
    int num;
    int temp_pa;
    int new_root_pos;

    Child *p;

    num = get_score->len;
    tree.capicity += num;   //计算得到数组实际的长度
    tree.root[root_id] = root_pos;    //root_id是数组下标，从0开始
    new_root_pos = root_pos+num;
    
    //创建双亲数组
    for (i = root_pos, j = i-root_pos; i < new_root_pos, j < num; i++, j++)
    {
        tree.base[i].id = i;
        tree.base[i].firstchild = (Child *)malloc(sizeof(Child));
        tree.base[i].firstchild->next = NULL;
        tree.base[i].score = get_score->small[j].score;
        tree.base[i].parent = get_score->small[j].parent; 
    }
    Calculate_score_mode1(tree,root_pos,new_root_pos);
    return new_root_pos;
}

/*
    头插法创建孩子链表，因为是从后往前扫描，所以链表的顺序是
    题目题号的顺序
    并且得到了每一题的总分
    */
void Calculate_score_mode1(ScoreTree tree,int start,int end)
{
    int i;
    int temp_pa;
    Child *p;

    for (i = end-1; i >= start; i--)
    {
        p = (Child *)malloc(sizeof(Child));
        p->child = tree.base[i].id;     //数组下标
        p->next = NULL;
        temp_pa = tree.base[i].parent;
        
        tree.base[temp_pa].score += tree.base[p->child].score;//将孩子的分数给双亲
        
        p->next = tree.base[temp_pa].firstchild->next;
        tree.base[temp_pa].firstchild->next = p;
    }

}

void Get_Tree_num(ScoreTree tree,TotalTitleNum totalnum)
{
    int i;
    int j;
    int k;
    int bigNum = tree.root_capicity;    //大题数
    int smlNum;
    
    //把结构体数组初始化为0
    memset (&totalnum,0,sizeof(totalnum));
    
    totalnum.capicity_big = bigNum;

    for (i = 0; i < bigNum; i++)
    {
        smlNum = stu_score[i].len;
        for (j = 0; j < smlNum; j++)
        {
           if(stu_score[i].small[j].flag == 2)
           {
                totalnum.num_s_ss[i].snum_capicity++;
           } 
           else if(stu_score[i].small[j].flag == 3)
           {
               k = stu_score[i].small[j].parent-1;//编号从0开始为了统一
               totalnum.num_s_ss[i].ssnum[k]++;
           }
        }
    }
}
/*
功能: 通过第一个学生的成绩，记忆登分模板
      即直接创建了一个正确大小的数组
*/
void Create_Tree_mode1_templete(ScoreTree tree,ALgraph g,TotalTitleNum totalnum)
{
    int i,j;
    int snum,ssnum,bnum;
    g.capicity_base = totalnum.capicity_big;   //大题数
    //创建了大题数组
    bnum = g.capicity_base;
    g.base = (sGraphNode *)calloc(bnum,sizeof(sGraphNode));
    
    //创建小题数组
    for(i = 0; i < bnum; i++)
    {
        g.base[i].capicity_sbase = totalnum.num_s_ss[i].snum_capicity;
        snum = g.base[i].capicity_sbase;    //第i个大题有的小题数
        g.base[i].sbase = (sGraphNode *)calloc(snum,sizeof(sGraphNode));
        //创建小小题数组
        for (j = 0; j < snum; j++)
        {
            g.base[i].sbase[j].capicity_ssbase = totalnum.num_s_ss[i].ssnum[j];
            ssnum = g.base[i].sbase[j].capicity_ssbase;
            g.base[i].sbase[j].ssbase = (ssGraphNode *)calloc(ssnum,sizeof(ssGraphNode));
        }
         
    }
}
//<---------------------------------END-MODE1-------------------------------------------->

//<---------------------------------STATR-MODE2-------------------------------------------->
void Init_Tree_mode2(ScoreTree tree2,int type_tree,int bigID,int stuNum)
{
    int root_pos = 0;   //第i道题：第i个学生大题登分处 一 的位置;下标从0开始
    int cnt = 0;        //第i个学生
    Data_front_one_title *getGrade; //一个学生一题的分数  

    tree2.base = (TreeNode *)calloc(stuNum*(MAX_STOTAL_NUM),sizeof(TreeNode));
    tree2.type = type_tree;
    tree2.capicity = 1; //数组中的实际元素

    while (!Get_finish_front())
    {
        if(Get_next_front())
        {
            getGrade = Get_one_title_score_from_front(cnt);
            root_pos = Create_Child_Tree_mode2(tree2,getGrade,root_pos,cnt);
            cnt++;
        }
    }
    

}

int Create_Child_Tree_mode2(ScoreTree tree2,Data_front_one_title *getGrade,int root_pos,int root_id)
{
    
}