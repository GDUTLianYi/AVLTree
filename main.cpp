#include<stdio.h>
#include<string.h>
#include <time.h>
#include<stdlib.h>
#define FALSE 0
#define TRUE 1
#define LH +1
#define EH 0
#define RH -1
typedef int Status;
typedef  int  KeyType;


//二叉链表存储结构
typedef struct BBSTNode{
    KeyType key;//键值，没有使用RcdType类型，为了方便在anyview显示树结构
    int bf;   //平衡因子
    struct BBSTNode *lchild,*rchild;
}*BBSTree;
void InitAVL(BBSTree &T);
void DestroyAVL(BBSTree T);
void R_Rotate(BBSTree &p);
void L_Rotate(BBSTree &p);
void LeftBalance(BBSTree &T);
void RightBalance(BBSTree &T);
Status InsertAVL(BBSTree &T,KeyType e,Status &taller);
Status balance_for_DeleteAVL(BBSTree &T,KeyType k,Status &isshort);
Status DeleteAVL(BBSTree &T,KeyType k);
BBSTree SearchAVL(BBSTree T,KeyType key);
int CountAVL(BBSTree T);
int DepthAVL(BBSTree T);
void  PreorderAVL(BBSTree T);
void InorderAVL(BBSTree T);
void  PostorderAVL(BBSTree T);
int check_height(BBSTree &T,int &flag);
int check_key(BBSTree &T,KeyType &key,int &num,int &sign);
int CheckAVL(BBSTree &T);
void PrintAVL(BBSTree T,int dep,int gab);
void build_process(int l,int r,int rt,BBSTree e,KeyType *a,KeyType *b);
void BuildAVL(BBSTree &T,KeyType * inorder,KeyType * preorder,int n);
int setbf(BBSTree &T);
void Special_InorderAVL(BBSTree T,KeyType *t,int &num);
Status  Special_buildAVL(BBSTree &T,KeyType *t,int l,int r);
void  Merge_Sort(KeyType  *t1,int num1,KeyType *t2,int num2,KeyType *t,int &num);
BBSTree MergeAVL(BBSTree &T1,BBSTree &T2);
int div(KeyType *t,int l,int r,KeyType x);
Status SplitAVL(BBSTree T,KeyType  x,BBSTree &left,BBSTree &right);

void InitAVL(BBSTree &T){
    T=NULL;
}

void DestroyAVL(BBSTree T){
    if(T==NULL) return ;
    DestroyAVL(T->lchild);
    DestroyAVL(T->rchild);
    free(T);
}
void R_Rotate(BBSTree &p){
    //printf("%d R_Rotate \n",p->data.key);
    BBSTree lc;
    lc=p->lchild;
    p->lchild=lc->rchild;
    lc->rchild=p;
    p=lc;
}

void L_Rotate(BBSTree &p){
    //printf("%d L_Rotate \n",p->data.key);
    BBSTree rc;
    rc=p->rchild;
    p->rchild=rc->lchild;
    rc->lchild=p;
    p=rc;
}


void LeftBalance(BBSTree &T){
    BBSTree lc,rd;
    lc=T->lchild;
    switch(lc->bf ){
        case LH:T->bf=lc->bf=EH;R_Rotate(T);break;
        case EH:T->bf=LH ; lc->bf=RH; R_Rotate(T);break;
        case RH:
            rd=lc->rchild;
            switch(rd->bf ){
                case LH:T->bf=RH; lc->bf=EH;break;
                case EH:T->bf=lc->bf=EH;break;
                case RH:T->bf=EH;lc->bf=LH;break;
            }
            rd->bf=EH;
            L_Rotate(T->lchild );
            R_Rotate(T);
            break;
    }
}


void RightBalance(BBSTree &T)
{
      BBSTree rc,ld;
      rc=T->rchild;
      switch(rc->bf){
      case RH:  rc->bf=EH;T->bf=EH;L_Rotate(T); break;
      case EH:  rc->bf=LH;T->bf=RH;L_Rotate(T); break;
      case LH: ld=rc->lchild;
      switch(ld->bf){
        case LH: T->bf=EH; rc->bf=RH;break;
        case EH: T->bf=EH; rc->bf=EH; break;
        case RH: rc->bf=EH; T->bf=LH; break;
       }
         ld->bf=EH;
         R_Rotate(T->rchild);
         L_Rotate(T);
         break;
      }
}




Status InsertAVL(BBSTree &T,KeyType e,Status &taller){
    if(NULL==T){
        T=(BBSTree)malloc(sizeof(BBSTNode));
        if(T==NULL) return FALSE;
        T->key=e;
        T->bf=EH; T->lchild=NULL; T->rchild=NULL;
        taller=1;
    }else if(e==T->key){
         taller=FALSE;
         return FALSE;
    }else if(e<T->key){
           if(FALSE==InsertAVL(T->lchild,e,taller)) return FALSE;
         if(TRUE==taller){
             switch(T->bf ){
                 case LH:
                 LeftBalance(T);taller=FALSE;break;//printf("lb..\n");
                 case EH:
                 T->bf=LH;taller=TRUE ;break;
                 case RH:
                 T->bf=EH;taller=FALSE;break;
             }
         }
    }else {
        if(FALSE==InsertAVL(T->rchild,e,taller)) return FALSE;
        if(TRUE==taller){
            switch(T->bf ){
                case LH:
                T->bf=EH; taller =FALSE ;break;
                case EH:
                T->bf=RH; taller =TRUE ;break;
                case RH:
                RightBalance(T); taller=FALSE;break;//printf("rb..\n");
            }
        }
    }
    return TRUE;
}





Status balance_for_DeleteAVL(BBSTree &T,KeyType k,Status &isshort){
    if(T==NULL){
    return FALSE;
    }else if(T->key==k){
           BBSTree p;
           p=T;
        if(T->lchild==NULL&&T->rchild==NULL){
            T=NULL;
        }else if(T->lchild!=NULL){
            T=T->lchild;
        }else {
            T=T->rchild;
        }
        isshort=TRUE;
        free(p);
    }else if(T->key>k){
       if(FALSE==balance_for_DeleteAVL(T->lchild,k,isshort)) return FALSE;
        if(isshort==TRUE){
            switch(T->bf){
                case LH: T->bf=EH; isshort=TRUE; break;
                case EH: T->bf=RH; isshort=FALSE; break;
                case RH: {
                             if(T->rchild->bf==EH)
                             isshort=FALSE;
                             else
                             isshort=TRUE;
                                RightBalance(T);
                             break;
                         }
            }
        }
    }else if(T->key<k){
       if(FALSE==balance_for_DeleteAVL(T->rchild,k,isshort)) return FALSE;
        if(isshort==TRUE){
            switch(T->bf){
                case LH: {
                            if(T->lchild->bf==EH)
                            isshort=FALSE;
                            else
                            isshort=TRUE;
                                  LeftBalance(T);
                            break;
                         }
                case EH: T->bf=LH; isshort=FALSE; break;
                case RH: T->bf=EH; isshort=TRUE; break;
            }
        }
    }
    return TRUE;
}

Status DeleteAVL(BBSTree &T,KeyType k){
    BBSTree x=T;
   while(x!=NULL){
     if(x->key==k){
        break;
     }else if(x->key>k){
        x=x->lchild;
     }else{
        x=x->rchild;
     }
   }
    if(x==NULL) return FALSE;
    BBSTree prenode;
    if(x->lchild!=NULL && x->rchild!=NULL){
        prenode=x->lchild;
        while(prenode->rchild!=NULL){
            prenode=prenode->rchild;
        }

     KeyType tmp;
     tmp=prenode->key;
     int is_short;
     is_short=0;
     balance_for_DeleteAVL(T,prenode->key,is_short);
     x->key=tmp;
    }else{
        int is_short=0;
    balance_for_DeleteAVL(T,x->key,is_short);
    }
    return TRUE;
}

BBSTree SearchAVL(BBSTree T,KeyType key){
     if(T==NULL)  return NULL;
     if(T->key<key) {
         return SearchAVL(T->rchild,key);
     }else if(T->key>key){
        return SearchAVL(T->lchild,key);
     }else {
        return T;
     }
}

int CountAVL(BBSTree T){
    if(T==NULL) return 0;
    return CountAVL(T->lchild)+CountAVL(T->rchild)+1;
}

int DepthAVL(BBSTree T){
    if(T==NULL) return 0;
    return DepthAVL(T->lchild)>DepthAVL(T->rchild)? DepthAVL(T->lchild)+1:DepthAVL(T->rchild)+1;
}

void  PreorderAVL(BBSTree T){
        if(T==NULL) return ;
    printf("%4d",T->key);
    PreorderAVL(T->lchild);
    PreorderAVL(T->rchild);
}
void InorderAVL(BBSTree T){
        if(T==NULL) return ;
    InorderAVL(T->lchild);
    printf("%4d",T->key);
    InorderAVL(T->rchild);
}
void  PostorderAVL(BBSTree T){
        if(T==NULL) return ;
    PostorderAVL(T->lchild);
    PostorderAVL(T->rchild);
    printf("%4d",T->key);
}



int check_key(BBSTree &T,KeyType &key,int &num,int &sign){
   if(T==NULL) return 0;
   if(!sign) return 0;
   check_key(T->lchild,key,num,sign);
   if(num==0){
    key=T->key;
   }else{
     if(T->key<=key){
      sign=0;
     }else{
      key=T->key;
     }
   }
   check_key(T->rchild,key,num,sign);
   return 0;
}
int check_height(BBSTree &T,int &flag){
    if(T==NULL) return 0;
    if(!flag)   return 0;
    int L,R,high;
    L=check_height(T->lchild,flag);
    R=check_height(T->rchild,flag);

    high=L-R;
    if(high<0) high=-high;
    if(high>=2)  flag=0;
    return L>R? L+1:R+1;
}
int CheckAVL(BBSTree &T){
   int flag=1;
   check_height(T,flag);
   if(flag==0){
    printf("平衡树失衡 高度问题\n");
   }
   int sign=1,num=0;
   KeyType key;
   check_key(T,key,num,sign);
   if(sign==0){
   printf("平衡树失衡 权值问题");
   }
   return flag&sign;
}
void PrintAVL(BBSTree T,int dep,int gab){
    if(T==NULL){
        return ;
    }
     int tmp;
     tmp =dep+gab;
    PrintAVL(T->rchild,tmp,gab);

      for(int i=0;i<dep;i++) printf(" ");
      printf("%d(%d)\n",T->key,T->bf);

    PrintAVL(T->lchild,tmp,gab);
}

void build_process(int l,int r,int rt,BBSTree e,KeyType *a,KeyType *b)
{
  //   printf("%d %d %d %c\n",l,r,rt,e->val);
   for(int i=l;i<=r;i++)
   {
      if(a[i]==b[rt])
      {
        if(l<=i-1)
        {
           BBSTree le;
          le=(BBSTree)malloc(sizeof(BBSTNode));
           le->key =b[rt+1]; le->lchild =NULL ; le->rchild =NULL;
           e->lchild =le;
            if(l<i-1)
           build_process(l,i-1,rt+1,le,a,b);
        }

         if(i+1<=r)
        {
            BBSTree re;
           re=(BBSTree)malloc(sizeof(BBSTNode));
            re->key  =b[rt+i-l+1]; re->lchild =NULL ; re->rchild =NULL;
            e->rchild =re;
            if(i+1<r)
            build_process(i+1,r,rt+i-l+1,re,a,b);
        }
        return ;
      }
   }
}
int setbf(BBSTree &T){
    if(T==NULL) return 0;

    int L,R,bf;
    L=setbf(T->lchild);
    R=setbf(T->rchild);

    bf=L-R;
    if(bf==0) T->bf =EH;
    else if(bf>0) T->bf=LH;
    else T->bf=RH;
return L>R? L+1:R+1;
}
void BuildAVL(BBSTree &T,KeyType * inorder,KeyType * preorder,int n){
    T=NULL;
    if(n<=0) return ;

      KeyType *a,*b;
      a=(KeyType *)malloc(sizeof(KeyType)*(n+1));
      b=(KeyType *)malloc(sizeof(KeyType)*(n+1));
      int i;
      if(a==NULL||b==NULL){
     printf("内存不够, 溢出");
     return;
     }

      for(i=1;i<=n;i++){
          a[i]=inorder[i-1];
          b[i]=preorder[i-1];
      }
     T=(BBSTree)malloc(sizeof(BBSTNode));
        T->key=b[1];
        T->lchild=NULL; T->rchild=NULL;
        build_process(1,n,1,T,a,b);
        //delete[]a;
       // delete[]b;
        setbf(T);



        if(!CheckAVL(T)){
        printf("新建立的树不平衡，先序中序序列可能有误\n");
        }else{
        printf("成功建立一棵平衡二叉树\n");
        }
}


void Special_InorderAVL(BBSTree T,KeyType *t,int &num){

    if(T==NULL) return ;
    Special_InorderAVL(T->lchild,t,num);
    t[++num]=T->key;
    Special_InorderAVL(T->rchild,t,num);

}
Status  Special_buildAVL(BBSTree &T,KeyType *t,int l,int r){

           if(l>r) return TRUE;
           //  printf("%d %d\n",l,r);
        int mid=(l+r)/2;
        T=(BBSTree)malloc(sizeof(BBSTNode));
        if(T==NULL ) {
            printf("内存不足,Special_buildAVL中-malloc操作失败\n");
            return FALSE;
        }
        T->key=t[mid];
        T->lchild=NULL;
        T->rchild=NULL;
        return Special_buildAVL(T->lchild,t,l,mid-1)&&Special_buildAVL(T->rchild,t,mid+1,r);
}
void  Merge_Sort(KeyType  *t1,int num1,KeyType *t2,int num2,KeyType *t,int &num){

    int i,j;
    i=1;j=1,num=0;

    while(i<=num1&&j<=num2){
       if( t1[i]<=t2[j] && (num==0||t1[i]!=t[num]) ){
             t[++num]=t1[i++];
       }else if(t1[i]>t2[j] && (num==0||t2[j]!=t[num]) ){
             t[++num]=t2[j++];
       }else if(t2[j]==t[num]){
             j++;
       }else if(t1[i]==t[num]){
             i++;
       }
    }
    while(i<=num1)  t[++num]=t1[i++];
    while(j<=num2)  t[++num]=t2[j++];
}

BBSTree MergeAVL(BBSTree &T1,BBSTree &T2){

    int num1,num2,num;
    KeyType *t1,*t2,*t;

    num1=CountAVL(T1)+1;  //树T1的结点总数
    num2=CountAVL(T2)+1;  //树T2的结点总数


    t1=(KeyType *) malloc (num1*sizeof(KeyType ));
    t2=(KeyType *) malloc (num2*sizeof(KeyType ));
    if(t1==NULL||t2==NULL){
       printf("操作  MergeAVL- malloc 分配内存失败");
    }

    num1=0;
    Special_InorderAVL(T1,t1,num1);  // 取出T1中序序列存储在数组t1中
    num2=0;
    Special_InorderAVL(T2,t2,num2);  // 取出中序T2中序序列存储在数组t2中

    num=num1+num2+1;
    t=(KeyType *) malloc (num*sizeof(KeyType ));
    if(t==NULL){
       printf("操作 t=(BBSTree *) malloc (num*sizeof(BBSTree ))分配内存失败");
    }


    Merge_Sort(t1,num1,t2,num2,t,num); // 合并t1,t2，并去掉重复的元素

    // 构造合并树
    BBSTree T; InitAVL(T);
    if(Special_buildAVL(T,t,1,num)==FALSE){
        printf("建树过程中发生错误\n");
    }
    setbf(T);

    //检查树

    if(!CheckAVL(T)){
        printf("平衡树失衡，合并平衡树失败\n");
         DestroyAVL(T);
    }

  //  delete[]t1;
  //  delete[]t2;
  //  delete[]t;

    return  T;
}

//  二分折半查找 x 在序列中的位置， x以左的序列用于建左树，以右建右树
int div(KeyType *t,int l,int r,KeyType x){
    int mid,pos=1;
    while(l<=r){
        mid=(l+r)/2;
        if(t[mid] <x){
          pos=mid;
          l=mid+1;
        }else if(t[mid]>x){
          r=mid-1;
        }else {
         return mid;
        }
    }
    return pos;
}

Status SplitAVL(BBSTree T,KeyType  x,BBSTree &left,BBSTree &right){

    left=NULL;right=NULL;
    KeyType *t;
    int num;
    num=CountAVL(T)+1;
    t=( KeyType *)malloc(num*sizeof(KeyType));

    if(t==NULL){
        printf("内存不足，SplitAVL-malloc操作执行失败\n");
        return FALSE;
    }
    num=0;
    Special_InorderAVL(T,t,num);

    int pos=div(t,1,num,x);
    int l,r;
    if(t[pos]==x){
       l=pos-1;
       r=pos+1;
    }else if(t[pos]<x){
        l=pos;
        r=pos+1;
    }else{
        l=pos-1;
        r=pos;
    }


    Special_buildAVL(left,t,1,l);
    setbf(left);
    if(!CheckAVL(left)){
        printf("构造的左子树失衡\n");
    }

    Special_buildAVL(right,t,r,num);
    setbf(right);
     if(!CheckAVL(right)){
        printf("构造的右子树失衡\n");
    }
  //  delete []t;
    return TRUE;
}

void  test_build(){
   int n,i;
    BBSTree T;
    printf("请输入要建立的树的节点数：");
    scanf("%d",&n);
    printf("请输入树的关键值的《先序》序列\n ");
    printf("(%d 个整数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *preorder1;
    preorder1=(KeyType *)malloc(sizeof(KeyType)*(n));
    if(preorder1==NULL){
        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&preorder1[i]);
    }
     printf("请输入树的关键值的《中序》序列\n ");
    printf("(%d 个数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *inorder1;
    inorder1=(KeyType *)malloc(sizeof(KeyType)*n);
    if(inorder1==NULL){

        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&inorder1[i]);
    }

    BuildAVL(T,inorder1,preorder1,n);
     // delete[] preorder1;
  //  delete[] inorder1;
    DestroyAVL(T);
}

void test_merge(){
    int n,i,m;
    BBSTree T1,T2;
    printf("请输入第一棵树的节点数：");
    scanf("%d",&n);
    printf("请输入第一棵树关键值的《先序》序列\n ");
    printf("(%d 个整数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *preorder1;
    preorder1=(KeyType *)malloc(sizeof(KeyType)*(n));
    if(preorder1==NULL){
        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&preorder1[i]);
    }
     printf("请输入第一棵树关键值的《中序》序列\n ");
    printf("(%d 个数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *inorder1;
    inorder1=(KeyType *)malloc(sizeof(KeyType)*n);
    if(inorder1==NULL){

        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&inorder1[i]);
    }

    BuildAVL(T1,inorder1,preorder1,n);

    printf("请输入第二棵树的节点数：");
    scanf("%d",&m);
    printf("请输入第二棵树关键值的《先序》序列\n ");
    printf("(%d 个数以空格隔开，最后按回车完成输入)\n",m);
    KeyType *preorder2;
    preorder2=(KeyType *)malloc(sizeof(KeyType)*m);
    if(preorder2==NULL){
        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<m;i++){
        scanf("%d",&preorder2[i]);
    }
     printf("请输入第二棵树关键值的《中序》序列\n ");
    printf("(%d 个数以空格隔开，最后按回车完成输入)\n",m);
    KeyType *inorder2;
    inorder2=(KeyType *)malloc(sizeof(KeyType)*m);
    if(inorder2==NULL){

        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<m;i++){
        scanf("%d",&inorder2[i]);
    }

    BuildAVL(T2,inorder2,preorder2,m);

   // delete[] inorder1;
  //  delete[] inorder2;
   // delete[] preorder1;
   // delete[] preorder2;
    BBSTree T;
    T=MergeAVL(T1,T2);

    printf("这是第一棵树\n");
    PrintAVL(T1,0,5);
    printf("这是第二棵树\n");
    PrintAVL(T2,0,5);
    printf("这是合并后的树\n");
    PrintAVL(T,0,5);

   /* PreorderAVL(T);
    printf("\n");
    InorderAVL(T);
     printf("\n");
    */
    DestroyAVL(T1);
    DestroyAVL(T2);
    DestroyAVL(T);
}

void test_split(){

    int n,i;
    BBSTree T;
    printf("请输入要分裂的树的节点数：");
    scanf("%d",&n);
    printf("请输入要分裂的树关键值的《先序》序列\n ");
    printf("(%d 个整数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *preorder1;
    preorder1=(KeyType *)malloc(sizeof(KeyType)*(n));
    if(preorder1==NULL){
        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&preorder1[i]);
    }
     printf("请输入第一棵树关键值的《中序》序列\n ");
    printf("(%d 个整数以空格隔开，最后按回车完成输入)\n",n);
    KeyType *inorder1;
    inorder1=(KeyType *)malloc(sizeof(KeyType)*n);
    if(inorder1==NULL){

        printf("操作中止 malloc 分配内存失败\n");
        return ;
    }
    for(i=0;i<n;i++){
        scanf("%d",&inorder1[i]);
    }

    BuildAVL(T,inorder1,preorder1,n);

    BBSTree T1,T2;
    int x;
    printf("请输入分裂值x:");
    scanf("%d",&x);
    SplitAVL(T,x,T1,T2);
    printf("这是原树\n");
    PrintAVL(T,0,5);
    printf("这是第一棵树\n");
    PrintAVL(T1,0,5);
    printf("这是第二棵树\n");
    PrintAVL(T2,0,5);
   // delete[] preorder1;
  //  delete[] inorder1;
    DestroyAVL(T1);
    DestroyAVL(T2);
    DestroyAVL(T);

}



  //测试函数 一
int main(){
    freopen("D:\\123.txt","r",stdin);
    BBSTree t;
    InitAVL(t);
    Status tall;
    int op;
    int key;

       printf(" -1----结束\n");
       printf(" 0-----插入\n");
       printf(" 1-----删除\n");
       printf(" 2-----查找\n");
       printf(" 3-----求深度\n");
       printf(" 4-----求树节点数目\n");
       printf(" 5-----凹入表打印树结构\n");
       printf(" 6-----用中序先序序列建立一棵平衡树\n");
       printf(" 7-----合并两棵平衡树\n");
       printf(" 8-----分裂一棵平衡树，使得一个值都大于x,另一小于x\n");
        printf(" 请输入操作:") ;
    while(scanf("%d",&op)){
     if(op==-1) break;
     switch(op){
     case -1: break;
     case 0:  {
         printf("请输入你要插入的整数:");
         tall=FALSE;
         scanf("%d",&key);

         InsertAVL(t,key,tall);
          printf("打印树形结构\n");
       PrintAVL(t,0,5);
         break;
        }

       case 1:{
          printf("请输入你要删除的整数:");
          scanf("%d",&key);

          DeleteAVL(t,key);
           printf("打印树形结构\n");
       PrintAVL(t,0,5);
          break;
       }

       case 2:{
          printf("请输入你要查找的整数:");
          scanf("%d",&key);

          if(SearchAVL(t,key)==NULL){
          printf("不存在\n");
          }else{
          printf("查找成功 \n");
          }
           printf("打印树形结构\n");
       PrintAVL(t,0,5);
          break;
       }

       case 3:{
           printf("树深度: %d\n",DepthAVL(t));
           break;
       }
       case 4:{
           printf("节点数 : %d\n",CountAVL(t));
           break;
       }
       case 5:{
           PrintAVL(t,0,5);
           break;
       }
       case 6:{
           test_build();
           break;
       }
       case 7:{
           test_merge();
           break;
       }
       case 8:{
           test_split();
           break;
       }


    }

        printf("\n\n");

        printf(" -1----结束\n");
        printf(" 0-----插入\n");
        printf(" 1-----删除\n");
        printf(" 2-----查找\n");
        printf(" 3-----求深度\n");
        printf(" 4-----求树节点数目\n");
        printf(" 5-----凹入表打印树结构\n");
        printf(" 6-----用中序先序序列建立一棵平衡树\n");
        printf(" 7-----合并两棵平衡树\n");
        printf(" 8-----分裂一棵平衡树，使得一个值都大于x,另一小于x\n");
        printf(" 请输入操作:") ;

}
     PrintAVL(t,0,5);

    return 0;
}





  //测试函数 二

/*
  int main(){
    BBSTree T; InitAVL(T);
    int rd,flag,key,tall,i,sign;
    flag=1;
    sign=1;
    //anyview 不能用时间种子
   // srand(time(NULL));
    for( i=0;i<100;i++){    //进行100轮随机测试
    for( rd=0;rd<1000;rd++){
        key=rand()%500;
    // printf("进行插入的元素为：%d\n",key);
    //产生0-499的随机元素插入到AVLTree树中
        tall=FALSE;
        InsertAVL(T,key,tall);
        check(T,flag); //判断每个节点的左子树和右子树高度差
          //   PrintAVL(T,0,5); //打印树形结构
     }
        for(rd=0;rd<5000;rd++){
        if(T==NULL) break;
        key=rand()%500;
      // printf("尝试删除的元素为：%d\n",key);
        //产生0-499的随机元素插入到AVLTree树中
         DeleteAVL(T,key);
        check(T,sign);
           // PrintAVL(T,0,5);  //打印树形结构
    }
      if(i%1000==0){
          printf("%d %d\n",flag,sign);
      }
  }
    if(flag)
    printf("AVLTree 树在插入过程一直处于平衡状态\n");
    else
    printf("AVLTree 树在插入过程中失衡\n");

     if(sign)
     printf("AVLTree 树在删除过程一直处于平衡状态\n");
     else
     printf("AVLTree 树在删除过程中失衡\n");


    return 0;
}
*/



/*
 int  main(){
     //测试组数1
      KeyType t1[]={10,20,30,40,50,60,70};
      KeyType t2[]={15,25,35,45,55,65,75};

     BBSTree T1,T2,T;

     InitAVL(T1);
     InitAVL(T2);
         int taller;
     for(int i=0;i<7;i++){
          taller=0;
        InsertAVL(T1,t1[i],taller);
     }
          for(int i=0;i<7;i++){
          taller=0;
        InsertAVL(T2,t2[i],taller);
     }
     T=MergeAVL(T1,T2);

     PrintAVL(T,0,5);

      BBSTree T1,T2,T;
      InitAVL(T1);InitAVL(T2);
      for(int i=0;i<27000;i++){

        int u=i;
       int v=rand()%1000000;
      int   taller=0;
         InsertAVL(T1,v,taller);
         taller=0;
        InsertAVL(T2,u,taller);
        T=MergeAVL(T1,T2);
        DestroyAVL(T);
    }


    // PrintAVL(T,0,5);

 }
  */
/*
 int main(){
          KeyType t[]={10,20,30,40,50,60,70};
          BBSTree T1,T2,T;

     InitAVL(T);
         int taller;
     for(int i=0;i<7;i++){
          taller=0;
        InsertAVL(T,t[i],taller);
     }
     KeyType x;
     x=30;
     SplitAVL(T,x,T1,T2);
     PrintAVL(T1,0,5);
     printf("\n\n");
     PrintAVL(T2,0,5);
     return 0;
 }
*/

