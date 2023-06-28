#!/usr/bin/env python
#
# Copyright (c) 2019, Pycom Limited.
#
# This software is licensed under the GNU GPL version 3 or any
# later version, with permitted additional terms. For more information
# see the Pycom Licence v1.0 document supplied with this file, or
# available at https://www.pycom.io/opensource/licensing
#

""" OTAA Node example compatible with the LoPy Nano Gateway """

# def lora_demo():


from network import LoRa
import socket
import binascii
import struct
import time
import config

try:
    # initialize LoRa in LORAWAN mode.
    # Please pick the region that matches where you are using the device:
    # Asia = LoRa.AS923
    # Australia = LoRa.AU915
    # Europe = LoRa.EU868
    # United States = LoRa.US915
    lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

    # create an OTA authentication params
    # <3 back-end team now we dont have to do this
    dev_eui = binascii.unhexlify('AABBCCDDEEFF7778')
    app_eui = binascii.unhexlify('70B3D57EF0003BFD')
    app_key = binascii.unhexlify('36AB7625FE770B6881683B495300FFD6')

    # set the 3 default channels to the same frequency (must be before sending the OTAA join request)
    lora.add_channel(0, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)
    lora.add_channel(1, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)
    lora.add_channel(2, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)

    # join a network using OTAA
    lora.join(activation=LoRa.OTAA, auth=(dev_eui, app_eui, app_key), timeout=0, dr=config.LORA_NODE_DR)

    # wait until the module has joined the network
    while not lora.has_joined():
        time.sleep(2.5)
        print('Not joined yet...')

    # remove all the non-default channels
    for i in range(3, 16):
        lora.remove_channel(i)

    # create a LoRa socket
    s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

    # set the LoRaWAN data rate
    s.setsockopt(socket.SOL_LORA, socket.SO_DR, config.LORA_NODE_DR)

    # make the socket non-blocking
    s.setblocking(False)

# // (T) temp = range: 0-100 - 1 byte (real temp = value / 2) (resolution = 0.5°C)
# // (H) humidity = range: 0-100 - 1 byte (real humidity = value) (resolution = 3%)
# // (C) co2 = range: 0-200 - 1 byte (real co2 = value * 10) (resolution = 30ppm + 3% measured value)
# // (P) pressure = range: 0-250 - 1 byte (real pressure = value + 800) (resolution = 0.25%)
# // (V) voltage = range: 0-25V - 1 byte (real voltage = value / 10) (reoslution = 0.1v)
# // (PM) particulate matter = range: 0-250 - 1 byte (times 3) (real PM = value)

# // sent message would be:
# // THCP (T H C P V PM1 PM2.5 PM10)

    # while True:
    #     print("sending data...")
    #     s.bind(1)
    #     s.send(bytes([50, 40, 41, 216, 126, 0, 2, 1, 8, 10]))
    #     time.sleep(30)


    while True:
        line = uart1.readline()
        # line = uart.readline()
        if (line is not None):
            line = line.decode('utf-8')
            values = line.split(",")
            intValues = []

            for value in values:
                intValues.append(int(value))
            

            s.bind(intValues[0])
            intValues.remove(intValues[0])
            s.send(bytes(intValues))
            time.sleep(5)
        else:
            uart.write("waiting for sensor data...\n")
            time.sleep(0.5)
            

    # if __name__ == '__main__':
    #     lora_demo()
finally:
    uart.write("lolrip")