#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void save_class_config  args(( void ));
void load_class_config  args(( void ));

CLASS_DATA *alie;
CLASS_DATA *apoc;
CLASS_DATA *drag;
CLASS_DATA *drow;
CLASS_DATA *jedi;
CLASS_DATA *monk;
CLASS_DATA *shad;
CLASS_DATA *vamp;
CLASS_DATA *were;

void do_class_config( CHAR_DATA *ch,char *argument )
{
 char buf[MAX_STRING_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);
 argument = one_argument(argument,arg3);

 if( arg3[0] == '\0' )
 {
  stc("#7Syntax:  classconfig <class> <field> <value>#n\n\r",ch);
  stc("-----------------------------------------------------------------------\n\r",ch);
  stc("| **** | Alie | Apoc | Drag | Drow | Jedi | Monk | Shad | Vamp | Were |\n\r",ch);
  stc("-----------------------------------------------------------------------\n\r",ch);
  sprintf(buf,"| Armo | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->armo,apoc->armo,drag->armo,drow->armo,jedi->armo,monk->armo,shad->armo,vamp->armo,were->armo);
   stc(buf,ch);
  sprintf(buf,"| Atta | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->atta,apoc->atta,drag->atta,drow->atta,jedi->atta,monk->atta,shad->atta,vamp->atta,were->atta);
   stc(buf,ch);
  sprintf(buf,"| Damr | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->damr,apoc->damr,drag->damr,drow->damr,jedi->damr,monk->damr,shad->damr,vamp->damr,were->damr);
   stc(buf,ch);
  sprintf(buf,"| Dodg | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->dodg,apoc->dodg,drag->dodg,drow->dodg,jedi->dodg,monk->dodg,shad->dodg,vamp->dodg,were->dodg);
   stc(buf,ch);
  sprintf(buf,"| Hitr | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->hitr,apoc->hitr,drag->hitr,drow->hitr,jedi->hitr,monk->hitr,shad->hitr,vamp->hitr,were->hitr);
   stc(buf,ch);
  sprintf(buf,"| Parr | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d |\n\r",
   alie->parr,apoc->parr,drag->parr,drow->parr,jedi->parr,monk->parr,shad->parr,vamp->parr,were->parr);
   stc(buf,ch);
  sprintf(buf,"| Powe | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f |\n\r",
   alie->powe,apoc->powe,drag->powe,drow->powe,jedi->powe,monk->powe,shad->powe,vamp->powe,were->powe);
   stc(buf,ch);
  sprintf(buf,"| Rege | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f |\n\r",
   alie->rege,apoc->rege,drag->rege,drow->rege,jedi->rege,monk->rege,shad->rege,vamp->rege,were->rege);
   stc(buf,ch);
  sprintf(buf,"| Toug | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f | %4.2f |\n\r",
   alie->toug,apoc->toug,drag->toug,drow->toug,jedi->toug,monk->toug,shad->toug,vamp->toug,were->toug);
   stc(buf,ch);
  stc("-----------------------------------------------------------------------\n\r",ch);
  return;
 }

 if( !str_prefix(arg1,"alien") )
 {
  if( !str_prefix(arg2,"armor") )
   alie->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   alie->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   alie->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   alie->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   alie->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   alie->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   alie->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   alie->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   alie->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"apocryphic") )
 {
  if( !str_prefix(arg2,"armor") )
   apoc->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   apoc->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   apoc->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   apoc->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   apoc->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   apoc->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   apoc->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   apoc->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   apoc->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"dragon") )
 {
  if( !str_prefix(arg2,"armor") )
   drag->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   drag->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   drag->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   drag->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   drag->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   drag->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   drag->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   drag->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   drag->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"drow") )
 {
  if( !str_prefix(arg2,"armor") )
   drow->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   drow->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   drow->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   drow->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   drow->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   drow->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   drow->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   drow->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   drow->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"jedi") )
 {
  if( !str_prefix(arg2,"armor") )
   jedi->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   jedi->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   jedi->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   jedi->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   jedi->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   jedi->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   jedi->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   jedi->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   jedi->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"monk") )
 {
  if( !str_prefix(arg2,"armor") )
   monk->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   monk->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   monk->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   monk->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   monk->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   monk->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   monk->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   monk->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   monk->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"shadow") )
 {
  if( !str_prefix(arg2,"armor") )
   shad->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   shad->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   shad->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   shad->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   shad->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   shad->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   shad->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   shad->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   shad->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"vampire") )
 {
  if( !str_prefix(arg2,"armor") )
   vamp->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   vamp->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   vamp->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   vamp->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   vamp->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   vamp->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   vamp->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   vamp->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   vamp->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else if( !str_prefix(arg1,"werewolf") )
 {
  if( !str_prefix(arg2,"armor") )
   were->armo = atoi(arg3);
  else if( !str_prefix(arg2,"attack") )
   were->atta = atoi(arg3);
  else if( !str_prefix(arg2,"damroll") )
   were->damr = atoi(arg3);
  else if( !str_prefix(arg2,"dodge") )
   were->dodg = atoi(arg3);
  else if( !str_prefix(arg2,"hitroll") )
   were->hitr = atoi(arg3);
  else if( !str_prefix(arg2,"parry") )
   were->parr = atoi(arg3);
  else if( !str_prefix(arg2,"power") )
   were->powe = atof(arg3);
  else if( !str_prefix(arg2,"regen") )
   were->rege = atof(arg3);
  else if( !str_prefix(arg2,"toughness") )
   were->toug = atof(arg3);
  else
  {
   stc("Invalid field selection.\n\r",ch);
   return;
  }
  save_class_config();
  return;
 }
 else
 {
  stc("Invalid class selection.\n\r",ch);
  return;
 }
 return;
}

void init_class( void )
{
 if( alie == NULL )
  alie = alloc_perm(sizeof(*alie));
 if( apoc == NULL )
  apoc = alloc_perm(sizeof(*apoc));
 if( drag == NULL )       
  drag = alloc_perm(sizeof(*drag)); 
 if( drow == NULL )       
  drow = alloc_perm(sizeof(*drow)); 
 if( jedi == NULL )       
  jedi = alloc_perm(sizeof(*jedi)); 
 if( monk == NULL )       
  monk = alloc_perm(sizeof(*monk)); 
 if( shad == NULL )       
  shad = alloc_perm(sizeof(*shad)); 
 if( vamp == NULL )       
  vamp = alloc_perm(sizeof(*vamp)); 
 if( were == NULL )       
  were = alloc_perm(sizeof(*were));

 /* Alien     */
 alie->armo = 0;
 alie->atta = 0;
 alie->damr = 0;
 alie->dodg = 0;
 alie->hitr = 0;
 alie->parr = 0;
 alie->powe = 1;
 alie->rege = 1;
 alie->toug = 1;

 /* Apocryphic*/
 apoc->armo = 0;
 apoc->atta = 0;
 apoc->damr = 0;
 apoc->dodg = 0;
 apoc->hitr = 0;
 apoc->parr = 0;
 apoc->powe = 1;
 apoc->rege = 1;
 apoc->toug = 1;

 /* Dragon    */
 drag->armo = 0;
 drag->atta = 0;
 drag->damr = 0;
 drag->dodg = 0;
 drag->hitr = 0;
 drag->parr = 0;
 drag->powe = 1;
 drag->rege = 1;
 drag->toug = 1;

 /* Drow      */
 drow->armo = 0;
 drow->atta = 0;
 drow->damr = 0;
 drow->dodg = 0;
 drow->hitr = 0;
 drow->parr = 0;
 drow->powe = 1;
 drow->rege = 1;
 drow->toug = 1;

 /* Jedi      */
 jedi->armo = 0;
 jedi->atta = 0;
 jedi->damr = 0;
 jedi->dodg = 0;
 jedi->hitr = 0;
 jedi->parr = 0;
 jedi->powe = 1;
 jedi->rege = 1;
 jedi->toug = 1;

 /* Monk      */
 monk->armo = 0;
 monk->atta = 0;
 monk->damr = 0;
 monk->dodg = 0;
 monk->hitr = 0;
 monk->parr = 0;
 monk->powe = 1;
 monk->rege = 1;
 monk->toug = 1;

 /* Shadow    */
 shad->armo = 0;
 shad->atta = 0;
 shad->damr = 0;
 shad->dodg = 0;
 shad->hitr = 0;
 shad->parr = 0;
 shad->powe = 1;
 shad->rege = 1;
 shad->toug = 1;

 /* Vampire   */
 vamp->armo = 0;
 vamp->atta = 0;
 vamp->damr = 0;
 vamp->dodg = 0;
 vamp->hitr = 0;
 vamp->parr = 0;
 vamp->powe = 1;
 vamp->rege = 1;
 vamp->toug = 1;

 /* Werewolf  */
 were->armo = 0;
 were->atta = 0;
 were->damr = 0;
 were->dodg = 0;
 were->hitr = 0;
 were->parr = 0;
 were->powe = 1;
 were->rege = 1;
 were->toug = 1;

 load_class_config();
 return;
}

void save_class_config( void )
{
 FILE *fp;
  
 if( (fp = fopen(CLASS_FILE,"w")) == NULL)
 { 
  log_string("Can not open CLASS_FILE for writing.");
  return;
 }

 /* Alien           */
 fprintf(fp,"Alie\n");
 fprintf(fp,"Armo %d\n",alie->armo);
 fprintf(fp,"Atta %d\n",alie->atta);
 fprintf(fp,"Damr %d\n",alie->damr);
 fprintf(fp,"Dodg %d\n",alie->dodg);
 fprintf(fp,"Hitr %d\n",alie->hitr);
 fprintf(fp,"Parr %d\n",alie->parr);
 fprintf(fp,"Powe %3.2f~\n",alie->powe);
 fprintf(fp,"Rege %3.2f~\n",alie->rege);
 fprintf(fp,"Toug %3.2f~\n",alie->toug);
 fprintf(fp,"End\n");

 /* Apocryphic      */
 fprintf(fp,"Apoc\n");
 fprintf(fp,"Armo %d\n",apoc->armo); 
 fprintf(fp,"Atta %d\n",apoc->atta);
 fprintf(fp,"Damr %d\n",apoc->damr);
 fprintf(fp,"Dodg %d\n",apoc->dodg);
 fprintf(fp,"Hitr %d\n",apoc->hitr);
 fprintf(fp,"Parr %d\n",apoc->parr);
 fprintf(fp,"Powe %3.2f~\n",apoc->powe);
 fprintf(fp,"Rege %3.2f~\n",apoc->rege);
 fprintf(fp,"Toug %3.2f~\n",apoc->toug);
 fprintf(fp,"End\n");

 /* Dragon          */
 fprintf(fp,"Drag\n");
 fprintf(fp,"Armo %d\n",drag->armo); 
 fprintf(fp,"Atta %d\n",drag->atta);
 fprintf(fp,"Damr %d\n",drag->damr);
 fprintf(fp,"Dodg %d\n",drag->dodg);
 fprintf(fp,"Hitr %d\n",drag->hitr);
 fprintf(fp,"Parr %d\n",drag->parr);
 fprintf(fp,"Powe %3.2f~\n",drag->powe);
 fprintf(fp,"Rege %3.2f~\n",drag->rege);
 fprintf(fp,"Toug %3.2f~\n",drag->toug);
 fprintf(fp,"End\n");

 /* Drow            */
 fprintf(fp,"Drow\n");
 fprintf(fp,"Armo %d\n",drow->armo); 
 fprintf(fp,"Atta %d\n",drow->atta);
 fprintf(fp,"Damr %d\n",drow->damr);
 fprintf(fp,"Dodg %d\n",drow->dodg);
 fprintf(fp,"Hitr %d\n",drow->hitr);
 fprintf(fp,"Parr %d\n",drow->parr);
 fprintf(fp,"Powe %3.2f~\n",drow->powe);
 fprintf(fp,"Rege %3.2f~\n",drow->rege);
 fprintf(fp,"Toug %3.2f~\n",drow->toug);
 fprintf(fp,"End\n");

 /* Jedi            */
 fprintf(fp,"Jedi\n");
 fprintf(fp,"Armo %d\n",jedi->armo); 
 fprintf(fp,"Atta %d\n",jedi->atta);
 fprintf(fp,"Damr %d\n",jedi->damr);
 fprintf(fp,"Dodg %d\n",jedi->dodg);
 fprintf(fp,"Hitr %d\n",jedi->hitr);
 fprintf(fp,"Parr %d\n",jedi->parr);
 fprintf(fp,"Powe %3.2f~\n",jedi->powe);
 fprintf(fp,"Rege %3.2f~\n",jedi->rege);
 fprintf(fp,"Toug %3.2f~\n",jedi->toug);
 fprintf(fp,"End\n");

 /* Monk            */
 fprintf(fp,"Monk\n");
 fprintf(fp,"Armo %d\n",monk->armo);
 fprintf(fp,"Atta %d\n",monk->atta);
 fprintf(fp,"Damr %d\n",monk->damr);
 fprintf(fp,"Dodg %d\n",monk->dodg);
 fprintf(fp,"Hitr %d\n",monk->hitr);
 fprintf(fp,"Parr %d\n",monk->parr);
 fprintf(fp,"Powe %3.2f~\n",monk->powe);
 fprintf(fp,"Rege %3.2f~\n",monk->rege);
 fprintf(fp,"Toug %3.2f~\n",monk->toug);
 fprintf(fp,"End\n");

 /* Shadow          */
 fprintf(fp,"Shad\n");
 fprintf(fp,"Armo %d\n",shad->armo);
 fprintf(fp,"Atta %d\n",shad->atta);
 fprintf(fp,"Damr %d\n",shad->damr);
 fprintf(fp,"Dodg %d\n",shad->dodg);
 fprintf(fp,"Hitr %d\n",shad->hitr);
 fprintf(fp,"Parr %d\n",shad->parr);
 fprintf(fp,"Powe %3.2f~\n",shad->powe);
 fprintf(fp,"Rege %3.2f~\n",shad->rege);
 fprintf(fp,"Toug %3.2f~\n",shad->toug);
 fprintf(fp,"End\n");

 /* Vampire         */
 fprintf(fp,"Vamp\n");
 fprintf(fp,"Armo %d\n",vamp->armo);
 fprintf(fp,"Atta %d\n",vamp->atta);
 fprintf(fp,"Damr %d\n",vamp->damr);
 fprintf(fp,"Dodg %d\n",vamp->dodg);
 fprintf(fp,"Hitr %d\n",vamp->hitr);
 fprintf(fp,"Parr %d\n",vamp->parr);
 fprintf(fp,"Powe %3.2f~\n",vamp->powe);
 fprintf(fp,"Rege %3.2f~\n",vamp->rege);
 fprintf(fp,"Toug %3.2f~\n",vamp->toug);
 fprintf(fp,"End\n");

 /* Werewolf        */
 fprintf(fp,"Were\n");
 fprintf(fp,"Armo %d\n",were->armo);
 fprintf(fp,"Atta %d\n",were->atta);
 fprintf(fp,"Damr %d\n",were->damr);
 fprintf(fp,"Dodg %d\n",were->dodg);
 fprintf(fp,"Hitr %d\n",were->hitr);
 fprintf(fp,"Parr %d\n",were->parr);
 fprintf(fp,"Powe %3.2f~\n",were->powe);
 fprintf(fp,"Rege %3.2f~\n",were->rege);
 fprintf(fp,"Toug %3.2f~\n",were->toug);
 fprintf(fp,"End\n");

 fprintf(fp,"EOF\n");
 log_string("Class data successfully saved.");
 fclose(fp);
 return;
}

void load_class_config( void )
{
	FILE *fp;
	char *word;
	bool done = FALSE;
	bool sub = FALSE;
	CLASS_DATA *class;
	char buf[MAX_STRING_LENGTH];

 	if( (fp = fopen(CLASS_FILE,"r")) == NULL)
 	{
  		log_string("Can not open CLASS_FILE for reading.");
  		return;
 	}
	word = fread_word(fp);
	while( !done )
	{
		if( !str_cmp( word, "Alie"))
			class = alie;
		else if( !str_cmp( word, "Apoc"))
			class = apoc;
		else if( !str_cmp( word, "Drag"))
			class = drag;
		else if( !str_cmp( word, "Drow"))
			class = drow;
		else if( !str_cmp( word, "Jedi"))
			class = jedi;
		else if( !str_cmp( word, "Monk"))
			class = monk;
		else if( !str_cmp( word, "Shad"))
			class = shad;
		else if( !str_cmp( word, "Vamp"))
			class = vamp;
		else if( !str_cmp( word, "Were"))
			class = were;
		else
		{
			sprintf(buf,"Unkown class %s in load_class_config",word);
			log_string(buf);
			fclose(fp);
			return;
		}
		while( !sub )
		{
			word = fread_word(fp);
			if( !str_cmp( word, "Armo"))
				class->armo = fread_number(fp);
			else if( !str_cmp( word, "Atta"))
				class->atta = fread_number(fp);
			else if( !str_cmp( word, "Damr"))
				class->damr = fread_number(fp);
			else if( !str_cmp( word, "Dodg"))
				class->dodg = fread_number(fp);		
			else if( !str_cmp( word, "Hitr"))
				class->hitr = fread_number(fp);
			else if( !str_cmp( word, "Parr"))
				class->parr = fread_number(fp);
			else if( !str_cmp( word, "Powe"))
                                class->powe = atof(fread_string(fp));
			else if( !str_cmp( word, "Rege"))
                                class->rege = atof(fread_string(fp));
			else if( !str_cmp( word, "Toug"))
                                class->toug = atof(fread_string(fp));
			else if( !str_cmp( word, "End"))
				sub = TRUE;
			else
			{
				sprintf(buf,"Unkown field %s, aborting.",word);
                                log_string(buf);
				fclose(fp);
				return;
			}
		}
		sub = FALSE;
		/* get the next word */
		word = fread_word(fp);
		if( !str_cmp(word, "EOF"))
			done = TRUE;
	}
	fclose(fp);
	return;
}
