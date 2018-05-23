#ifndef __HTTP_CONN_HEADER__
#define __HTTP_CONN_HEADER__
enum HTTP_CODE
{
	
};
class http_conn {
public:
	http_conn();
	virtual ~http_conn();

public:
	void init();
	void answer();
	HTTP_CODE parse_header();
	HTTP_CODE parse_content();
	HTTP_CODE parse_request_line();
};
#endif