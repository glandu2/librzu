#include "ClassCounter.h"

//For detail about this variable, see ClassCounter.h
unsigned int ClassCounter__internal_object_engine_class_counter = 0;

//generated with notepad++ with this pattern: (.*) ->
//#include "\1.h"\nDECLARE_CLASSCOUNT_STATIC\(\1\)\n
/* Class list:
Object
CircularBuffer
ConfigInfo
EncryptedSocket
EventLoop
RappelzSocket
RC4Cipher
Server
Socket
*/

#include "Object.h"
DECLARE_CLASSCOUNT_STATIC(Object)

#include "CircularBuffer.h"
DECLARE_CLASSCOUNT_STATIC(CircularBuffer)

#include "ConfigInfo.h"
DECLARE_CLASSCOUNT_STATIC(ConfigInfo)

#include "EncryptedSocket.h"
DECLARE_CLASSCOUNT_STATIC(EncryptedSocket)

#include "EventLoop.h"
DECLARE_CLASSCOUNT_STATIC(EventLoop)

#include "Log.h"
DECLARE_CLASSCOUNT_STATIC(Log)

#include "RappelzSocket.h"
DECLARE_CLASSCOUNT_STATIC(RappelzSocket)

#include "RC4Cipher.h"
DECLARE_CLASSCOUNT_STATIC(RC4Cipher)

#include "Socket.h"
DECLARE_CLASSCOUNT_STATIC(Socket)
