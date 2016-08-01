#include <zebra.h>

#include "lib/if.h"
#include "memory.h"
#include "thread.h"
#include "prefix.h"
#include "table.h"
#include "vty.h"
#include "lib/command.h"
#include "plist.h"
#include "log.h"
#include "zclient.h"
#include <signal.h>
#include <bits/sockaddr.h>
#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include "zebra/rib.h"

#include "i2rs/i2rs_main.h"

#include "i2rs/i2rs_vty.h"
struct{
	struct nlmsghdr nl;
	struct rtmsg rt;
	char buf[8192];
}request;
int fd;
int nllength;
int rtl;
int rtlength = sizeof(struct rtmsg);//RT Length
char buffer[8192], dsts[24], gws[24], ifs[16], ms[24];
struct sockaddr_nl localAddr;
struct nlmsghdr *nlpointer;
struct msghdr msg;
struct iovec iov;
struct rtmsg *rtpointer;
struct rtattr *rtap;
struct rtattr *rtattrpointer; //RT Attribute Pointer


DEFUN (show_ip_i2rs_route,
       show_ip_i2rs_route_cmd,
       "show ip i2rs route",
       "I2RS information\n")
{
	fd =socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	


	bzero(&localAddr, sizeof(localAddr));

	localAddr.nl_family = AF_NETLINK;
	localAddr.nl_pad=0;
	localAddr.nl_pid=getpid();
	localAddr.nl_groups=0;

	bind (fd,(struct sockaddr*) &localAddr, sizeof(localAddr));
	
	//getroutingtable
	bzero(&request, sizeof(request));

	request.nl.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
	request.nl.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
	request.nl.nlmsg_type = RTM_GETROUTE;

	request.rt.rtm_family = AF_INET;
	request.rt.rtm_table = RT_TABLE_MAIN;
	//end

	//sendrequest
	bzero(&localAddr, sizeof(localAddr));

	localAddr.nl_family = AF_NETLINK;

	bzero(&msg, sizeof(msg));
	msg.msg_name = (void *) &localAddr;
	msg.msg_namelen = sizeof(localAddr);

	iov.iov_base =(void *)&request.nl;
	iov.iov_len = request.nl.nlmsg_len;
	msg.msg_iov=&iov;
	msg.msg_iovlen =1;
	int temp = sendmsg(fd,&msg,0);
	if(temp<0){
		printf("send message failure\n");
	}
	else{
		printf("send message succesfull\n");
	}
	//end

	//recvrequest
	char *pointer;
	
	bzero(buffer,sizeof(buffer));
	nllength = 0;
	pointer = buffer;
	while(1){
		int rtn = recv(fd,pointer,sizeof(buffer)-nllength,0);

		nlpointer = (struct nlmsghdr *)pointer;

		if(nlpointer->nlmsg_type == NLMSG_DONE)
			break;

		pointer += rtn;
		nllength +=rtn;

		if((localAddr.nl_groups &RTMGRP_IPV4_ROUTE) == RTMGRP_IPV4_ROUTE)
			break;
	}
	//end


	//read reply
	nlpointer = (struct nlmsghdr *) buffer;
	for(;NLMSG_OK(nlpointer,nllength);nlpointer=NLMSG_NEXT(nlpointer,nllength)){
		rtpointer = (struct rtmsg *) NLMSG_DATA(nlpointer);
		if(rtpointer->rtm_table !=RT_TABLE_MAIN)
			continue;

		bzero(dsts, sizeof(dsts));
		bzero(gws, sizeof(gws));
		bzero(ifs, sizeof(ifs));
		bzero(ms, sizeof(ms));

		rtap = (struct rtattr *) RTM_RTA(rtpointer);
		rtl = RTM_PAYLOAD(nlpointer);
		for(;RTA_OK(rtap,rtl);rtap = RTA_NEXT(rtap, rtl))
		{
			switch(rtap->rta_type)
			{
			case RTA_DST:
				inet_ntop(AF_INET,RTA_DATA(rtap),dsts,24);
				break;
			case RTA_GATEWAY:
				inet_ntop(AF_INET,RTA_DATA(rtap),gws,24);
				break;
			case RTA_OIF:
				sprintf(ifs,"%d", *((int *)RTA_DATA(rtap)));
			default:
				break;
			}
		}
		sprintf(ms,"%d", rtpointer->rtm_dst_len);
		printf("dst %s/%s gw %s if %s\n",dsts,ms,gws,ifs);
	}
	//end
	
	close(fd);
	return CMD_SUCCESS;
}
	


	
//new route add funktion
DEFUN (route_ip_i2rs_add,
	route_ip_i2rs_add_cmd,
	"route ip i2rs add IFNAME",
	"Add new Route to the routing table\n")
{
	struct interface *ifp;//interface struct
	struct prefix_ipv4 *p;	//
	struct in_addr gate;	//gateway address
	struct in_addr src;	//source address
	struct in_addr temp;
	struct ifaddrs *iftemp,*tempaddrs;
	char tempGATE[24] = "100.100.100.1";
	char tempSRC[24] = "10.10.10.1";
	char tempDST[24] = "200.200.200.1";

	getifaddrs(&iftemp);
	tempaddrs = iftemp;
	while(tempaddrs){
		printf("%s\n", tempaddrs->ifa_name);
		if(strcmp(tempaddrs->ifa_name, argv[0]) == 0)
			break;
		
		tempaddrs = tempaddrs->ifa_next;
	}
	//ifp = if_get_by_name ("eth0");//get interface struct
	
	printf("prefix temp \n");
	inet_aton(tempDST, &temp);
	printf("prefix temp %s\n", inet_ntoa(temp));//test if it is right
	printf("I2rs ADD\n");
	p->family = AF_INET;//ip4
	p->prefixlen =(u_char) 32;	
	p->prefix = temp;
	inet_aton(tempGATE, &gate);
	inet_aton(tempSRC, &src);
	
	//rib_add_ipv4 (ZEBRA_ROUTE_CONNECT, 0, p, &gate, &src, 1, VRF_DEFAULT, RT_TABLE_MAIN, 0, 0, 0, SAFI_UNICAST);
	
	//rib_update(VRF_DEFAULT);


	/////////////////////////////////////
	/*int sockfd;
	struct rtentry route;
	struct sockaddr_in *addr;
	struct ifnet *ifaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	
	memset(&route, 0, sizeof(route));
	addr = (struct sockaddr_in*) &route.rt_gateway;
	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr = inet_addr(tempGATE);

	addr = (struct sockaddr_in*) &route.rt_dst;
	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr= inet_addr(tempDST);

	addr = (struct sockaddr_in*) &route.rt_genmask;
	addr->sin_family=AF_INET;
	addr->sin_addr.s_addr = inet_addr(tempSRC);

	route.rt_flags = RTF_UP | RTF_GATEWAY;
	route.rt_metric = 10;

	ioctl(sockfd,SIOCADDRT,&route);*/
	/////////////////////////////////////////
	printf("Add route");
	return CMD_SUCCESS;
}
DEFUN (show_ip_i2rs_delete,
       show_ip_i2rs_delete_cmd,
       "show ip i2rs delete",
       "Delete Route information\n")
{
	printf("Delete Route");
	return CMD_SUCCESS;
}



/* i2rs's interface node. */
static struct cmd_node interface_node =
{
  INTERFACE_NODE,
  "%s(config-if)# ",
  1
};
static struct cmd_node i2rs_node =
{
  I2RS_NODE,
  "%s(config-if)# ",
  1
};

DEFUN(router_i2rs,
		router_i2rs_cmd,
		"router i2rs",
		"Enable i2rs")
{
	vty->node = I2RS_NODE;
	//vty->index = i2rs_global;
	return CMD_SUCCESS;
}
static int
config_write_interface (struct vty *vty)
{
 printf("dummyfunktion\n");
  return write;
}
static int
i2rs_config_write (struct vty *vty)
{
	printf("dummyfunktion\n");
  return write;
}


/* Initialization of OSPF interface. */
void
i2rs_vty_init (void)
{
  /* Install interface node. */
  install_node (&i2rs_node, i2rs_config_write);
  install_node (&interface_node, config_write_interface);
  install_element (CONFIG_NODE, &router_i2rs_cmd);
  install_element (I2RS_NODE,  &show_ip_i2rs_route_cmd);
  install_element (I2RS_NODE,  &route_ip_i2rs_add_cmd);



  install_element (VIEW_NODE, &show_ip_i2rs_route_cmd);
  install_element (ENABLE_NODE, &show_ip_i2rs_route_cmd);

  /*install_element (VIEW_NODE, &route_ip_i2rs_add_cmd);
  install_element (ENABLE_NODE, &route_ip_i2rs_add_cmd);*/
 }



