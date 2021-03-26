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

int sock_init(char *port){
    int ld,sd,bd;
    struct addrinfo settings,*list;
    //addreses list configurating
    memset(&settings,0,sizeof(settings));
	settings.ai_family=AF_UNSPEC;
    settings.ai_flags=SOCK_STREAM;
    settings.ai_flags=AI_PASSIVE;
    //get addresses list
    ld=getaddrinfo(NULL,port,&settings,&list);
    if(ld!=0){perror("address list");return -1;}
    //bind address
    for(;list!=NULL;list=list->ai_next){
        sd=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
        if(sd<0){continue;}
        bd=bind(sd,list->ai_addr,list->ai_addrlen);
        if(bd<0){close(sd);continue;}
        break;
    }
    if(list==NULL){
        perror("port bind");
        exit(-1);
    }
    cout<<"Server created\n";
    //cleaning
    freeaddrinfo(list);
    return sd;
}

int main(int argc, char **argv){
	//vars init
	struct sockaddr client_addr;
	int sd,ad,nl;
	char sender_ip[INET_ADDRSTRLEN],server_ip[INET_ADDRSTRLEN],*msg,*name,pass[2],a='a',d='d';
    void *reply;
    //chars buffers malloc
    reply=static_cast<void*>(malloc(1));
	msg=static_cast<char*>(malloc(1024));
    //database init
    char **db;
	db=static_cast<char**>(malloc(2*sizeof(char*)));
    db[0]=static_cast<char*>(malloc(4*sizeof(char)));
    db[1]=static_cast<char*>(malloc(2*sizeof(char)));
    db[0][0]='u';db[0][1]='s';db[0][2]='e',db[0][3]='r';//login is "user"
    db[1][0]='p',db[1][1]='w';//password is "pw"
    //socket bind
    sd=sock_init(argv[1]);
    //action time
	while(1){
		//check for server stop
		if(cin.peek()!=EOF){
			break;
		}
		unsigned int k = static_cast<unsigned int>(sizeof(client_addr));
        //listening
		listen(sd,100);
		//connection accept
		ad=accept(sd,&client_addr,&k);
		if(ad<0){perror("connacept");}
		//store connected address
		inet_ntop(AF_INET, &(((struct sockaddr_in*)((struct sockaddri*)&(client_addr))) ->sin_addr),sender_ip, sizeof(sender_ip));
        //read
		read(ad,&nl,sizeof(int));
        name=static_cast<char*>(malloc(nl));
        read(ad,name,nl);
        read(ad,pass,2);
        if((strcmp(name,db[0])!=0)||(sizeof(name)!=sizeof(db[0]))||(strcmp(pass,db[1])!=0)){
            reply=static_cast<void*>(&d);
            write(ad,reply,1);//deny
            cout<<"wrong login or password from "<<sender_ip<<"\n";
        }
        else{
            reply=static_cast<void*>(&a);
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
		//connection close
		free(name);
        close(ad);
	}
	//cleaning and finishing
    free(msg);
    free(name);
    free(reply);
	close(sd);
	return 0;
}
