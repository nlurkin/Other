#!/bin/env python
# encoding: utf-8
'''
LVControl -- shortdesc

LVControl is a description

It defines classes_and_methods

@author:     Antonino Sergi
@contact:    Antonino.Sergi@cern.ch
@deffield    updated: Updated
'''

import socket
import os
import sys
import re
from argparse import ArgumentParser, RawDescriptionHelpFormatter

__all__ = []
__version__ = 0.1
__date__ = '2014-04-11'
__updated__ = '2014-04-11'

class Args(object):
        pass

class LowVoltageControl():
        def __init__(self):
                self.sock = None

        def Connect(self):
	        try:
	        	self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	        	self.sock.connect(("ql355tp", 9221))
	        except socket.error as e:
	        	print "Unable to establish communication with tdspy [Error {0}]: {1}".format(e.errno, e.strerror)
	        	sys.exit(2)
	        self.sock.settimeout(0.1)

        def ProcessCommand(self, command):
                command = command.lower()
                print command
                args = Args()
                args.HV = None
                args.Value = None
                args.Set = None

                channels = re.findall('ch (\d+)', command)
                if not channels:
                        print "Invalid request\n"
                        return "Invalid request\n"
                if 'off' in command:
                        args.Output = 'Off'
                elif 'on' in command:
                        args.Output = 'On'
                else:
                        args.Output = None

                if ' v' in command:
                        args.Type = 'V'
                elif ' i' in command:
                        args.Type = 'I'
                elif args.Output is None:
                        print "Invalid request\n"
                        return "Invalid request\n"

                if ' hv' in command:
                        args.HV = True

                values = []
                if "setval" in command and args.Type == 'V':
                        if re.findall('setval (\d+\.*\d*)', command):
                                values = re.findall('setval (\d+\.*\d*)', command)
                response = ''
                for i in range(len(channels)):
                        args.Channel = channels[i]
                        if len(values) > i:
                                args.Value = values[i]
                                args.Set = True
                        else:
                                args.Set = False
                        response = response + self.Process(args)
                return response
                        

        def Process(self, args):
                if args.HV:
                        if args.Channel == '3':
                                args.Value = "%3.2f" % (float(args.Value) * 0.00236)
                        elif args.Channel == '2':
                                args.Value = "%3.2f" % (float(args.Value) * 0.00226)

                if args.Set:
                        command = args.Type + args.Channel + " " + args.Value
                else:
                        if args.Output is not None:
                                if args.Output == 'On':
                                        State = '1'
                                else:
                                        State = '0'
                                command = "OP" + args.Channel + " " + State
                        else:
                                if args.Type == 'I':
                                        command = args.Type + args.Channel + "O?"
                                else:
                                        command = args.Type + args.Channel + "?"
                print command
                response = command + '\n'
        	self.sock.sendall(command)
        	while True:
        		try:
        			data = self.sock.recv(4096, 0)
        			print data,
        			if not data: break
                                response = response + data
        		except IOError:
        			break
                return response

        def Disconnect(self):
                if self.sock:
	                self.sock.close()


def main(argv=None):  # IGNORE:C0111
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
		parser.add_argument("-c", "--channel", dest="Channel", required=True, help="Channel (from 1 to 3)")
		parser.add_argument("-H", "--HV", dest="HV", action="store_true", help="Use HV mapping")
		parser.add_argument("-o", "--output", dest="Output", help="On or Off")
		parser.add_argument("-v", "--value", dest="Value", help="Value to set")
		parser.add_argument("-t", "--type", dest="Type", help="I or V")
		parser.add_argument("-s", "--set", dest="Set", action="store_true", help="Set value")
		
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

        if args.Type is None and args.Output is None:
		print "Either type or output required"
		return 2

        if args.Type != 'I' and args.Type != 'V' and args.Type is not None:
		print "Only I or V allowed"
		return 2

        if args.Output != 'On' and args.Output != 'Off' and args.Output is not None:
		print "Only On or Off allowed"
		return 2

        if args.Value is not None and (float(args.Value) > 7.5 or float(args.Value) < 0):
                print "What?? "+args.Value
		return 2

        LVCtrl = LowVoltageControl()
	
        LVCtrl.Connect()

        LVCtrl.Process(args)

        LVCtrl.Disconnect()

if __name__ == "__main__":
	sys.exit(main())
