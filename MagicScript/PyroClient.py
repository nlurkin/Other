
import OpenOPC
import Pyro.core

host = '192.168.1.10'
port = 7766
Pyro.core.initClient(banner=0)
server_obj = Pyro.core.getProxyForURI("PYROLOC://%s:%s/opc" % (host, port))
print server_obj
server_obj.create_client()
print server_obj

