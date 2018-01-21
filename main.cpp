#include <ws2tcpip.h>
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
		Thank Z3r0n3 for suggestion
		https://01day.wordpress.com/tag/root-me-org-solutions/
*/



int main() 
{   

	
	int result;
	
	// Initialize Winsock
	
    WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0) {
		cout<<"Initialize Winsock Fail: "<<result<<endl;
		return 1;
	}
	else cout<<"Initialize Winsock OK"<<endl;

    // Resolve the server address and port
    addrinfo hints,							  	  //A pointer to an addrinfo structure that provides hints about the type of socket the caller supports.
			 *res;                                //A linked list of one or more addrinfo structures that contains response information about the host.
    ZeroMemory( &hints, sizeof(hints) );		  //The ai_addrlen, ai_canonname, ai_addr, and ai_next members of the addrinfo structure pointed to by the pHints parameter must be zero or NULL. 
												  //Otherwise the GetAddrInfoEx function will fail with WSANO_RECOVERY.
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    result = getaddrinfo( "irc.root-me.org", "6667" , &hints, &res);
	if ( result != 0) {
		cout<<"getaddrinfo failed: "<<result<<endl;
		return 1;
	}
	else cout<<"getaddrinfo OK"<<endl;


	// Create connecting Socket
	SOCKET connectSOCK = socket( res->ai_family , res->ai_socktype , res->ai_protocol );
	if ( connectSOCK == INVALID_SOCKET ) {
		cout<<"Create connect sock failed: "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 1;
	}
	else cout<<"Create connect sock OK"<<endl;
	
	// Connect 
	result = connect( connectSOCK , res->ai_addr , (int) res->ai_addrlen);
	if ( result != 0) {
		cout<<"connect failed: "<<WSAGetLastError()<<endl;
		closesocket(connectSOCK );
		WSACleanup();
		return 1;
	}
	else cout<<"connect OK"<<endl;
	
    freeaddrinfo(res);
    // Do the registration stuff and join challenge’s room
    
	const char *user="USER 01110129 ignore ignore :ChallengeBot\n";		//send USER
    result = send( connectSOCK, user, strlen(user), 0 );
    if (result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connectSOCK);
        WSACleanup();
        return 1;
    }
    cout<<"USER command sended"<<endl;
    
    const char *nick="NICK du0n9\n";												//send NICK
    send( connectSOCK, nick, strlen(nick), 0 );
	cout<<"NICK command sended"<<endl;
	cout<<endl;
	
	int registered =0;
	do {
		char rec1[DEFAULT_BUFLEN]="\0";
        result=recv(connectSOCK, rec1, DEFAULT_BUFLEN, 0);
        if ( result > 0 ){
        	cout<<rec1<<endl;
        	cout<<"Bytes received: "<<result<<endl;
        	cout<<endl;
        }
        else if ( result == 0 )
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
    while((!registered)&&(result>0));

	if (!registered) {
		cout<<"Register FAIL"<<endl;
		return 1;
	}
	
	
	
	
	const char *join="JOIN #Root-Me_Challenge\n";										//send JOIN
	send( connectSOCK, join, strlen(join), 0 );
	char room_info[DEFAULT_BUFLEN]="\0";												//receive room_info									
	recv(connectSOCK,room_info, DEFAULT_BUFLEN, 0);
	cout<<room_info<<endl;
	cout<<endl;
	cout<<endl;									
	
		// I don't  care about PING PONG because bot doesn't need much time to chat with Candy  
		// (PING timeout is 121 seconds)
	
	
	
	
	
	//Receive flag
	const char *candy="PRIVMSG Candy :!ep1\r\n";			//PM Candy
	send( connectSOCK, candy, strlen(candy), 0 );
	char candy_rep[DEFAULT_BUFLEN]="\0";
	recv(connectSOCK,candy_rep, DEFAULT_BUFLEN, 0);		//receive candy reply
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
	send( connectSOCK,message, strlen(message), 0 );
	char flag[DEFAULT_BUFLEN]="\0";
	recv(connectSOCK,flag, DEFAULT_BUFLEN, 0);
	cout<<"LET VALIDATE THIS CHALLENGE <3<3<3"<<endl;
	cout<<flag<<endl;	
	send(connectSOCK,"QUIT :By3 By3!\n",strlen("QUIT :By3 By3!"),0);		//Quit IRC












    // cleanup
    shutdown(connectSOCK, SD_SEND);
    closesocket(connectSOCK);
    WSACleanup();
    cout<<"PRESS ENTER TO EXIT"<<endl;
	getchar();
    return 0;
}
