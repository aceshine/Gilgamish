#include <sys/socket.h>

struct sockaddr
{
	uint8_t sa_len;
	sa_family_t sa_family;
	char sa_data[14];
};

struct in_addr
{
	in_addr_t s_addr;
};

struct sockaddr_in
{
	uint8_t sin_len;
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
};

#include <arpa/inet.h>
int inet_aton(const char* strptr, struct in_addr* addrptr);
in_addr_t inet_addr(const char* strptr);
char* inet_ntoa(struct in_addr inaddr);
int inet_pton(int family, const char* strptr, void* addrptr);
