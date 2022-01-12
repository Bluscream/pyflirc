#!/usr/bin/env python3

"""python program to solve the world problems..."""

import os, sys, string, time, logging, math, argparse
from logging import debug,warning,info,error,fatal,critical; warn=warning

_version = "0.1"

import pyflirc

def cmd_transmit(args):
  if 1:
    code = [0,2193,717,694,1408,695,1403,707,1385,695,730,695,725,695,720,695,716,694,774,694,746,694,741,694,735,695,730,695,726,694,721,694,1377,695]
    print (code)
    if 1:
      pyflirc.transmit(code, ik_delay=0, repeat=0)
      time.sleep(.1)
      pyflirc.transmit(code, ik_delay=100, repeat=0)

def cmd_log(args):
  print ("ready")
  pyflirc.set_normal()
  if 1:
    pyflirc.set_debug_pipe(0, 1)
    while 1:
      buf = []
      while 1:
        s = pyflirc.getlog()
        if s:
          buf.append(s)
          if s.endswith("\r\n"):
            buf = ''.join(buf)
            lines = buf.split('\r\n')
            for line in lines:
              print(line)
            break
        time.sleep(.05)
  
def cmd_record(args):
  key = args.files[0]
  print ("ready to record '%s'" % key)
  rc = pyflirc.set_record(key, 100)
  print ("rc:", rc)

def cmd_format(args):
  rc = pyflirc.format_config()
  print ("rc:", rc)
  
def cmd_save(args):
  fn = args.files[0]
  rc = pyflirc.save_config(fn)
  print ("wrote to %s" % fn)
  print ("rc:", rc)

def cmd_load(args):
  fn = args.files[0]
  rc = pyflirc.load_config(fn)
  print ("loaded from %s" % fn)
  print ("rc:", rc)


def cmd_info(args):
  if 1:
    ptype = pyflirc.get_product_type()
    print("product type:", ptype)
    print("version:", pyflirc.get_version())

    try:
      c = pyflirc.get_noise_cancel()
      print("noise cancel:", c)
    except pyflirc.error:
      pass
    
    try:
      n = pyflirc.get_interkey_delay()
      print("interkey_delay:", n)
    except pyflirc.error:
      pass
    try:
      n = pyflirc.get_keys_recorded()
      print("keys_recorded:", n)
    except pyflirc.error:
      pass
    try:
      n = pyflirc.get_keys_remaining()
      print("keys_remaining:", n)
    except pyflirc.error:
      pass
    
def start(args):
  try:
    #print (dir(pyflirc))
    if 1:
      rq = pyflirc.open()
    else:
      rq = pyflirc.wait_for_device()
    if rq < 0:
      print ("unable to open device", rq)
      return
    if len(args.files) == 0:
      cmd = "info"
    else:
      cmd = args.files[0]
    args.files = args.files[1:]
    f = globals().get("cmd_%s" % cmd)
    if f:
      f(args)
    else:
      print ("error: no such command %s" % cmd)
  finally:
    pyflirc.close()

def test():
  logging.warn("Testing")

def parse_args(argv):
  parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter, description=__doc__)

  parser.add_argument("-t", "--test", dest="test_flag", default=False, action="store_true", help="Run test function")
  parser.add_argument("--log-level", type=str, choices=["DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"], help="Desired console log level")
  parser.add_argument("-d", "--debug", dest="log_level", action="store_const", const="DEBUG", help="Activate debugging")
  parser.add_argument("-q", "--quiet", dest="log_level", action="store_const", const="CRITICAL", help="Quite mode")
  parser.add_argument("files", type=str, nargs='*')

  args = parser.parse_args(argv[1:])

  return parser, args

def main(argv, stdout, environ):
  if sys.version_info < (3, 0): reload(sys); sys.setdefaultencoding('utf8')

  parser, args = parse_args(argv)

  logging.basicConfig(format="[%(asctime)s] %(levelname)-6s %(message)s (%(filename)s:%(lineno)d)", 
                      datefmt="%m/%d %H:%M:%S", level=args.log_level)

  if args.test_flag:  test();   return

  start(args)

if __name__ == "__main__":
  main(sys.argv, sys.stdout, os.environ)
