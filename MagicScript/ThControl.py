#!/bin/env python
# encoding: utf-8
'''
ThControl -- shortdesc

ThControl is a description

It defines classes_and_methods

@author:     Antonino Sergi
@contact:    Antonino.Sergi@cern.ch
@deffield    updated: Updated
'''

import OpenOPC
import types
import time
import sys
from argparse import ArgumentParser, RawDescriptionHelpFormatter

__all__ = []
__version__ = 0.1
__date__ = '2014-05-11'
__updated__ = '2014-05-11'

class OPCRemoteClient():
        def __init__(self, RemoteHost = 'dcspc', RemotePort = 7766, OPCHost = 'epdt120', OPCServer = 'OPC20CanOpen+'):
                self.RemoteHost = RemoteHost
                self.RemotePort = RemotePort
                self.OPCHost = OPCHost
                self.OPCServer = OPCServer
                self.property_ids = None
                self.group_size = None
                self.tx_pause = 1000
                self.include_err_msg = False

        def Connect(self, RemoteHost = 'dcspc', RemotePort = 7766, OPCHost = 'epdt120', OPCServer = 'OPC20CanOpen+'):
                self.RemoteHost = RemoteHost
                self.RemotePort = RemotePort
                self.OPCHost = OPCHost
                self.OPCServer = OPCServer
                print "Connectining to OPC server "+self.OPCServer+" on "+self.RemoteHost
                self.OPC = OpenOPC.open_client(self.RemoteHost, self.RemotePort)
                self.OPC.connect(self.OPCServer, self.OPCHost)
                print "Connected\nWaiting for OPC server to read all values from devices ..."
                time.sleep(10)

        def Read(self, tags):
                return self.OPC.properties(tags, self.property_ids)
                
        def Write(self, tag_value_pairs):
                return self.OPC.write(tag_value_pairs, size=self.group_size, pause=self.tx_pause, include_error=self.include_err_msg)
                
        def Close(self):
                self.OPC.close()

class ThresholdControl():
        def __init__(self):
                print "Initializing Threshold Control..."
                self.opc = OPCRemoteClient()
                self.opc.Connect()
                self.isCalibrated = True 
                self.minThr = [95.979, 94.148, 94.224, 1537.422, 1519.111, 94.377, 91.63, 93.842]
                self.maxThr = [344.319, 342.412, 342.793, 1363.469, 871.595, 343.633, 340.733, 343.099]
                print "...Threshold Control initialized"

        def ReadThreshold(self, channels):
                tags = ['BUS.KTAG_TH.ai_'+str(ch) for ch in channels]
                return self.opc.Read(tags)

        def SetThreshold(self, channels_values_pairs):
                response = ''
                for pair in channels_values_pairs:
                        Channel = pair[0]
                        if Channel == 0: Channel = 1
                        elif Channel == 1: Channel = 0
                        elif Channel == 4: Channel = 5
                        elif Channel == 5: Channel = 4
                        ChipSelect = 0xFF - (1 << Channel)
                        Channel = pair[0]
                        Threshold = pair[1]
                        if self.isCalibrated:
                                print "Setting threshold to "+str(Threshold)+" mV for channel "+str(Channel)
                                response = response + "Setting threshold to "+str(Threshold)+" mV for channel "+str(Channel)+'\n'
                                DAC = self.mVToDAC(Channel, Threshold)
                        else:
                                DAC = int(Threshold)
                                print "Setting threshold to "+str(DAC)+" for channel "+str(Channel)
                                response = response + "Setting threshold to "+str(DAC)+" for channel "+str(Channel)+'\n'
                        tag_value_pairs = [['BUS.KTAG_TH.cs',str(ChipSelect)], ['BUS.KTAG_TH.dacvalue',str(DAC)], ['BUS.KTAG_TH.cs','255']]     
                        self.opc.Write(tag_value_pairs)
                return response

        def SetAllThresholds(self, Threshold):
                if not self.isCalibrated:
                        print "Setting all thresholds to "+str(int(Threshold))
                        tag_value_pairs = [['BUS.KTAG_TH.cs','0'], ['BUS.KTAG_TH.dacvalue',str(int(Threshold))], ['BUS.KTAG_TH.cs','255']]     
                        self.opc.Write(tag_value_pairs)
                        return "Setting all thresholds to "+str(int(Threshold))
                        
                response = ''
                for ch in range(0, 8):
                        response = response + self.SetThreshold([[ch, Threshold]])
                return response

        def ShowThresholds(self):
                response = ''
                for ch in range(0, 8):
                        ChProperties = self.ReadThreshold([ch])
                        print "Channel "+str(ch)+" = %5.1f mV" % float(ChProperties[2][3]/1000.)
                        response = response + "Channel "+str(ch)+" = %5.1f mV\n" % float(ChProperties[2][3]/1000.)
                return response

        def Calibrate(self):
                response = ''
                self.isCalibrated = False
                self.SetAllThresholds(0)
                time.sleep(10)
                for ch in range(0, 8):
                        ChProperties = self.ReadThreshold([ch])
                        self.minThr[ch] = ChProperties[2][3]/1000. 
                self.SetAllThresholds(4095)
                time.sleep(10)
                for ch in range(0, 8):
                        ChProperties = self.ReadThreshold([ch])
                        self.maxThr[ch] = ChProperties[2][3]/1000.
                self.isCalibrated = True 
                print self.minThr
                print self.maxThr
                response = response + str(self.minThr) + '\n'
                response = response + str(self.maxThr) + '\n'
                return response + "Calibrated\n"

        def mVToDAC(self, channel, threshold):
                DAC = int((threshold - self.minThr[channel]) * 4095 /( self.maxThr[channel] - self.minThr[channel]))
                if DAC > 0:
                        return min(DAC, 4095)
                else:
                        return 0

        def Reset(self):
                print "Resetting Bus"
                tag_value_pairs = [['BUS.NMT','129'], ['BUS.NMT','1']]     
                self.opc.Write(tag_value_pairs)
                time.sleep(10)
                return 'Done\n'

        def Close(self):
                self.opc.Close()

def main(argv=None): 
	'''Command line options.'''
	
	if argv is None:
		argv = sys.argv
	else:
		sys.argv.extend(argv)

	program_name = os.path.basename(sys.argv[0])
	program_version = "v%s" % __version__
	program_build_date = str(__updated__)
	program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
	program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
	program_license = '''%s

	Created by Antonino Sergi on %s.

USAGE
''' % (program_shortdesc, str(__date__))

	try:
		# Setup argument parser
		parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)
		parser.add_argument('-V', '--version', action='version', version=program_version_message)
		parser.add_argument("-a", "--all", dest="All", action="store_true", help="All channels")
		parser.add_argument("-C", "--calib", dest="Calib", action="store_true", help="Calibrate")
		parser.add_argument("-c", "--channel", dest="Channel", help="Channel (from 0 to 7")
		parser.add_argument("-q", "--query", dest="Query", action="store_true", help="Show thresholds")
		parser.add_argument("-r", "--reset", dest="Reset", action="store_true", help="Reset Bus")
		parser.add_argument("-s", "--set", dest="SetVal", help="Set threshold value")
		parser.add_argument("-u", "--uncal", dest="Uncal", action="store_true", help="Force uncalibrated value")
		
		# Process arguments
		args = parser.parse_args()
		
	except KeyboardInterrupt:
		### handle keyboard interrupt ###
		return 0
	except Exception, e:
		indent = len(program_name) * " "
		sys.stderr.write(program_name + ": " + repr(e) + "\n")
		sys.stderr.write(indent + "  for help use --help")
		return 2

	ThCtrl = ThresholdControl()

        if args.Reset:
                ThCtrl.Reset()

        if args.Query:
                ThCtrl.ShowThresholds()

        if args.Calib:
                ThCtrl.Calibrate()

        if args.SetVal is not None:
                if args.Uncal:
                        ThCtrl.isCalibrated = False
                if args.All:
                        ThCtrl.SetAllThresholds(float(args.SetVal))
                elif args.Channel is not None:
                        ThCtrl.SetThreshold([[int(args.Channel), float(args.SetVal)]])
                else:
                        ThCtrl.Close()
                        return 2
                time.sleep(10)
                ThCtrl.ShowThresholds()
    
        ThCtrl.Close()

if __name__ == "__main__":
	sys.exit(main())






