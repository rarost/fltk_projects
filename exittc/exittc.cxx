// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include <libintl.h>
#include "exittc.h"
// (c) Robert Shingledecker 2008-2010
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <FL/fl_message.H>
#include <locale.h>
using namespace std;
static string backup_device="", command; 
static int action=1; 
static bool backup=true; 
static string commandStr = "exitcheck.sh "; 
static string actionStr, backup_options, backup_command; 

void btn_callback(Fl_Widget*, void*userdata) {
  if (userdata == "cancel")
   exit(0);

if (userdata == "prompt") {
  choices->hide();
  action = 3;
} else if (userdata == "shutdown") {
  choices->show();
  action = 1;
} else if (userdata == "reboot") {
  choices->show();
  action = 2;
}


int backup_choice = choices->value();
switch(backup_choice)
{
   case 0  : backup = false;
             break;
   case 1  : backup = true;
             backup_options = "-b";
             break;
   case 2  : backup = true;
             backup_options = "-bs";
             break;
   default : backup = true;
             backup_options = "-b";
}             
   
   
if (backup) {
   if ( backup_device.size() == 0)
   {
      fl_message("No backup device has been selected.\nUse Control Panel Backup/Restore option to set");
      exit(1);
   } else {      
     device_output->value(backup_device.c_str());   
   }
}
         
if (userdata == "ok") {
  if (action == 3) {
     command = "sudo kill $( cat /tmp/.X${DISPLAY:1:1}-lock ) &";
     system(command.c_str());
     exit(0);
  } else {
    if (backup) {
      window->cursor(FL_CURSOR_WAIT);
      Fl::flush();
      backup_command = "filetool.sh " + backup_options;
      cout << backup_command << endl;
      int results = system(backup_command.c_str());
      window->cursor(FL_CURSOR_DEFAULT);
      Fl::flush();
      if ( results != 0 )
      {
         fl_message("Errors occured during backup!\nSee: /tmp/backup_status.");
         exit(1);
      }
    } else {
      system(">/etc/system/backup_device");
    }
    if (action == 1) {
       actionStr = "shutdown ";
    } else {
       actionStr = "reboot ";
    }
    commandStr += actionStr;
    system( (const char *)commandStr.c_str());
    exit(0);
  }  
}
}

Fl_Double_Window *window=(Fl_Double_Window *)0;

Fl_Output *device_output=(Fl_Output *)0;

Fl_Choice *choices=(Fl_Choice *)0;

Fl_Menu_Item menu_choices[] = {
 {gettext("None"), 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {gettext("Backup"), 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {gettext("Safe"), 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
bindtextdomain("tinycore","/usr/local/share/locale");
textdomain("tinycore");
  { window = new Fl_Double_Window(180, 180, gettext("TC Exit Options"));
    window->box(FL_BORDER_BOX);
    window->align(FL_ALIGN_LEFT);
    { Fl_Group* o = new Fl_Group(25, 10, 115, 70);
      { Fl_Round_Button* o = new Fl_Round_Button(25, 10, 85, 15, gettext("Shutdown"));
        o->type(102);
        o->down_box(FL_ROUND_DOWN_BOX);
        o->value(1);
        o->callback((Fl_Callback*)btn_callback, (void*)("shutdown"));
      } // Fl_Round_Button* o
      { Fl_Round_Button* o = new Fl_Round_Button(25, 30, 65, 15, gettext("Reboot"));
        o->type(102);
        o->down_box(FL_ROUND_DOWN_BOX);
        o->callback((Fl_Callback*)btn_callback, (void*)("reboot"));
      } // Fl_Round_Button* o
      { Fl_Round_Button* o = new Fl_Round_Button(25, 50, 110, 15, gettext("Exit to Prompt"));
        o->type(102);
        o->down_box(FL_ROUND_DOWN_BOX);
        o->callback((Fl_Callback*)btn_callback, (void*)("prompt"));
      } // Fl_Round_Button* o
      o->end();
    } // Fl_Group* o
    { device_output = new Fl_Output(15, 120, 155, 25);
      device_output->color((Fl_Color)29);
      device_output->labeltype(FL_NO_LABEL);
    } // Fl_Output* device_output
    { choices = new Fl_Choice(42, 87, 110, 25, gettext("Backup Options:"));
      choices->down_box(FL_BORDER_BOX);
      choices->align(FL_ALIGN_TOP_LEFT);
      choices->menu(menu_choices);
    } // Fl_Choice* choices
    { Fl_Return_Button* o = new Fl_Return_Button(15, 150, 70, 20, gettext("&OK"));
      o->callback((Fl_Callback*)btn_callback, (void*)("ok"));
    } // Fl_Return_Button* o
    { Fl_Button* o = new Fl_Button(100, 150, 70, 20, gettext("&Cancel"));
      o->callback((Fl_Callback*)btn_callback, (void*)("cancel"));
    } // Fl_Button* o
    window->end();
  } // Fl_Double_Window* window
  ifstream backup_device_file("/etc/sysconfig/backup_device");
if ( backup_device_file.is_open())
{
	getline(backup_device_file,backup_device);                                        
	backup_device_file.close();
}


string backupDefault="1";
if ( getenv("BACKUP"))
	backupDefault  = getenv("BACKUP");

if ( backupDefault == "0" )
{
  backup = false;
  choices->value(0);
} else {
  backup = true;
  choices->value(1);
}
  
   
string cmdline;   
ifstream proc_cmdline_file("/proc/cmdline");                                  
getline(proc_cmdline_file,cmdline);                                        
proc_cmdline_file.close();

int loc = cmdline.find("norestore");
if ( loc == string::npos )
{
  if ( backup_device.size() == 0 )
  {
    string tce_dir;   
    ifstream tce_dir_file("/opt/.tce_dir");                                  
    getline(tce_dir_file,tce_dir);                                        
    tce_dir_file.close();
    if ( tce_dir == "/tmp/tce" )
    {
      backup = false;
      choices->value(0);
    } else {  
      backup_device = tce_dir;
      choices->value(1);
    }
  }      
  
} else {
  backup = false;
  choices->value(0);
}


int option = cmdline.find("safebackup");
if ( option != string::npos )
{
  backup = true;
  choices->value(2);
}
 

if (backup)
   device_output->value(backup_device.c_str());
  window->show(argc, argv);
  return Fl::run();
}
