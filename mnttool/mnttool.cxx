// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include <libintl.h>
#include "mnttool.h"
// (c) Robert Shingledecker 2008-2011
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale.h>
#include <FL/Fl_Button.H>
using namespace std;
static int size, selected, xPos, yPos; 
static string filemgr, winPos; 
static istringstream ins; 
vector<string> mountList; 
static vector<int> mountState; 
static vector<Fl_Button*> btn; 
vector<string> mountLabels; 

void getPos() {
  xPos = w->x();
yPos = w->y();
}

void refresh() {
  getMountables();
if ( size == 0 ) 
  exit(1);

pack->clear();

for (int i=0; i < size; i++)
{  
   Fl_Button* btn[i];
   
   btn[i] = new Fl_Button(0,0,80,25);
   btn[i]->label(mountList[i].c_str());
   btn[i]->tooltip(mountLabels[i].c_str());
   btn[i]->callback((Fl_Callback*)btnCallback,(void*)i);

   if ( mountState[i] == 0)
      btn[i]->color((Fl_Color)2);
   else
      btn[i]->color((Fl_Color)1);
      
   pack->add(btn[i]);
      
}

Fl_Button* btnRefresh;
btnRefresh = new Fl_Button(0,0,80,25);
btnRefresh->label("Refresh");
btnRefresh->callback((Fl_Callback*)btnRefreshCallback);
pack->add(btnRefresh);

selected = 0;
pack->redraw();
w->resize(0,0,80,(25*(size+1)));
w->position(xPos,yPos);
w->redraw();
}

int getMountables() {
  mountList.clear();
mountState.clear();

 system("/usr/bin/mountables.sh");

ifstream fin("/tmp/mountables");
string line;
string commandHead = "grep '/mnt/";
string commandTail = " ' /etc/mtab >/dev/null";

while ( getline(fin,line) )
{
   int sepator = line.find("~",0);
   string device = line.substr(0, sepator);
   string label = line.substr(sepator+1);
   mountList.push_back(device);
   mountState.push_back(system((commandHead + device + commandTail).c_str()));
   mountLabels.push_back(label);
}   
fin.close();
unlink("/tmp/mountables");

size = mountList.size();
}

void btnCallback(Fl_Widget*, void* userdata) {
  getPos();
int results;
selected = (long)userdata;
if (mountState.at(selected) == 0)   // mounted
{
   results = system(("sudo umount /dev/" + mountList.at(selected)).c_str());
   if (results == 0)
   {
      pack->child(selected)->color((Fl_Color)1);         
      mountState.at(selected)=256;
   }    
}   
else
{
   results = system(("sudo mount /dev/" + mountList.at(selected)).c_str());
   if (results == 0)
   {
      pack->child(selected)->color((Fl_Color)2);         
      mountState.at(selected)=0;
      if (filemgr.length() > 0)
      {
         system((filemgr + " /mnt/" + mountList.at(selected) +"&" ).c_str());
         exit(0);
      }
   }
}
}

void btnRefreshCallback(Fl_Widget*, void* userdata) {
  getPos();
refresh();
}

Fl_Double_Window *w=(Fl_Double_Window *)0;

Fl_Pack *pack=(Fl_Pack *)0;

int main(int argc, char **argv) {
  Fl::args(argc, argv);
setlocale(LC_ALL, "");
bindtextdomain("tinycore","/usr/local/share/locale");
textdomain("tinycore");
  { w = new Fl_Double_Window(85, 745);
    { pack = new Fl_Pack(0, 0, 80, 500);
      pack->end();
    } // Fl_Pack* pack
    w->end();
  } // Fl_Double_Window* w
  if (getenv("FILEMGR"))
   filemgr = getenv("FILEMGR");

xPos=80;
yPos=60;   

if (getenv("MNTTOOL")) { 
   winPos = getenv("MNTTOOL");
   ins.str(winPos);
   ins >> xPos >> yPos;
}   
   
refresh();
  w->show(argc, argv);
  return Fl::run();
}
