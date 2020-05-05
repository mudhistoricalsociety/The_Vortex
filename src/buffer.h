typedef struct buffer_type BUFFER;

struct buffer_type
{
	char *data;
	int len;
	int size;
	bool overflowed;
};

#define buffer_new(size)           __buffer_new (size)
#define buffer_strcat(buffer,text) __buffer_strcat (buffer,text)

BUFFER * __buffer_new (int size);
void __buffer_strcat (BUFFER *buffer, const char *text);


void buffer_free (BUFFER *buffer);
void buffer_clear (BUFFER *buffer);
int find_mem_size (int min_size);
int bprintf (BUFFER *buffer, char *fmt, ...);

