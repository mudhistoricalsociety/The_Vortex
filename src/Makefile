CC      = gcc
#Uncomment the next line if you want request support
#DBUGFLG = -DREQUESTS
C_FLAGS = -Wall -g $(DBUGFLG)
L_FLAGS = -lz -lpthread -lcrypt -ldl -rdynamic -L/usr/include/mysql -lmysqlclient

SO_I = -c -fpic
SO_O = -shared -lm -o

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o special.o kav_info.o \
          kav_fight.o kav_wiz.o rot_wiz.o wizutil.o arena.o detox.o rotain.o \
          update.o clan.o powers.o clans.o stat_sav.o build.o copyover.o \
	  buffer.o class.o alien.o shadow.o monk.o ww.o who.o vampire.o \
          dragon.o wrapper.o class_config.o sql_handler.o
SO_FILE_SO = jedilib.so drowlib.so sociallib.so
SO_FILE_C = jedilib.c drowlib.c sociallib.c
SO_FILE_O = jedilib.o drowlib.o sociallib.o
	  

merc: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES)
	chmod g+w merc
	chmod g+w $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f $(O_FILES) merc ../area/core
	make

so:
	rm -f $(SO_FILE_SO) $(SO_FILE_O)
	$(CC) $(SO_I) jedilib.c
	$(CC) $(SO_O) jedilib.so jedilib.o
	$(CC) $(SO_I) drowlib.c
	$(CC) $(SO_O) drowlib.so drowlib.o 
	$(CC) $(SO_I) sociallib.c
	$(CC) $(SO_O) sociallib.so sociallib.o
