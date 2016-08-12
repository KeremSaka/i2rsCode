#ifndef _ZEBRA_I2RS_ZEBRA_H
#define _ZEBRA_I2RS_ZEBRA_H

#include "vty.h"

#define EXTERNAL_METRIC_TYPE_1      0
#define EXTERNAL_METRIC_TYPE_2      1

#define DEFAULT_ROUTE		    ZEBRA_ROUTE_MAX
#define DEFAULT_ROUTE_TYPE(T) ((T) == DEFAULT_ROUTE)

/* Prototypes */

extern int i2rs_route_add ();//struct prefix_ipv4 *);
extern int i2rs_route_delete (struct prefix_ipv4 *);
extern int i2rs_interface_add(struct zclient *);
extern int i2rs_interface_delete(struct zclient *);
extern int i2rs_interface_state_up(struct zclient *);
extern int i2rs_interface_state_down(struct zclient *);
extern int i2rs_interface_address_add(int, struct zclient *,zebra_size_t);
extern int i2rs_interface_address_delete(int , struct zclient *,zebra_size_t );
//extern int zclient_read_zapi_ipv4(struct zclient* , struct zapi_ipv4 *, struct prefix_ipv4* , unsigned long* , struct in_addr* );
extern int i2rs_zebra_route_manage(int , struct zclient *, zebra_size_t );

extern void i2rs_zclient_init (struct thread_master *);
extern void i2rs_zclient_start (void);


#endif /* _ZEBRA_I2RS_ZEBRA_H */

