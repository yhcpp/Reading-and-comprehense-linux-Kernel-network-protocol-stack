DDD# Reading-and-comprehense-linux-Kernel-network-protocol-stack

QQ交流群1(针对在校生)-(群号：568892619 )
===================================    
群号：568892619  
    在校生nginx、redis、memcache、twemproxy等源码实现交流，指引在校生快速阅读这些底层开源基础中间件代码，对以后工作很有帮助  
  
QQ交流群2(针对已工作，有一些技术基础的同学)-(581089275)
===================================    
群号: 581089275  
    主要针对有一定技术基础的开发人员，提升其基础组件源码技术深度(如nginx、redis、memcache、twemproxy、mongodb、存储引擎、内核网络协议栈等)，同时可以帮助业务开发、运维开发、嵌入式等其他工作方向的同学转基础组件、中间件、分布式存储、高性能服务器等开发
  
===================================
linux内核网络协议栈阅读理解

    本代码是linux内核网络协议栈源码(版本为2.6.35)，本代码对三层及其以上部分主要功能做
了详细注释，并对重要数据结构的各个成员进行了备注，同时对各个函数的调用关系都有详尽备注。主要参考资料为樊东东
老师的<linux内核源码剖析(上下册)>，再此对樊老师表示感谢，这是一本理解网络协议栈很好的书，两本合计1000多页，内
容比较多，但都很经典，对阅读内核协议栈源码有很大的帮助。

	由于工作中经常需要修改协议栈源码，包括封包解包各种隧道报文， NAT功能添加，路径修改，添加自己的netfilter代
码等，由于部分内容涉及到公司保密内容，因此这些工作中新增修改的内核代码已经从本分github中移除，因此可能造成部分
函数没有定义，但不会影响各位同行阅读和理解协议栈源码。

该代码已经详细分析和注释的协议栈功能有:
.二层 三层 4层 发包收包调用流程，及各层头部封包解包分析注释。
.netfilter框架代码分析注释
.sock处理过程中的各种系统调用以及内核处理过程分析注释
.SKB封包 解包 克隆复制 拆分 拉直等详细分析注释
.网络dev设备注册 注销详解。
.内核态用户态通信netlink代码分析注释
.ip分片重组分析注释
.套接口层代码分析及详细注释
.传输控制块分析注释
.TCP链接建立过程分析注释
.链接终止过程分析分析注释
.ip层输入输出分析，TCP输入输出分析 及其层次调用过程分析注释
.tc限速源码分析注释
.ppp内核模块分析注释
.相关模块功能分析注释



待分析主要模块:网桥模块，vlan模块，邻居子系统，路由表模块。

