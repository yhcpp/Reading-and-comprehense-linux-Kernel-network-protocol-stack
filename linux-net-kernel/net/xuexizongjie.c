/*
TSO��ȫ����TCP Segmentation Offload������֪��ͨ����̫����MTU(��ȥ14�ֽ�ETHͷ��4�ֽ�β��У��ֵ���������ETHͷ��У��ʵ������1518)��1500����ȥTCP/IP�İ�ͷ��TCP��MSS (Max Segment Size)��С��1460��ͨ�������Э��ջ��Գ���
1460��TCP payload����segmentation����֤���ɵ�IP��������MTU�Ĵ�С�����Ƕ���֧��TSO/GSO���������ԣ���û�����Ҫ�ˣ����ǿ��԰����64K��С��TCP payload
ֱ�����´���Э��ջ����ʱIP��Ҳ�������segmentation��һֱ�ᴫ������������֧��TSO/GSO���������Լ�����TCP/IP��ͷ��֡ͷ����������offload�ܶ�Э��ջ�ϵ�
�ڴ������checksum�����ԭ����CPU�����Ĺ������Ƹ�������GSO��TSO����ǿ http://lwn.net/Articles/188489/ ��GSO��ֻ���TCP�����Ƕ�����Э�飬�����ܰ�
segmentation�ƺ󵽽���������һ�̣���ʱ���ж��������Ƿ�֧��SG��GSO�������֧������Э��ջ����segmentation�����֧�����payloadֱ�ӷ�������

TSO:Ч�ʵĽ�ʡԴ�ڶԴ��ֻ��һ��Э��ջ�������Ƕ��.����������ƳٷֶΣ�segmentation), ���������������������ֶΣ�������������Ѵ����super-packet)
�𿪣����SG list������һ��Ԥ�ȷ���õ��ڴ���������Σ�Ȼ�󽻸�������tso����ֻ�ܶ�TCP��Ч��GSO����ǿ�汾��������Э�鶼��Ч��

�ο�:http://www.smithfox.com/?e=191
�����ͷ������1�3
����˵ ������ ������ ʱ, Ҫ���ֳ��Ϸ�Χ, ���� Linux��˵�� ������I/O �� Java��NIO1.0�е� ������I/O ������ͬ�ĸ���. ���������˵, �������ǽ��� "��" ��Ϣ, CPU������������ȥ��. ��������������: �������Ƭʱ��������ֳ�N���С������,
���Խ��̲��ϵ��л��� "��" CPU���, �����Ͽ�������������. ����ȥ Linux������I/O Ҫ��������, ��CPU��ܴ���ʄ1�3��socketû���݄1�3����ת. ��Ȼ���������ˬ��, ���Ǵ�����������Ч����˵, �˷Ѹ�����

�첽??
�첽����˵��I/O�������ģ��: CPU��ԭ����, �б�Ҫ��ʱ��Ż����, �Ȳ��˷�, Ҳ������.
�����е��첽I/O: Application����ȴ�socket����(Ҳ������˽��̶��� "��Ϣ"), Ҳ���� copy socket data, ����������ͬѧ(����״̬, ����CPU) ����socket data copy��Appliation����ָ�����ڴ��, ֪ͨһ��Appliation(һ���ǻص�����).
copy socket data, Application�ǲ�������, ������������Ҫ��, ������˭��, CPU�Ƿ���Ҫ���Ѿ���ȥ������?
������ "�ڴ�ӳ��" �Լ� DMA�ȷ�ʽ���ﵽ "����CPUȥ���뷱�صĹ���" ��Ŀ��. "�ڴ�ӳ��1�3" �ǲ���copy, ��DMA��������оƬ������CPU����.


IP�㾡���ܿ�Ľ������Դ�ڵ㷢�͵�Ŀ�Ľڵ㣬�����ṩ�κοɿ��Ա�֤����Ƭ��ʶ����Ϣ��IP����䡣TCP�����ͨ����ʱ�ش���ȷ��Ӧ����Ʊ�֤�ɿ����䡣
�ֲ�:ARP RARP����·�㣬IP ICMP IGMP��������㣬TCP UDP���ڴ���㣬FTP HTTP������Ӧ�ò�
��������ͨ�������������͵�ʱ��������һ������һ�����صĶ����Ʊ����������ȥ�ġ�
ƽʱ����ָ��MTU����1500ָ���� IP�ײ�+TCP�ײ�+Ӧ�����ݵ��ܳ���(������14�ֽ���̫ͷ��ĩβ4�ֽ�У����)
��̫���ײ�:14�ֽڣ� IP�ײ�Ĭ��20�ֽ�(�����IPѡ���ֹ)��TCP�ײ�Ĭ��20�ֽڣ�UDP�ײ�8�ֽڡ�
��̫���ײ��е�type��ʾ��������ʱIP����ARP����RARP����PPP����·��Э�顣IP�е�type��ʾ�������TCP UDP ICMP IGMP�ȣ�ͨ��type���ֺ���ӵ���ʲôЭ�顣�����ͨ���˿ں��������ϲ�Ӧ��Э�顣
/etc/servers�ļ��д���ǳ��õ�Ӧ�ó���˿ڼ���ӦӦ��Э�飬����telnetΪ23

�׽���Ӧ�ó�����Բο�:http://blog.csdn.net/water_cow/article/details/7027032


//pf_inet��net_families[]Ϊinet_family_ops����Ӧ���׽ӿڲ�ops�ο�inetsw_array�е�inet_stream_ops inet_dgram_ops inet_sockraw_ops�������������ֱ�Ϊtcp_prot udp_prot raw_prot
//netlink��net_families[]netlink_family_ops����Ӧ���׽ӿڲ�opsΪnetlink_ops
Ӧ�ò�ִ��socket��ʱ���ں�����Ҫ�ж���ʲôЭ���壬�Ӷ�ִ�ж�Ӧ��ops(netlink_family_ops����inet_family_ops)����ͬЭ�������׽ӿڲ���һ����Ӧ��sock ops(��inetsw_array�еĶ�Ӧops)��������Ӧ��opsΪinetsw_array��tcp_prot��

�׽����б��κͶԶ˵������Ϣ������inet_sock�У����Ա�֤��Э���޹أ�����Э�鶼�øýṹ�洢���ص�ַ�˿ںͶԶ˵�ַ�˿��Ѿ�����״̬��
��tcpΪ����struct tcp_sock����struct inet_connection_sock,inet_connection_sock���� struct inet_sock��struct inet_sock����struct sock, struct sock������ struct sock_common��������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct tcp_sock)
��udpΪ����struct udp_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct udp_sock)
��rawΪ����struct raw_sock����struct inet_sock��struct inet_sock����struct sock�� struct sock������ struct sock_common����������struct socket�����skָ��Ŀ��ٿռ��С��sizeof(struct raw_sock)
//tcp_timewait_sock����inet_timewait_sock��inet_timewait_sock����sock_common
tcp_request_sock����inet_request_sock��inet_request_sock����request_sock
tcp_sock->inet_connection_sock->inet_sock->sock(socket�����skָ��sock)

struct sockһ����sk;  Ҳ��鴫����ƿ�
struct socketһ����Ϊsock��
Ӧ�ò�socket����������ں˵�__sock_createȻ��inet_create����netlink_create
��������:
bind--->inet_bind����netlink_bind ��Ҫ�ǰѱ���ip�Ͷ˿���ӵ�sock����ı��س�Ա�С�bind��ʱ�򱾵�ip���ΪINADDR_ANY(0)��ʾ�����ַ��������ض˿�Ϊ0�����ں�inet_bind�����һ���˿�
listen--->inet_listen   ��Ҫ�Ƿ������������request_sock_queue�����û��bind���Ļ�����Ϊ��sock��Ƭһ���˿ڡ��˿ں͵�ַ��Ϣ�����struct inet_sock��
accept--->inet_accept Ĭ���������ģ���SYSCALL_DEFINE3(accept...)������flag=0,�������������� sk->sk_rcvtimeo =MAX_SCHEDULE_TIMEOUT,���Ӧ�ò�accept�����ڶ���������Ҫ��ȡ�Է���ַ��
�����ﻹҪ����getname������ȡ�Է���ַ,  accept��ʱ���ں˻ᴴ��һ���µ�struct socket�� ���е�struct sock�Ǵ����rskq_accept_head��������ȡ������(�����socket�����ں˵��ò�һ���ĵط���:
sys_socket��ʱ��struct socket��struct sock�����ں��Լ�������)��acceptһֱ������ֱ��rskq_accept_head����������Ϣ��Ȼ��ȡ�����struct sock�ṹ��

send��recv��Ĭ�ϳ�ʱʱ��Ϊ���޴�

�ͻ���:socke��ֱ��connect��connctҪ���������Ǹ��ͻ��˷���һ���˿ڣ�Ȼ����syn tcp�������� 
tcp״̬��Ǩ������Ҫ����:tcp_rcv_state_process��connect������syn����󣬻�һֱ�ȴ�֪��3��������ɲŷ��ء�connect�ں˲��ᴴ���µ��׽ӿ�struct socket��struct sock
Ӧ�ò����ý��պͷ��ͻ������ߴ��ʱ���ں�ʵ���ϻ�*2

tcp sendtoĬ�Ϸ������ģ�sk->sk_sndtimeo		=	MAX_SCHEDULE_TIMEOUT;�����ڷ��͵�ʱ������Ƿ�tcp�Ѿ��������ֳɹ��������û����������᷵�س�����Ϣ��

Ϊʲô��Ӧ�ò��׽��ַ��ʹ������(����MCP�е�ͬ��)��ʱ��sendto����writeÿ��ֻ���ز��ֳ��ȴ�С���������һ��write�ɹ������Բο��ں˲��ֵ�tcp_sendmsg

�׽���hashinfo�������:
/*
 * TCP���������һ��inet_hashinfo�ṹ���͵�ȫ�ֱ���
 * tcp_hashinfo�����е�ɢ�б���м��й���
  //tcp_death_row�����hashinfoָ�����tcp_hashinfo��Ҳ�������е�inethash_info��ͷ������
 */
/*
tcp udp��raw��hash�ֱ�洢��tcp_hashinfo  udp_table  raw_v4_hashinfo  

�׽���Ӧ�ò��close poll ioctl�����ں��ж�Ӧ�ĵ��õط���socket_file_ops
shutdown close����:
shutdown�������ð����Ӷ˿ڻ���˫��ȫ���˿ڣ�����ȷ�����Ͷ��кͽ��ն��е����ݱ����ͽ�����ϡ�
closeֻ����˫��Ͽ������close��ʱ����ն��������ݣ���ֱ���ͷŵ���ֱ�ӷ���rst�������������Ĵλ��ֹ��̣����Ͷ����е������Ƿ��ͳ�ȥ����ֱ���ͷŵ���Ҫ��SO_LINGER�����õ�ʱ�䣬���ʱ��Ϊ0�����Ͷ�������ֱ���ͷţ�
�����Ϊ0����û������SO_LINGERѡ���ȴ����ݷ�����ϻ��߳�ʱ,��tcp_close��

//tcp_request_sock��������飬�����ӵĽ���������ʹ�ã� tcp_sock�����ӽ���֮������ֹ֮ǰʹ�ã�tcp_timewait_sock����ֹ���ӹ�����ʹ��
////ipv4_specific��TCP����㵽��������ݷ����Լ�TCP�������̵�����OPS����tcp_prot->init�б���ֵ��inet_connection_sock->icsk_af_ops


����ѧϰ��:poll epoll select fcntl
union {
		struct inet_hashinfo	*hashinfo; //tcp_hashinfo
		struct udp_table	*udp_table; //udp_table
		struct raw_hashinfo	*raw_hash; //raw_v4_hashinfo
	} h;

������ƿ���sk_alloc����
�׽��ַ������ݵ�ʱ��struct sock��SKB�Ĺ�ϵ����ͨ��sock_alloc_send_pskb(UDP��RAW�׽��������)������ϸ�˽�, skb_set_owner_w��TCP�ڹ���SYN+ACKʱʹ��sock_wmalloc�������û�����ʱͨ��ʹ��sk_stream_alloc_skb()���䷢�ͻ���
���ڴ����������������ݵ�SKB��ͨ��dev_alloc_skb����alloc_skb���з���, skb_set_owner_r�����⣬��������(Ҳ��ѡ���)�ķ���ʹ��sock_kmalloc����

familyЭ����ͨ��sock_registerע��  �����ӿ�tcp_prot udp_prot netlink_prot��ͨ��proto_registerע��   IP��ӿ�ͨ��inet_add_protocol(&icmp_protocol��ע�� ����Щ��ɹ��̲ο�inet_init����
�鿴����Э��Ĵ����ops�Լ�socket��ops������������inetsw_array

ÿ��Э����԰������socket���������ÿ��Э���һЩȫ����Ϣ�����統ǰ�ж��ٸ�TCP socket����ǰTCP�ܵ��ڴ��С�ȴ�����Ӧ����socket�У�����Э����,��tcp_prot udp_prot raw_prot��
	
�׽��ֵĴ���  �� ���� ���ں��еĹ��̣��Լ�inetsw_array  prot(tcp_prot  udp_prot  raw_prot)�Ĺ�ϵ
struct socket-->ops (inetsw_array), Ȼ����õ�inet_bind(inet_lisent)�ȣ�Ȼ���ִ�е�tcp_prot(udp_prot raw_prot)�е���Ӧbind listen accept(ע��:�����������ЩЭ��û��bind listen��)��

���̡��ļ����׽ӿڲ��ϵ���Բο��������²� P616
getsockname��ȡ���˵ĵ�ַ��getpeername��ȡ�Զ˵ĵ�ַ

tcpdumpʹ�÷���:tcpdump src host 210.27.48.1 and ! 210.27.48.2 and src port 100 and dst port 200 and ! 3


//ִ�иú���sk_wake_async(��SIGIO��SIGURG�źŷ��͸����׽����ϵĽ���,�����첽I/O����)�ĵط���sk_send_sigurg(���յ���������)��sock_def_write_space��sk_stream_write_space(���ͻ����������仯)�����µ����ݵ���(sock_def_readable)
//sock_def_error_report������ƿ鷢��ĳ�ִ���sock_def_wakeup����״̬�����仯��ʹ�øù��ܵ�ʱ��Ӧ�ò���Ҫͨ��fcntl(��Ӧ��ϵͳ���ú���Ϊdo_fcntl)
//ͨ��fcntl����F_SETOWN F_SETFL�󣬽���ϵͳ������ô�����Ӧ��fasync_struct, �����ط���sock_fasync

http://blog.chinaunix.net/uid-13059622-id-3335321.htmlѧϰ ��APUE��--UNIX�����߼���� �͡�UNP��--UNIX������  �����Բο������ַ�������д���


���ؽ��ں˵�ko�ļ���Ҳ�ǲ���ִ�е���? http://bbs.chinaunix.net/thread-4060983-1-1.html  ��Ҫ�˽��ں�ģ�����ԭ��:http://blog.chinaunix.net/uid-20626696-id-77689.html
�ں˲�����Щ��Ҫ���ȣ��Լ���Ӧ�ò㾭��֮��ĵ��ȹ��̣����Բο����һ��Ҫ��������Ҫ:http://blog.csdn.net/allen6268198/article/details/7567679
TCPΪʲô����Ҫ�ֶεĴ����ˣ���IP��������? ��ΪTCP��ͷ���и�����ֶΣ���֤���������ݰ�����ȷ�ԣ��ǻ������ģ�

TCP�ײ��е�У��͸���TCP�ײ���TCP���ݣ���IP�ײ���У���ֻ����IP�ײ�����������IP���ݱ��е��κ����ݡ�
IP�ײ�У����ֶκ�TCP�ײ�У����ֶΣ�������ն�У��ʧ�ܣ������������ģ�����ֱ�Ӷ�����
*/

