import time
import uuid
import math

from beacontools import BeaconScanner, IBeaconFilter, IBeaconAdvertisement

def twos_comp(val, bits):
    """compute the 2's complement of int value val"""
    if (val & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    return val

first_ref = 1

def callback(bt_addr, rssi, packet, additional_info):

    global first_ref, ref
    uuidStr = packet.uuid
    uuidByt = bytearray.fromhex(uuid.UUID(uuidStr).hex)
    if "e1ffa103-64" in uuidStr: 
        xaxis = (uuidByt[5] << 8) | uuidByt[6]
        yaxis = (uuidByt[7] << 8) | uuidByt[8]
        zaxis = (uuidByt[9] << 8) | uuidByt[10]
        print(xaxis)
        print(yaxis)
        print(zaxis)

        xaxis = twos_comp(xaxis,16)
        yaxis = twos_comp(yaxis,16)
        zaxis = twos_comp(zaxis,16)
        xaxis = xaxis/256
        yaxis = yaxis/256
        zaxis = zaxis/256
        print(xaxis)
        print(yaxis)
        print(zaxis)
        
        final= math.sqrt((xaxis*xaxis)+ (yaxis*yaxis)+ (zaxis*zaxis)) 
        
        #considering first detecting packet as a reference packet 
        if first_ref == 1:
            ref= final
            first_ref= 2
            print("Reference value", ref)
            
        print("final value=", final)

        if final > ref:
            print("Tag is Moving")
        else:
            print("Tag is stationary")

# scan for all iBeacon advertisements regardless from which beacon
while True:
  scanner = BeaconScanner(callback, packet_filter=IBeaconAdvertisement)
  scanner.start()
  time.sleep(1)
  scanner.stop() 


