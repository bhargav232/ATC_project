#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#define ALERT_POINT "OtoD"

typedef struct _pulse msg_header_t;

/* Our real data comes after the header */
typedef struct _my_data {
    msg_header_t hdr;
    char message[100];
} my_data_t;

/*** Server Side of the code ***/
int server() {
   name_attach_t *attach;
   my_data_t msg;
   int rcvid;
  // printf("Server receive %d \n", 5);


   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, ALERT_POINT, 0)) == NULL) {
       return EXIT_FAILURE;
   }

   /* Do your MsgReceive's here now with the chid */
   while (1) {

       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);


       if (rcvid == -1) {/* Error condition, exit */
           break;
       }

       if (rcvid == 0) {/* Pulse received */
           switch (msg.hdr.code) {
           case _PULSE_CODE_DISCONNECT:

               ConnectDetach(msg.hdr.scoid);
               break;
           case _PULSE_CODE_UNBLOCK:

               break;
           default:

               break;
           }
           continue;
       }


       if (msg.hdr.type == _IO_CONNECT ) {
           MsgReply( rcvid, EOK, NULL, 0 );
           continue;
       }

       if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
           MsgError( rcvid, ENOSYS );
           continue;
       }

	   if (msg.hdr.type == 0x00) {
	      if (msg.hdr.subtype == 0x01) {
              // seperation violation message will be printed over here!
              printf("Server receive %d \n", msg.message);
              //print message of violation constraint here
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

        printf("Running Server ... \n");
        ret = server();   /* see name_attach() for this code */


    return ret;
}
