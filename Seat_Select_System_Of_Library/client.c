/*client.c*/

#include "globalapi.h"
#include "seat.h"
#include <gtk/gtk.h>

GtkWidget *window_must=NULL;
GtkWidget *window_choose=NULL;
GtkWidget *window_main=NULL;

GtkWidget *lb2_2;//显示所选功能
GtkWidget *lb4_2;//显示所选room
GtkWidget *lb6_2;//填入的座位

int socket_fd;  //连接socket
struct sockaddr_in server; //服务器地址信息，客户端地址信息
int ret,i;
int flag=1;
char c;
int isconnected=FALSE; //是否已连接服务器
int room_id;


//根据用户按键情况设置查询位置
void room1(GtkWidget *widget, gpointer data)
{
    room_id =LIBRADY_FLOOR_5;
}

//根据用户按键情况设置查询位置
void room2(GtkWidget *widget, gpointer data)
{
    room_id = STUDY_ROOM;
}


//查询图书馆座位情况
void library_window(GtkWidget *widget, gpointer data) {
    gtk_label_set_text((GtkLabel *) lb2_2, "图书馆五楼座位情况");

    GtkWidget *library_window;
    library_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新窗口
    gtk_window_set_default_size(GTK_WINDOW(library_window), 400, 350);//设置窗体大小
    gtk_window_set_title(GTK_WINDOW(library_window), "图书馆五楼座位情况");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(library_window), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(library_window), 10);//窗体边框设置
    
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(library_window), vb);//放入窗口
    GtkWidget *hb1 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb1), 0, 0, 0);
    
    /*-----------------向服务器查询信息----------------*/
    int i,pos,room_ID,seat_ID;
	char msg[512];
	char send_buf[512], recv_buf[512];
	
	init_message();
	message.msg_type=INQUIRE_ALL;
    message.room_id = LIBRADY_FLOOR_5;
      
	memcpy(send_buf,&message,sizeof(message));
	
	int ret=send(socket_fd, send_buf,sizeof(message),0);
	ret=recv(socket_fd,recv_buf,sizeof(recv_buf),0);
	pos=0;
	
	for (i=0;i<ret;i=i+sizeof(message)) {
		memcpy(&message,recv_buf+pos,sizeof(message));
		if(message.msg_type==INQUIRE_SUCCEED){
		    sprintf(msg,"\t%d号位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		    GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);
		}
		//sprintf(msg,"\t\t\t %d:    位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		else{
		sprintf(msg,"\t%d号位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		    GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);

		}
		//sprintf(msg,"\t\t\t查询失败！%d:      位置占用情况：未知",message.seat_id);
		pos+=sizeof(message);
		
	}
	gtk_widget_show_all(library_window);//显示查询窗口
}

//查询自习室座位情况
void study_room_window(GtkWidget *widget, gpointer data) {
    gtk_label_set_text((GtkLabel *) lb2_2, "一楼自习室座位情况");

    GtkWidget *study_room_window;
    study_room_window= gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新窗口

    gtk_window_set_default_size(GTK_WINDOW(study_room_window), 400, 350);//设置窗体大小

    gtk_window_set_title(GTK_WINDOW(study_room_window), "一楼自习室座位情况");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(study_room_window), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(study_room_window), 10);//窗体边框设置
    
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(study_room_window), vb);//放入窗口
    GtkWidget *hb1 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb1), 0, 0, 0);
    
    /*---------------向服务器查询信息----------------*/
    int i,pos,room_ID,seat_ID;
	char msg[512];
	char send_buf[512], recv_buf[512];
	
	init_message();
	message.msg_type=INQUIRE_ALL;
        message.room_id = STUDY_ROOM;
      
	memcpy(send_buf,&message,sizeof(message));
	
	int ret=send(socket_fd, send_buf,sizeof(message),0);
	ret=recv(socket_fd,recv_buf,sizeof(recv_buf),0);
	pos=0;
	
	for (i=0;i<ret;i=i+sizeof(message)) {
		memcpy(&message,recv_buf+pos,sizeof(message));
		if(message.msg_type==INQUIRE_SUCCEED){
		    sprintf(msg,"\t%d号位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		    GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);
		}
		//sprintf(msg,"\t\t\t %d:    位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		else{
		sprintf(msg,"\t%d号位置占用情况：%s",message.seat_id,change_empty(message.is_empty));
		    GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);

		}
		//sprintf(msg,"\t\t\t查询失败！%d:      位置占用情况：未知",message.seat_id);
		pos+=sizeof(message);
		
	}
	gtk_widget_show_all(study_room_window);//显示查询窗口
}


//----------------------------------------------------------------------查询座位窗口设置
GtkWidget *create_inquery_window() {
    GtkWidget *window_inquery;
    window_inquery = gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新窗口

    gtk_window_set_default_size(GTK_WINDOW(window_inquery), 400, 350);//设置窗体大小

    gtk_window_set_title(GTK_WINDOW(window_inquery), "座位情况查询");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(window_inquery), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(window_inquery), 10);//窗体边框设置

	GtkWidget *button3_1 = gtk_button_new_with_label("图书馆五楼");
    GtkWidget *button3_2 = gtk_button_new_with_label("一楼自习室");
    
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(window_inquery), vb);//放入窗口
    
    GtkWidget *hb3 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb3), 0, 0, 0);


    gtk_box_pack_start(GTK_BOX(hb3), button3_1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hb3), button3_2, FALSE, FALSE, 0);


    g_signal_connect(G_OBJECT(button3_1),"clicked",G_CALLBACK(library_window),NULL);
    g_signal_connect(G_OBJECT(button3_2),"clicked",G_CALLBACK(study_room_window),NULL);

    return window_inquery;
}

/*---------------显示座位申请结果-----------------*/
void apply_result_window(GtkWidget *widget, gpointer data) {
    gtk_label_set_text((GtkLabel *) lb2_2, "申请座位结果");

    GtkWidget *apply_result_window;
    apply_result_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新窗口

    gtk_window_set_default_size(GTK_WINDOW(apply_result_window), 300, 100);//设置窗体大小

    gtk_window_set_title(GTK_WINDOW(apply_result_window), "申请座位结果");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(apply_result_window), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(apply_result_window), 10);//窗体边框设置
    
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(apply_result_window), vb);//放入窗口
    GtkWidget *hb1 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb1), 0, 0, 0);
      
	char msg[512];
	char send_buf[512], recv_buf[512];
	
	init_message();
	message.msg_type=APPLY_SEAT;
    message.room_id = room_id;
    const gchar *sid = gtk_entry_get_text((GtkEntry *)lb6_2);//从输入框获取学号
   message.seat_id = atoi(sid);;//从输入框获取学号
//    message.seat_id=1;
      
	memcpy(send_buf,&message,sizeof(message));
	
	int ret=send(socket_fd, send_buf,sizeof(message),0);
	ret=recv(socket_fd,recv_buf,sizeof(message),0);
	
	memcpy(&message,recv_buf,sizeof(message));
	if(message.msg_type==APPLY_SUCCEED){
			sprintf(msg,"  %s 座位号：%d 申请成功！\n",change_id(message.room_id),message.seat_id);
			GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);
	}
	else{
			sprintf(msg,"  申请失败！%s, 座位号：%d 已被占用\n",change_id(message.room_id),message.seat_id);
			GtkWidget *lb1 = gtk_label_new(msg);//新建标签
    		gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);
	}
		
	gtk_widget_show_all(apply_result_window);//显示查询窗口
}

//----------------------------------------------------------------------申请座位窗口设置
GtkWidget *create_apply_window() {
    GtkWidget *window_apply;
    window_apply = gtk_window_new(GTK_WINDOW_TOPLEVEL);//创建一个新窗口

    gtk_window_set_default_size(GTK_WINDOW(window_apply), 350, 300);//设置窗体大小

    gtk_window_set_title(GTK_WINDOW(window_apply), "自习座位申请");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(window_apply), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(window_apply), 10);//窗体边框设置

	GtkWidget *button3_1 = gtk_button_new_with_label("图书馆五楼");
    GtkWidget *button3_2 = gtk_button_new_with_label("一楼自习室");

    
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(window_apply), vb);//放入窗口
    
    GtkWidget *hb3 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb3), 0, 0, 0);

	
	GtkWidget *lb1 = gtk_label_new("请选择申请位置");//新建标签
    gtk_box_pack_start(GTK_BOX(hb3), lb1, 0, 0, 0);


    gtk_box_pack_start(GTK_BOX(hb3), button3_1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hb3), button3_2, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button3_1),"clicked",G_CALLBACK(room1),NULL);
    g_signal_connect(G_OBJECT(button3_2),"clicked",G_CALLBACK(room2),NULL);

	//------------------------------------------------------
    GtkWidget *hb6 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb6), 0, 0, 0);

    GtkWidget *lb6_1 = gtk_label_new("请输入选择的座位：");//新建标签
    lb6_2 = gtk_entry_new();//新建标签
    gtk_entry_set_text((GtkEntry *)lb6_2,"");

    GtkWidget *button6= gtk_button_new_with_label("申请座位");//新建按钮

    gtk_box_pack_start(GTK_BOX(hb6), lb6_1, 0, 0, 0);//从左到右放入盒子当中
    gtk_box_pack_start(GTK_BOX(hb6), lb6_2, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hb6), button6, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(button6),"clicked",G_CALLBACK(apply_result_window),NULL);

    return window_apply;
}


/*功能1：自习座位情况查询*/
void func1(GtkWidget *widget, gpointer data) {
	//inquery
	GtkWidget *window_inquery;
	window_inquery = gtk_window_new(GTK_WINDOW_TOPLEVEL);
       gtk_label_set_text((GtkLabel *) lb2_2, "自习座位情况查询");
	window_inquery=create_inquery_window();//创建查询窗口
 	gtk_widget_show_all(window_inquery);//显示查询窗口
}

/*功能2：自习座位申请*/
void func2(GtkWidget *widget, gpointer data)
{
	//apply
	GtkWidget *window_apply;
	window_apply = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_label_set_text((GtkLabel *) lb2_2, "自习座位申请");
	window_apply=create_apply_window();//创建查询窗口
 	gtk_widget_show_all(window_apply);//显示查询窗口
}



//------------------------------------------------------------主页面的创建
GtkWidget *create_main_window() {
    GtkWidget *window_main;
    window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size(GTK_WINDOW(window_main), 450, 400);//设置窗体大小
    g_signal_connect(G_OBJECT(window_main), "delete_event", G_CALLBACK(gtk_main_quit), NULL);//关闭窗体

    gtk_window_set_title(GTK_WINDOW(window_main), "简易自习选座系统");    //窗口标题
    gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);//窗体显示位置
    gtk_container_set_border_width(GTK_CONTAINER(window_main), 10);//窗体边框设置

    //-----------------------------------------------------最外层盒子
    GtkWidget *vb = gtk_box_new(1, 0);//横向盒子
    gtk_container_add(GTK_CONTAINER(window_main), vb);//放入窗口
    //------------------------------------------------------
    GtkWidget *hb1 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb1), 0, 0, 0);

    GtkWidget *lb1 = gtk_label_new("欢迎来到自习选座系统:D");//新建标签
    gtk_box_pack_start(GTK_BOX(hb1), lb1, 0, 0, 0);

    //---------------------------------------------------
    GtkWidget *hb2 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb2), 0, 0, 0);

    GtkWidget *lb2_1 = gtk_label_new("请选择功能：");//新建标签
    lb2_2 = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hb2), lb2_1, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hb2), lb2_2, 0, 0, 0);
    //------------------------------------------------------
    GtkWidget *hb3 = gtk_box_new(1, 0);//横向盒子
    gtk_box_pack_start(GTK_BOX(vb), GTK_WIDGET(hb3), 0, 0, 0);

    GtkWidget *button3_1 = gtk_button_new_with_label("自习座位情况查询");
    GtkWidget *button3_2 = gtk_button_new_with_label("自习座位申请");


    gtk_box_pack_start(GTK_BOX(hb3), button3_1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hb3), button3_2, FALSE, FALSE, 0);


    g_signal_connect(G_OBJECT(button3_1),"clicked",G_CALLBACK(func1),NULL);
    g_signal_connect(G_OBJECT(button3_2),"clicked",G_CALLBACK(func2),NULL);

    return window_main;
}


//消息内容输出函数
void display_info(char *msg)
{
	printf("%s\n",msg);
}

/*连接服务器操作*/
void connectserver()
{
	char msg[512]; //提示信息
	int i;

	if(!isconnected)
	{
	/*创建套接字*/
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd<0) {
		sprintf(msg,"创建套接字出错！ \n");
		display_info(msg);
		return;
	} 
	/*设置接收、发送超时值*/
	struct timeval time_out;
	time_out.tv_sec=5;
	time_out.tv_usec=0;
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &time_out, sizeof(time_out));

	/*填写服务器的地址信息*/
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");//htonl(INADDR_ANY);
	server.sin_port=htons(SERVER_PORT_NO);

	/*连接服务器*/
	ret=connect(socket_fd,(struct sockaddr*)&server, sizeof(server));
	if(ret<0) {
		sprintf(msg,"连接服务器 %d 出错！\n",SERVER_PORT_NO);
		display_info(msg);
		close(socket_fd);
		re_connect();
		return ;
	}

	//成功后输出提示信息
	printf("\t\t连接服务器成功！");
	
	isconnected=TRUE;

    	}
}

/*断开连接操作*/
void disconnect()
{
	char msg[512];
	if(isconnected)
	{
		close(socket_fd);
		sprintf(msg,"断开连接成功！\n");
		display_info(msg);
		isconnected=FALSE;
	}
}


//是否重连？
int re_connect(){
	if(ret<0){
		printf("重连？【y/n】");
		c=getchar();
		getchar();
		if(c=='y'){connectserver();return 1;}
		else return 0;
	}
}


int main(int argc, char *argv[])
{
	connectserver();
	if (ret<0)return 0;//连接服务器失败，退出
	//------gtk------
	gtk_init(&argc, &argv);
    window_main=create_main_window();//主页面创建
    gtk_widget_show_all(window_main);//显示所有窗体
    gtk_main();
	//------gtk------
	disconnect();
	return 0;
}
