#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include "merc.h"

#define DB_HOST "localhost"
#define DB_USER "icrontic"
#define DB_PASS "citnorci"
#define DB_DB   "icrontic"
#define DB_PORT        0
#define DB_SOCKET      0
#define DB_CLIENT_FLAG 0
#define DB_CONNECT_TIME_LIMIT 5

MYSQL  *db;
void   connect_db();
void   disconnect_db();
MYSQL_RES *query_db(char *query);
bool   db_connect;
sh_int db_last_action;
void   db_check_disconnect();

void connect_db()
{
 char buf[MAX_STRING_LENGTH];
	if ((db = mysql_init(NULL)) == NULL)
	{
		log_string("connect_db: error on initialize");
		return;
	}
	if (!mysql_real_connect(db, DB_HOST, DB_USER, DB_PASS,
		DB_DB, DB_PORT, DB_SOCKET, DB_CLIENT_FLAG))
	{
		log_string("connect_db: error on connect");
		sprintf(buf,"Error: %s", mysql_error(db));
                log_string(buf);
 		return;
	}
        db_connect = TRUE;
        db_last_action = 0;
	return;
}

void disconnect_db()
{
	mysql_close(db);
	db_connect = FALSE;
	db = NULL;
	return;
}

MYSQL_RES *query_db(char *query)
{
	char buf[MAX_STRING_LENGTH];
	buf[0] = '\0';

	if( !db_connect )
		connect_db();
	if( db == NULL )
	{
		log_string("NULL db in query_db.");
		return NULL;
	}
	if( query == NULL || query[0] == '\0')
	{
		log_string("Null query in query_db()");
		return NULL;
	}
 	if (mysql_real_query(db, query, strlen(query)))
	{
		log_string("query_db(): error on query");
		sprintf(buf,"Error: %s", mysql_error(db));
                log_string(buf);
		return NULL;
	}
 	return mysql_store_result(db);
}

void check_db_disconnect()
{
	/* first check if we're even connected */
	if( !db_connect)
		return;
	/* No we see if we have been connected for too long,
	   if we have, lets disconnect and save resources
        */
	if( db_last_action >= DB_CONNECT_TIME_LIMIT)
	{
		disconnect_db();
		return;
	}
	/* at this point we must still be connected, so lets increment our time */
	db_last_action++;
	return;
}

char *sql_escape_string( char *source ) 
{ 
 char *buf = '\0';  
 char *ret = '\0';  
 int len = 0; 
  
 len = strlen( source );  
 buf = malloc( (2 *len) + 1); 

 mysql_real_escape_string( db, buf, source, len);  
 ret = str_dup( buf );  
 free( buf );  
 return ret; 
}
