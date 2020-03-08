#ifndef DBSTRING_H
#define DBSTRING_H

#include <string>
#include <uv.h>

#include <sqlext.h>

#include <algorithm>
#include <array>

class DbStringBase {
public:
	virtual ~DbStringBase() {}
	virtual std::string to_string() = 0;
	virtual void assign(const std::string& input) = 0;

	virtual char* data() noexcept = 0;
	virtual size_t size() const noexcept = 0;
	virtual size_t max_size() const noexcept = 0;
	virtual SQLLEN* sql_size_ptr() noexcept = 0;
};

template<size_t N> class DbString : public DbStringBase {
public:
	DbString() : strSize(SQL_NULL_DATA) { strData[0] = '\0'; }
	template<size_t StrSize> explicit DbString(const char input[StrSize]) noexcept { assign<StrSize>(input); }
	explicit DbString(const std::string& input) noexcept { assign(input); }
	explicit DbString(const char* input) noexcept { assign(input); }
	explicit DbString(const char* input, size_t count) noexcept { assign(input, count); }
	explicit DbString(const DbString<N>& input) noexcept { assign(input); }
	virtual ~DbString() {}

	template<size_t StrSize> DbString& operator=(const char input[StrSize]) noexcept {
		assign<StrSize>(input);
		return *this;
	}
	DbString& operator=(const std::string& input) noexcept {
		assign(input);
		return *this;
	}
	DbString& operator=(const char* input) noexcept {
		assign(input);
		return *this;
	}
	DbString& operator=(const DbString<N>& input) noexcept {
		assign(input);
		return *this;
	}

	virtual std::string to_string() override { return std::string(begin(), end()); }

	template<size_t OtherSize> void assign(const DbString<OtherSize>& input) noexcept {
		assign(input.c_str(), input.size());
	}
	virtual void assign(const std::string& input) noexcept override { assign(input.c_str(), input.size()); }
	template<size_t StrSize> void assign(const char input[StrSize]) noexcept { assign(input, StrSize); }
	void assign(const char* input) noexcept { assign(input, std::char_traits<char>::length(input)); }

	void assign(const char* input, size_t count) noexcept {
		strSize = std::min(count, strData.size() - 1);
		std::copy(input, input + strSize, strData.begin());
		strData[strSize] = '\0';
		if(!strSize)
			strSize = SQL_NULL_DATA;
	}

	constexpr const char* c_str() const noexcept { return strData.data(); }
	void clear() noexcept { assign(nullptr, 0); }
	char& operator[](size_t index) noexcept { return strData[index]; }
	const char& operator[](size_t index) const noexcept { return strData[index]; }

	auto begin() { return strData.begin(); }
	auto begin() const { return strData.begin(); }
	auto end() { return strData.begin() + size(); }

	virtual char* data() noexcept override { return strData.data(); }
	virtual size_t size() const noexcept override { return strSize == SQL_NULL_DATA ? 0 : strSize; }
	virtual size_t max_size() const noexcept override { return strData.size(); }
	virtual SQLLEN* sql_size_ptr() noexcept override { return &strSize; }

private:
	std::array<char, N * sizeof(SQLWCHAR)> strData;
	SQLLEN strSize;
};

template<typename> struct is_DbString : std::false_type {};
template<std::size_t N> struct is_DbString<DbString<N>> : std::true_type {};

#endif  // DBTYPEHELPERS_H
