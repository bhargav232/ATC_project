#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#define ATTACH_POINT "myname"
#define ALERT_POINT "OtoD"

typedef struct _pulse msg_header_t;

typedef struct _aircraft_data {
    msg_header_t hdr;
    int current_time;
    int id;
    int x;
    int y;
    int z;
    int speed_x;
    int speed_y;
    int speed_z;
} aircraft_data_t;

typedef struct _my_data {
    msg_header_t hdr;
    int data =5;
} my_data_t;


// LOGIC FOR CHECKING SEPERARTIION VILATION CONSTRAINT AS PER PROJECT DESCRIPTION
int check_separation_violation(aircraft_data_t* aircraft1, aircraft_data_t* aircraft2) {
    int dx = aircraft1->x - aircraft2->x;
    int dy = aircraft1->y - aircraft2->y;
    int dz = aircraft1->z - aircraft2->z;
    int result = (dx*dx + dy*dy < 3000*3000) && (dz*dz < 1000*1000);

// LOGIC FOR SENDING VIOLATION MESSAGE TO DATA DISPLAY, THERFORE AT THIS TIME THIS BELOW LOGIC WILL ACT
// LIKE A CLIENT FOR THE DATA DISPLAY(WHICH IS SERVER WAITING FOR MESSAGE IN WHILE LOOP)
//    if (result) {
//    	my_data_t constraint_msg;
//        constraint_msg.hdr.type = 0x00;
//        constraint_msg.hdr.subtype = 0x02;
//        constraint_msg.id1 = aircraft1->id;
//        constraint_msg.id2 = aircraft2->id;

//        int server_coid = name_open(ALERT_POINT, 0);
//        if (server_coid == -1) {
//            printf("Failed to connect to the alert server\n");
//        }
//        else {
//            if (MsgSend(server_coid, &constraint_msg, sizeof(constraint_msg), NULL, 0) == -1) {
//                printf("Failed to send the constraint message\n");
//            }
//            else {
//                printf("Constraint message sent to the alert server\n");
//            }
//            name_close(server_coid);
//        }
//    }

    return result;
}


int server() {
   name_attach_t *attach;
   aircraft_data_t msg;
   int rcvid;

   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
       return EXIT_FAILURE;
   }

   aircraft_data_t aircrafts[100]; // maximum 100 aircrafts

   while (1) {


// RECEIVE AIRCRAFT DATA WITH UPDATED POSITION EVERY SECOND
       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

       if (rcvid == -1) {/* Error condition, exit */
           break;
       }

       if (msg.hdr.type == 0x00) {
           if (msg.hdr.subtype == 0x01) {

               printf("Aircraft data received:\n");
               printf("Current_Time: %d\n", msg.current_time);
               printf("ID: %d\n", msg.id);
               printf("X: %d\n", msg.x);
               printf("Y: %d\n", msg.y);
               printf("Z: %d\n", msg.z);
               printf("Speed X: %d\n", msg.speed_x);
               printf("Speed Y: %d\n", msg.speed_y);
               printf("Speed Z: %d\n", msg.speed_z);

               // LOGIC FOR TAKING ANY TWO AIRCRAFT AND CALLING THE SEPARATION VIOLATION FUNCTION, WHICH
              // CALCULATE DISTANCE BETWEEN TWO AIRCRAFTS AND PRINT CONSTRAINT MESSAGE IF DISTANCE BETWEEN
              // TWO AIRCRAFT IS LESS THAN MENTION DISTANCE(AS PER PROJECT DESCRIPTION)

               int i;
               for (i = 0; i < 100; i++) {
                   if (aircrafts[i].id == msg.id) {
                       aircrafts[i] = msg;
                       break;
                   }
                   else if (aircrafts[i].id == 0) {
                       aircrafts[i] = msg;
                       break;
                   }
               }

               for (i = 0; i < 100; i++) {
                   if (aircrafts[i].id == 0) {
                       break;
                   }
                   int j;
                   for (j = i+1; j < 100; j++) {
                       if (aircrafts[j].id == 0) {
                           break;
                       }
                     if (check_separation_violation(&aircrafts[i], &aircrafts[j])) {
                           printf("Separation violation: Aircraft %d and %d are too close\n",
                                   aircrafts[i].id, aircrafts[j].id);
                       }
                   }
               }
           }
       }

       MsgReply(rcvid, EOK, 0, 0);

   }

   /* Remove the name from the space */
   name_detach(attach, 0);

   return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int ret;

    printf("Running Server ...\n");
    ret = server();
    return ret;
  }

