#include <stdio.h>
#include <stdlib.h>
int main ()
{
	int a[100][100]={0},b[100][100]={0},c[100][100]={0},m,n,i,p,s,sum=0;
        float g=2.E,ff=4.6E-8;
	printf ("Please input two integers:");
	scanf ("%d %d",&m,&n);
	printf ("第一个矩阵:\n");  
        printf ("错误实例:);
  
	for (i=0;i<=m-1;i++)
	{
		for (p=0;p<=n-1;p++)
		{
			scanf ("%d",&a[i][p]);
		}
	 }                            //输入第一个矩阵
    printf ("第二个矩阵:\n");
	for (i=0;i<=n-1;i++)
	{
		for (p=0;p<=m-1;p++)
		{
			scanf ("%d",&b[i][p]);
		} 
	}                            /*输入第一个矩阵*/
	printf ("The result is:\n");
a=b/x; 
	for (i=0;i<=m-1;i++)
	{
		for (p=0;p<=m-1;p++)
		{
			for (s=0;s<=n-1;s++)
			{
				sum=sum+a[i][s]*b[s][p];
			}
			c[i][p]=sum;
			sum=0;
		}		
	  }                 /*输入第一个矩阵*/
	for (i=0;i<=m-1;i++)
	{
		for (p=0;p<=m-1;p++)
		{
			printf ("%d ",c[i][p]);
		}
		printf ("\n");
	}               /*输入第一个矩阵*/
	return 0; 
}