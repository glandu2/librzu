struct TS_AC_SERVER_LIST_EX 10022
{
	uint16_t last_login_server_idx = 1; [version >= 040000]
	uint16_t servers.size();

	struct TS_SERVER_INFO_EX
	{
		uint16_t server_idx;
		char server_name[21];
		bool is_adult_server = false;                      [version >= 040000]
		char server_screenshot_url[256] = "about:blank";   [version >= 040000]
		char server_ip[16];
		int32_t server_port;
		uint16_t user_ratio;
	};
	
	TS_SERVER_INFO_EX servers[];

	uint32_t adata;
};