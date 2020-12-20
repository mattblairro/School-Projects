import socket
import os
import sys
import struct
import time
import select
import binascii
ICMP_ECHO_REQUEST = 8

# Generate checksum for packet
def checksum(pkt_string):
    pkt_string = bytearray(pkt_string)
    check_sum = 0
    countTo = (len(pkt_string) // 2) * 2

    for count in range(0, countTo, 2):
        thisVal = pkt_string[count+1] * 256 + pkt_string[count]
        check_sum = check_sum + thisVal
        check_sum = check_sum & 0xffffffff

    if countTo < len(pkt_string):
        check_sum = check_sum + pkt_string[-1]
        check_sum = check_sum & 0xffffffff

    check_sum = (check_sum >> 16) + (check_sum & 0xffff)
    check_sum = check_sum + (check_sum >> 16)
    check_sum_result = ~check_sum
    check_sum_result = check_sum_result & 0xffff
    check_sum_result = check_sum_result >> 8 | (check_sum_result << 8 & 0xff00)
    return check_sum_result


def receiveOnePing(hostSocket, ID, timeout, destAddr):
    global rtt_min, rtt_max, rtt_sum, rtt_cnt
    timeLeft = timeout
    while 1:
        startedSelect = time.time()
        whatReady = select.select([hostSocket], [], [], timeLeft)
        howLongInSelect = (time.time() - startedSelect)

        # Timeout
        if whatReady[0] == []:
            return "Request timed out."

        timeReceived = time.time()
        recPacket, addr = hostSocket.recvfrom(1024)

        icmpHeader = recPacket[20:28]
        icmpType, code, mychecksum, packetID, sequence = struct.unpack(
            "bbHHh", icmpHeader)

        # if success
        # collect metrics on packet and return packet dely
        if type != 8 and packetID == ID:
            bytesInDouble = struct.calcsize("d")
            timeSent = struct.unpack("d", recPacket[28:28 + bytesInDouble])[0]

            rtt = (timeReceived - timeSent) * 1000
            rtt_cnt += 1
            rtt_sum += rtt
            rtt_min = min(rtt_min, rtt)
            rtt_max = max(rtt_max, rtt)

            return timeReceived - timeSent

        timeLeft = timeLeft - howLongInSelect

        # Timeout
        if timeLeft <= 0:
            return "Request timed out."


def sendOnePing(hostSocket, destAddr, ID):
    # Header is type (8), code (8), checksum (16), id (16), sequence (16)

    myChecksum = 0
    header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
    data = struct.pack("d", time.time())
    # Calculate the checksum on empty header
    myChecksum = checksum(header + data)

    # Get right checksum, put in header
    if sys.platform == 'darwin':
        myChecksum = socket.htons(myChecksum) & 0xffff
    # Convert 16-bit integers from host to network byte order.
    else:
        myChecksum = socket.htons(myChecksum)

    header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
    packet = header + data
    hostSocket.sendto(packet, (destAddr, 1))


def doOnePing(destAddr, timeout):
    icmp = socket.getprotobyname("icmp")
    # Create socket
    hostSocket = socket.socket(socket.AF_INET, socket.SOCK_RAW, icmp)

    # Get process ID
    processID = os.getpid() & 0xFFFF
    # Send one ping to host
    sendOnePing(hostSocket, destAddr, processID)
    # Revice response
    delay = receiveOnePing(hostSocket, processID, timeout, destAddr)

    hostSocket.close()
    return delay


def ping(host, timeout=1):

    global rtt_min, rtt_max, rtt_sum, rtt_cnt
    rtt_min = float('+inf')
    rtt_max = float('-inf')
    rtt_sum = 0
    rtt_cnt = 0
    cnt = 0

    dest = socket.gethostbyname(host)
    print("Pinging {}".format(dest))
    print("")

    # Ping on 1 second interval
    while 1:
        cnt += 1
        delay = doOnePing(dest, timeout)
        time.sleep(1)

        if cnt != 0:
            print('--- {} ping metrics ---'.format(host))
            print('{} packets transmitted, {} packets received, {:.1f}% packet loss'.format(
                cnt, rtt_cnt, 100.0 - rtt_cnt * 100.0 / cnt))
            if rtt_cnt != 0:
                print('round-trip min/avg/max: [{:.3f}][{:.3f}][{:.3f}] ms'.format(
                    rtt_min, rtt_sum / rtt_cnt, rtt_max))
                print('delay:                  {:.9f} ms'.format(delay))
    return delay


# host IP is passed in by cmd line arg
ping(sys.argv[1])
