#include <iostream>
#include "Winsock2.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "WS2_32.lib")  

using namespace std;

SOCKET ss;
char name[10] = "hello";
char buf[10] = {};
SOCKADDR_IN serv, clnt;

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEACCES: msgText = "WSAEACCES"; break;
	case WSAEFAULT: msgText = "WSAEFAULT"; break;
	case WSAEINVAL: msgText = "WSAEINVAL"; break;
	case WSAEMFILE: msgText = "WSAEMFILE"; break;
	case WSAEWOULDBLOCK: msgText = "WSAEWOULDBLOCK"; break;
	case WSAEINPROGRESS: msgText = "WSAEINPROGRESS"; break;
	case WSAEALREADY: msgText = "WSAEALREADY"; break;
	case WSAENOTSOCK: msgText = "WSAENOTSOCK"; break;
	case WSAEDESTADDRREQ: msgText = "WSAEDESTADDRREQ"; break;
	case WSAEMSGSIZE: msgText = "WSAEMSGSIZE"; break;
	case WSAEPROTOTYPE: msgText = "WSAEPROTOTYPE"; break;
	case WSAENOPROTOOPT: msgText = "WSAENOPROTOOPT"; break;
	case WSAEPROTONOSUPPORT: msgText = "WSAEPROTONOSUPPORT"; break;
	case WSAESOCKTNOSUPPORT: msgText = "WSAESOCKTNOSUPPORT"; break;
	case WSAEOPNOTSUPP: msgText = "WSAEOPNOTSUPP"; break;
	case WSAEPFNOSUPPORT: msgText = "WSAEPFNOSUPPORT"; break;
	case WSAEAFNOSUPPORT: msgText = "WSAEAFNOSUPPORT"; break;
	case WSAEADDRINUSE: msgText = "WSAEADDRINUSE"; break;
	case WSAEADDRNOTAVAIL: msgText = "WSAEADDRNOTAVAIL"; break;
	case WSAENETDOWN: msgText = "WSAENETDOWN"; break;
	case WSAENETUNREACH: msgText = "WSAENETUNREACH"; break;
	case WSAENETRESET: msgText = "WSAENETRESET"; break;
	case WSAECONNABORTED: msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET: msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS: msgText = "WSAENOBUFS"; break;
	case WSAEISCONN: msgText = "WSAEISCONN"; break;
	case WSAENOTCONN: msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN: msgText = "WSAESHUTDOWN"; break;
	case WSAETIMEDOUT: msgText = "WSAETIMEDOUT"; break;
	case WSAECONNREFUSED: msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN: msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH: msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM: msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY: msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED: msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED: msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON: msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND: msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND: msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN: msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY: msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA: msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE: msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE: msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING: msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED"; break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	case EWOULDBLOCK:msgText = "EWOULDBLOCK"; break;
	default: msgText = "***ERROR***"; break;
	};
	return msgText;
};

string  SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

bool  GetRequestFromClient(
	char* name,												//[in] позывной сервера  
	short            port,									//[in] номер просушиваемого порта 
	struct sockaddr* from,									//[out] указатель на SOCKADDR_IN
	int* flen												//[out] указатель на размер from 
)
{
	struct sockaddr_in* sin = (struct sockaddr_in*) from;
	int res = 0;

	char buf[10] = {};

	while (strcmp(buf, name) != 0)
	{
		res = recvfrom(ss, buf, sizeof(buf), NULL, from, flen);

		if (res == WSAETIMEDOUT)
			return false;
		else if (res == SOCKET_ERROR)
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

		if (strcmp(buf, name) == 0) {
			cout << "client IP: " << inet_ntoa(sin->sin_addr) << endl;
			return true;
		}
	}
	return true;
}

bool  PutAnswerToClient(
	char* name,												//[in] позывной сервера  
	struct sockaddr* to,									//[in] указатель на SOCKADDR_IN
	int* lto												//[in] указатель на размер from 
)
{
	if (sendto(ss, name, sizeof(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
		throw  SetErrorMsgText("sendto: ", WSAGetLastError());
}

void CheckRetrieves(
	short            port,									//[in] номер просушиваемого порта 
	struct sockaddr* from,									//[out] указатель на SOCKADDR_IN
	int* flen												//[out] указатель на размер from 
)
{
	SOCKET ss;
	int res = 0, optval = 1;
	ss = socket(AF_INET, SOCK_DGRAM, NULL);
	setsockopt(ss, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int));

	SOCKADDR_IN all;
	all.sin_family = AF_INET;
	all.sin_port = htons(2000);
	all.sin_addr.s_addr = INADDR_BROADCAST;

	if ((res = sendto(ss, name, sizeof(name), NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
		throw  SetErrorMsgText("sendto:", WSAGetLastError());

	timeval timeout;
	timeout.tv_sec = 5000;
	timeout.tv_usec = 0;

	setsockopt(ss, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	while (res = recvfrom(ss, buf, sizeof(buf), NULL, from, flen) != SOCKET_ERROR)
	{
		if (strcmp(buf, name) == 0) {
			cout << "server IP: " << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << endl;
		}
	}
	cout << "check done." << endl;
}

int main() 
{
	WSADATA wsaData;
	memset(&clnt, 0, sizeof(clnt));
	int size = sizeof(clnt);

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		throw SetErrorMsgText("startup: ", WSAGetLastError());

	try {
		CheckRetrieves(htons(2000), (sockaddr*)&clnt, &size);

		if ((ss = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = INADDR_ANY;

		if (bind(ss, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind: ", WSAGetLastError());

		for (;;) 
		{
			memset(&clnt, 0, sizeof(clnt));
			int size = sizeof(clnt);
			while (GetRequestFromClient(name, htons(2000), (sockaddr*)&clnt, &size) == false) {};
			PutAnswerToClient(name, (sockaddr*)&clnt, &size);
		}

		closesocket(ss);

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
	}
	catch (string msg) {
		cout << msg << endl;
	}
	return 0;
}