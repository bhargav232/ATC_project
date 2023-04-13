#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#define ATTACH_POINT "myname"


typedef struct _pulse msg_header_t;


typedef struct _my_data {
    msg_header_t hdr;
    int speed;
    int altitude;
    int position;
} my_data_t;


int client() {
    my_data_t msg;
    int server_coid; //server connection ID.
    int command;

    if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
        return EXIT_FAILURE;
    }

    while (1) {
        printf("\nPlease enter command: \n");
        printf("1: Change Speed\n2: Change Altitude\n3: Change Position\n4: Request Aircraft Information\n5: Exit\n");
        scanf("%d", &command);

        switch (command) {
            case 1:
                printf("Enter new speed: ");
                scanf("%d", &msg.speed);
                msg.hdr.type = 0x00;
                msg.hdr.subtype = 0x01;
                if (MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1) {
                    printf("Error sending speed command\n");
                }
                break;
            case 2:
                printf("Enter new altitude: ");
                scanf("%d", &msg.altitude);
                msg.hdr.type = 0x00;
                msg.hdr.subtype = 0x02;
                if (MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1) {
                    printf("Error sending altitude command\n");
                }
                break;
            case 3:
                printf("Enter new position: ");
                scanf("%d", &msg.position);
                msg.hdr.type = 0x00;
                msg.hdr.subtype = 0x03;
                if (MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1) {
                    printf("Error sending position command\n");
                }
                break;
            case 4:
                printf("Requesting aircraft information...\n");
                msg.hdr.type = 0x00;
                msg.hdr.subtype = 0x04;
                if (MsgSend(server_coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
                    printf("Error sending information request\n");
                } else {
                    printf("Aircraft information:\nSpeed: %d\nAltitude: %d\nPosition: %d\n", msg.speed, msg.altitude, msg.position);
                }
                break;
            case 5:
                printf("Exiting...\n");
                goto exit_loop;
            default:
                printf("Invalid command\n");
        }
    }

    exit_loop:
    /* Close the connection */
    name_close(server_coid);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int ret;
    printf("Running Aircraft_Client ... \n");
    ret = client();  // calling client function

    return ret;
}
