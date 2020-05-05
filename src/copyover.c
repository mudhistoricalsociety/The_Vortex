#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h> 
#include "merc.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH
void logf(char * fmt, ...);

void logf (char * fmt, ...)
{
	char buf[2*MSL];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	
	log_string (buf);
}


#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)

extern int port,control; /* db.c */

void do_copyover (CHAR_DATA *ch, char * argument)
{
	FILE *fp;
	DESCRIPTOR_DATA *d, *d_next;
	char buf [100], buf2[100];
	
	fp = fopen (COPYOVER_FILE, "w");
	
	if (!fp)
	{
		send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
		logf ("Could not write to copyover file: %s", COPYOVER_FILE);
		perror ("do_copyover:fopen");
		return;
	}
	
	/* Consider changing all saved areas here, if you use OLC */
	
	/* do_asave (NULL, ""); - autosave changed areas */
	
	if( IS_SET(ch->act,PLR_WIZINVIS) || IS_SET(ch->act,PLR_INCOG) )
	 sprintf(buf,"\n\r *** COPYOVER by an Immortal - please remain seated!\n\r");
	else	
	 sprintf(buf, "\n\r *** COPYOVER by %s - please remain seated!\n\r", ch->name);
	
	/* For each playing descriptor, save its state */
	for (d = descriptor_list; d ; d = d_next)
	{
		CHAR_DATA * och = CH (d);
		d_next = d->next; /* We delete from the list , so need to save this */
		
		if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
		{
			write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
			close_socket (d); /* throw'em out */
		}
		else
		{
			fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
			if(och->level < 2)
			{
			 write_to_descriptor (d->descriptor, "Since you are a newbie, and have not saved, you have been advanced!\n\r", 0);
			 och->level = 2;
			 och->trust = 2;
			 och->mkill += 5;
			}
			stop_fighting(och,TRUE);
			och->challenged = NULL;
			och->challenger = NULL;
			if( IS_SET(och->newbits, HAS_HADES) )
				REMOVE_BIT(och->newbits, HAS_HADES);
			if( IS_SET(och->act,PLR_CHALLENGER) )
        			REMOVE_BIT(och->act,PLR_CHALLENGER);
			if( IS_SET(och->act,PLR_CHALLENGED) )
        			REMOVE_BIT(och->act,PLR_CHALLENGED);
        		arena = FIGHT_OPEN;
                        if( in_arena(och) )
                        {
                         char_from_room(och);
                         char_to_room(och,get_room_index(3001));
                        }
			do_stand(och,"");
			do_call(och,"all");
			save_char_obj (och);
			write_to_descriptor (d->descriptor, buf, 0);
		}
	}
	fprintf (fp, "-1\n");
        log_string("Closing file.");
	fclose (fp);
	
	/* Close reserve and other always-open files and release other resources */
	log_string("Closing reserve stream.");
	fclose (fpReserve);
	
	/* exec - descriptors are inherited */
	log_string("Attempting new execution.");
	sprintf (buf, "%d", port);
	sprintf (buf2, "%d", control);
	execl (EXE_FILE, "Vortex", buf, "copyover", buf2, (char *) NULL);

	/* Failed - sucessful exec will not return */
	
	perror ("do_copyover: execl");
	send_to_char ("Copyover FAILED!\n\r",ch);
	
	/* Here you might want to reopen fpReserve */
        fpReserve = fopen(NULL_FILE, "r");
}

/* Recover from a copyover - load players */
void copyover_recover ()
{
	DESCRIPTOR_DATA *d;
	FILE *fp;
	char name [100];
	char host[MSL];
	int desc;
	bool fOld;
	
	logf ("Copyover recovery initiated");
	
	fp = fopen (COPYOVER_FILE, "r");
	
	if (!fp) /* there are some descriptors open which will hang forever then ? */
	{
		perror ("copyover_recover:fopen");
		logf ("Copyover file not found. Exitting.\n\r");
		exit (1);
	}

	unlink (COPYOVER_FILE); /* In case something crashes - doesn't prevent reading	*/
	
	for (;;)
	{
		fscanf (fp, "%d %s %s\n", &desc, name, host);
		if (desc == -1)
			break;

		/* Write something, and check if it goes error-free */		
		if (!write_to_descriptor (desc, "\n\rRestoring from copyover...\n\r",0))
		{
			close (desc); /* nope */
			continue;
		}
		
		d = alloc_perm (sizeof(DESCRIPTOR_DATA));
		init_descriptor (d,desc); /* set up various stuff */
		
		d->host = str_dup (host);
		d->next = descriptor_list;
		descriptor_list = d;
		d->connected = CON_COPYOVER_RECOVER; /* -15, so close_socket frees the char */
		
	
		/* Now, find the pfile */
		
		fOld = load_char_obj (d, name);
		
		if (!fOld) /* Player file not found?! */
		{
			write_to_descriptor (desc, "\n\rSomehow, your character was lost in the copyover. Sorry.\n\r", 0);
			close_socket (d);			
		}
		else /* ok! */
		{
			write_to_descriptor (desc, "\n\rCopyover recovery complete.\n\r",0);
	
			/* Just In Case */
			if (!d->character->in_room)
				d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);

			/* Insert in the char_list */
			d->character->next = char_list;
			char_list = d->character;

			char_to_room (d->character, d->character->in_room);
			do_look (d->character, "");
			act ("$n materializes!", d->character, NULL, NULL, TO_ROOM);
			d->connected = CON_PLAYING;
		}
		
	}
	
	fclose (fp);
	update_players_logged();
}

void auto_copyover()
{
        FILE *fp;
	FILE *last;
        DESCRIPTOR_DATA *d, *d_next;
        char buf [100], buf2[100];

        fp = fopen (COPYOVER_FILE, "w");

        if (!fp)
        {
                logf("Could not write to copyover file: %s", COPYOVER_FILE);
                perror ("do_copyover:fopen");
                return;
        }
       fclose(fpReserve);
       if((last = fopen(LAST_COMMAND,"a")) == NULL)
         bug("Error in do_auto_save opening last_command.txt",0);

       fprintf(last,"Last Command: %s\n",
            last_command);

       fclose( last );
       fpReserve = fopen( NULL_FILE, "r" );
	sprintf(buf,"Last command was %s",last_command);
	log_string(buf);
        /* Consider changing all saved areas here, if you use OLC */

        /* do_asave (NULL, ""); - autosave changed areas */

         sprintf(buf,"\n\r *** CRASH DETECTED :: Attempting copyover...\n\r");

        /* For each playing descriptor, save its state */
        for (d = descriptor_list; d ; d = d_next)
        {
               CHAR_DATA * och = CH (d);
                d_next = d->next; /* We delete from the list , so need to save this */

                if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
                {
                        write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
                        close_socket (d); /* throw'em out */
                }
                else
                {
                        fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
                        if(och->level < 2)
                        {
                         write_to_descriptor (d->descriptor, "Since you are a newbie, and have not saved, you have been advanced!\n",0);
                         och->level = 2;
                         och->trust = 2;
                         och->mkill += 5;
                        }
                        stop_fighting(och,TRUE);
                        och->challenged = NULL;
                        och->challenger = NULL;
                        if( IS_SET(och->newbits, HAS_HADES) )
                                REMOVE_BIT(och->newbits, HAS_HADES);
                        if( IS_SET(och->act,PLR_CHALLENGER) )
                                REMOVE_BIT(och->act,PLR_CHALLENGER);
                        if( IS_SET(och->act,PLR_CHALLENGED) )
                                REMOVE_BIT(och->act,PLR_CHALLENGED);
                        arena = FIGHT_OPEN;
			if( in_arena(och) )
			{
			 char_from_room(och);
			 char_to_room(och,get_room_index(3001));
			}
                        do_stand(och,"");
                        do_call(och,"all");
                        save_char_obj (och);
                       write_to_descriptor (d->descriptor, buf, 0);
                }
        }
        fprintf (fp, "-1\n");
        log_string("Closing file.");
        fclose (fp);
                        
        /* Close reserve and other always-open files and release other resources */
        log_string("Closing reserve stream.");
        fclose (fpReserve);
                        
        /* exec - descriptors are inherited */
        log_string("Attempting new execution.");
        sprintf (buf, "%d", port);
        sprintf (buf2, "%d", control);  
        execl (EXE_FILE, "Vortex", buf, "copyover", buf2, (char *) NULL);
                         
        /* Failed - sucessful exec will not return */
                        
        perror ("do_copyover: execl");
        logf("Copyover FAILED!\n\r");
                        
        /* Here you might want to reopen fpReserve */
        fpReserve = fopen(NULL_FILE, "r");
}
