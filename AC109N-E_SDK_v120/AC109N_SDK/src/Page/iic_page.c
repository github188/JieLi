/*! \page4 IIC 编程指南
 *
 * \section iic0 IIC 介绍
 * - IIC有两个出口：\n
 *   出口1：CLK为P00、DATA为P01\n
 *   出口2：CLK为P26、DATA为P27\n
 *   出口3：CLK为P32、DATA为P33\n
 *   出口4：CLK为P36（USBDP）、DATA为P37（USBDM）\n
 *
 * \section iic1 IIC 寄存器：
 * - IIC_CON：可读可写\n
 *   Bit0：IIC Enable，Reset值为0\n
 *   Bit1：Slave Mode选择，0为主模式、1为从模式，Reset值为0\n
 *   Bit2："加起始位"选择，0为不加起始位，1为加起始位。加起始位只有在主模式才有效，Reset值为0\n
 *   Bit3："加结束位"选择，0为不加结束位，1为加结束位。加结束位只有在主模式才有效，Reset值为0\n
 *   Bit5~4：Reserved，读为0\n
 *   Bit6：结束位中断允许，高有效，Reset值为0\n
 *   Bit7：IIC中断允许，高有效，Reset值为0\n
 *
 * - IIC_STA：可读可写\n
 *   Bit0：清空起始位标志，写1清空，写0没意义\n
 *   Bit1：起始位标志，当检查到起始位，则自动置1，软件读到后应通过设置IICSTA.0=1作为标志位清零，否则无法检测到下一次起始位，Reset值为0\n
 *   Bit2：输出ACK设置，0和1对应信号线上的低电平和上拉。，Reset值为0\n
 *   Bit3：输入的ACK或NACK，0和1对应信号线上检测到的低电平和高电平，Reset值为0\n
 *   Bit4:   清空"结束位Pending", 写1清空，写0没意义\n
 *   Bit5:   清空"普通Pending", 写1清空，写0没意义\n
 *   Bit6：结束位Pending，在做从机时，收到结束位，硬件会把结束位pending置1，CPU可清0，Reset值为0\n
 *   Bit7：普通Pending，做主机或从机，收或发完一个Byte，硬件都会把普通Pending置1，CPU可清0。，Reset值为0\n
 *
 * - IIC_BAUD：波特率寄存器，Reset值为x，用之前需要初始化\n
 *   波特率计算公式：波特率 = Freq_sys / ((IICBAUD+1) *4)\n
 *   IICBAUD寄存器在做从机时则作为从机的地址寄存器，高7位为7位地址，最低1位为从机自动响应允许，高有效。自动响应是指检测到起始位后自动回Ack（低电平）\n
 *
 * - IIC_BUF：
 * - 做主机时，写IICBUF会启动一次通信，写什么值就会发什么值出去，当普通pending出现，则表示发送完毕。此时可检查确认位（ACK， IICSTA[3]），以了解从机是否接收到。\n
 *   若在启动通信前选择了"加结束位"，则发送完该字节后会顺便发送结束位。\n
 *   若在启动通信前选择了"加起始位"，则发送该字节前会先发送起始位。若启动通信前时钟和数据线处于空闲状态（上拉），则无论有无选择"加起始位"，起始位都会自动加上。\n
 *   若要接收则需要往IICBUF写0xFF，等到普通Pending出现，则表示接收完毕，CPU可以读IICBUF得到接收的值。若在写0xFF前选择了"加结束"，则接收完会顺便发送结束位。\n
 *
 * - 做从机时，写IICBUF会启动一次等待通信，若要发送，则应写入要发送的值，若要接收，则应写入0xFF。当普通Pending出现时，表示通信结束，若刚刚这次通信是发送，则CPU应检查确认位，以了解从机是否接收到。若刚刚这次通信是接收，则CPU可以通过读IICBUF，得到接收的内容。\n
 *   每次通信结束时，CPU可以通过检查起始位标志，以了解刚刚完成的这次通信，有没有起始位。\n
 *   做从机时，若出现结束位Pending，则表示结束位出现。\n
 *   做从机时，系统时钟频率（IIC模块的工作时钟）必须是IIC接口时钟频率的16倍以上。
 */
