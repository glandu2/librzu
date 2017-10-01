#include "Packet/JSONWriter.h"
#include "AuthClient/TS_AC_SERVER_LIST.h"
#include <iostream>

/* Tests:
 * TestPacketServer => Test, name
 * TestPacketSession => TestPacketServer
 * Test: onPacketReceived(TestPacketSession, std::string sourceName / enum, const Packet)
 * test dans dll
 * ProcessSpawner => config => name / enum (used to generate pipe name)
 */

/*
class JSONWriter {
public:
    JSONWriter() : depth(0), version(0), newList(true) {}

    int depth;
    int version;
    bool newList;

    int getVersion() { return version; }

    void printIdent() {
        if(!newList)
            std::cout << ",\n";
        else
            std::cout << "\n";

        newList = false;
        for(int i = 0; i < depth; i++)
            std::cout << '\t';
    }

    // Write functions /////////////////////////

    //Primitives
    void write(const char* fieldName, uint8_t val) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << (int)val;
    }
    void write(const char* fieldName, int8_t val) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << (int)val;
    }

    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value, void>::type
    write(const char* fieldName, T val) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << val;
    }

    //Objects
    template<typename T>
    typename std::enable_if<!std::is_fundamental<T>::value, void>::type
    write(const char* fieldName, const T& val) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << '{';

        newList = true;
        depth++;
        val.serialize(this);
        depth--;

        newList = true;
        printIdent();
        std::cout << "}";
    }

    //Fixed array
    template<typename T>
    void writeArray(const char* fieldName, const T* val, size_t size) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << '[';

        newList = true;
        depth++;
        for(size_t i = 0; i < size; i++) {
            write(nullptr, val[i]);
        }
        depth--;

        newList = true;
        printIdent();
        std::cout << "]";
    }

    //String
    void writeArray(const char* fieldName, const char* val, size_t size) {
        printIdent();
        std::cout << '\"' << fieldName << "\": \"" << val << "\"";
    }

    //Dynamic array of object
    template<class U>
    void writeDynArray(const char* fieldName, const std::vector<U>& val) {
        printIdent();
        if(fieldName)
            std::cout << '\"' << fieldName << "\": ";
        std::cout << '[';

        newList = true;
        depth++;
        auto it = val.begin();
        auto itEnd = val.end();
        for(; it != itEnd; ++it)
            write(nullptr, *it);
        depth--;

        newList = true;
        printIdent();
        std::cout << "]";
    }

    // Read functions /////////////////////////

    //Primitives via arg
    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value, void>::type
    read(const char*, T& val) {
    }

    //Primitives via return type (for vector.resize())
    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value, T>::type
    read(const char*) {
    }

    //Objects
    template<typename T>
    typename std::enable_if<!std::is_fundamental<T>::value, void>::type
    read(const char*, T& val) {
    }

    //Fixed array of primitive
    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value, void>::type
    readArray(const char*, T* val, size_t size) {
    }

    //Fixed array of objects
    template<typename T>
    typename std::enable_if<!std::is_fundamental<T>::value, void>::type
    readArray(const char*, T* val, size_t size) {
    }

    //String
    void readArray(const char*, std::string& val, size_t size) {
    }

    //Dynamic array of object
    template<class U>
    void readDynArray(const char*, std::vector<U>& val) {
    }

    //Dummy read
    void discard(const char*, int size) {
    }
};
*/

int main(int argc, char* argv[]) {
	TS_AC_SERVER_LIST packet;
	TS_SERVER_INFO serverInfo[2];

	packet.last_login_server_idx = 5;

	serverInfo[0].server_idx = 1;
	serverInfo[0].server_name = "server 1";
	serverInfo[0].is_adult_server = false;
	serverInfo[0].server_screenshot_url = "http://screenshot.com/server1";
	serverInfo[0].server_ip = "127.0.0.1";
	serverInfo[0].server_port = 4514;
	serverInfo[0].user_ratio = 30;

	serverInfo[1].server_idx = 2;
	serverInfo[1].server_name = "server 2";
	serverInfo[1].is_adult_server = true;
	serverInfo[1].server_screenshot_url = "http://screenshot.com/server2";
	serverInfo[1].server_ip = "192.168.89.24";
	serverInfo[1].server_port = 4515;
	serverInfo[1].user_ratio = 64;

	packet.servers.push_back(serverInfo[0]);
	packet.servers.push_back(serverInfo[1]);

	{
		std::cout << "Version 0 serialization :\n";
		JSONWriter jsonWriter(0, false);
		packet.serialize(&jsonWriter);
		jsonWriter.finalize();
		std::cout << jsonWriter.toString();
	}

	{
		std::cout << "Version 100000000 serialization :\n";
		JSONWriter jsonWriter(100000000, false);
		packet.serialize(&jsonWriter);
		jsonWriter.finalize();
		std::cout << jsonWriter.toString();
	}

	return 0;
}
