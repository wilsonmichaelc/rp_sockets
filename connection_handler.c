#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <ctype.h>
#include <sys/socket.h>
#include <unistd.h>    //write
#include <pthread.h>
#include <rp.h>

#include "acquire_to_socket.h"
#include "generate_arbitrary_waveform.h"
#include "str_to_enum.h"

void *connection_handler(void *);

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    /* Get the socket descriptor */
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];

    /* Commands */
    static const char quit[] = "EXIT";
    static const char acquire[] = "ACQUIRE";
    static const char generate_arb[] = "GENERATE_ARB";

    /* Messages */
    static const char input_channel[] = "Select a channel (RP_CH_1, RP_CH_2): ";
    static const char input_acq_len[] = "How many point would you like to acquire (0=continuous): ";
    static const char input_frequency[] = "Select a frequency (0.0 - 6.2e+07 Hz): ";
    static const char input_amplitude[] = "Select an amplitude (0.0-2.0 Vpp): ";
    static const char imput_sample_rate[] = "Select sample rate (RP_SMP_125M, RP_SMP_15_625M, RP_SMP_1_953M, RP_SMP_122_070K, RP_SMP_15_258K, RP_SMP_1_907K) : ";
    static const char input_trigger[] = "Select a trigger (RP_TRIG_SRC_NOW, RP_TRIG_SRC_CHA_PE, RP_TRIG_SRC_CHA_NE, RP_TRIG_SRC_CHB_PE, RP_TRIG_SRC_CHB_NE): ";
    static const char error[] = "invalid input."; 

    char general_buffer[1024];

    int bytesRead;
     
    /* Receive a message from client */
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {

        /* end of string marker */
        client_message[read_size] = '\0';
        memset(general_buffer, 0, 1024);

        // QUIT
        if( read_size >= (sizeof(quit) - 1) && memcmp(client_message, quit, (sizeof(quit) - 1) ) == 0 ) {
            
            /* Exit */
            rp_GenOutDisable(RP_CH_1);
            rp_GenOutDisable(RP_CH_2);

            close(sock);
            read_size = 0;
            break;

        // ACQUIRE
        }else if( read_size >= (sizeof(acquire) - 1) && memcmp(client_message, acquire, (sizeof(acquire) - 1) ) == 0 ){
            
            /* 
                Request channel, sampling rate, trigger
            */

            struct acq_params_tag params;

            /* Get the channel */
            write(sock, input_channel, strlen(input_channel));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';
            params.channel = getChannel(general_buffer);
            
            /* Get the sample rate */
            write(sock, imput_sample_rate, strlen(imput_sample_rate));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';
            params.sample_rate = getSampleRate(general_buffer);

            /* Get the trigger */
            write(sock, input_trigger, strlen(input_trigger));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';
            params.trigger = getTrigger(general_buffer);

            /* How many points are we acquiring? */
            write(sock, input_acq_len, strlen(input_acq_len));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            char tmp_pts[bytesRead + 1];
            strncpy(tmp_pts, general_buffer, bytesRead);
            int points = atoi(tmp_pts);

            if(points != 0){
                params.num_points = points;
            }else{
                write(sock, "Invalid num points.\n", strlen("Invalid num points.\n"));
                continue;
            }

            params.socket = &sock;

            /* Begin acquisition */
            acquire_to_socket(&params);

        }else if( read_size >= (sizeof(generate_arb) - 1) && memcmp(client_message, generate_arb, (sizeof(generate_arb) - 1) ) == 0 ){

            /* 
                Request the channel (1/2), amplitude (0.0-2.0), frequency () 
            */

            struct gen_params_tag params;

            /* Get the channel */
            write(sock, input_channel, strlen(input_channel));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';
            params.channel = getChannel(general_buffer);

            /* Get the frequency */
            write(sock, input_frequency, strlen(input_frequency));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* 
                Make sure user sent us a valid frequency 
                0.0 - 6.2e+07 Hz
            */
            double frequency = strtod(general_buffer,NULL);
            if( (frequency < 0.0) || (frequency > 62000000) ) {
                write(sock, error, strlen(error));
                continue;
            }else{
                params.frequency = frequency;
            }

            /* Get the amplitude */
            write(sock, input_amplitude, strlen(input_amplitude));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* 
                Make sure user sent us a valid amplitude 
                0.0 - 2.0 Vpp
            */
            double amplitude = strtod(general_buffer,NULL);
            if( (amplitude < 0.0) || (amplitude > 2.0) ) {
                write(sock, error, strlen(error));
                continue;
            }else{
                params.amplitude = amplitude;
            }

            /* Get the waveform to generate */
            //write(sock, input_waveform, strlen(input_waveform));

            // READ IN ARB WAVEFORM HERE

            generate_arbitrary_waveform(&params);

        }else{

            /* Send the message back to client */
            char response[] = "Not a valid command. \n";
            write(sock , response , strlen(response));

        }

        /* clear the message buffer */
        memset(client_message, 0, 2000);
        
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;
}