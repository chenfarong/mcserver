﻿

/*
消息总线服务器

负责将接收到的消息转发给消息的订阅者

发送失败后将此订阅者移除掉

*/



//#include "fe.h"
//#include "global.h"
#include "server.h"
#include "mcs.h"
//#include "fileSys.h"
//#include "dbSys.h"
//#include "./MySQL/DbMySQL.h"
//#include "SSOpcodes.h"
//#include "command.h"

#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define IDS_VERSION  "0.9.1 2016-10-20"



#ifdef _MSC_VER

#include "ServiceWin32.h"



/*
* -1 - not in service mode
*  0 - stopped
*  1 - running
*  2 - paused
*/




#else
#include "PosixDaemon.h"
#endif

int App_ServiceStatus = -1;
std::string myPidFile="/var/tmp/mcs";

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
	printf("Usage: \n %s [<options>]\n"
		"    -v, --version            print version and exist\n\r"
		"    -c config_file           use config_file as configuration file\n\r"
		"    -a, --ahbot config_file  use config_file as ahbot configuration file\n\r"
#ifdef WIN32
		"    Running as service functions:\n\r"
		"    -s run                   run as service\n\r"
		"    -s install               install service\n\r"
		"    -s uninstall             uninstall service\n\r"
#else
		"    Running as daemon functions:\n\r"
		"    -s run                   run as daemon\n\r"
		"    -s stop                  stop daemon\n\r"
#endif
		, prog);
}

/*
分析ip地址和端口号  -p 端口号 -ip 地址
*/
#ifdef __WINDOWS__
int proc_arg(int argc, const char* argv[])
{

	return 0;
}
#else

int proc_arg(int argc, const char* argv[])
{
	return 0;
}

#endif


int main_server()
{
	ServerInit();

	if (0 != ServerListen("0", 8989))
	{
		printf("服务器启动失败\n");
		exit(0);
	}

	App_ServiceStatus=1;

	//进入程序主循环
	while (App_ServiceStatus != 0)
	{

		//这里没考虑Windows中的暂停和继续的问题
		if(App_ServiceStatus==1) ServerStep();

	}

	ServerClean();
	subscriber_clear();

	return 0;
}


int main(int argc, const char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		if (0 == strcmp(argv[i], "-v")) {
			printf("version:%s\n",IDS_VERSION);
			exit(0);
		}
	}

	proc_arg(argc, argv);

#ifdef __WINDOWS__
	ServiceMainProc(argc, argv);
	return 0;
#else

	bool bDaemon=false;
	for (int i = 0; i < argc; i++)
	{
		if (0 == strcmp(argv[i], "-d")) {
			bDaemon=true;
		}

		if (0 == strcmp(argv[i], "-stop")) {
			stopDaemon(myPidFile.c_str());
			//detachDaemon();
			exit(0);
		}

	}


	if(bDaemon){
		startDaemon(myPidFile.c_str());
	}


	//CreatePIDFile(myPidFile);
	main_server();

#endif

//	main_server();

}
