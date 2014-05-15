#!/bin/env python
# encoding: utf-8

import types
import time
import sys
from argparse import ArgumentParser, RawDescriptionHelpFormatter
from ThControl import ThresholdControl 
from LVControl import LowVoltageControl 
import SocketServer
import re

DCS = None

class BhamLabDCS():
        def __init__(self):
	        self.ThCtrl = ThresholdControl()
                self.LVCtrl = LowVoltageControl()
	

        def Close(self):
	        self.ThCtrl.Close()

class BhamLabDCSHandler(SocketServer.StreamRequestHandler, object):
        def setup(self):
                global DCS
                super(BhamLabDCSHandler, self).setup()
                self.DCS = DCS

        def handle(self):
                # self.rfile is a file-like object created by the handler;
                # we can now use e.g. readline() instead of raw recv() calls
                self.data = self.rfile.readline().strip()
                #print "{} wrote:".format(self.client_address[0])
                #print self.data
                request = self.data.lower()
                if request.split(' ')[0] == 'th':
                        if "reset" in request:
                                self.wfile.write(self.DCS.ThCtrl.Reset())
                        if "show" in request:
                                self.wfile.write(self.DCS.ThCtrl.ShowThresholds())
                        if "cal" in request:
                                self.wfile.write(self.DCS.ThCtrl.Calibrate())
                        if re.findall('setval (\d+\.*\d*)', request):
                                values = re.findall('setval (\d+\.*\d*)', request)
                                channels = re.findall('ch (\d+)', request)
                                if "dac" in request:
                                        self.DCS.ThCtrl.isCalibrated = False
                                if "all" in request:
                                        self.wfile.write(self.DCS.ThCtrl.SetAllThresholds(float(values[0])))
                                elif channels:
                                        self.wfile.write(self.DCS.ThCtrl.SetThreshold([[int(channels[i]), float(values[i])] for i in range( min(len(channels), len(values)))]))
                                else:
                                        self.wfile.write("Invalid request\n")
                                        return 
                                time.sleep(10)
                                self.wfile.write(self.DCS.ThCtrl.ShowThresholds())
                elif request.split(' ')[0] == 'lv':
                        self.DCS.LVCtrl.Connect()
                        self.wfile.write(self.DCS.LVCtrl.ProcessCommand(request[3:]))
                        self.DCS.LVCtrl.Disconnect()

                # Likewise, self.wfile is a file-like object used to write back
                # to the client
                #self.wfile.write(self.data.upper())

if __name__ == "__main__":
        HOST, PORT = "eplxp001", 9999
    
        DCS = BhamLabDCS()

        # Create the server, binding to host on port 9999
        server = SocketServer.TCPServer((HOST, PORT), BhamLabDCSHandler)

        # Activate the server; this will keep running until you
        # interrupt the program with Ctrl-C
        try:
                server.serve_forever()
	except KeyboardInterrupt:
                print "Closing"
                DCS.Close()
