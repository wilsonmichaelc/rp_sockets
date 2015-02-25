/*
    C socket server example, handles multiple clients using threads
    Compile
    gcc server.c -lpthread -o server
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include <stdbool.h>
#include <rp.h>
 
//the thread function
void *connection_handler(void *);
void *acquire_to_socket(void *);
bool inArray(char *val, const char *arr[]);
 
int main(int argc , char *argv[])
{
    /* Print error, if rp_Init() function failed */
    if(rp_Init() != RP_OK){
        fprintf(stderr, "Rp api init failed!\n");
    }

    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    //puts("Waiting for incoming connections...");
    //c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        // Now join the thread , so that we dont terminate before the thread
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    rp_Release();
    return 0;
}
 
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
    //static const char input_frequency[] = "Select a frequency (0.0 - 6.2e+07 Hz): ";
    //static const char input_amplitude[] = "Select an amplitude (0.0-2.0 Vpp): ";
    static const char imput_sample_rate[] = "Select sample rate (RP_SMP_125M, RP_SMP_15_625M, RP_SMP_1_953M, RP_SMP_122_070K, RP_SMP_15_258K, RP_SMP_1_907K) : ";
    static const char input_trigger[] = "Select a trigger (RP_TRIG_SRC_NOW, RP_TRIG_SRC_CHA_PE, RP_TRIG_SRC_CHA_NE, RP_TRIG_SRC_CHB_PE, RP_TRIG_SRC_CHB_NE): ";
    static const char error[] = "Invalid input detected. Start over.\n";

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
            if( !inArray( general_buffer, valid_channels ) ) {
                write(sock, error, strlen(error));
                continue;
            }
            
            /* Get the sample rate */
            write(sock, imput_sample_rate, strlen(imput_sample_rate));
            bytesRead = read(sock, general_buffer, 1024);
            general_buffer[bytesRead] = '\0';

            /* Make sure the user sent us a valid sample rate */
            if( !inArray( general_buffer, valid_sample_rates ) ){
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

            if( !inArray( general_buffer, valid_triggers ) ){
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

            /* Request the channel (1/2), amplitude (0.0-2.0), frequency () */
            char response[] = "Not implemented yet... \n";
            write(sock , response , strlen(response));

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

void *acquire_to_socket(void *socket_desc){

    /* Get the socket */
    int sock = *(int*)socket_desc;

    /* Create buffer and size to hold data points */
    uint32_t array_size = 16 * 1024; 
    float *buff;
    buff = (float *)malloc(array_size * sizeof(float));

    /* Starts acquisition */
    rp_AcqStart();
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    rp_AcqSetSamplingRate(RP_SMP_15_625M);
    rp_AcqGetOldestDataV(RP_CH_1, &array_size, buff);

    /* Send buffer to socket */
    int i;
    for (i = 0; i < array_size; ++i)
    {
        send(sock, &buff[i], sizeof(float), 0);
    }

    /* Releasing resources */
    free(buff);
    return 0;
}

bool inArray(char *val, const char *arr[]){
    int i;
    int size = sizeof(arr) / sizeof(arr[0]);
    for (i=0; i < size; i++) {
        if( memcmp(val, arr[i], (sizeof(arr[i]) - 1) ) == 0 ){
            return true;
        }            
    }
    return false;
}