// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include <libintl.h>
#include "fdtool.h"
// (c) Robert Shingledecker
#include <cstdlib>
#include <iostream>
#include <locale.h>
using namespace std;

void btnCallback(Fl_Widget*, void* userdata) {
  const string userdatastr = userdata ? (char *) userdata : "";

if (userdatastr == "cancel")
   exit(1);

string command;   
string commandA = "aterm +tr -bg white -fg black -geometry 80x4 -T ";
string commandB = " -e floppytool.sh ";  
if (userdatastr == "format")
{
   command = commandA + "\"Resizing and Formatting Floppy\" " + commandB;
   if (btn1440 -> value() == 1)
      command += "format 1440";
   else
      command += "format 1722";
} else
{
   string strImagePath = inpImagePath -> value();     

   if (userdatastr == "mkimage" )
   {
      command = commandA + "\"Creating Floppy Image\"" + commandB;
      if (btn1440 -> value() == 1)
         command += "makeimage 1440 " + strImagePath;
      else
         command += "makeimage 1722 " + strImagePath;
   }

   if (userdatastr == "apimage")
   {
      command = commandA + "\"Creating Floppy from Image File\"" + commandB;
      if (btn1440 -> value() == 1)
         command += "applyimage 1440 " + strImagePath;
      else
         command += "applyimage 1449 " + strImagePath;
   }
}
system(command.c_str());
}

void inp_callback(Fl_Widget*, void*) {
  btnMkImage->activate();
btnApImage->activate();
}

Fl_Round_Button *btn1440=(Fl_Round_Button *)0;

Fl_Round_Button *btn1722=(Fl_Round_Button *)0;

Fl_Input *inpImagePath=(Fl_Input *)0;

Fl_Button *btnMkImage=(Fl_Button *)0;

Fl_Button *btnApImage=(Fl_Button *)0;

int main(int argc, char **argv) {
  Fl_Double_Window* w;
  setlocale(LC_ALL, "");
bindtextdomain("tinycore","/usr/local/share/locale");
textdomain("tinycore");
  { Fl_Double_Window* o = new Fl_Double_Window(490, 155, gettext("FloppyTool"));
    w = o;
    { Fl_Box* o = new Fl_Box(10, 29, 470, 116, gettext("Floppy Disk Tool"));
      o->box(FL_EMBOSSED_FRAME);
      o->align(FL_ALIGN_TOP);
    } // Fl_Box* o
    { btn1440 = new Fl_Round_Button(125, 40, 64, 15, gettext("1440k"));
      btn1440->down_box(FL_ROUND_DOWN_BOX);
      btn1440->value(1);
      btn1440->selection_color((Fl_Color)2);
      btn1440->type(FL_RADIO_BUTTON);
    } // Fl_Round_Button* btn1440
    { btn1722 = new Fl_Round_Button(265, 40, 64, 15, gettext("1722k"));
      btn1722->down_box(FL_ROUND_DOWN_BOX);
      btn1722->selection_color((Fl_Color)2);
      btn1722->type(FL_RADIO_BUTTON);
    } // Fl_Round_Button* btn1722
    { inpImagePath = new Fl_Input(105, 65, 355, 25, gettext("Image Path:"));
      inpImagePath->callback((Fl_Callback*)inp_callback);
      inpImagePath->when(FL_WHEN_CHANGED);
    } // Fl_Input* inpImagePath
    { Fl_Button* o = new Fl_Button(40, 100, 90, 25, gettext("&Format"));
      o->tooltip(gettext("Formats FAT of selected size."));
      o->callback((Fl_Callback*)btnCallback, (void*)("format"));
    } // Fl_Button* o
    { btnMkImage = new Fl_Button(145, 100, 90, 25, gettext("&Make Image"));
      btnMkImage->tooltip(gettext("Copies floppy to image path entered,"));
      btnMkImage->callback((Fl_Callback*)btnCallback, (void*)("mkimage"));
      btnMkImage->deactivate();
    } // Fl_Button* btnMkImage
    { btnApImage = new Fl_Button(250, 100, 90, 25, gettext("&Apply Image"));
      btnApImage->tooltip(gettext("Writes image path file to floppy."));
      btnApImage->callback((Fl_Callback*)btnCallback, (void*)("apimage"));
      btnApImage->deactivate();
    } // Fl_Button* btnApImage
    { Fl_Button* o = new Fl_Button(355, 100, 90, 25, gettext("&Cancel"));
      o->callback((Fl_Callback*)btnCallback, (void*)("cancel"));
    } // Fl_Button* o
    o->end();
  } // Fl_Double_Window* o
  w->show(argc, argv);
  return Fl::run();
}
