# DDoS

### ***DDoS attack by SYN flood in C and Python***

![K5OvB-What-Are-Distributed-Denial-of-Service-DDoS-Attacks](https://user-images.githubusercontent.com/86716307/208322310-2424bc66-5a1b-4d24-9eb7-471b4433b6fe.png)


We will try to do a DDOS attack
We will need 3 computers: Attacker, Target, Monitor.
The attacker will send 10K tcp syn packets to the target machine, in a 100 iterations loop. Total of 1M packets. 
The target machine will run an apache server.
The monitor will send pings to the server during the attack.
The attacker will run two types of programs, one in C and one in Python.
For each program, we will measure:
The time the attacker needs to send a packet (avg).
The ping’s RTT from the monitor. You will have to send one ping every 5 seconds.


 DDOS (Distributed Denial of Service) attack is a type of cyber attack in which a large number of requests are sent to a server in an attempt to overwhelm it and cause it to become unavailable to legitimate users. 

If a DDOS attack is successful, it can have a number of negative consequences for the targeted server, including:

Slowed or completely halted processing of legitimate requests
Reduced availability of the server to legitimate users
Increased resource usage, potentially leading to higher hosting costs

The differences between a DDOS program written in Python versus C are primarily in the language used to write the program and the speed at which the program can run. 
Python is a high-level, interpreted language that is generally easier to write and read, but may be slower in terms of execution time compared to a program written in a lower-level language like C.
C is a compiled language, which means that it is converted into machine code that can be directly executed by the computer's processor. 
This can make C programs faster and more efficient, but they may be more difficult to write and debug compared to Python programs.

Apache is a widely used web server software that is known for its reliability, security, and performance. 
It is capable of handling a large number of requests and is often used to host high-traffic websites. 

However, like any web server, an Apache server can be vulnerable to DDOS attacks and other types of cyber attacks if proper precautions are not taken.

Apache is only as secure as the system it is running on, and if the underlying operating system is vulnerable to attack, the Apache server may also be at risk.

### Lab results - 
***About the DDOS Attack and the differences between them***

> ***syn_packet_c && syn_packet_p***
   
   *C -* 
   The attack ended quickly and most of the packets were sent in a similarly fast time of 0.00001-         
   0.00002 milliseconds
   
    Elapsed time: 28.29994 (sec)
    Average time to send a packet: 0.000003 seconds

   
   
   *Python -* 
   After several failed attempts to run the attack (the machines crashed) we managed to run the               attack and it managed to send just half a million packets until the machines crashed again
    the time to send the packet varies but the majority is around 0.04 milliseconds.
    
    Elapsed time: 8 hours

For the C attack the packets were sent many times faster than the Python attack. The speed of sending packets makes the attack stronger and in a better quality


> ***Pings_c && Pings_p***
   
   *C -* 
The program ended quickly and we sent a ping every five seconds, we did not see sharp              fluctuations in the return times of the pingץ
   
     Average RTT: 2.395909090909091
   
   
   *Python -* 
While running the Python program, something interesting happened, a certain sampler of pings had significantly higher RTT than the others, and we even got to the point where the Internet of the machine running the Apache server went down due to the attack.
However, in most pings the time was similar.
  
    Average RTT: 1.5961964795432948


### ***conclusion -***

In terms of running times, the program in C is better.
However for the same amount of packets, the program in C was not able to achieve the same effectiveness that the program in Python achieved - long ping's return times and maybe if it is related to the attack,
The fall of the Internet in the machine of the Apache server.
If we increase the amount of sending packets in the C program and increase the amount of computers that will run the program, we *may* reach the effectiveness that was in Python with much less running time







