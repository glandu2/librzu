#include "ClassCounter.h"

//For detail about this variable, see ClassCounter.h
unsigned int ClassCounter__internal_object_engine_class_counter = 0;

//generated with notepad++ with this pattern: (.*) ->
//#include "\1.h"\ntemplate<> unsigned long ClassCounter<\1>::objectCount = 0;\ntemplate<> const unsigned int ClassCounter<\1>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;\n
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
template<> unsigned long ClassCounter<Object>::objectCount = 0;
template<> const unsigned int ClassCounter<Object>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "CircularBuffer.h"
template<> unsigned long ClassCounter<CircularBuffer>::objectCount = 0;
template<> const unsigned int ClassCounter<CircularBuffer>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "ConfigInfo.h"
template<> unsigned long ClassCounter<ConfigInfo>::objectCount = 0;
template<> const unsigned int ClassCounter<ConfigInfo>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "EncryptedSocket.h"
template<> unsigned long ClassCounter<EncryptedSocket>::objectCount = 0;
template<> const unsigned int ClassCounter<EncryptedSocket>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "EventLoop.h"
template<> unsigned long ClassCounter<EventLoop>::objectCount = 0;
template<> const unsigned int ClassCounter<EventLoop>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "RappelzSocket.h"
template<> unsigned long ClassCounter<RappelzSocket>::objectCount = 0;
template<> const unsigned int ClassCounter<RappelzSocket>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "RC4Cipher.h"
template<> unsigned long ClassCounter<RC4Cipher>::objectCount = 0;
template<> const unsigned int ClassCounter<RC4Cipher>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "Server.h"
template<> unsigned long ClassCounter<Server>::objectCount = 0;
template<> const unsigned int ClassCounter<Server>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;

#include "Socket.h"
template<> unsigned long ClassCounter<Socket>::objectCount = 0;
template<> const unsigned int ClassCounter<Socket>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;
