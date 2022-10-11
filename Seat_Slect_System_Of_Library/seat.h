/*seat.h*/
#ifndef __SEAT_H
#define __SEAT_H
#include "globalapi.h"

#define TOTAL_SEAT_NUM 50 //座位总数
#define LIBRADY_FLOOR_5 5
#define STUDY_ROOM 6
#define STUDY_ROOM_SEAT_NUM 25
#define LIBRADY_FLOOR_5_SEAT_NUM 25


/*教室的一个简单描述，room_id表示楼号，room_num表示剩余教室数*/
typedef struct seat_struct_t {
	int room_id;
	int seat_id;
    int is_empty;
	
	pthread_mutex_t seat_mutex;
} seat_struct;
seat_struct seat_list[TOTAL_SEAT_NUM];

/*init_seat_list:初始化seat_list数组*/
void init_seat_list(int room_id)
{
	FILE *fp;
    int seat_num;
    if(room_id == LIBRADY_FLOOR_5) {
        fp = fopen("library_floor_5_seat.txt","r");
        seat_num = LIBRADY_FLOOR_5_SEAT_NUM;
    }
    else if (room_id == STUDY_ROOM) {
        fp = fopen("study_room_seat.txt", "r");
        seat_num = STUDY_ROOM_SEAT_NUM;
    }

	int id_t,is_empty_t;
	int i;
	for(i=0; i<seat_num;i++) {
		fscanf(fp,"%d %d\n",&id_t,&is_empty_t);
		seat_list[i].room_id = room_id;
		seat_list[i].seat_id = id_t;
        seat_list[i].is_empty = is_empty_t;
	}
	fclose(fp);
	
}

//room_id 转换
char * change_id(int room_id){
	char *changed_id;
	switch(room_id){
	case STUDY_ROOM:changed_id="一楼自习室";break;
	case LIBRADY_FLOOR_5:changed_id="图书馆五楼";break;
	default:break;
	}
	return changed_id;
}


#endif

