#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd, sockfd2;

    char data[] = "application/x-www-form-urlencoded";
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    char **login = malloc(2 * sizeof(char *));
    login[0] = malloc(BUFLEN);
    login[1] = malloc(BUFLEN);

    sockfd2 = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);

    strcpy(login[0], "username=student");
    strcpy(login[1], "password=student");
        
    // Ex 1.1: GET dummy from main server
    message = compute_get_request("34.118.48.238", "/api/v1/dummy", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 1.2: POST dummy and print response from main server
    message = compute_post_request("34.118.48.238", "/api/v1/dummy", data, login, 2, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 2: Login into main server
    message = compute_post_request("34.118.48.238", "/api/v1/auth/login", data, login, 2, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 3: GET weather key from main server
    char cookie[] = "connect.sid=s%3AR2-XOznczKTxiWdJfA1ovWjNwAcJ11GP.jbMyDt4BBaTRg5f%2FlL4Nq590GrSNP30Gib9z6%2FW9cuE";
    char **cookies = malloc(sizeof(char *));
    cookies[0] = malloc(strlen(cookie) + 1);
    strcpy(cookies[0], cookie);

    message = compute_get_request("34.118.48.238", "/api/v1/weather/key", NULL, cookies, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    //{"key":"b912dd495585fbf756dc6d8f415a7649"}

    // Ex 4: GET weather data from OpenWeather API
    char *queryParams = "lat=1.1&lon=1.1&appid=b912dd495585fbf756dc6d8f415a7649";

    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", queryParams, NULL, 0);
    puts(message);
    send_to_server(sockfd2, message);
    response = receive_from_server(sockfd2);
    puts(response);

    close_connection(sockfd2);

    // recieved: {"coord":{"lon":1.1,"lat":1.1},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],
    // "base":"stations","main":{"temp":301.01,"feels_like":303.67,"temp_min":301.01,"temp_max":301.01,"pressure":1011,"humidity":71,
    // "sea_level":1011,"grnd_level":1011},"visibility":10000,"wind":{"speed":3.5,"deg":168,"gust":3.69},"clouds":{"all":4},"dt":1620744849,
    // "sys":{"sunrise":1620712026,"sunset":1620755816},"timezone":0,"id":0,"name":"","cod":200}

    // Ex 5: POST weather data for verification to main server

    // Ex 6: Logout from main server
    message = compute_get_request("34.118.48.238", "/api/v1/auth/logout", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!
    free(login[0]);
    free(login[1]);
    free(login);
    close_connection(sockfd);
    return 0;
}
