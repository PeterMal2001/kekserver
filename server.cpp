#include<iostream>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
using namespace std;

int main(int argc, char **argv){
	//vars init
	struct sockaddr client_addr;
    struct addrinfo settings;
    struct addrinfo *list;
	int ld,sd,bd,ad,nl;
	char sender_ip[INET_ADDRSTRLEN],server_ip[INET_ADDRSTRLEN],*msg,*name,pass[2];
    void *reply;
    reply=(void*)malloc(1);
	msg=(char*)malloc(1024);
    //database init
    char **db;
	db=(char**)malloc(2*sizeof(char*));
    db[0]=(char*)malloc(4*sizeof(char));
    db[1]=(char*)malloc(2*sizeof(char));
    db[0][0]='u';db[0][1]='s';db[0][2]='e',db[0][3]='r';//login is "user"
    db[1][0]='p',db[1][1]='w';//password is "pw"
    //addreses list configurating
    memset(&settings,0,sizeof(settings));
	settings.ai_family=AF_UNSPEC;
    settings.ai_flags=SOCK_STREAM;
    settings.ai_flags=AI_PASSIVE;
    //get addresses list
    ld=getaddrinfo(NULL,argv[1],&settings,&list);
    if(ld!=0){perror("address list");return -1;}
    //bind address
    for(;list!=NULL;list->ai_next){
        sd=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
        if(sd<0){continue;}
        bd=bind(sd,list->ai_addr,list->ai_addrlen);
        if(bd<0){close(sd);continue;}
        break;
    }
    if(list==NULL){
        perror("port bind");
        return -1;    }
    cout<<"Server created";
    freeaddrinfo(list);

    //action time
	while(1){
		//check for server stop
		if(cin.peek()!=EOF){
			break;
		}
		unsigned int k = (unsigned int) sizeof(client_addr);
        //listening
		listen(sd,100);
		//connection accept
		ad=accept(sd,&client_addr,&k);
		if(ad<0){perror("connacept");}
		//store connected address
		inet_ntop(AF_INET, &(((struct sockaddr_in*)((struct sockaddri*)&(client_addr))) ->sin_addr),sender_ip, sizeof(sender_ip));
        //read
		read(ad,&nl,sizeof(int));
        name=(char*)malloc(nl);
        read(ad,name,nl);
        read(ad,pass,2);
        if((strcmp(name,db[0])!=0)||(sizeof(name)!=sizeof(db[0]))||(strcmp(pass,db[1])!=0)){
            reply=(void*)'d';
            write(ad,reply,1);//deny
            cout<<"wrong login or password from "<<sender_ip<<"\n";
        }
        else{
            reply=(void*)'a';
            write(ad,reply,1);//accept
            cout<<"correct login and password from "<<sender_ip<<"\n";
            while(1){
                if(cin.peek()!=EOF){
                    break;
                }
                read(ad,msg,1024);
                cout<<msg;
            }
        }      
		//print ip and it's message
		cout<<sender_ip<<":"<<msg;
		//connection close
		free(name);
        close(ad);
	}
	//finishing
    free(msg);
	close(sd);
	return 0;
}
