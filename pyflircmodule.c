#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <flirc.h>


static PyObject *FlircError;

static PyObject *
open_device(PyObject *self, PyObject *args, PyObject *keywds)
{
    char mf[2048];
    int vendorid=0x20A0;
    int rc;

    strcpy(mf, "flirc.tv");

    static char *kwlist[] = {"vendorid", "mf", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|is", kwlist, &vendorid, &mf))
        return NULL;
    
    rc = fl_open_device(vendorid, mf);
    if(rc < 0) {
      PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
      return NULL;
    }

    return PyLong_FromLong(rc);
}

static PyObject *
get_product_type(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_product_type();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
close_device(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  fl_close_device();
  return Py_BuildValue("");
}

static PyObject *
get_version(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  int rc;
  int major, minor, pversion;
  
  rc = fl_major_version();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  major = rc;

  rc = fl_minor_version();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  minor = rc;
  
  rc = fl_patch_version();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  pversion = rc;

  char *vstr = fl_version_str();
  if(vstr < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", vstr));
    return NULL;
  }

  PyObject *ret;
  ret = Py_BuildValue("(iiis)", major, minor, pversion, vstr);
    
  return ret;
}

static PyObject *
reboot(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  int rc = fl_reboot();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return Py_BuildValue("");
}

static PyObject *
set_delete(PyObject *self, PyObject *args)
{
  int wait=0;
  if(!PyArg_ParseTuple(args, "i", &wait)) return NULL;
  int rc = fl_set_delete(wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record(PyObject *self, PyObject *args)
{
  int wait=0;
  char *key=NULL;
  
  if(!PyArg_ParseTuple(args, "si", &key, &wait)) return NULL;
  if(strlen(key) == 0) {
    PyErr_SetString(PyExc_TypeError, "key must be a string");
    return NULL;
  }
  
  int rc = fl_set_record(key,wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record_api(PyObject *self, PyObject *args)
{
  int wait=0;
  int key=0;
  int modifier=0;
  int mode=0;
  
  if(!PyArg_ParseTuple(args, "iiii", &mode, &modifier, &key, &wait)) return NULL;
  
  int rc = fl_set_record_api(mode, modifier, key, wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record_api_new(PyObject *self, PyObject *args)
{
  int wait=0;
  int key=0;
  int modifier=0;
  int report_id=0;
  
  if(!PyArg_ParseTuple(args, "iiii", &report_id, &modifier, &key, &wait)) return NULL;
  
  int rc = fl_set_record_api_new(report_id, modifier, key, wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record_lp(PyObject *self, PyObject *args)
{
  int wait=0;
  char *key=NULL;
  
  if(!PyArg_ParseTuple(args, "si", &key, &wait)) return NULL;
  if(strlen(key) == 0) {
    PyErr_SetString(PyExc_TypeError, "key must be a string");
    return NULL;
  }
  
  int rc = fl_set_record_lp(key, wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record_macro(PyObject *self, PyObject *args)
{
  int wait=0;
  char *key=NULL;
  
  if(!PyArg_ParseTuple(args, "si", &key, &wait)) return NULL;
  if(strlen(key) == 0) {
    PyErr_SetString(PyExc_TypeError, "key must be a string");
    return NULL;
  }
  
  int rc = fl_set_record_macro(key, wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_normal(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_set_normal();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_interrupt(PyObject *self, PyObject *args)
{
  int timeout=0;
  
  if(!PyArg_ParseTuple(args, "i", &timeout)) return NULL;
  
  int rc = fl_set_interrupt(timeout);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_record_toggle(PyObject *self, PyObject *args)
{
  int wait=0;
  
  if(!PyArg_ParseTuple(args, "i", &wait)) return NULL;
  
  int rc = fl_record_toggle(wait);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
delete_buttons(PyObject *self, PyObject *args)
{
  char *user_input=NULL;
  
  if(!PyArg_ParseTuple(args, "s", &user_input)) return NULL;
  if(strlen(user_input) == 0) {
    PyErr_SetString(PyExc_TypeError, "user_input must be a string");
    return NULL;
  }
  
  int rc = fl_delete_buttons(user_input);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_noise_cancel(PyObject *self, PyObject *args)
{
  int val=0;
  
  if(!PyArg_ParseTuple(args, "i", &val)) return NULL;
  
  int rc = fl_set_noise_cancel(val);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_noise_cancel(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_get_noise_cancel();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_sensitivity(PyObject *self, PyObject *args)
{
  int val=0;
  
  if(!PyArg_ParseTuple(args, "i", &val)) return NULL;
  
  int rc = fl_set_sensitivity(val);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_sensitivity(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_get_sensitivity();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
set_interkey_delay(PyObject *self, PyObject *args)
{
  int val=0;
  
  if(!PyArg_ParseTuple(args, "i", &val)) return NULL;
  
  int rc = fl_set_interkey_delay(val);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_interkey_delay(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_get_interkey_delay();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
enable_rom_table(PyObject *self, PyObject *args)
{
  int val=0;
  int en=0;
  
  if(!PyArg_ParseTuple(args, "ii", &val, &en)) return NULL;
  
  int rc = fl_enable_rom_table(val, en);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_rom_table(PyObject *self, PyObject *args)
{
  int table;
  if(!PyArg_ParseTuple(args, "i", &table)) return NULL;
  
  int rc = fl_get_rom_table(table);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}


static PyObject *
format_config(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_format_config();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
save_config(PyObject *self, PyObject *args)
{
  char *fn=NULL;
  if(!PyArg_ParseTuple(args, "s", &fn)) return NULL;
  
  int rc = fl_save_config(fn);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
load_config(PyObject *self, PyObject *args)
{
  char *fn=NULL;
  if(!PyArg_ParseTuple(args, "s", &fn)) return NULL;
  
  int rc = fl_load_config(fn);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_keys_recorded(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_keys_recorded();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
get_keys_remaining(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_keys_remaining();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}

static PyObject *
delete_index(PyObject *self, PyObject *args)
{
  int index=0;
  if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
  
  int rc = fl_delete_index(index);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return PyLong_FromLong(rc);
}


static PyObject *
libusb_logging(PyObject *self, PyObject *args)
{
  int enable=0;
  if(!PyArg_ParseTuple(args, "i", &enable)) return NULL;
  
  fl_libusb_logging(enable);
  return Py_BuildValue("");
}

static PyObject *
libusb_log(PyObject *self, PyObject *args)
{
  char buf[2048];
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_log(buf);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return Py_BuildValue("s", buf);
}


static PyObject *
get_sku(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  char *sku = fl_get_sku();
  if(sku == NULL) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "No Device", -1));
    return NULL;
  }
  return Py_BuildValue("s", sku);
}

static PyObject *
ir_transmit_kill(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  int rc = fl_ir_transmit_kill();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  return Py_BuildValue("");
}

static PyObject *
getlog(PyObject *self, PyObject *args)
{
  if(!PyArg_ParseTuple(args, "")) return NULL;
  
  char buf[2048];
  int rc;

  rc = fl_get_state();
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }
  
  rc = fl_log(buf);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }

  PyObject *ret;
  ret = Py_BuildValue("s", buf);
    
  return ret;
}

static PyObject *
set_debug_pipe(PyObject *self, PyObject *args)
{
  int enable = 0;
  int pipe = 0;
  
  if(!PyArg_ParseTuple(args, "ii", &pipe, &enable)) return NULL;
  
  int rc = fl_set_debug_pipe(pipe, enable);

  return PyLong_FromLong(rc);
}

static PyObject *
get_debug_pipe(PyObject *self, PyObject *args)
{
  int pipe = 0;
  if(!PyArg_ParseTuple(args, "i", &pipe)) return NULL;
  
  int rc = fl_get_debug_pipe(pipe);

  return PyLong_FromLong(rc);
}

static PyObject *
wait_for_device(PyObject *self, PyObject *args, PyObject *keywds)
{
  char mf[2048];
  int vendorid=0x20A0;
  int timeout=0;
  strcpy(mf, "flirc.tv");
  
  static char *kwlist[] = {"vendorid", "mf", "timeout", NULL};
  
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "|isi", kwlist, &vendorid, &mf, &timeout))
    return NULL;

  int rc;
  if(timeout == 0) {
    rc = fl_wait_for_device(vendorid, mf);
  } else {
    rc = fl_wait_for_device_timeout(vendorid, mf, timeout);
  }
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }

  return PyLong_FromLong(rc);
}

static PyObject *
transmit(PyObject *self, PyObject *args, PyObject *keywds)
{
  PyObject *code;
  int ik_delay=0;
  int repeat=0;

  uint16_t buf[100];
  int buf_size = 0;
    
  int rc = 0;
    
  static char *kwlist[] = {"code", "ik_delay", "repeat", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, keywds, "O|ii", kwlist, &code, &ik_delay, &repeat))
    return NULL;

  PyObject *iter = PyObject_GetIter(code);
  if (!iter) {
    PyErr_SetString(PyExc_TypeError, "must be an iterable type with integers.");
    return NULL;
  }

  while (1) {
    PyObject *next = PyIter_Next(iter);
    if (!next) {
      // nothing left in the iterator
      break;
    }

    if (!PyLong_Check(next)) {
      // error, we were expecting an integer value
      PyErr_SetString(PyExc_TypeError, "list items must be integers.");
      return NULL;
    }

    int n = (int) PyLong_AsLong(next);

    buf[buf_size] = n;
    buf_size++;
  }

  if(buf_size < 3) {
    PyErr_SetString(PyExc_TypeError, "invalid length, must be 3 or more integers.");
    return NULL;
  }

  if((buf_size % 2) == 1) {
    PyErr_SetString(PyExc_TypeError, "invalid length, must be an even number of edges.");
    return NULL;
  }

  if(buf[0] != 0) {
    PyErr_SetString(PyExc_TypeError, "buffer incorrect, the first integer must be a 0.");
    return NULL;
  }

  //printf("%s\n", buf);
    
  rc = fl_transmit_raw(buf, buf_size, ik_delay, repeat);
  if(rc < 0) {
    PyErr_SetObject(FlircError, Py_BuildValue("(si)", "ecode", rc));
    return NULL;
  }

  return PyLong_FromLong(rc);
}

static PyMethodDef FlircMethods[] = {
    {"open",  (PyCFunction)(void(*)(void))open_device, METH_VARARGS| METH_KEYWORDS,
     "open(venderid=0x20A0, mf=\"flirc.tv\")\n--\n\nSearches for and opens flirc."},
    {"wait_for_device", (PyCFunction)(void(*)(void))wait_for_device, METH_VARARGS| METH_KEYWORDS,
     "wait_for_device(venderid=0x20A0, mf=\"flirc.tv\", timeout=0)\n--\n\nWaits for flirc to be present in the system, and then opens the device. If timeout is set, it will fail after timeout has passed."},
    {"close",  close_device, METH_VARARGS,
     "close()\n--\n\nCloses flirc. Must be called before the program exits.\nErroneous behavior will be seen if this isn't called."},
    {"get_product_type",  get_product_type, METH_VARARGS,
     "get_product_type()\n--\n\nreturns the product type."},
    {"get_version",  get_version, METH_VARARGS,
     "get_version()\n--\n\nreturn the version."},
    {"getlog",  getlog, METH_VARARGS,
     "getlog()\n--\n\nget usb event log."},
    {"set_debug_pipe", set_debug_pipe, METH_VARARGS,
     "set_debug_pipe(pipe, enable)\n--\n\nenables a specific log pipe"},
    {"get_debug_pipe", get_debug_pipe, METH_VARARGS,
     "get_debug_pipe(pipe)\n--\n\nget a specific log pipe"},
    {"set_delete", set_delete, METH_VARARGS,
     "set_delete(wait)\n--\n\nPuts flirc into delete mode. When calling this function, the next remote button that flirc 'see's' will be deleted from the flirc. If the button is not found in flirc's memory, flirc will ignore the request and go back into normal operation. If the button is found in memory, flirc will delete the button, and go back into normal operation."},
    {"delete_buttons", delete_buttons, METH_VARARGS,
     "delete_buttons(user_input)\n--\n\ndeletes multiple buttons if they exist from the device."},
    {"set_record", set_record, METH_VARARGS,
     "set_record(key, wait)\n--\n\nputs flirc into record mode. The next button pressed will be saved in flirc and linked with the keyboard key user_input. This function can be passed either a character or a word. Any character will be translated into the USB HID equivalent value automatically before being sent down to flirc.   * Example Key Strings: escape, return, enter, backspace, delete, tab, space, F[1-12], printscreen, scroll, pause, insert, home, pageup, pagedown, end, right, left, down, up, wake"},
    {"set_record_api", set_record_api, METH_VARARGS,
     "set_record_api(mode, modifier, key, wait)\n--\n\nthis function calls the same USB command as setRecord, but there is no translation layer. For example, in both examples below, lower case 'a' will be paired with the next remote button seen by flirc."},
    {"set_record_api_new", set_record_api_new, METH_VARARGS,
     "set_record_api_new(report_id, modifier, key, wait)\n--\n\nthis function calls the same USB command as setRecord. Raw record api for new version of the hardware. No table translation."},
    {"set_record_lp", set_record_lp, METH_VARARGS,
     "set_record_lp(key, wait)\n--\n\nwill assign a second hid function to the same remote control button press. The second key will be sent out after half a second of holding down the button."},
    {"set_record_macro", set_record_macro, METH_VARARGS,
     "set_record_macro(key, wait)\n--\n\nwill record another hid key to the same remote control button as a macro. All keys assigned to a single remote control press will be sent out sequentially."},
    {"set_normal", set_normal, METH_VARARGS,
     "set_normal()\n--\n\nPuts flirc into normal mode. In normal operation, flirc sends keyboard keys to the PC when recorded infrared signals are seen. Can be called to take the device out of record mode."},
    {"set_interrupt", set_interrupt, METH_VARARGS,
     "set_interrupt(timeout)\n--\n\nWaits for an interrupt from flirc to be created. An interrupt is only generated after recording or deleting a button."},
    {"set_record_toggle", set_record_toggle, METH_VARARGS,
     "set_record_toggle(wait)\n--\n\nwill record the toggle button. Will allow flirc to be disabled or enabled by recorded key."},
    {"set_noise_cancel", set_noise_cancel, METH_VARARGS,
     "set_noise_cancel(val)\n--\n\nenables the noise canceler. This is used when phantom keys are detected on flirc."},
    {"get_noise_cancel", get_noise_cancel , METH_VARARGS,
     "get_noise_cancel()\n--\n\nget currently state of noise cancel"},
    {"set_sensitivity", set_sensitivity, METH_VARARGS,
     "set_sensitivity(val)\n--\n\nSet's the sensitivity of flirc. Changing the sensitivity will require all buttons to be re-recorded. This function is only for advanced users and has not been brought to the GUI yet."},
    {"get_sensitivity", get_sensitivity, METH_VARARGS,
     "get_sensitivity()\n--\n\neget currently set sensitivity."},
    {"set_interkey_delay", set_interkey_delay, METH_VARARGS,
     "set_interkey_delay(val)\n--\n\nSets the interkey delay. This will help with button repeat issues. This function is only for advanced users."},
    {"get_interkey_delay", get_interkey_delay, METH_VARARGS,
     "get_interkey_delay()\n--\n\ngets the interkey delay."},
    {"enable_rom_table", enable_rom_table, METH_VARARGS,
     "enable_rom_table(val, enable)\n--\n\nEnable or disables a particular rom table"},
    {"get_rom_table", get_rom_table, METH_VARARGS,
     "get_rom_table(table)\n--\n\ngets the passed table state"},
    {"format_config", format_config, METH_VARARGS,
     "format_config()\n--\n\nclears all saved configuration data in the device. All recorded buttons will be erased."},
    {"save_config", save_config, METH_VARARGS,
     "save_config(filename)\n--\n\nthis function downloads the configuration file from a flirc device and saves it to the location specified by the user. This is a raw binary file in a proprietary format."},
    {"load_config", load_config, METH_VARARGS,
     "load_config(filename)\n--\n\nuploads a .fcfg file to a flirc device that was previously saved with fl_save_config()."},
    {"get_keys_recorded", get_keys_recorded, METH_VARARGS,
     "get_keys_recorded()\n--\n\ngets the used space, in keys"},
    {"get_keys_remaining", get_keys_remaining, METH_VARARGS,
     "get_keys_remaining()\n--\n\ngets the space free, in keys"},
    {"delete_index", delete_index, METH_VARARGS,
     "delete_index(index)\n--\n\nspecify the index of the key to delete"},
    {"libusb_log", libusb_log, METH_VARARGS,
     "libusb_log()\n--\n\nget usb event log"},
    {"libusb_logging", libusb_logging, METH_VARARGS,
     "libusb_logging(enable)\n--\n\nenable/disable libusb logging"},
    {"get_sku", get_sku, METH_VARARGS,
     "get_sku()\n--\n\nparses header and gets the sku so you don't have to"},
    {"ir_transmit_kill", ir_transmit_kill, METH_VARARGS,
     "ir_transmit_kill()\n--\n\nwill stop the transmitter from transmission"},
    {"reboot_device", reboot, METH_VARARGS,
     "reboot_device()\n--\n\nSends a command down that issues a reset.\nThis is the equivalent as unplugging and plugging in the device. No data is lost with this command and this command should never need to be called."},
    {"transmit", (PyCFunction)(void(*)(void))transmit, METH_VARARGS | METH_KEYWORDS,
     "transmit(code, ik_delay=0, repeat=0)\n--\n\nsends a raw packet"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef flircmodule = {
    PyModuleDef_HEAD_INIT,
    "pyflirc",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    FlircMethods
};



PyMODINIT_FUNC
PyInit_pyflirc(void)
{
    PyObject *m;

    m = PyModule_Create(&flircmodule);
    if (m == NULL)
        return NULL;
    
    PyModule_AddObject(m, "EOK", PyLong_FromLong(EOK));
    PyModule_AddObject(m, "STATE_PLAYBACK", PyLong_FromLong(STATE_PLAYBACK));
    PyModule_AddObject(m, "STATE_RECORD", PyLong_FromLong(STATE_RECORD));
    PyModule_AddObject(m, "STATE_DELETE", PyLong_FromLong(STATE_DELETE));
    PyModule_AddObject(m, "NO_INTERRUPT", PyLong_FromLong(NO_INTERRUPT));
    PyModule_AddObject(m, "ENOMEM", PyLong_FromLong(-ENOMEM));
    PyModule_AddObject(m, "EBADF", PyLong_FromLong(-EBADF));
    PyModule_AddObject(m, "EFAULT", PyLong_FromLong(-EFAULT));
    PyModule_AddObject(m, "EINVAL", PyLong_FromLong(-EINVAL));
    PyModule_AddObject(m, "ENODEV", PyLong_FromLong(-ENODEV));
    PyModule_AddObject(m, "ENOSYS", PyLong_FromLong(-ENOSYS));
    PyModule_AddObject(m, "ECANCELED", PyLong_FromLong(-ECANCELED));
    PyModule_AddObject(m, "EWRONGDEV", PyLong_FromLong(-EWRONGDEV));
    PyModule_AddObject(m, "EUPGRADE_FAILED", PyLong_FromLong(-EUPGRADE_FAILED));
    PyModule_AddObject(m, "EIDXRANGE", PyLong_FromLong(-EIDXRANGE));
    PyModule_AddObject(m, "ENXIO", PyLong_FromLong(-ENXIO));
    PyModule_AddObject(m, "LIBUSBERR", PyLong_FromLong(LIBUSBERR));
    PyModule_AddObject(m, "ETIMEOUT", PyLong_FromLong(-ETIMEOUT));
    
    PyModule_AddObject(m, "FUNK_SUCCESS", PyLong_FromLong(-FUNK_SUCCESS));
    PyModule_AddObject(m, "ERR_NO_SPACE", PyLong_FromLong(-ERR_NO_SPACE));
    PyModule_AddObject(m, "ERR_BUTTON_EXISTS", PyLong_FromLong(-ERR_BUTTON_EXISTS));
    PyModule_AddObject(m, "ERR_KEY_NOT_FOUND", PyLong_FromLong(-ERR_KEY_NOT_FOUND));
    
    PyModule_AddObject(m, "FL_BOOTLOADER", PyLong_FromLong(BOOTLOADER));
    PyModule_AddObject(m, "FL_FIRMWARE_FLIRC", PyLong_FromLong(FIRMWARE_FLIRC));
    PyModule_AddObject(m, "FL_FIRMWARE", PyLong_FromLong(FIRMWARE));
    PyModule_AddObject(m, "FL_FIRMWARE_FLIRC_AMAZON", PyLong_FromLong(FIRMWARE_FLIRC_AMAZON));
    PyModule_AddObject(m, "FL_UNKNOWN_DEV", PyLong_FromLong(FL_UNKNOWN_DEV));
    PyModule_AddObject(m, "MaxTimeout", PyLong_FromLong(MAX_TIMEOUT));

    PyModule_AddObject(m, "RM_NORMAL", PyLong_FromLong(RM_NORMAL));
    PyModule_AddObject(m, "RM_MACRO", PyLong_FromLong(RM_MACRO));
    PyModule_AddObject(m, "RM_LONG_PRESS", PyLong_FromLong(RM_LONG_PRESS));

    
    FlircError = PyErr_NewException("pyflirc.error", NULL, NULL);
    Py_XINCREF(FlircError);
    if (PyModule_AddObject(m, "error", FlircError) < 0) {
        Py_XDECREF(FlircError);
        Py_CLEAR(FlircError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
