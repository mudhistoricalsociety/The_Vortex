#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>  
#include "merc.h"

#define EMEM_SIZE -1 /* find_mem_size returns this when block is too large */
#define NUL '\0'
#define MSL MAX_STRING_LENGTH
extern const int rgSizeList [MAX_MEM_LIST];

/* Find in rgSizeList a memory size at least this long */
int find_mem_size (int min_size)
{
	int i;
	
	for (i = 0; i < MAX_MEM_LIST; i++)
		if (rgSizeList[i] >= min_size)
			return rgSizeList[i];
	
	/* min_size is bigger than biggest allowable size! */
	
	return EMEM_SIZE;
}

/* Create a new buffer, of at least size bytes */

BUFFER * __buffer_new (int min_size)
{
	int size;
	BUFFER *buffer;
	char buf[200]; /* for the bug line */
	
	size = find_mem_size (min_size);
	
	if (size == EMEM_SIZE)
	{
		sprintf (buf, "Buffer size too big: %d bytes.", min_size);
		bug (buf,0);
		abort();
	}
	
	buffer = alloc_mem (sizeof(BUFFER));
	
	buffer->size = size;
	buffer->data = alloc_mem (size);
	buffer->overflowed = FALSE;
	
	buffer->len = 0;
	
	return buffer;
} /* __buf_new */

/* Add a string to a buffer. Expand if necessary */

void __buffer_strcat (BUFFER *buffer, const char *text)
{
	int new_size;
	int text_len;
	char *new_data;
	char buf[200];
	
	if (buffer->overflowed) /* Do not attempt to add anymore if buffer is already overflowed */
		return;

	if (!text) /* Adding NULL string ? */
		return;
	
	text_len = strlen(text);
	
	if (text_len == 0) /* Adding empty string ? */
		return;
		
	/* Will the combined len of the added text and the current text exceed our buffer? */

	if ((text_len+buffer->len+1) > buffer->size) /* expand? */
	{
		new_size = find_mem_size (buffer->size + text_len + 1);
		if (new_size == EMEM_SIZE) /* New size too big ? */
		{
			sprintf (buf, "Buffer overflow, wanted %d bytes.",text_len+buffer->len);			
			bug (buf, 0);
			buffer->overflowed = TRUE;
			return;
		}

		/* Allocate the new buffer */
		
		new_data = alloc_mem (new_size);		
		
		/* Copy the current buffer to the new buffer */
		
		memcpy (new_data, buffer->data, buffer->len);
		free_mem (buffer->data, buffer->size);
		buffer->data = new_data;
		buffer->size = new_size;

	} /* if */

	memcpy (buffer->data + buffer->len, text, text_len);	/* Start copying */
	buffer->len += text_len;	/* Adjust length */
	buffer->data[buffer->len] = NUL; /* Null-terminate at new end */
	
} /* __buf_strcat */

/* Free a buffer */
void buffer_free (BUFFER *buffer)
{
	/* Free data */
	free_mem (buffer->data, buffer->size);
	
	/* Free buffer */
	
	free_mem (buffer, sizeof(BUFFER));
}

/* Clear a buffer's contents, but do not deallocate anything */

void buffer_clear (BUFFER *buffer)
{
	buffer->overflowed = FALSE;
	buffer->len = 0;
        buffer->data[0] = '\0';
}

/* print stuff, append to buffer. safe. */
int bprintf (BUFFER *buffer, char *fmt, ...)
{
	char buf[MSL];
	va_list va;
	int res;
	
	va_start (va, fmt);
	res = vsnprintf (buf, MSL, fmt, va);
	va_end (va);

	if (res >= MSL-1)	
	{
		buf[0] = NUL;
		sprintf(buf,"Overflow when printing string %s",fmt);
                bug(buf,0);
                buf[0] = NUL;
	}
	else
		buffer_strcat (buffer, buf);

	return res;	
}


