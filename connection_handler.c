#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <unistd.h>    //write
#include <pthread.h>
#include <rp.h>

#include "connection_handler.h"
#include "acquire_to_socket.h"
#include "generate_arbitrary_waveform.h"
#include "in_array.h"

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
    static const char input_channel[] = "Select a channel (RP_CH_1, RP_CH_2, RP_CH_ALL): ";
    static const char input_frequency[] = "Select a frequency (0.0 - 6.2e+07 Hz): ";
    static const char input_amplitude[] = "Select an amplitude (0.0-2.0 Vpp): ";
    static const char imput_sample_rate[] = "Select sample rate (RP_SMP_125M, RP_SMP_15_625M, RP_SMP_1_953M, RP_SMP_122_070K, RP_SMP_15_258K, RP_SMP_1_907K) : ";
    static const char input_trigger[] = "Select a trigger (RP_TRIG_SRC_NOW, RP_TRIG_SRC_CHA_PE, RP_TRIG_SRC_CHA_NE, RP_TRIG_SRC_CHB_PE, RP_TRIG_SRC_CHB_NE): ";
    static const char input_waveform[] = "Send a waveform: ";
    static const char error[] = "Invalid input detected. Start over. \n";

    const char *valid_channels[] = {"RP_CH_1", "RP_CH_2", "RP_CH_ALL"};
    const char *valid_triggers[] = {"RP_TRIG_SRC_NOW", "RP_TRIG_SRC_CHA_PE", "RP_TRIG_SRC_CHA_NE", "RP_TRIG_SRC_CHB_PE", "RP_TRIG_SRC_CHB_NE"}; 
    const char *valid_sample_rates[] = {"RP_SMP_125M", "RP_SMP_15_625M", "RP_SMP_1_953M", "RP_SMP_122_070K", "RP_SMP_15_258K", "RP_SMP_1_907K"}; 

    char general_buffer[1024];

    int bytesRead;
     
    /* Receive a message from client */
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {

        /* end of string marker */
        client_message[read_size] = '\0';

        if( read_size >= (sizeof(quit) - 1) && memcmp(client_message, quit, (sizeof(quit) - 1) ) == 0 ) {
            
            /* Exit */
            close(sock);
            read_size = 0;
            break;

        }else if( read_size >= (sizeof(acquire) - 1) && memcmp(client_message, acquire, (sizeof(acquire) - 1) ) == 0 ){
            
            /* 
                Request channel, sampling rate, trigger
            */

            /* Get the channel */
            write(sock, input_channel, strlen(input_channel));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* Make sure user sent us a valid channel */
            if( !in_array( general_buffer, valid_channels ) ) {
                write(sock, error, strlen(error));
                continue;
            }
            
            /* Get the sample rate */
            write(sock, imput_sample_rate, strlen(imput_sample_rate));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* Make sure the user sent us a valid sample rate */
            if( !in_array( general_buffer, valid_sample_rates ) ){
                write(sock, error, strlen(error));
                continue;
            }else{
                printf("%i\n", RP_SMP_15_625M);
                //rp_AcqSetSamplingRate(general_buffer);
            }

            /* Get the trigger */
            write(sock, input_trigger, strlen(input_trigger));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            if( !in_array( general_buffer, valid_triggers ) ){
                write(sock, error, strlen(error));
                continue;
            }else{
                printf("%i\n", RP_TRIG_SRC_CHA_PE);
                //rp_AcqSetTriggerSrc(general_buffer);
            }

            /* Begin acquisition */
            char response[] = "Input checks out!! \n";
            write(sock, response, strlen(response));
            //acquire_to_socket(&sock);

        }else if( read_size >= (sizeof(generate_arb) - 1) && memcmp(client_message, generate_arb, (sizeof(generate_arb) - 1) ) == 0 ){

            /* 
                Request the channel (1/2), amplitude (0.0-2.0), frequency () 
            */

            /* Get the channel */
            write(sock, input_channel, strlen(input_channel));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* Make sure user sent us a valid channel */
            char channel[bytesRead];
            strcpy(channel, general_buffer);
            if( !in_array( channel, valid_channels ) ) {
                write(sock, error, strlen(error));
                continue;
            }

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
            }

            /* Get the waveform */
            const int size = 16384;
            write(sock, input_waveform, strlen(input_waveform));
            bytesRead = read(sock, general_buffer, size);
            general_buffer[bytesRead] = '\0';

            /* 
                Make sure user sent us a valid waveform 
                Size is 16 * 1024 of floats.
            */
            //int i;
            //float waveform[size];
            //for(i=0; i<size; i++){
            //    waveform[i] = atof(general_buffer[i]);
            //}

            //generate_arbitrary_waveform(channel, amplitude, frequency, waveform);

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