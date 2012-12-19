// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include <libintl.h>
#include "services.h"
// Services gui v0.2.1 (C) Curaga
// Under the GPLv3
#include <locale.h>
#define bufsize 96
static int s_status[26]; 
static int s_path[26]; 
static char *services[26]; 
static char paths[2][24]={"/usr/local/etc/init.d/","/etc/init.d/services/"}; 
static char mydir[]="/tmp/servicesXXXXXX"; 

static int scan_services() {
  int i=0, total=0;
FILE *lst;
char buffer[bufsize];

snprintf(buffer,bufsize,"ls -1 %s > services.lst 2>/dev/null",paths[0]);
system(buffer);

snprintf(buffer,bufsize,"ls -1 %s >> services.lst 2>/dev/null",paths[1]);
system(buffer);

system("sort services.lst > services.lst2");
rename("services.lst2","services.lst");


lst=fopen("services.lst","r");

while (fgets(buffer,bufsize,lst)!=NULL) {
	if (i>=26) break; // At most 26 services.
	
	for (total=0;total<bufsize;total++) if (buffer[total]=='\n') buffer[total]='\0';
	
	services[i]=strdup(buffer);
	
	i++;
}

fclose(lst);
total=i;

// Services located. Now check status

for (i=0; i<total; i++) {

	snprintf(buffer,bufsize,"%s%s",paths[0],services[i]);
	if (access(buffer,X_OK)==0) s_path[i]=0;
	else {
		snprintf(buffer,bufsize,"%s%s",paths[1],services[i]);
		if (access(buffer,X_OK)==0) s_path[i]=1;
		else s_path[i]=-1;

	}

	if (s_path[i]>=0) {
		snprintf(buffer,bufsize,"sudo %s%s status 2>&1 >/dev/null",paths[s_path[i]],services[i]);
		s_status[i]=system(buffer);
		if (s_status[i]) s_status[i]=0;
		else s_status[i]=1;
	}
}

return total;
}

static void toggle_service(Fl_Widget*, void* userdata) {
  char buffer[bufsize];
int num=(long)userdata;

if (s_status[num]) fprintf(stderr,"Shutting down %s\n",services[num]);
else fprintf(stderr,"Starting %s\n",services[num]);

if (s_path[num]>=0) {
	switch (s_status[num]) {
		case 0:
			snprintf(buffer,bufsize,"sudo %s%s start",paths[s_path[num]],services[num]);
		break;
		default:
			snprintf(buffer,bufsize,"sudo %s%s stop",paths[s_path[num]],services[num]);
		break;
	}
	system(buffer);
}

if (s_status[num]) s_status[num]=0;
else s_status[num]=1;
}

static void cleanup(Fl_Widget*,void*) {
  char buffer[bufsize];

chdir("/");
snprintf(buffer,bufsize,"rm -rf %s",mydir);
system(buffer);

exit(0);
}

static Fl_Double_Window *window=(Fl_Double_Window *)0;

static Fl_Pack *left=(Fl_Pack *)0;

static Fl_Pack *right=(Fl_Pack *)0;

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
bindtextdomain("tinycore","/usr/local/share/locale");
textdomain("tinycore");
  int i, total=0, topa=13, topb=26;
char wintitle[]="Services: XX total";
Fl_Light_Button *lght[26];

memset(s_status,0,sizeof(int)*26);

mkdtemp(mydir);
chdir(mydir);

total=scan_services();

if (total<=13) {
	topa=total;
	topb=total;
} else if (total<=26) topb=total;

fprintf(stderr,"Found total %d services.\n",total);
  { window = new Fl_Double_Window(480, 425, gettext("Services"));
    { left = new Fl_Pack(20, 15, 200, 390);
      left->spacing(5);
      for (i=0; i<topa; i++) { lght[i]=new Fl_Light_Button(0,0,25,25,services[i]); lght[i]->selection_color(FL_GREEN); lght[i]->value(s_status[i]); lght[i]->callback((Fl_Callback*)toggle_service, (void*)i); }
      left->end();
    } // Fl_Pack* left
    { right = new Fl_Pack(260, 15, 200, 390);
      right->spacing(5);
      for (i=13; i<topb; i++) { lght[i]=new Fl_Light_Button(0,0,25,25,services[i]); lght[i]->selection_color(FL_GREEN); lght[i]->value(s_status[i]); lght[i]->callback((Fl_Callback*)toggle_service, (void*)i); }
      right->end();
    } // Fl_Pack* right
    sprintf(wintitle,"Services: %d total",total); window->label(wintitle);
    if (total<=13) window->size(window->w()/2,window->h());
    window->size(window->w(),35+((total<14?total:13)*30));
    window->callback((Fl_Callback*)cleanup);
    window->end();
  } // Fl_Double_Window* window
  window->show(argc, argv);
  return Fl::run();
}
