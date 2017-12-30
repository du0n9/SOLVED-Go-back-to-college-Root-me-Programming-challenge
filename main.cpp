#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>


#define DEFAULT_BUFLEN 5120
using namespace std;

/*
		Bot to solve Go back to college Root-me Programing challenge.
		CODE C++
		Author: Du0n9
		I used Dev-C++5.11 and if you use Dev-C++ too,you need to link libws2_32.a (WinSock2) and libadvapi32.a (Instruction:https://www.tinkerforge.com/en/doc/Software/API_Bindings_C.html)
		or just open Project3.dev with Dev-C++
		You can test it by running Project3.exe
		Thank Z3r0n3 for suggestion
		https://01day.wordpress.com/tag/root-me-org-solutions/
		
*/



int __cdecl main() 
{   

    int iResult;
    
    //Initialize WSA
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    cout<<"Initialize WSA OK"<<endl;
	
	
	//Create Socket for Client
	struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    getaddrinfo("irc.root-me.org", "6667", &hints, &result);   		//change "127.0.0.1" to host (node) name or a numeric host address :
    cout<<"Resolve the server address and port OK"<<endl;
    

    // Attempt to connect to an address until one succeeds
    cout<<"Attempt to connect to  address"<<endl;
    SOCKET ConnectSocket = INVALID_SOCKET;
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
        	cout<<"socket failed with error: "<<WSAGetLastError()<<endl;
            WSACleanup();
            return 1;
        }
        else{
        	cout<<"Connected to irc.root-me.org Port:6667"<<endl;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
	cout<<endl;
	
    // Do the registration stuff and join challenge’s room
    
	const char *user="USER 01110129 ignore ignore :ChallengeBot\n";		//send USER
    iResult = send( ConnectSocket, user, strlen(user), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    cout<<"USER command sended"<<endl;
    
    const char *nick="NICK du0n9\n";												//send NICK
    send( ConnectSocket, nick, strlen(nick), 0 );
	cout<<"NICK command sended"<<endl;
	cout<<endl;
	
	int registered =0;
	do {
		char rec1[DEFAULT_BUFLEN]="\0";
        iResult=recv(ConnectSocket, rec1, DEFAULT_BUFLEN, 0);
        if ( iResult > 0 ){
        	cout<<rec1<<endl;
        	cout<<"Bytes received: "<<iResult<<endl;
        	cout<<endl;
        }
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
           
        if(strstr(rec1,"Current Global Users")) {
        	registered = 1;
        	cout<<endl;
    		cout<<"Registered"<<endl;
    		cout<<endl;
        }
    }
    while((!registered)&&(iResult>0));

	if (!registered) {
		cout<<"Register FAIL"<<endl;
		return 1;
	}
	
	
	
	
	const char *join="JOIN #Root-Me_Challenge\n";										//send JOIN
	send( ConnectSocket, join, strlen(join), 0 );
	char room_info[DEFAULT_BUFLEN]="\0";												//receive room_info									
	recv(ConnectSocket,room_info, DEFAULT_BUFLEN, 0);
	cout<<room_info<<endl;
	cout<<endl;
	cout<<endl;									
	
		// I don't  care about PING PONG because bot doesn't need much time to chat with Candy  
		// (PING timeout is 121 seconds)
	
	
	
	
	
	//Receive flag
	const char *candy="PRIVMSG Candy :!ep1\r\n";			//PM Candy
	send( ConnectSocket, candy, strlen(candy), 0 );
	char candy_rep[DEFAULT_BUFLEN]="\0";
	recv(ConnectSocket,candy_rep, DEFAULT_BUFLEN, 0);		//receive candy reply
	cout<<candy_rep<<endl;

	int num1,num2;											// 2 numbers to calculate
	float ans;												// answer
   	char num[10];											// get number in candy_rep
   	memcpy(num,strchr(strchr(candy_rep,':')+1,':')+1,strchr(candy_rep,'/')-strchr(strchr(candy_rep,':')+1,':')-1);
   	num1=atoi(num);											//number1
  	memcpy(num,strchr(candy_rep,'/')+2,candy_rep+strlen(candy_rep)-strchr(candy_rep,'/'));
	num2=atoi(num);											//number2
	
//	cout<<sqrt(num1)*num2<<endl;
//	ans= roundf((sqrt(num1)*num2 ) * 100) / 100;
//	cout<<ans<<endl;
	char answer[30]="\0";
//	snprintf(answer, sizeof answer, "%f", sqrt(num1)*num2);
	sprintf(answer, "%.2f", sqrt(num1)*num2);				// Calculate answer and round to two decimals .
	cout<<"Answer: "<<answer<<"  Send to Candy  "<<endl;
	char message[100]="PRIVMSG Candy :!ep1 -rep ";
	strcat(message,answer);
	strcat(message,"\n");
	send( ConnectSocket,message, strlen(message), 0 );
	char flag[DEFAULT_BUFLEN]="\0";
	recv(ConnectSocket,flag, DEFAULT_BUFLEN, 0);
	cout<<"LET VALIDATE THIS CHALLENGE <3<3<3"<<endl;
	cout<<flag<<endl;	
	send(ConnectSocket,"QUIT :By3 By3!\n",strlen("QUIT :By3 By3!"),0);		//Quit IRC












    // cleanup
    shutdown(ConnectSocket, SD_SEND);
    closesocket(ConnectSocket);
    WSACleanup();
    cout<<"PRESS ENTER TO EXIT"<<endl;
	getchar();
    return 0;
}
