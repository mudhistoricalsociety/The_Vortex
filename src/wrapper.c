#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void *drowlib;
void *jedilib;
void *sociallib;

/* Snarf a fugly dll-wrapper file! (ala Kline style :)*/

void do_edit_social( CHAR_DATA *ch, char *argument )
{
 void (*handle)();
 
 if(sociallib == NULL)
  sociallib = dlopen(SOCIAL_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 if( (handle = dlsym(sociallib,"edit_social")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_delete_social( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(sociallib == NULL)
  sociallib = dlopen(SOCIAL_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(sociallib,"delete_social")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_create_social( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(sociallib == NULL)
  sociallib = dlopen(SOCIAL_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(sociallib,"create_social")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_strangle( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"strangle")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_levitate( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"levitate")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drowpowers( CHAR_DATA *ch, char *argument )
{
 void (*handle)();
 
 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 if( (handle = dlsym(drowlib,"drowpowers")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drow_grant( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"drowgrant")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_chaosblast( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"chaosblast")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drowfire( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"drowfire")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drow_heal( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"heal")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drowhate( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"drowhate")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_spiderform( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"spiderform")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_drowshield( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"drowshield")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_darkness( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"darkness")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_earthshatter( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"earthshatter")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_garotte( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"garotte")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_dgarotte( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if(drowlib == NULL)
  drowlib = dlopen(DROW_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(drowlib,"dgarotte")) !=NULL )
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_combat_style( CHAR_DATA *ch, char *argument)
{
 void (*handle)();

 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(jedilib,"combat_style")) != NULL)
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_jedi_show_powers( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 if( (handle = dlsym(jedilib,"jedi_show_powers")) != NULL)
  (*handle)(ch,argument);
 else
  log_string(dlerror());
 return;
}

void do_saber_craft( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"saber_craft");
 (*handle)(ch,argument);
 return;
}

void do_jover( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jover");
 (*handle)(ch,argument);
 return;
}

void do_jflip( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jflip");
 (*handle)(ch,argument);
 return;
}

void do_jupper( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jupper");
 (*handle)(ch,argument);
 return;
}

void do_jback( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jback");
 (*handle)(ch,argument);
 return;
}

void do_jslice( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jslice");
 (*handle)(ch,argument);
 return;
}

void do_jswipe( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jswipe");
 (*handle)(ch,argument);
 return;
}

void do_jslash( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jslash");
 (*handle)(ch,argument);
 return;
}

void do_jroll( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
   
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);

 handle = dlsym(jedilib,"jroll");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_heal( CHAR_DATA *ch, char *argument)           
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_heal");                    
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_absorb( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_absorb");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_shield( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_shield");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_mind_trick( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_mind_trick");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_sight( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_sight");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_speed( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_speed");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_jump( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_jump");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_push( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_push");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_pull( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_pull");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_grip( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_grip");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_rage( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_rage");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_drain( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_drain");
 (*handle)(ch,argument);
 return;
}

void do_jedi_force_lightning( CHAR_DATA *ch, char *argument)
{
 void (*handle)();
 
 if(jedilib == NULL)
  jedilib = dlopen(JEDI_LIB,RTLD_NOW | RTLD_GLOBAL);
 
 handle = dlsym(jedilib,"jedi_force_lightning");
 (*handle)(ch,argument);
 return;
}
