
import time
import Pyro.core

class opc(Pyro.core.ObjBase):
        def __init__(self):
                    Pyro.core.ObjBase.__init__(self)
                    self._remote_hosts = {}
                    self._init_times = {}
                    self._tx_times = {}


print 'crap'
daemon = Pyro.core.Daemon(host='192.168.1.10', port=7766)
print 'crap'
print daemon.connect(opc(), "opc")
print 'crap'
print Pyro.core.PyroURI

while True:
        time.sleep(1)
