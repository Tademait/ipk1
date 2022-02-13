/* -----------------------------------------
    IPK Projekt #1 - TCP Server
    Author: Tadeas Kozub [ xkozub06 ]
    5. 2. 2022

------------------------------------------*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

/**
 * @brief  Get the current load of the server cpu
 * @return Returns the current load of the server as integer in range 0 to 100
 */

int get_cpu_load()
{
    FILE *cmd;
    char result[1024];
    cmd = popen("cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}' | awk '{print (100 - $1)}'", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    fgets(result, sizeof(result), cmd);
    int result_int = atoi(result);
    pclose(cmd);
    return result_int;
}

/**
 * @brief  Get the manufacturer and model of the server's CPU
 * @param  Pointer to a buffer that will contain the cpu information
 */

void get_cpu_info(char* buffer)
{
    FILE *cmd;
    char result[1024];

    // cpu info is on the 13th line. We also need to cut the start of the message
    // cmd = popen("lscpu | sed -n 13p | cut -c 22-", "r");
    cmd = popen("lscpu | sed -n 13p | awk '{print substr($0, index($0, $3))}' ", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    fgets(result, sizeof(result), cmd);
    pclose(cmd);
    strcpy(buffer, result);
}


int main(int argc, char const *argv[])
{
    int port_num;
    if (argc == 2)
    {
        port_num = atoi(argv[1]);
    }
    else if (argc > 2)
    {
        printf("Error: too many arguments\n");
        exit(1);
    }
    else
    {
        // default port number
        port_num = 8000; 
    }
    
    char http_response[512] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length:";

    // init the socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);


    // set up the server address
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to the address
    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    // listen for connections and serve the requested info
    listen(server_socket, 1); 


    int client_socket;
    printf("-- server is waiting for connections on port %d --\n", port_num);
    printf("--       press Ctrl + C to stop the server.       --\n");
    while (1) {
        // accept is blocking until client connects
        client_socket = accept(server_socket, NULL, NULL);
        char buffer[1024] = {0}; 
        int valread = read(client_socket, buffer, 1024);
        //printf("%s\n",buffer);
        char http_response_temp[512] = {0};
        strcpy(http_response_temp, http_response);
        if (!strncmp(buffer, "GET /hostname", 13))
        {
            char hostname[64] = {0};
            gethostname(hostname, 128);
            char info[128] = {0};
            snprintf(info, sizeof(info), "%ld\r\n\r\n%s\r\n", strlen(hostname) + 2, hostname);
            strcat(http_response_temp, info);
        }
        else if (!strncmp(buffer, "GET /cpu-name", 13))
        {
            char cpu_name[64] = {0};
            get_cpu_info(cpu_name);
            char info[128] = {0};
            snprintf(info, sizeof(info), "%ld\r\n\r\n%s\r\n", strlen(cpu_name), cpu_name);
            strcat(http_response_temp, info);
        }
        else if (!strncmp(buffer, "GET /load", 9))
        {
            int load_percent = get_cpu_load();
            char info[128] = {0};
            snprintf(info, sizeof(info), "%ld\r\n\r\n%d%%\r\n", strlen(info) + 4, load_percent);
            strcat(http_response_temp, info);
        }
        else
        {
            char info[128] = "18\r\n\r\n400: Bad Request\r\n";
            strcat(http_response_temp, info);
        }
        //printf("%s", http_response_temp);
        send(client_socket, http_response_temp, sizeof(http_response_temp), 0);
        // clearing out the request buffer
        memset(buffer, 0, sizeof(buffer));
        // client served, close socket
        close(client_socket);
    }
    return 0;
}
