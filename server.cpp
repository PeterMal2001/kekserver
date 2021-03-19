#include<iostream>
#include<cmath>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

int main(int argc, char **argv){
	//vars init
	struct sockaddr_in addr;
	int sd,bd,ad;
	unsigned int k;
	char *ip,*msg;
	ip=(char*)malloc(64);
	msg=(char*)malloc(1024);
	//addres setup
	addr.sin_family=AF_INET;
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	//socket init
	sd=socket(AF_INET,SOCK_STREAM,0);
	if(sd<0){perror("sockinit");return -1;}
	//socket bind
	bd=bind(sd,(struct sockaddr*)&addr,sizeof(addr));
	if(bd<0){perror("sockbind");return -1;}
	//action time
	while(1){
		//check for server stop
		if(cin.peek()!=EOF){
			break;
		}
		//listening
		listen(sd,5);
		//connection accept
		ad=accept(sd,(struct sockaddr*)&addr,&k);
		if(ad<0){perror("connacept");return -1;}
		//store connected address
		ip=inet_ntoa(addr.sin_addr);
		//read a message
		read(ad,msg,1024);
		//print ip and it's message
		cout<<ip<<":"<<msg;
		//connection close
		close(ad);
	}
	//finishing
	close(sd);
	return 0;
}
