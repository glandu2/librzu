#ifndef PACKETS_EPICS_H
#define PACKETS_EPICS_H

class packet_version_t {
public:
	packet_version_t(unsigned int version) : version(version) {}
	packet_version_t(packet_version_t const& other) : version(other.version) {}

	int compare(unsigned int epic) const { return (this->version & 0x00FFFFFF) - epic; }
	int compare(packet_version_t other) const { return this->version - other.version; }

	bool operator==(unsigned int epic) const { return compare(epic) == 0; }
	bool operator==(packet_version_t other) const { return compare(other) == 0; }

	bool operator!=(unsigned int epic) const { return !operator==(epic); }
	bool operator!=(packet_version_t other) const { return !operator==(other); }

	bool operator>=(unsigned int epic) const { return compare(epic) >= 0; }
	bool operator>=(packet_version_t other) const { return compare(other) >= 0; }

	bool operator>(unsigned int epic) const { return compare(epic) > 0; }
	bool operator>(packet_version_t other) const { return compare(other) > 0; }

	bool operator<=(unsigned int epic) const { return compare(epic) <= 0; }
	bool operator<=(packet_version_t other) const { return compare(other) <= 0; }

	bool operator<(unsigned int epic) const { return compare(epic) < 0; }
	bool operator<(packet_version_t other) const { return compare(other) < 0; }

	int flag(unsigned int flag) const { return (version >> 24) & flag; }

	packet_version_t& operator=(unsigned int epic) {
		this->version = epic;
		return *this;
	}

	packet_version_t& operator=(packet_version_t other) {
		this->version = other.version;
		return *this;
	}

private:
	unsigned int version;
};

// Epics

#define EPIC_2 0x020100
#define EPIC_3 0x030100
#define EPIC_4_1 0x040100
#define EPIC_4_1_1 0x040101
#define EPIC_5_1 0x050100
#define EPIC_5_2 0x050200
#define EPIC_6_1 0x060100
#define EPIC_6_2 0x060200
#define EPIC_6_3 0x060300
#define EPIC_7_1 0x070100
#define EPIC_7_2 0x070200
#define EPIC_7_3 0x070300
#define EPIC_7_4 0x070400
#define EPIC_8_1 0x080100
#define EPIC_8_1_1_RSA 0x080101
#define EPIC_8_2 0x080200
#define EPIC_8_3 0x080300
#define EPIC_9_1 0x090100
#define EPIC_9_2 0x090200
#define EPIC_9_3 0x090300
#define EPIC_9_4 0x090400
#define EPIC_9_4_AR 0x090401
#define EPIC_9_4_2 0x090402  // newer login packet id
#define EPIC_9_5 0x090500
#define EPIC_9_5_1 0x090501  // 2017/09/26
#define EPIC_9_5_2 0x090502  // GS Version packet modified, 2018 02 27
#define EPIC_9_5_3 0x090503  // GS Version packet modified, 20181211
#define EPIC_9_6 0x090600    // GS Version packet modified, 20190102

// Latest released epic
#define EPIC_LATEST EPIC_9_6

// Flags

#define EPIC_FLAG_TESTSERVER 1

#endif  // PACKETS_EPICS_H
