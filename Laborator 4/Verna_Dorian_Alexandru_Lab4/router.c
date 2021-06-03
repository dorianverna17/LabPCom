#include "skel.h"

int interfaces[ROUTER_NUM_INTERFACES];
struct route_table_entry *rtable;
int rtable_size;

struct arp_entry *arp_table;
int arp_table_len;

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
struct route_table_entry *get_best_route(__u32 dest_ip) {
	/* TODO 1: Implement the function */
	struct route_table_entry *res = NULL;

	for (int i = 0; i < rtable_size; i++) {
        if ((dest_ip & rtable[i].mask) == rtable[i].prefix) {
            if (res == NULL)
                res = &rtable[i];
            else if (ntohl(rtable[i].mask) > ntohl(res->mask))
                res = &rtable[i];
        }
    }
    return res;
}

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(__u32 ip) {
    /* TODO 2: Implement */
    for (int i = 0; i < arp_table_len; i++) {
        if (arp_table[i].ip == ip)
            return &arp_table[i];
    }
    return NULL;
}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();
	rtable = malloc(sizeof(struct route_table_entry) * 100);
	arp_table = malloc(sizeof(struct  arp_entry) * 100);
	DIE(rtable == NULL, "memory");
	rtable_size = read_rtable(rtable);
	parse_arp_table();
	/* Students will write code here */

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		// headerul de ethernet
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
		// iphdr->daddsr - adresa destinatie
		struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));
		/* TODO 3: Check the checksum */
		uint16_t checksum = 0;
		checksum = ip_checksum(ip_hdr, sizeof(struct iphdr));
		if (checksum != 0)
			continue;
		/* TODO 4: Check TTL >= 1 */
		if (--ip_hdr->ttl <= 0)
			continue;
		/* TODO 5: Find best matching route (using the function you wrote at TODO 1) */
		struct route_table_entry* route = get_best_route(ip_hdr->daddr);
		/* TODO 6: Update TTL and recalculate the checksum */
		// Am decrementat TTL-ul inainte
		ip_hdr->check = 0;
        checksum = ip_checksum(ip_hdr, sizeof(struct iphdr));
        ip_hdr->check = checksum;
		/* TODO 7: Find matching ARP entry and update Ethernet addresses */
        struct arp_entry* entry;
        entry = get_arp_entry(ip_hdr->daddr);

        // aici copiez adresa MAC a destinatiei in header-ul Ethernet
        // (acestea sunt sub forma de array si de aceea imi trebuie un for)
        for (int i = 0; i < 6; i++) {
        	eth_hdr->ether_dhost[i] = entry->mac[i];
        }
        int result = get_interface_mac(route->interface, eth_hdr->ether_shost);
        if (result != 1) {
        	continue;
        }
		/* TODO 8: Forward the pachet to best_route->interface */
		send_packet(route->interface, &m);
	}
}
