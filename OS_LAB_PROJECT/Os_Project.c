/*---------------- -------OS LAB PROJECT ----------------------------*/
/*----------- Context switching && Round Robin Scheduling------------*/
/*
	 ABHISHEK JHAWAR
*/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "Stack/stack_implementation.h"
#include "Queue/queue_implementation.h"
#define max 1024
#define lsize 256

int c[4]={0,0,0,0};
bool res[4]={false,false,false,false};//For buttons 
int count=0;
GtkWidget *pcb1; // PCB state before execution
GtkWidget *pcb2; //PCB state after execution
GtkWidget *align;

//GUI button for allocating resources
int occ_1(GtkWidget *widget,gpointer   data)
{
	g_print ("Resource 1 is occupied!\n");
	res[1]=true;
	return 0;
}

//GUI button to release resource
int free_1(GtkWidget *widget,gpointer   data)
{
	g_print ("Resource 1 released!\n");
	res[1]=false;
	return 0;
}
int occ_2(GtkWidget *widget,gpointer   data)
{
	g_print ("Resource 2 is occupied!\n");
	res[2]=true;
	return 0;
}
int free_2(GtkWidget *widget,gpointer   data)
{
	g_print ("Resource 2 released!\n");
	res[2]=false;
	return 0;
}

void updateLabel(GtkLabel *disp,int x,int y,char* z,void* w)
{
    gchar *display;
	/*Updates PCB*/
    display = g_strdup_printf("PID		:%d\nPC			:%d\nState		:%s\nSP			:%p\n",x,y,z,w);
    gtk_label_set_text (GTK_LABEL(disp), display); 		
    g_free(display);                              		
}

void updateL(GtkLabel *disp,int x,int y,char* z,void * w)
{
    gchar *display;
    display = g_strdup_printf("PID		:%d\nPC			:%d\nState		:%s\nSP			:%p\n",x,y,z,w);         //concate data to display
    gtk_label_set_text (GTK_LABEL(disp), display); 		//set label to "display"
    g_free(display);                              		//free display
}
void* threadFunction(void* args)
{   
	//files of processes containing instructions where we made our own instructions  
	static const char* filename[4];
	filename[0] = "process1.txt";   
	filename[1] = "process2.txt";
	filename[2] = "process3.txt";
	filename[3] = "process4.txt";
	int size=4;

	//Constructing Ready queue
    Queue *ready_queue = createQueue(size);

	//Constructing Blocked queue for Some I/P or fork Processes call
	Queue *blocked_queue = createQueue(size);

	//Stack register to Build PCB block which have Linked List
	struct stack_t stack_p[4];

	//PID
	int process[] = { 0,1,2,3 }; 
	//arrivaltime
  	int arrivaltime[] = {0,0,0,0};
	int burst_time[4];
	int pc[]={0,0,0,0};
	int len[size];
    //Stack pointer
	void*sp[]={stackpointer(&(stack_p[0])),stackpointer(&(stack_p[1])),stackpointer(&(stack_p[2])),stackpointer(&(stack_p[3]))};
	int t[]={0,0,0,0};
	int l, u ; 
	printf("\nEnter the LOWER LIMIT of the PROCESS INTIAL : \n\n");
	scanf("%d",&l);
	printf("\nEnter the UPPER LIMIT of the PROCESS INITAL : \n\n");
	scanf("%d",&u);

	//Defining States of the  processes

    char* state[size];
	state[0] = "ready";
	state[1] = "ready";
	state[2] = "ready";
	state[3] = "ready";
   	int tot_time=0;
	
	pc[0]=1000;//Just Program pointer intialization
	int rval;
	printf("Enter the Quantum Time For Round Robin Algorthim : \n");
 	scanf("%d",&rval);
	for (int i = 0; i < size; i++)
	{ 
		burst_time[i] =rval*((rand() %(u - l + 1)) + l);
		len[i]=burst_time[i]; 
		
	} 
	for (int i = 1; i < size; i++) 	
	{ 
		pc[i] = pc[i-1]+len[i-1]; 
		
	}

	//total time for RR
	for (int i = 0; i < size; i++) 	
	{ 
		tot_time =tot_time+ burst_time[i];  
	} 

	//print process intialization description
    printf("process\tArrival time\t burst time\tPC\tSize\tOccurances\n"); 
    for (int i = 0; i < size; i++)  
    { 
          printf("%d\t%d\t\t%d\t\t %d \t%d\t%d\n",process[i],arrivaltime[i],burst_time[i],pc[i],len[i],c[i]);
	}


	//Initially Add all processes to ready queue for Execution
	for (int i= 0; i<size; i++) 
	{
 	   	Enqueue(ready_queue,process[i]);
    }

	//Implementation of RR scheduling and context switch
	for(int i= 0; i<(tot_time/rval); i++)
	{		
			
			int blocked=front(blocked_queue);				
			if(res[blocked]==false && blocked!=-1)
			//check resource is released for blocked process
			{
				Enqueue(ready_queue,process[blocked]);		
				Dequeue(blocked_queue);
				state[blocked]="Ready";
			}
			// Just if it is blocked we are putting it back to block queue
			else
			{
				if(blocked!=-1)
				{
					Dequeue(blocked_queue);
					Enqueue(blocked_queue,process[blocked]);
				}
		
			}
		int running;
		//display ready and blocked queue
		printf("Ready ");
		display(ready_queue);
		printf("Blocked ");
		display(blocked_queue);
		running=Dequeue(ready_queue);//Add process for running 
		
		//For I/O process or Fork processes check if resource is available
		if(res[running]==false)
		{
			if(t[running]<burst_time[running])
			{
				state[running]="Running";
				//print state before execution
				printf("\n----------------------------------------------------------------------------\n");
				printf("				Before execution\n");
				printf("\n----------------------------------------------------------------------------\n");
				printf("Process\t\tPC\t\tState\t\tSP\t\tOccurances\n"); 
				for (int j = 0; j < size; j++)  
				{ 
					printf("%d\t\t%d\t\t%s\t\t%p\t\t%d\n",process[j],pc[j],state[j],sp[j],c[j]);
					      
				}
				//update pcb value of process before running
				updateLabel(GTK_LABEL(pcb1),process[running],pc[running],state[running],sp[running]);
					/*run process for quantum 2*/
					for(int k=0;k<rval;k++)
					{
						t[running]++;
						pc[running]=pc[running]+1;
						sleep(1);
					}
					c[running]++;
					//open file of process to execute
					FILE *file = fopen(filename[running], "r");
					int count = 0;
					if ( file != NULL )
					{
						char string1[max][max];
						int ctr=0;
						int q=0;
						char line[lsize]; 

						while (fgets(line, sizeof line, file) != NULL) 
						{
							if (count == t[running])
							{
								break;
							}
							else
							{	//to read single word
								for(int p=0;p<=(strlen(line));p++)
								{
									// if space or NULL found, assign NULL into newString[ctr]
									if(line[p]==' '|| line[p]=='\0')
									{
										string1[ctr][q]='\0';
										ctr++;  //for next word
										q=0;
									}
									else
									{
										string1[ctr][q]=line[p];
            							q++;
									}
								}
								//check whether variable is declared or not
    							if(strcmp(string1[0],"add")!=0||strcmp(string1[0],"sub")!=0||strcmp(string1[0],"div")!=0||strcmp(string1[0],"mult")!=0)
								{
								    // printf("%s",string1[1]);
									push(&(stack_p[running]),string1[1]);						
								}
								else
								{
									pop(&(stack_p[running]));
								}
								count++;
							}
						}
						fclose(file);
					}
					state[running]="Ready";
					sp[running]=stackpointer(&(stack_p[running]));
					//print state after execution
					printf("\n----------------------------------------------------------------------------\n");
					printf("				After execution\n");
					printf("\n----------------------------------------------------------------------------\n");
					printf("Process\t\tPC\t\tState\t\tSP\t\tOccurances\n"); 
					
					for (int i = 0; i < size; i++)  
					{ 
						  
						  printf("%d\t\t%d\t\t%s\t\t%p\t\t%d\n",process[i],pc[i],state[i],sp[i],c[i]);
						  
					}

					// update pcb value of process after running 
					updateL(GTK_LABEL(pcb2),process[running],pc[running],state[running],sp[running]);
					sleep(2);

					//check if Process is completed then don't add it to ready queue
					if(t[running]==burst_time[running]){
						printf("process %d is completed\n",process[running]);
						state[running]="Ended";}

					//if Process is not completed then add it to ready queue
					else
						Enqueue(ready_queue,process[running]);

			}
			
		}
		//if resource is unavailable, add it to blocked queue
		else{
			state[running]="Blocked";
			Enqueue(blocked_queue,process[running]);
			printf("process %d is blocked\n",process[running]);
			i--;
		}
	}	
}
int main(int argc, char * argv[])
{
 	//declaration of variables For GUI
 	printf("\n\n\t----------------OS LAB PROJECT---------------\n\n");
 	printf("\n\n\t---------CONTEXT SWITCHING BETWEEN PROCESSES------------\n\n");
 	printf("\n\n---------------------------------------------------------------------\t\t\n\n");
 	printf("\n\n\t\t* ABHISHEK JHAWAR\n\t\t* DEVASHISH AGRAWAL\n\t\t* VEDIC KALRA\n\t\t* MONARCH SHARMA\n\t\t* GRANTH KOHLI\n");
 	printf("\n\n---------------------------------------------------------------------\t\t\n\n");
    pthread_t id; 
	pthread_create(&id,NULL,&threadFunction,NULL);
    gtk_init (&argc, &argv);
    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *l1;
	GtkWidget *l2;
	GtkWidget *l3;
	GtkWidget *l4;
	
	align = gtk_alignment_new(0, 1, 0, 0);	

	//to show pcb data on gui screen

	l1 = gtk_label_new ("Before Execution:\n");
	l2 = gtk_label_new ("After Execution:\n");
    pcb1 = gtk_label_new ("PID		:-\nPC			:-\nState		:-\nSP			:-\n");
    pcb2 = gtk_label_new ("PID		:-\nPC			:-\nState		:-\nSP			:-\n");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW (window), "OS_PROJECT_CONTEXT SWITCHING");
    gtk_window_set_default_size (GTK_WINDOW (window), 450, 450);

    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);



    GdkColor red = {0xffff, 0xffff, 0x0000, 0x0000};
    GdkColor yellow = {0xffff, 0xffff, 0xffff,0x0000};
 	GdkColor green = {0x0000, 0x0fff, 0xffff,0x0fff};
 	GdkColor blue={0, 0x0000, 0x0000, 0x0000};
 	GdkColor white={0xffff, 0xffff, 0xffff, 0xffff};

 	gtk_widget_modify_bg(pcb1,GTK_STATE_NORMAL, &red);
 	gtk_widget_modify_bg(pcb2,GTK_STATE_NORMAL, &green);
 	gtk_widget_modify_bg(l1,GTK_STATE_NORMAL, &yellow);
 	gtk_widget_modify_bg(l2,GTK_STATE_NORMAL, &yellow);


    //create grid for alignment  
    grid = gtk_grid_new ();
    
	//add grid to window  
    gtk_container_add (GTK_CONTAINER (window), grid);

    button = gtk_button_new_with_label ("Resource 1 occupy");
    g_signal_connect (button, "clicked", G_CALLBACK (occ_1), NULL);
	
	//attach buttons to grid
    gtk_grid_attach (GTK_GRID (grid), button, 5, 4, 1.8, 1.8);

    button = gtk_button_new_with_label ("Resource 1 release");
    g_signal_connect (button, "clicked", G_CALLBACK (free_1), NULL);

    gtk_grid_attach (GTK_GRID (grid), button, 6, 4, 1.8, 1.8);

	button = gtk_button_new_with_label ("Resource 2 occupy");
    g_signal_connect (button, "clicked", G_CALLBACK (occ_2), NULL);
	
	//attach buttons to grid
    gtk_grid_attach (GTK_GRID (grid), button, 5, 5, 1.8, 1.8);

    button = gtk_button_new_with_label ("Resource 2 release");
    g_signal_connect (button, "clicked", G_CALLBACK (free_2), NULL);

    gtk_grid_attach (GTK_GRID (grid), button, 6, 5, 1.8, 1.8);

 	gtk_grid_attach (GTK_GRID(grid),l1,5, 10, 1.5, 1.5);
 	gtk_grid_attach (GTK_GRID(grid),pcb1,5,11, 2.5, 1.5);
	gtk_grid_attach (GTK_GRID(grid),l2,5, 12, 1.5, 1.5);
 	gtk_grid_attach (GTK_GRID(grid),pcb2,5, 13, 2.5, 1.5);
     

 	//gtk_widget_modify_bg(,GTK_STATE_NORMAL, &blue);
  	gtk_widget_modify_bg(window,GTK_STATE_NORMAL, &blue);
  	gtk_widget_show_all (window);
    gtk_main ();           
}

// EXEC :  gcc `pkg-config gtk+-3.0 --cflags` os_pro.c stack_implementation.c queue_implementation.c -o os `pkg-config gtk+-3.0 --libs`
// ./os
// OUTPUT : GUI CONTAINING THE PROCESS AND THERE CONSUMPTIONS OF RESOURCES