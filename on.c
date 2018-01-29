#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void main()
{

    int  count = 0, flag = 1;
    FILE *pipein;
    pipein  = popen("ffmpeg -f alsa -i hw:0,0 -af astats=metadata=1:reset=1,ametadata=print:key=lavfi.astats.Overall.RMS_level -f null - 2>&1", "r");
     
    char line[1024];
    char *s;
    double om_rms;

    while(1)
    {
        
        fscanf(pipein, "%[^\n]\n", line);

        s = strstr(line, "RMS_level");
        if (s)
        {
            s += 10;
             
            om_rms = atof(s);
             
            // Print the RMS value
            //fprintf(stderr, "  RMS_VALUE = %lf\n", om_rms);
        
                if(om_rms > -14.50000)
                {
                    count ++;
                    if((count == 10) && (flag == 1))
                        {
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
						printf("Alert OFF\n");
						fprintf(stderr, "Train announcement have been stoped..... Start othe process\nRMS_VALUE = %lf dB\n\n", om_rms);
						flag = 1;
						}
					else if (count < -300) count = -300;
				}	
				else count = 0;
         }
    
}
}
