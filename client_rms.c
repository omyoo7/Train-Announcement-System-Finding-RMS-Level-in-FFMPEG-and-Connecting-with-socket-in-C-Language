

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
        int network_socket;

        network_socket=socket(AF_INET,SOCK_STREAM,0);

        struct sockaddr_in server_address;
        server_address.sin_family=AF_INET;
        server_address.sin_port=htons(2020);
        server_address.sin_addr.s_addr=INADDR_ANY;

		int  count = 0, flag = 1;
		char alert_on[20] = "Alert ON\n";
		char alert_off[20] = "Alert OFF\n";
		FILE *pipein;
		pipein  = popen("ffmpeg -f alsa -i hw:0,0 -af astats=metadata=1:reset=1,ametadata=print:key=lavfi.astats.Overall.RMS_level -f null - 2>&1", "r");

		char line[1024];
		char *s;
		double om_rms;

int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	if(connection_status == -1){
                printf("Error in connection\n");
    }
while(1){       
	 fscanf(pipein, "%[^\n]\n", line);

        s = strstr(line, "RMS_level");
        if (s)
        {
            s += 10;

            om_rms = atof(s);
		//	fprintf(stderr, "  RMS_VALUE = %lf\n", om_rms);
			 if(om_rms > -14.50000)
                {
                    count ++;
                    if((count == 10) && (flag == 1))
                        {
						if(send(network_socket,alert_on, strlen(alert_on), 0) == -1){
                        fprintf(stderr, "failure sending msg\n");
                        }
                        printf("Alert ON\n");
                        fprintf(stderr, "Train announcement is going on....Stop othe process\nRMS_VALUE = %lf dB\n\n", om_rms);
                        flag = 0;
                        }
                }
                else if (om_rms < -16.00000)
                {
                    count --;
                    if((count == -300) && (flag == 0))
                        {
						if(send(network_socket,alert_off, strlen(alert_off), 0) == -1){
						fprintf(stderr, "failure sending msg\n");
						}
                        printf("Alert OFF\n");
                        fprintf(stderr, "Train announcement have been stoped..... Start othe process\nRMS_VALUE = %lf dB\n\n", om_rms);
                        flag = 1;
                        }
                    else if (count < -300) count = -300;
                }
                else count = 0;
         }

}
close(network_socket);
}
