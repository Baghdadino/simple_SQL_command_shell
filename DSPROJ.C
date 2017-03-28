#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<STDLIB.H>
#include<ctype.h>

struct cell
{
	int colorder;
	int roworder;
	char value[20];
};

struct meta
{
	int colorder;
	char name[20];
};

void createtable(void);
void insertrow(void);
void deleterow(void);
void updaterow(void);
void select(void);

char * command;

//char *command="create table student(id int,name char(25),salary int,car char(10));";

int main()
{

	char *tcommand;
	char *p;
	long length;
	FILE *fp=fopen("c:\\sqlcom.txt","r");

	clrscr();
	flushall();



	if(fp)
	{
		fseek(fp,0,SEEK_END);
		length=ftell(fp);
		//printf("%l",length);
		fseek(fp,0,SEEK_SET);  //== rewind(fp);

   //length+1 .. theadded one is for the null character to get rid of the rubbish data
		command=(char*)malloc((length+1)*sizeof(char));
		if(command)
		{
			//fread(command,1,length,fp);
			fgets(command,length+1,fp);

		}
		fclose(fp);
	}



	printf("command is %s\n",command);


	//a problem I forgot

	strcpy(tcommand,command);
	p=strtok(tcommand," ");
	printf("%s %s \n",p,tcommand);



	if(!stricmp(p,"create"))
		createtable();
	else if(!stricmp(p,"insert"))
		insertrow();
	else if(!stricmp(p,"update"))
		updaterow();

	else if(!stricmp(p,"delete"))
		deleterow();
	else if(!stricmp(p,"select"))
		select();


	free(command);


  getch();

  return 0;
}


//----------------------------  create table ------------------------

void createtable(void)
{
	int i=0;
	char *ptr=NULL;
	int comma=0;
	int colno=0,count=0;
	char type[5];

	//char *keyword;

	char file[30]="c:\\";
	char filemeta[40]="c:\\";
	struct meta *createmeta=NULL;
	struct meta n[4];
	//int numch=1; //num of char col
	FILE * table=NULL ;
	FILE *metatable=NULL;




 // printf("String \"%s\" \nis split into tokens:\n",command);

	while(i<strlen(command))
	{
		if(command[i]==',')
		{
			comma++;
		}
		i++;
	}
	colno=++comma;
	//printf("%d \n",colno);

	ptr=strtok(command," ");
	if(!stricmp(ptr,"create"))
	{
		printf("%s\n ",ptr);  //call create function
		createmeta=(struct meta*)malloc((colno+1)*sizeof(struct meta));
		while(ptr!=NULL)
		{
			ptr=strtok(NULL," ( ");
			count++;
			if(count==2)
			{
				createmeta[0].colorder=colno;
				strcpy(createmeta[0].name,ptr);
				//strcpy(createmeta[0].type,"tab");
				//printf("%d  %s  %s  vvvvv\n ",createmeta[0].num,createmeta[0].type,createmeta[0].col_name);
				strcat(file,createmeta[0].name);
				strcat(file,".txt");
				strcat(filemeta,createmeta[0].name);
				strcat(filemeta,"m.txt");
			      //	printf("%s %s \n ",file,filemeta);


			}

			else
			{
				if(count>2)
				{
					for(i=1;i<colno+1;i++)
					{
						//printf("%d \t %s  1111\n ",i,ptr);
						strcpy(createmeta[i].name,ptr);
						ptr=strtok(NULL," (,);");
					     //	printf(" %s  1111\n ",ptr);

						strcpy(type,ptr);
						if(!stricmp(type,"char"))
						{
							ptr=strtok(NULL,"( ,)");
						     //	printf(" %s  222\n ",ptr);
						       //	numch=atoi(ptr);
						}
						createmeta[i].colorder=i;
						//numch=1;
						ptr=strtok(NULL," (,);");

					    //	printf("\n%d  %s  vvvvv\n ",createmeta[i].colorder,createmeta[i].name);
					}
				}
			}


		}

		metatable=fopen(filemeta,"w");
		if(metatable)
		{
			for(i=0;i<colno+1;i++)
			{
				fprintf(metatable,"%d %s \n",createmeta[i].colorder,createmeta[i].name);
			}
			printf("meta table created\n");

			table=fopen(file,"w");
			if(table)
			{
				printf("table created\n");
			}
			else
				printf("table not created\n");
			fclose(table);
		}
		else
			printf("meta table not created\n");
		fclose(metatable);

		metatable=fopen(filemeta,"r");
		for(i=0;i<colno+1;i++)
		{
			fscanf(metatable,"%d %s",&n[i].colorder,n[i].name);
		}
		for(i=0;i<=colno;i++)
		{
			printf("\n %d  %s  vvvvv\n ",n[i].colorder,n[i].name);
		}

		fclose(metatable);

	}
	free(createmeta) ;

}


//---------------------------------- insert row ----------------------------


void insertrow(void)
{

	int flag=0,i=0;


	char *ptr;
	char *ch;
	int comma=0;
	int colno=0,count=0;

	char file[30]="c:\\";
	char filemeta[40]="c:\\";

	FILE * table=NULL ;
       //	FILE * tablemeta=NULL;
	struct cell * row ;
	struct meta * colmeta;
	struct meta firstcolmeta;
	int j;

	int rowno,col;
	struct cell info;



	char **colname,**colvalue;

  // char *command="insert into emp(id,name) values(3,mohamed)";
   //char *command="insert into emp values(20,aly,7000)";
   char *temp;




	strcpy(temp,command);

	ptr=strtok(temp," ;,'()=");

	while(ptr!=NULL && count!=4)
	{
		if(stricmp(ptr,"values")==0 && count==3)
		{
			flag=1;     //if 4th position flag=1
			ptr=NULL;
		}
		ptr=strtok(NULL," ;,()'=");
		count++;
	}
	count=0;

	//printf("\n Flag is %d\n",flag);

	//no.of commas

	while(i<strlen(command))
	{
		if(command[i]==',')
		{
			comma++;
		}
		i++;
	}

	strcpy(temp,command);
	ptr=strtok(temp," ");     //first token


	if(flag)     //colno=++comma    values 4th position
	{
		colno=++comma;
		colvalue=(char**)malloc(colno*sizeof(char*));
		while(ptr!=NULL && count<(colno+4))
		{
			if(count==2)
			{

				strcat(file,ptr);
				strcat(file,".txt");
				strcat(filemeta,ptr);
				strcat(filemeta,"m.txt");
			      //	printf("%s %s \n ",file,filemeta);


				table=fopen(file,"r");
				while(!feof(table))
				{
					fscanf(table,"%d %d %s",&info.roworder,&info.colorder,info.value);
				}
				fclose(table);

				rowno=info.roworder;
				col=info.colorder;

			}

			if(count>2)
			{
				for(i=0;i<colno;i++)
				{
					ptr=strtok(NULL,", ();");
					strcpy(colvalue[i],ptr);
				      //	printf(" %s\n",colvalue[i]);

				}
				count+=colno;
			}
			count++;
			ptr=strtok(NULL,", ();");
		}

		row=(struct cell*)malloc(colno*sizeof(struct cell));
		for(i=0 ; i<=colno ; i++)
		{
			row[i].colorder=i+1;
			row[i].roworder=rowno+1;
			strcpy(row[i].value,colvalue[i]);
		}

		table=fopen(file,"a");
		for(i=0;i<colno;i++)
		{
			fprintf(table,"%d %d %s \n",row[i].roworder , row[i].colorder,row[i].value);

		}
		fclose(table);



    }


   /*

	else         //colno=(comma/2)+1
	{
		colno=(comma/2)+1;
		colname=(char**)malloc(colno*sizeof(char*));
		colvalue=(char**)malloc(colno*sizeof(char*));

		while(ptr!=NULL)
		{
			if(count==2)
			{
				strcat(file,ptr);
				strcat(file,".txt");
				strcat(filemeta,ptr);
				strcat(filemeta,"m.txt");
				printf("%s %s \n ",file,filemeta);

			}

			if(count==3)
			{
				for(i=0;i<colno;i++)
				{
					strcpy(colname[i],ptr);
					printf("column name no. %d is %s\n",i+1,colname[i]);
					ptr=strtok(NULL,", ();");
				}
				count+=colno;
			}

			if(strcmp(ptr,"values")==0)
			{
				ptr=strtok(NULL,", ();");
				for(i=0;i<colno;i++)
				{
					strcpy(colvalue[i],ptr);
					printf(" %d is %s\n",i+1,colvalue[i]);
					ptr=strtok(NULL,", ();");
				}

			}


			count++;
			ptr=strtok(NULL,", ();");
		} //end while

		tablemeta=fopen(filemeta,"r");
		fscanf(tablemeta,"%d %s",&firstcolmeta.colorder,&firstcolmeta.name);
		printf("%s",firstcolmeta.name);
		fclose(tablemeta);

		colmeta=(struct meta*)malloc(firstcolmeta.colorder*sizeof(struct meta));
		for(i=0;i<firstcolmeta.colorder;i++)
		{
			fscanf(tablemeta,"%d %s" , &colmeta[i].colorder,colmeta[i].name);

		}

		for(i=0;i<firstcolmeta.colorder;i++)
		{
			row[i].roworder=rowno+1;
			row[i].colorder=i+1;
			for(j=0;j<colno&&f==0;j++)
			{
				if(!stricmp(colmeta[i].name,colname[j]))
				{
				       strcpy(row[i].value,colvalue[j]);
				       f=1;
				}
			}
			if(f==0)
			{
				strcpy(row[i].value,"NULL" );
			}
		}
		table=fopen(file,"a");
		for(i=0;i<firstcolmeta.colorder;i++)
		{
			fprintf(table,"%d %d %s \n",row[i].roworder , row[i].colorder,row[i].value);

		}
		fclose(table);


	}
	     */


	if(!flag)
	{
		free(colname);
	}
	free(colvalue);
	free(row);

}



//-----------------------------------  delete row -----------------

void deleterow()
{
	char* temp ;
	char*ptr=NULL;

	char file[30]="c:\\";
	char filemeta[40]="c:\\";

	FILE *table ,*tablemeta;

	struct cell** tableinfo;

	char *colname , *colvalue ;
	struct meta * colmeta;
	int colsearch ,rowcount=0;

	int rowno,col;
	struct cell info;



	int i,j;
	int count = 0 ;
	char ch ;


	strcpy(temp,command);
	ptr=strtok(temp, " ");
	while (ptr!=NULL&& count<6)
	{

		if (count==2)
		{

			strcat(file,ptr);
			strcat(file,".txt");
			strcat(filemeta,ptr);
			strcat(filemeta,"m.txt");
		      //	printf("%s %s \n ",file,filemeta);

				table=fopen(file,"r");
				while(!feof(table))
				{
					fscanf(table,"%d %d %s",&info.roworder,&info.colorder,info.value);
				}
				fclose(table);

				rowno=info.roworder;
				col=info.colorder;

		}

		if (count==4)
		{
			strcpy(colname,ptr);
		     //	printf("the condition col is %s\n", colname);
		}

		if (count>4)
		{
			strcpy(colvalue,ptr);
		       //	printf("the condition value is %s\n", colvalue);
		}

		count++;
		ptr=strtok(NULL, " = < > ; " );

	}

	colmeta=(struct meta*)malloc((col+1)*sizeof(struct meta));

	tablemeta=fopen(filemeta,"r");

	for(i=0;i<col+1;i++)
	{
		fscanf(tablemeta,"%d %s" , &colmeta[i].colorder,colmeta[i].name);
		if(!stricmp(colmeta[i].name,colname))
			colsearch=colmeta[i].colorder;

	}
	fclose(tablemeta);

	tableinfo=(struct cell**)malloc(rowno*sizeof(struct cell*));
	for(i=0;i<rowno;i++)
	{
		tableinfo[i]=(struct cell*)malloc(col*sizeof(struct cell));
	}

	//Reading data from file to table

	table=fopen(file,"r");
	for(i=0;i<rowno;i++)
	{
		for(j=0;j<col;j++)
		{
			if(!feof(table))
			{
				//fscanf(fp,"%d %d %s",&info.roworder,&info.colorder,info.value);
				fscanf(table,"%d %d %s",&tableinfo[i][j].roworder,&tableinfo[i][j].colorder,tableinfo[i][j].value);
			}
		}
	}
	fclose(table);

	for(i=0;i<rowno;i++)
	{
		if(!stricmp(tableinfo[i][colsearch-1].value ,colvalue))
		{
			rowcount--;
		}
		else
		{
			for(j=0;j<col;j++)
			{
				tableinfo[i][j].roworder+=rowcount;

			}
			if(tableinfo[i][0].roworder==1)
			{
				table=fopen(file,"w");
				for(j=0;j<col;j++)
				{
					fprintf(table,"%d %d %s\n" ,tableinfo[i][j].roworder,tableinfo[i][j].colorder,tableinfo[i][j].value);
				}
				fclose(table);
			}
			else
			{
				table=fopen(file,"a");
				for(j=0;j<col;j++)
				{
					fprintf(table,"%d %d %s\n" ,tableinfo[i][j].roworder,tableinfo[i][j].colorder,tableinfo[i][j].value);
				}
				fclose(table);

			}

		}

	}

	/*
    /// operator
	strcpy(temp,command);
	for(i=0;i < strlen(temp);i++)
	{
		if( temp[i]== '=' ||   temp[i]== '>' ||   temp[i]== '<' )
		{
			ch=temp[i];
		}
	}
	printf(" %c" , ch);      */
	for(i=0;i<rowno;i++)
	{
		free(tableinfo[i]);
	}

	free(tableinfo);
	free(colmeta);





}




//----------------------------update row ----------------



void updaterow(void)
{

	 int i=0,j=0,k=0,l=0;

   char *ptr;
   char *ch;
   int comma=0;
   int colno=0,count=0;

   char *keyword;

   char file[30]="c:\\";
   char filemeta[40]="c:\\";

   char **coln,**colv;
   char *cond, *condv;


   //char *command="update emp set salary=2500 ,name='roro' where  id=1;";
   //==========================================================

   struct meta *m;
   struct cell info;
   struct cell **table;
   int condorder,colorder;
   int row,column;

   FILE *fp;

   //===============================

   flushall();

   ///  Tokinizing the command

   while(i<strlen(command))
   {
     if(command[i]==',')
     {
      comma++;
     }
     i++;
   }

   // number of updated values
   colno=++comma;

  // printf("num of columns are %d\n",colno);

   coln=(char**)malloc(colno*sizeof(char*));
   colv=(char**)malloc(colno*sizeof(char*));

  ptr=strtok(command," ");

  while(ptr!=NULL && count<(colno+6))
  {
    if(count==0)
    {
     strcpy(keyword,ptr);
     //printf("Keyword is %s\n",keyword);
    }

   if(count==1)
   {

			strcat(file,ptr);
			strcat(file,".txt");
			strcat(filemeta,ptr);
			strcat(filemeta,"m.txt");
		     //	printf("%s %s \n ",file,filemeta);



   }


   if(count==(colno+4))
   {
	strcpy(cond,ptr);
      //	printf("colomn used in update: %s\n",cond);


   }

   if(count==(colno+5))
   {
     strcpy(condv,ptr);
    // printf("value used in update: %s\n",condv);

   }

   if(count>2&&count<2+colno)
   {
    for(i=0;i<colno;i++)
    {
     strcpy(coln[i],ptr);
     ptr=strtok(NULL," ,'=()");
     //printf("column %d name is %s\n",i+1,coln[i]);
     strcpy(colv[i],ptr);
     //printf("column %d value is %s\n",i+1,colv[i]);
     ptr=strtok(NULL," ',=()");
    }
    count+=colno;
   }



   count++;
   ptr=strtok(NULL,",' =();");
  }

  ///======================= End of tokinizing

  for(i=0;i<colno;i++)
  {
   printf("%s\t%s\n",coln[i],colv[i]);
  }
  //=============================================

  ///Getting table info
  //storing table in memory

  fp=fopen(file,"r");
  while(!feof(fp))
  {
   fscanf(fp,"%d %d %s",&info.roworder,&info.colorder,info.value);
  }
  fclose(fp);

  row=info.roworder;
  column=info.colorder;

  //Allocating table
  table=(struct cell**)malloc(row*sizeof(struct cell*));
  for(i=0;i<row;i++)
  {
	table[i]=(struct cell*)malloc(column*sizeof(struct cell));
  }

  //Reading data from file to table

  fp=fopen(file,"r");
  for(i=0;i<row;i++)
  {
	for(j=0;j<column;j++)
	{
		if(!feof(fp))
		{
		 //fscanf(fp,"%d %d %s",&info.roworder,&info.colorder,info.value);
		 fscanf(fp,"%d %d %s",&table[i][j].roworder,&table[i][j].colorder,table[i][j].value);

		}
	}
  }

  fclose(fp);

  //=====printing the table data table===============

     for(i=0;i<row;i++)
  {
	for(j=0;j<column;j++)
	{
	  printf("%s\t",table[i][j].value);
	}
	printf("\n");
  }


  //========Getting meta info============
  m=(struct meta*)malloc((column+1)*sizeof(struct meta));

  fp=fopen(filemeta,"r");
  for(i=0;i<column+1;i++)
  {
		if(!feof(fp))
		{

		 fscanf(fp,"%d %s",&m[i].colorder,m[i].name);

		}
  }

  fclose(fp);


  //=======Printing meta file==================
  for(i=0;i<column+1;i++)
  {
    printf("%d\t%s\n",m[i].colorder,m[i].name);
  }


  //=====================
  // getting condition column order
    for(i=1;i<=column;i++)
    {
     if(strcmp(cond,m[i].name)==0)
     {
      condorder=i;
      i=column+1;
     }
    }


  //=======================
  /*
  for(i=0;i<row;i++)
  {
   if(strcmp(table[i][condorder-1].value,condv)==0)
   {
    //for(j=0;j<column-1;j++)
    //{
     //if(j!=condorder-1)

     for(k=0;k<colno;k++)     //**coln
     {
      for(l=1;l<column+1;l++)    //*m
      {
       if(strcmp(coln[k],m[l].name)==0 && j!=(condorder-1))
       {
	strcpy(table[i][(m[l].colorder)-1].value,colv[k]);  //value updated // m[l].colorder)-1
	//l=column+1;
       }
      }
     }

    //}
   }
  }
   */

   for(i=0;i<row;i++)
   {
    if(strcmp(table[i][condorder-1].value,condv)==0)
    {
     for(l=0;l<colno;l++)
     {
      for(k=0;k<=column;k++)
      {
       if(strcmp(coln[l],m[k].name)==0)
       {
	strcpy(table[i][(m[k].colorder)-1].value,colv[l]);
       }
      }
     }
    }
   }

   /////////////////
  // after update
     for(i=0;i<row;i++)
  {
	for(j=0;j<column;j++)
	{
	  printf("%s\t",table[i][j].value);
	}
	printf("\n");
  }

  //==========================
  for(i=0;i<row;i++)
  {
	free(table[i]);
  }

  free(table);
  free(m);
  free(coln);
  free(colv);

}

//-----------------  select ------------------
void select(void)
{


 //tokenizing data
    //============================
   int flag=0,i=0,j=0,k=0;


   char *ptr;

   int comma=0;
   int colno=0,count=0;

   char *keyword;

   char file[30]="c:\\";
   char filemeta[40]="c:\\";

   char **coln;

   char *command="select name,salary from emp;";
   //char *command="select * from emp;";
   char *temp;
    //====================================================

     //select function variables

   struct meta *m;
   struct cell info;
   struct cell **table;

   int row,column;

   FILE *fp;




   ////===================================================
   flushall();

   strcpy(temp,command);

   ptr=strtok(temp," ;,'()=");

  while(ptr!=NULL && count!=2)
  {
   if(strcmp(ptr,"*")==0 && count==1)
   {
    flag=1;     //if * exists .. print all table
    ptr=NULL;
   }
   ptr=strtok(NULL," ;,()'=");
   count++;
  }
   count=0;

 //  printf("Flag is %d\n",flag);

   //no.of commas



   strcpy(temp,command);
   ptr=strtok(temp," ");     //first token

   strcpy(keyword,ptr);
 //  printf("Keyword is %s\n",keyword);


  // while(strcmp(keyword,"select"))


      if(flag)
       {
	while(ptr && count<4)
	 {
		if(count==3)
		 {

			strcat(file,ptr);
			strcat(file,".txt");
			strcat(filemeta,ptr);
			strcat(filemeta,"m.txt");
		  //	printf("%s %s \n ",file,filemeta);

		 }
	    ptr=strtok(NULL," ;");
	    count++;

	 }
	 //printf("All data of table %s will be dislayed",file);

       }

       else
       {
	  while(i<strlen(command))
	  {
		if(command[i]==',')
		{
		 comma++;
		}
		i++;
	  }
	 colno=++comma;

	 coln=(char**)malloc(colno*sizeof(char*));
	 while(ptr && count<colno+3)
	   {
		if(count==0)     //keyword
		{
			strcpy(keyword,ptr);
		       //	printf("keyword is %s\n",keyword);
		}

		if(count==(colno+2))     //tablename
		{

			strcat(file,ptr);
			strcat(file,".txt");
			strcat(filemeta,ptr);
			strcat(filemeta,"m.txt");
		  //	printf("%s %s \n ",file,filemeta);

		}

		if(count>0&&count<colno+1)  //column names
		{
		    //	printf("Selected columns: \n");


			for(i=0;i<colno;i++)
			{
				strcpy(coln[i],ptr);

			      //	printf("column no: %d is: %s \n",i+1,coln[i]);
				ptr=strtok(NULL," ,");
			}
		       count+=colno;
		}


		ptr=strtok(NULL," ,;");
		count++;

	   }
       }

 //  }
  ////============== End of tokenizing
  //=====================================================

   ///Getting table info
  //storing table in memory

  fp=fopen(file,"r");
  while(!feof(fp))
  {
   fscanf(fp,"%d %d %s",&info.roworder,&info.colorder,info.value);
  }
  fclose(fp);

  row=info.roworder;
  column=info.colorder;

  //Allocating table
  table=(struct cell**)malloc(row*sizeof(struct cell*));
  for(i=0;i<row;i++)
  {
	table[i]=(struct cell*)malloc(column*sizeof(struct cell));
  }

  //Reading data from file to table

  fp=fopen(file,"r");
  for(i=0;i<row;i++)
  {
	for(j=0;j<column;j++)
	{
		if(!feof(fp))
		{
		 //fscanf(fp,"%d %d %s",&info.roworder,&info.colorder,info.value);
		 fscanf(fp,"%d %d %s",&table[i][j].roworder,&table[i][j].colorder,table[i][j].value);

		}
	}
  }

  fclose(fp);



  //========Getting meta info============
  m=(struct meta*)malloc((column+1)*sizeof(struct meta));

  fp=fopen(filemeta,"r");
  for(i=0;i<column+1;i++)
  {
		if(!feof(fp))
		{

		 fscanf(fp,"%d %s",&m[i].colorder,m[i].name);

		}
  }

  fclose(fp);

   /*
  //=======Printing meta file==================
  for(i=0;i<column+1;i++)
  {
    printf("%d\t%s\n",m[i].colorder,m[i].name);
  }

  */
  //=====================

  /// Select

    if(flag)     // print all table
    {
      //=====printing the table data table===============
     //meta headers
     for(i=1;i<column+1;i++)
	{
		printf("%s\t",m[i].name);
	}
     printf("\n");
    //printing the cells

     for(i=0;i<row;i++)
	{
		for(j=0;j<column;j++)
		{
			printf("%s\t",table[i][j].value);
		}
			printf("\n");
	}

    }
    else        // print specific columns
    {

     //======printing specific meta headers ===========
      for(j=0;j<colno;j++)
      {
       for(k=1;k<=column;k++)
       {
	if(strcmp(coln[j],m[k].name)==0)
	{
	   printf("%s\t",m[k].name);

	}
       }
      }

      printf("\n");

     //=================================
     for(i=0;i<row;i++)
     {
      for(j=0;j<colno;j++)
      {
       for(k=1;k<=column;k++)
       {
	if(strcmp(coln[j],m[k].name)==0)
	{
	  printf("%s\t",table[i][(m[k].colorder)-1].value);
	}

       }
      }
      if(j==colno)
	{
		printf("\n");
	}
     }
    }

  //=======Free all dynamically allocated memory

   for(i=0;i<row;i++)
  {
	free(table[i]);
  }

  free(table);
  free(m);
  free(coln);




}